// (c) 2020 Takamitsu Endo
//
// This file is part of OddPowShaper.
//
// OddPowShaper is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// OddPowShaper is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OddPowShaper.  If not, see <https://www.gnu.org/licenses/>.

#include "dspcore.hpp"

#include "../../../lib/vcl/vectormath_exp.h"

#include <algorithm>
#include <numeric>

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

  startup();
}

void DSPCORE_NAME::reset()
{
  for (auto &shpr : shaper) shpr.reset();
  limiter.reset();
  startup();
}

void DSPCORE_NAME::startup() {}

uint32_t DSPCORE_NAME::getLatency() { return activateLimiter ? limiter.latency() : 0; }

void DSPCORE_NAME::setParameters()
{
  using ID = ParameterID::ID;
  auto &pv = param.value;

  SmootherCommon<float>::setTime(pv[ID::smoothness]->getFloat());

  interpDrive.push(pv[ID::drive]->getFloat() * pv[ID::boost]->getFloat());
  interpOutputGain.push(pv[ID::outputGain]->getFloat());

  oversample = pv[ID::oversample]->getInt();
  for (auto &shpr : shaper) {
    shpr.flip = pv[ID::flip]->getInt();
    shpr.inverse = pv[ID::inverse]->getInt();
    shpr.order = pv[ID::order]->getInt();
  }

  activateLimiter = pv[ID::limiter]->getInt();
  limiter.prepare(
    sampleRate, pv[ID::limiterAttack]->getFloat(), pv[ID::limiterRelease]->getFloat(),
    pv[ID::limiterThreshold]->getFloat());
}

void DSPCORE_NAME::process(
  const size_t length, const float *in0, const float *in1, float *out0, float *out1)
{
  SmootherCommon<float>::setBufferSize(length);

  std::array<float, 2> frame;
  for (uint32_t i = 0; i < length; ++i) {
    auto drive = interpDrive.process();
    auto outGain = interpOutputGain.process();

    frame[0] = in0[i];
    frame[1] = in1[i];

    shaper[0].drive = drive;
    shaper[1].drive = drive;

    if (oversample) {
      frame[0] = outGain * shaper[0].process16(frame[0]);
      frame[1] = outGain * shaper[1].process16(frame[1]);
    } else {
      frame[0] = outGain * shaper[0].process(frame[0]);
      frame[1] = outGain * shaper[1].process(frame[1]);
    }

    if (activateLimiter) frame = limiter.process(frame);

    out0[i] = frame[0];
    out1[i] = frame[1];
  }
}
