// Copyright (c) 2019, Paul Ferrand
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "FilePool.h"
#include "AudioBuffer.h"
#include "Config.h"
#include "Debug.h"
#include "Oversampler.h"
#include "absl/types/span.h"
#include <chrono>
#include <memory>
#include <mutex>
#include <sndfile.hh>
#include <thread>
using namespace std::chrono_literals;

template <class T>
std::unique_ptr<sfz::AudioBuffer<T>> upsample2x(const sfz::AudioBuffer<T>& buffer)
{
    // auto tempBuffer = std::make_unique<sfz::Buffer<T>>(buffer.getNumFrames() * 2);
    auto outputBuffer = std::make_unique<sfz::AudioBuffer<T>>(buffer.getNumChannels(), buffer.getNumFrames() * 2);
    for (int channelIdx = 0; channelIdx < buffer.getNumChannels(); channelIdx++) {
        sfz::upsample2xStage(buffer.getConstSpan(channelIdx), outputBuffer->getSpan(channelIdx));
    }
    return outputBuffer;
}

template <class T>
std::unique_ptr<sfz::AudioBuffer<T>> upsample4x(const sfz::AudioBuffer<T>& buffer)
{
    auto tempBuffer = std::make_unique<sfz::Buffer<T>>(buffer.getNumFrames() * 2);
    auto outputBuffer = std::make_unique<sfz::AudioBuffer<T>>(buffer.getNumChannels(), buffer.getNumFrames() * 4);
    for (int channelIdx = 0; channelIdx < buffer.getNumChannels(); channelIdx++) {
        sfz::upsample2xStage(buffer.getConstSpan(channelIdx), absl::MakeSpan(*tempBuffer));
        sfz::upsample4xStage(absl::MakeConstSpan(*tempBuffer), outputBuffer->getSpan(channelIdx));
    }
    return outputBuffer;
}

template <class T>
std::unique_ptr<sfz::AudioBuffer<T>> upsample8x(const sfz::AudioBuffer<T>& buffer)
{
    auto tempBuffer2x = std::make_unique<sfz::Buffer<T>>(buffer.getNumFrames() * 2);
    auto tempBuffer4x = std::make_unique<sfz::Buffer<T>>(buffer.getNumFrames() * 4);
    auto outputBuffer = std::make_unique<sfz::AudioBuffer<T>>(buffer.getNumChannels(), buffer.getNumFrames() * 8);
    for (int channelIdx = 0; channelIdx < buffer.getNumChannels(); channelIdx++) {
        sfz::upsample2xStage(buffer.getConstSpan(channelIdx), absl::MakeSpan(*tempBuffer2x));
        sfz::upsample4xStage(absl::MakeConstSpan(*tempBuffer2x), absl::MakeSpan(*tempBuffer4x));
        sfz::upsample8xStage(absl::MakeConstSpan(*tempBuffer4x), outputBuffer->getSpan(channelIdx));
    }
    return outputBuffer;
}

template <class T>
std::unique_ptr<sfz::AudioBuffer<T>> readFromFile(SndfileHandle& sndFile, uint32_t numFrames, sfz::Oversampling factor)
{
    auto baseBuffer = std::make_unique<sfz::AudioBuffer<T>>(sndFile.channels(), numFrames);
    if (sndFile.channels() == 1) {
        sndFile.readf(baseBuffer->channelWriter(0), numFrames);
    } else if (sndFile.channels() == 2) {
        auto tempReadBuffer = std::make_unique<sfz::AudioBuffer<T>>(1, 2 * numFrames);
        sndFile.readf(tempReadBuffer->channelWriter(0), numFrames);
        auto fileBuffer = std::make_unique<sfz::AudioBuffer<T>>(2, numFrames);
        sfz::readInterleaved<T>(tempReadBuffer->getSpan(0), baseBuffer->getSpan(0), baseBuffer->getSpan(1));
    }

    switch (factor) {
    case sfz::Oversampling::x1:
        return baseBuffer;
    case sfz::Oversampling::x2:
        return upsample2x(*baseBuffer);
    case sfz::Oversampling::x4:
        return upsample4x(*baseBuffer);
    case sfz::Oversampling::x8:
        return upsample8x(*baseBuffer);
    default:
        return {};
    }
}

absl::optional<sfz::FilePool::FileInformation> sfz::FilePool::getFileInformation(const std::string& filename) noexcept
{
    fs::path file { rootDirectory / filename };
    if (!fs::exists(file))
        return {};

    SndfileHandle sndFile(reinterpret_cast<const char*>(file.c_str()));
    if (sndFile.channels() != 1 && sndFile.channels() != 2) {
        DBG("Missing logic for " << sndFile.channels() << " channels, discarding sample " << filename);
        return {};
    }

    FileInformation returnedValue;
    returnedValue.end = static_cast<uint32_t>(sndFile.frames());
    returnedValue.sampleRate = static_cast<double>(sndFile.samplerate());
    returnedValue.numChannels = sndFile.channels();

    SF_INSTRUMENT instrumentInfo;
    sndFile.command(SFC_GET_INSTRUMENT, &instrumentInfo, sizeof(instrumentInfo));
    if (instrumentInfo.loop_count == 1) {
        returnedValue.loopBegin = instrumentInfo.loops[0].start;
        returnedValue.loopEnd = instrumentInfo.loops[0].end;
    }

    return returnedValue;
}

bool sfz::FilePool::preloadFile(const std::string& filename, uint32_t maxOffset) noexcept
{
    fs::path file { rootDirectory / filename };
    if (!fs::exists(file))
        return false;

    SndfileHandle sndFile(reinterpret_cast<const char*>(file.c_str()));
    if (sndFile.channels() != 1 && sndFile.channels() != 2)
        return false;

    // FIXME: Large offsets will require large preloading; is this OK in practice? Apparently sforzando does the same
    const uint32_t frames = sndFile.frames();
    const auto framesToLoad = [&]() {
        if (preloadSize == 0)
            return frames;
        else
            return min(frames, maxOffset + preloadSize);
    }();

    if (preloadedFiles.contains(filename)) {
        if (framesToLoad > preloadedFiles[filename].preloadedData->getNumFrames()) {
            preloadedFiles[filename].preloadedData = readFromFile<float>(sndFile, framesToLoad, oversamplingFactor);
        }
    } else {
        preloadedFiles.insert_or_assign(filename, { readFromFile<float>(sndFile, framesToLoad, oversamplingFactor), static_cast<float>(sndFile.samplerate() * oversamplingFactor) });
    }

    return true;
}

sfz::FilePromisePtr sfz::FilePool::getFilePromise(const std::string& filename) noexcept
{
    auto promise = std::make_shared<FilePromise>();
    const auto preloaded = preloadedFiles.find(filename);
    if (preloaded != preloadedFiles.end()) {
        promise->filename = preloaded->first;
        promise->preloadedData = preloaded->second.preloadedData;
        promise->sampleRate = preloaded->second.sampleRate;
        promise->oversamplingFactor = oversamplingFactor;
        promiseQueue.enqueue(promise);
    }
    return promise;
}

void sfz::FilePool::setPreloadSize(uint32_t preloadSize) noexcept
{
    // Update all the preloaded sizes
    for (auto& preloadedFile : preloadedFiles) {
        const auto numFrames = preloadedFile.second.preloadedData->getNumFrames();
        const uint32_t maxOffset = numFrames > this->preloadSize ? numFrames - this->preloadSize : 0;
        fs::path file { rootDirectory / std::string(preloadedFile.first) };
        SndfileHandle sndFile(reinterpret_cast<const char*>(file.c_str()));
        preloadedFile.second.preloadedData = readFromFile<float>(sndFile, preloadSize + maxOffset, oversamplingFactor);
    }
    this->preloadSize = preloadSize;
}

void sfz::FilePool::loadingThread() noexcept
{
    FilePromisePtr promise;
    while (!quitThread) {
        promisesToClean.clear();
        if (!promiseQueue.try_dequeue(promise)) {
            std::this_thread::sleep_for(0.1ms);
            continue;
        }

        fs::path file { rootDirectory / std::string(promise->filename) };
        SndfileHandle sndFile(reinterpret_cast<const char*>(file.c_str()));
        if (sndFile.error() != 0)
            continue;

        DBG("Loading file for " << promise->filename << " in the background");
        const uint32_t frames = sndFile.frames();
        promise->fileData = readFromFile<float>(sndFile, frames, oversamplingFactor);
        promise->dataReady = true;
        temporaryFilePromises.push_back(promise);
        promise.reset();
    }
}

void sfz::FilePool::clear()
{
    preloadedFiles.clear();
    temporaryFilePromises.clear();
    promisesToClean.clear();
}

void sfz::FilePool::cleanupPromises() noexcept
{
    if (temporaryFilePromises.empty())
        return;

    auto promise = temporaryFilePromises.begin();
    auto sentinel = temporaryFilePromises.end() - 1;
    while (promise != temporaryFilePromises.end()) {
        if (promise->use_count() == 1) {
            promisesToClean.push_back(*promise);
            std::iter_swap(promise, sentinel);
            sentinel--;
            temporaryFilePromises.pop_back();
        } else {
            promise++;
        }
    }
}

void sfz::FilePool::setOversamplingFactor(sfz::Oversampling factor) noexcept
{
    float samplerateChange { static_cast<float>(factor) / static_cast<float>(this->oversamplingFactor) };
    for (auto& preloadedFile : preloadedFiles) {
        const auto numFrames = preloadedFile.second.preloadedData->getNumFrames();
        const uint32_t maxOffset = numFrames > this->preloadSize ? numFrames - this->preloadSize : 0;
        fs::path file { rootDirectory / std::string(preloadedFile.first) };
        SndfileHandle sndFile(reinterpret_cast<const char*>(file.c_str()));
        preloadedFile.second.preloadedData = readFromFile<float>(sndFile, preloadSize + maxOffset, factor);
        preloadedFile.second.sampleRate *= samplerateChange;
    }

    this->oversamplingFactor = factor;
}

sfz::Oversampling sfz::FilePool::getOversamplingFactor() const noexcept
{
    return oversamplingFactor;
}

uint32_t sfz::FilePool::getPreloadSize() const noexcept
{
    return preloadSize;
}
