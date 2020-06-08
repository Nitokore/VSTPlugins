// (c) 2019-2020 Takamitsu Endo
//
// This file is part of EnvelopedSineDebug.
//
// EnvelopedSineDebug is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// EnvelopedSineDebug is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with EnvelopedSineDebug.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include "pluginterfaces/base/funknown.h"

namespace Steinberg {
namespace Synth {

// https://www.guidgenerator.com/
static const FUID ProcessorUID(0x752E10A2, 0x58F8412A, 0x8CB78348, 0x39220D5A);
static const FUID ControllerUID(0x14149A18, 0x9830480A, 0x91139299, 0xE76DF293);

} // namespace Synth
} // namespace Steinberg
