// (c) 2020 Takamitsu Endo
//
// This file is part of UhhyouDebug.
//
// UhhyouDebug is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// UhhyouDebug is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with UhhyouDebug.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include "../../../lib/ghc/fs_std.hpp"

#include <string>

class Logger {
public:
  static void open();
  static void close();
  static void write(std::string log);

private:
  static std::ofstream ofs;
};
