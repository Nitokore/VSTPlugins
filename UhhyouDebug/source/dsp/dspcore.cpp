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

#include "dspcore.hpp"

#if INSTRSET >= 10
#define DSPCORE_NAME DSPCore_AVX512
#elif INSTRSET >= 8
#define DSPCORE_NAME DSPCore_AVX2
#elif INSTRSET >= 7
#define DSPCORE_NAME DSPCore_AVX
#else
#error Unsupported instruction set
#endif

void DSPCORE_NAME::setup(double sampleRate)
{
  this->sampleRate = sampleRate;

  SmootherCommon<float>::setSampleRate(sampleRate);
  SmootherCommon<float>::setTime(0.2f);

  reset();
}

inline std::array<float, 2> calcOffset(float offset, float mul)
{
  if (offset >= 0) return {1.0f, 1.0f - mul * offset};
  return {1.0f + mul * offset, 1.0f};
}

void DSPCORE_NAME::reset() { using ID = ParameterID::ID; }

void DSPCORE_NAME::startup() {}

void DSPCORE_NAME::setParameters(float tempo) { using ID = ParameterID::ID; }

void DSPCORE_NAME::process(
  const size_t length, const float *in0, const float *in1, float *out0, float *out1)
{
  SmootherCommon<float>::setBufferSize(length);

  for (size_t i = 0; i < length; ++i) {
    for (size_t idx = 0; idx < nestingDepth; ++idx) {
      out0[i] = 0.0f;
      out1[i] = 0.0f;
    }
  }
}
