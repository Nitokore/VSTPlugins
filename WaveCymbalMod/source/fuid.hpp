// (c) 2019 Takamitsu Endo
//
// This file is part of WaveCymbalMod.
//
// WaveCymbalMod is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// WaveCymbalMod is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with WaveCymbalMod.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include "pluginterfaces/base/funknown.h"

namespace Steinberg {
namespace Synth {

// https://www.guidgenerator.com/
static const FUID ProcessorUID(0x63D0D714, 0x107E4FBA, 0x9092DDE6, 0x99A64043);
static const FUID ControllerUID(0x5285C8E2, 0x3D0E473E, 0x87100B54, 0xBBB0471B);

} // namespace Synth
} // namespace Steinberg
