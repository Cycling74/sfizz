/* -*- mode: c++; -*- */
// SPDX-License-Identifier: BSD-2-Clause

// This code is part of the sfizz library and is licensed under a BSD 2-clause
// license. You should have receive a LICENSE.md file along with the code.
// If not, contact the sfizz maintainers at https://github.com/sfztools/sfizz

#include "Opcode.h"
#include "absl/strings/string_view.h"
#include "absl/strings/str_cat.h"
#include <string>

namespace sfz {

static std::string cleanUpOpcodeName(absl::string_view rawOpcode, OpcodeScope scope)
{
    std::string opcode { rawOpcode };

    /*!re2c
    re2c:flags:posix-captures = 1;
    re2c:define:YYCTYPE = char;
    re2c:yyfill:enable = 0;
    */
    /*!maxnmatch:re2c*/

    const char* YYCURSOR;
    const char* YYMARKER;
    const char* yypmatch[2 * YYMAXNMATCH];
    size_t yynmatch;

    /*!stags:re2c format = "const char* @@; "; */

    auto group = [&yypmatch](size_t i) -> absl::string_view {
        const char *beg = yypmatch[2 * i];
        const char *end = yypmatch[2 * i + 1];
        return absl::string_view(beg, end - beg);
    };

    /*!re2c
    digit = [0-9];
    number = digit+;
    any = [^\x00]*;
    END = [\x00];
    */

    //--------------------------------------------------------------------------

    YYCURSOR = opcode.c_str();

    /*!re2c

    (any) "_cc" (number) END {
        opcode = absl::StrCat(group(1), "_oncc", group(2));
        goto end_generic;
    }

    * {
        goto end_generic;
    }

    */

end_generic:
    /* end */;

    //--------------------------------------------------------------------------

    if (scope == kOpcodeScopeRegion) {

    YYCURSOR = opcode.c_str();

    /*!re2c

    lfoV1 = "amplfo"|"fillfo"|"pitchlfo";
    egV1 = "ampeg"|"fileg"|"pitcheg";
    eqV1 = "eq" number;
    lfoV2 = "lfo" number;

    (lfoV1) "_" ("depth"|"freq"|"fade") "cc" (number) END {
        opcode = absl::StrCat(group(1), "_", group(2), "_oncc", group(3));
        goto end_region;
    }
    (egV1) "_" ("delay"|"start"|"attack"|"hold"|"decay"|"sustain"|"release") "cc" (number) END {
        opcode = absl::StrCat(group(1), "_", group(2), "_oncc", group(3));
        goto end_region;
    }
    (eqV1) "_" ("bw"|"freq"|"gain") "cc" (number) END {
        opcode = absl::StrCat(group(1), "_", group(2), "_oncc", group(3));
        goto end_region;
    }
    (lfoV2) "_" ("wave"|"offset"|"ratio"|"scale") END {
        opcode = absl::StrCat(group(1), "_", group(2), "1");
        goto end_region;
    }

    "loop" ("mode"|"start"|"end") END {
        opcode = absl::StrCat("loop_", group(1));
        goto end_region;
    }
    "off" ("by"|"mode") END {
        opcode = absl::StrCat("off_", group(1));
        goto end_region;
    }
    "bend" ("up"|"down") END {
        opcode = absl::StrCat("bend_", group(1));
        goto end_region;
    }
    "filtype" END {
        opcode = absl::StrCat("fil1_type");
        goto end_region;
    }
    "fil" (number) "type" END {
        opcode = absl::StrCat("fil", group(1), "_type");
        goto end_region;
    }

    "polyphony_group" END {
        opcode = "group";
        goto end_region;
    }

    "gain" ("_" any)? END {
        opcode = absl::StrCat("volume", group(1));
        goto end_region;
    }
    "tune" ("_" any)? END {
        opcode = absl::StrCat("pitch", group(1));
        goto end_region;
    }

    "on_" ("hi"|"lo") "cc" (number) END {
        opcode = absl::StrCat("start_", group(1), "cc", group(2));
        goto end_region;
    }

    "fil_" (any) END {
        opcode = absl::StrCat("fil1_", group(1));
        goto end_region;
    }
    "cutoff" ("_" any)? END {
        opcode = absl::StrCat("cutoff1", group(1));
        goto end_region;
    }
    "resonance" ("_" any)? END {
        opcode = absl::StrCat("resonance1", group(1));
        goto end_region;
    }

    * {
        goto end_region;
    }

    */

end_region:
    /* end */;

    } // scope == kOpcodeScopeRegion

    //--------------------------------------------------------------------------

    #undef YYMAXNMATCH

    return opcode;
}

Opcode Opcode::cleanUp(OpcodeScope scope) const
{
    return Opcode(cleanUpOpcodeName(opcode, scope), value);
}

} // namespace sfz