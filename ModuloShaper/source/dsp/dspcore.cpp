// (c) 2020 Takamitsu Endo
//
// This file is part of ModuloShaper.
//
// ModuloShaper is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// ModuloShaper is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with ModuloShaper.  If not, see <https://www.gnu.org/licenses/>.

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
  for (auto &shaper : shaperNaive) shaper.reset();
  for (auto &shaper : shaperBlep) shaper.reset();
  for (auto &lp : lowpass) lp.reset();
  limiter.reset();
  startup();
}

void DSPCORE_NAME::startup() {}

uint32_t DSPCORE_NAME::getLatency()
{
  auto latency = activateLimiter ? limiter.latency() : 0;
  if (shaperType == 2) // 4 point PolyBLEP residual.
    return 4 + latency;
  else if (shaperType == 3) // 8 point PolyBLEP residual.
    return 8 + latency;
  return latency;
}

void DSPCORE_NAME::setParameters()
{
  using ID = ParameterID::ID;
  auto &pv = param.value;

  SmootherCommon<float>::setTime(pv[ID::smoothness]->getFloat());

  interpInputGain.push(pv[ID::inputGain]->getFloat());
  interpClipGain.push(pv[ID::clipGain]->getFloat());
  interpOutputGain.push(pv[ID::outputGain]->getFloat());
  interpAdd.push(pv[ID::add]->getFloat() * pv[ID::moreAdd]->getFloat());
  interpMul.push(pv[ID::mul]->getFloat() * pv[ID::moreMul]->getFloat());
  interpCutoff.push(pv[ID::lowpassCutoff]->getFloat());

  shaperType = pv[ID::type]->getInt();
  activateLowpass = pv[ID::lowpass]->getInt();

  bool hardclip = pv[ID::hardclip]->getInt();
  for (auto &shaper : shaperNaive) shaper.hardclip = hardclip;
  for (auto &shaper : shaperBlep) shaper.hardclip = hardclip;

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
    auto inGain = interpInputGain.process();
    auto clipGain = interpClipGain.process();
    auto outGain = interpOutputGain.process();
    auto add = interpAdd.process();
    auto mul = interpMul.process();
    auto cutoff = interpCutoff.process();

    if (mul > 1.0f) clipGain /= mul;

    if (activateLowpass) {
      lowpass[0].setCutoff(sampleRate, cutoff);
      lowpass[1].setCutoff(sampleRate, cutoff);
      frame[0] = lowpass[0].process(in0[i]);
      frame[1] = lowpass[1].process(in1[i]);
    } else {
      frame[0] = in0[i];
      frame[1] = in1[i];
    }

    switch (shaperType) {
      case 0: // Naive.
        shaperNaive[0].add = add;
        shaperNaive[1].add = add;
        shaperNaive[0].mul = mul;
        shaperNaive[1].mul = mul;

        frame[0] = clipGain * shaperNaive[0].process(inGain * frame[0]);
        frame[1] = clipGain * shaperNaive[1].process(inGain * frame[1]);
        break;

      case 1: // Naive 16x oversampling.
        shaperNaive[0].add = add;
        shaperNaive[1].add = add;
        shaperNaive[0].mul = mul;
        shaperNaive[1].mul = mul;

        frame[0] = clipGain * shaperNaive[0].process16x(inGain * frame[0]);
        frame[1] = clipGain * shaperNaive[1].process16x(inGain * frame[1]);
        break;

      case 2: // 4 point PolyBLEP residual.
        shaperBlep[0].add = add;
        shaperBlep[1].add = add;
        shaperBlep[0].mul = mul;
        shaperBlep[1].mul = mul;

        frame[0] = clipGain * shaperBlep[0].process4(inGain * frame[0]);
        frame[1] = clipGain * shaperBlep[1].process4(inGain * frame[1]);
        break;

      case 3: // 8 point PolyBLEP residual.
        shaperBlep[0].add = add;
        shaperBlep[1].add = add;
        shaperBlep[0].mul = mul;
        shaperBlep[1].mul = mul;

        frame[0] = clipGain * shaperBlep[0].process8(inGain * frame[0]);
        frame[1] = clipGain * shaperBlep[1].process8(inGain * frame[1]);
        break;
    }

    frame[0] *= outGain;
    frame[1] *= outGain;

    if (activateLimiter) frame = limiter.process(frame);

    out0[i] = frame[0];
    out1[i] = frame[1];
  }
}
