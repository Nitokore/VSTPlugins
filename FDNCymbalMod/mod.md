# FDNCymbal Modification
## Output for each processing section
Channel mapping:
0. Oscillator in/out.
1. FDN in/out.
2. Allpass 1st stage in/out.
3. Allpass 2nd stage in/out.
4. Tremolo in/out.

## Changes
- Added `Pulse` parameter to control pulse gain.
- Added `Velvet` parameter to control velvet noise gain.
- Changed `ToneMix` to `Tone`. Value range is increased from [0, 0.002] to [0, 0.02].
- Changed algorithm to allocate sine frequency.
- Fixed velvet noise oscillator to randomize interval.
