// (c) 2019 Takamitsu Endo
//
// This file is part of FDNCymbalMod.
//
// FDNCymbalMod is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// FDNCymbalMod is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with FDNCymbalMod.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include "pluginterfaces/base/funknown.h"

namespace Steinberg {
namespace Synth {

// https://www.guidgenerator.com/
static const FUID ProcessorUID(0x62babd68, 0x3b2e49f5, 0xa5a6e61c, 0x35183323);
static const FUID ControllerUID(0x8c93a0c2, 0xe8bd401f, 0x8d4c6feb, 0x42b5ec15);

} // namespace Synth
} // namespace Steinberg
