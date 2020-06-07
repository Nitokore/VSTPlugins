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

#include "logger.hpp"

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>

std::ofstream Logger::ofs;

inline fs::path getLogDir()
{
#ifdef _WIN32
  const char *appdataDir = std::getenv("AppData");
  if (appdataDir != nullptr) return fs::path(appdataDir);

  std::cerr << "%AppData% is empty.\n";
#else
  const char *home = std::getenv("HOME");
  if (home != nullptr) return fs::path(home);

  std::cerr << "$HOME is empty.\n";
#endif
  return fs::path("");
}

void Logger::open()
{
  auto logPath = getLogDir();

  if (logPath == fs::path("")) {
    std::cerr << "Failed to get log directory.\n";
    return;
  }

  logPath /= fs::path("UhhyouDebug.log");

  if (ofs.is_open()) ofs.close();
  ofs.open(logPath, std::ios::app);
}

void Logger::close()
{
  if (!ofs.is_open()) return;
  ofs << "\n";
  ofs.close();
}

void Logger::write(std::string log)
{
  if (!ofs.is_open()) return;

  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::system_clock::now().time_since_epoch());
  ofs << ms.count() << ": " << log << "\n";
}
