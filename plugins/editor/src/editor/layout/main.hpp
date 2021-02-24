/* This file is generated by the layout maker tool. */
auto* const view__0 = createLogicalGroup(CRect(0, 0, 800, 475), -1, "", kCenterText, 14);
mainView = view__0;
auto* const view__1 = createBackground(CRect(190, 110, 790, 390), -1, "", kCenterText, 14);
view__0->addView(view__1);
enterTheme(darkTheme);
auto* const view__2 = createLogicalGroup(CRect(0, 0, 800, 110), -1, "", kCenterText, 14);
view__0->addView(view__2);
auto* const view__3 = createRoundedGroup(CRect(5, 4, 180, 105), -1, "", kCenterText, 14);
view__2->addView(view__3);
auto* const view__4 = createSfizzMainButton(CRect(30, 5, 150, 65), kTagFirstChangePanel+kPanelGeneral, "", kCenterText, 14);
view__3->addView(view__4);
auto* const view__5 = createHomeButton(CRect(44, 69, 69, 94), kTagFirstChangePanel+kPanelGeneral, "", kCenterText, 24);
view__3->addView(view__5);
auto* const view__6 = createCCButton(CRect(76, 69, 101, 94), kTagFirstChangePanel+kPanelControls, "", kCenterText, 24);
view__3->addView(view__6);
auto* const view__7 = createSettingsButton(CRect(107, 69, 132, 94), kTagFirstChangePanel+kPanelSettings, "", kCenterText, 24);
view__3->addView(view__7);
auto* const view__8 = createRoundedGroup(CRect(185, 5, 565, 105), -1, "", kCenterText, 14);
view__2->addView(view__8);
auto* const view__9 = createHLine(CRect(10, 36, 370, 41), -1, "", kCenterText, 14);
view__8->addView(view__9);
auto* const view__10 = createHLine(CRect(10, 68, 370, 73), -1, "", kCenterText, 14);
view__8->addView(view__10);
auto* const view__11 = createClickableLabel(CRect(10, 6, 260, 37), kTagLoadSfzFile, "DefaultInstrument.sfz", kLeftText, 20);
sfzFileLabel_ = view__11;
view__8->addView(view__11);
auto* const view__12 = createLabel(CRect(10, 39, 260, 69), -1, "Key switch:", kLeftText, 20);
view__8->addView(view__12);
auto* const view__13 = createLabel(CRect(10, 71, 70, 96), -1, "Voices:", kRightText, 12);
view__8->addView(view__13);
auto* const view__14 = createPreviousFileButton(CRect(295, 9, 320, 34), kTagPreviousSfzFile, "", kCenterText, 24);
view__8->addView(view__14);
auto* const view__15 = createNextFileButton(CRect(320, 9, 345, 34), kTagNextSfzFile, "", kCenterText, 24);
view__8->addView(view__15);
auto* const view__16 = createChevronDropDown(CRect(345, 9, 370, 34), kTagFileOperations, "", kCenterText, 24);
fileOperationsMenu_ = view__16;
view__8->addView(view__16);
auto* const view__17 = createLabel(CRect(75, 71, 125, 96), -1, "", kCenterText, 12);
infoVoicesLabel_ = view__17;
view__8->addView(view__17);
auto* const view__18 = createLabel(CRect(130, 71, 190, 96), -1, "Max:", kRightText, 12);
view__8->addView(view__18);
auto* const view__19 = createLabel(CRect(195, 71, 230, 96), -1, "", kCenterText, 12);
numVoicesLabel_ = view__19;
view__8->addView(view__19);
auto* const view__20 = createLabel(CRect(250, 71, 310, 96), -1, "Memory:", kRightText, 12);
view__8->addView(view__20);
auto* const view__21 = createLabel(CRect(315, 71, 365, 96), -1, "", kCenterText, 12);
memoryLabel_ = view__21;
view__8->addView(view__21);
auto* const view__22 = createChevronValueDropDown(CRect(230, 75, 250, 95), kTagSetNumVoices, "", kCenterText, 16);
numVoicesSlider_ = view__22;
view__8->addView(view__22);
auto* const view__23 = createRoundedGroup(CRect(570, 5, 795, 105), -1, "", kCenterText, 14);
view__2->addView(view__23);
auto* const view__24 = createKnob48(CRect(45, 15, 93, 63), -1, "", kCenterText, 14);
view__23->addView(view__24);
view__24->setVisible(false);
auto* const view__25 = createValueLabel(CRect(40, 65, 100, 70), -1, "Center", kCenterText, 12);
view__23->addView(view__25);
view__25->setVisible(false);
auto* const view__26 = createStyledKnob(CRect(110, 15, 158, 63), kTagSetVolume, "", kCenterText, 14);
volumeSlider_ = view__26;
view__23->addView(view__26);
auto* const view__27 = createValueLabel(CRect(105, 65, 165, 87), -1, "0.0 dB", kCenterText, 12);
volumeLabel_ = view__27;
view__23->addView(view__27);
auto* const view__28 = createVMeter(CRect(175, 15, 210, 70), -1, "", kCenterText, 14);
view__23->addView(view__28);
enterTheme(defaultTheme);
auto* const view__29 = createLogicalGroup(CRect(5, 110, 796, 395), -1, "", kCenterText, 14);
subPanels_[kPanelGeneral] = view__29;
view__0->addView(view__29);
view__29->setVisible(false);
auto* const view__30 = createRoundedGroup(CRect(0, 0, 175, 280), -1, "", kCenterText, 14);
view__29->addView(view__30);
auto* const view__31 = createLabel(CRect(15, 10, 75, 35), -1, "Curves:", kLeftText, 14);
view__30->addView(view__31);
auto* const view__32 = createLabel(CRect(15, 35, 75, 60), -1, "Masters:", kLeftText, 14);
view__30->addView(view__32);
auto* const view__33 = createLabel(CRect(15, 60, 75, 85), -1, "Groups:", kLeftText, 14);
view__30->addView(view__33);
auto* const view__34 = createLabel(CRect(15, 85, 75, 110), -1, "Regions:", kLeftText, 14);
view__30->addView(view__34);
auto* const view__35 = createLabel(CRect(15, 110, 75, 135), -1, "Samples:", kLeftText, 14);
view__30->addView(view__35);
auto* const view__36 = createLabel(CRect(115, 10, 155, 35), -1, "0", kCenterText, 14);
infoCurvesLabel_ = view__36;
view__30->addView(view__36);
auto* const view__37 = createLabel(CRect(115, 35, 155, 60), -1, "0", kCenterText, 14);
infoMastersLabel_ = view__37;
view__30->addView(view__37);
auto* const view__38 = createLabel(CRect(115, 60, 155, 85), -1, "0", kCenterText, 14);
infoGroupsLabel_ = view__38;
view__30->addView(view__38);
auto* const view__39 = createLabel(CRect(115, 85, 155, 110), -1, "0", kCenterText, 14);
infoRegionsLabel_ = view__39;
view__30->addView(view__39);
auto* const view__40 = createLabel(CRect(115, 110, 155, 135), -1, "0", kCenterText, 14);
infoSamplesLabel_ = view__40;
view__30->addView(view__40);
auto* const view__41 = createLogicalGroup(CRect(5, 110, 795, 395), -1, "", kCenterText, 14);
subPanels_[kPanelControls] = view__41;
view__0->addView(view__41);
view__41->setVisible(false);
auto* const view__42 = createRoundedGroup(CRect(0, 0, 790, 285), -1, "", kCenterText, 14);
view__41->addView(view__42);
auto* const view__43 = createControlsPanel(CRect(0, 0, 790, 285), -1, "", kCenterText, 14);
controlsPanel_ = view__43;
view__42->addView(view__43);
auto* const view__44 = createLogicalGroup(CRect(5, 109, 795, 425), -1, "", kCenterText, 14);
subPanels_[kPanelSettings] = view__44;
view__0->addView(view__44);
auto* const view__45 = createTitleGroup(CRect(290, 26, 495, 126), -1, "Engine", kCenterText, 12);
view__44->addView(view__45);
auto* const view__46 = createValueMenu(CRect(35, 60, 95, 85), kTagSetOversampling, "", kCenterText, 12);
oversamplingSlider_ = view__46;
view__45->addView(view__46);
auto* const view__47 = createValueLabel(CRect(25, 20, 105, 45), -1, "Oversampling", kCenterText, 12);
view__45->addView(view__47);
auto* const view__48 = createValueLabel(CRect(110, 20, 190, 45), -1, "Preload size", kCenterText, 12);
view__45->addView(view__48);
auto* const view__49 = createValueMenu(CRect(120, 60, 180, 85), kTagSetPreloadSize, "", kCenterText, 12);
preloadSizeSlider_ = view__49;
view__45->addView(view__49);
auto* const view__50 = createTitleGroup(CRect(170, 161, 585, 261), -1, "Tuning", kCenterText, 12);
view__44->addView(view__50);
auto* const view__51 = createValueLabel(CRect(155, 20, 235, 45), -1, "Root key", kCenterText, 12);
view__50->addView(view__51);
auto* const view__52 = createValueMenu(CRect(250, 60, 310, 85), kTagSetTuningFrequency, "", kCenterText, 12);
tuningFrequencySlider_ = view__52;
view__50->addView(view__52);
auto* const view__53 = createValueLabel(CRect(240, 20, 320, 45), -1, "Frequency", kCenterText, 12);
view__50->addView(view__53);
auto* const view__54 = createStyledKnob(CRect(340, 45, 388, 93), kTagSetStretchedTuning, "", kCenterText, 14);
stretchedTuningSlider_ = view__54;
view__50->addView(view__54);
auto* const view__55 = createValueLabel(CRect(325, 20, 405, 45), -1, "Stretch", kCenterText, 12);
view__50->addView(view__55);
auto* const view__56 = createValueLabel(CRect(20, 20, 120, 45), -1, "Scala file", kCenterText, 12);
view__50->addView(view__56);
auto* const view__57 = createValueButton(CRect(20, 60, 120, 85), kTagLoadScalaFile, "DefaultScale", kCenterText, 12);
scalaFileButton_ = view__57;
view__50->addView(view__57);
auto* const view__58 = createValueMenu(CRect(165, 60, 200, 85), kTagSetScalaRootKey, "", kCenterText, 12);
scalaRootKeySlider_ = view__58;
view__50->addView(view__58);
auto* const view__59 = createValueMenu(CRect(200, 60, 230, 85), kTagSetScalaRootKey, "", kCenterText, 12);
scalaRootOctaveSlider_ = view__59;
view__50->addView(view__59);
auto* const view__60 = createResetSomethingButton(CRect(120, 60, 145, 85), kTagResetScalaFile, "", kCenterText, 12);
scalaResetButton_ = view__60;
view__50->addView(view__60);
auto* const view__61 = createTitleGroup(CRect(615, 161, 754, 261), -1, "Files", kCenterText, 12);
userFilesGroup_ = view__61;
view__44->addView(view__61);
auto* const view__62 = createValueLabel(CRect(20, 20, 120, 45), -1, "User SFZ folder", kCenterText, 12);
view__61->addView(view__62);
auto* const view__63 = createValueButton(CRect(20, 60, 120, 85), kTagChooseUserFilesDir, "DefaultPath", kCenterText, 12);
userFilesDirButton_ = view__63;
view__61->addView(view__63);
auto* const view__64 = createPiano(CRect(5, 400, 795, 470), -1, "", kCenterText, 12);
piano_ = view__64;
view__0->addView(view__64);