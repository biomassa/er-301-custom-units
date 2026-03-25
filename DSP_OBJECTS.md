# ER-301 Core DSP Objects (Detailed)

This document provides detailed information on all `libcore` DSP objects available to Lua via SWIG, including their expected datatypes, defaults, and all possible Option macros.

## Core Data Types & Controls
- **`Inlet` / `Outlet`**: Audio or Control Voltage signals (floating-point streams at `FRAMELENGTH` boundaries). In Lua, these are usually connected using `self:tie(src, dest)` or `connect(src, ...)`.
- **`Parameter`**: Continuous floating-point state variables (e.g. `0.0f` to `1.0f`, or direct Hz/Seconds). Often assigned to `GainBias`, `Pitch`, or `Fader` UI controls in Lua.
- **`Option`**: Discrete integer-based state choices (e.g., `0`, `1`, `2`). Often assigned to `List` (dropdown menus) or `Toggle` (checkboxes) UI controls in Lua.

## Math / Arithmetic

### `Constant`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Value` (Default: `0.0f`)

### `ConstantOffset`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Offset` (Default: `0.0f`)

### `ConstantGain`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Gain` (Default: `1.0f`)

### `Gain`
- **Inlets (Audio/CV)**: `In`, `Gain`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `BaseGain` (Default: `1.0f`)

### `GainBias`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Gain` (Default: `1.0f`)
  - `Bias` (Default: `0.0f`)

### `Sum`
- **Inlets (Audio/CV)**: `Left`, `Right`
- **Outlets (Audio/CV)**: `Out`

### `Multiply`
- **Inlets (Audio/CV)**: `Left`, `Right`
- **Outlets (Audio/CV)**: `Out`

### `Spread`
- **Inlets (Audio/CV)**: `In`, `Spread`
- **Outlets (Audio/CV)**: `Left Out`, `Right Out`

### `RationalMultiply`
- **Inlets (Audio/CV)**: `In`, `Numerator`, `Divisor`
- **Outlets (Audio/CV)**: `Out`

## Signal Shaping

### `Clipper`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`

### `Fold`
- **Inlets (Audio/CV)**: `In`, `Threshold`, `Upper Gain`, `Lower Gain`
- **Outlets (Audio/CV)**: `Out`

### `Rectify`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`
- **Options (Integer / List / Toggle)**:
  - `Type` (Default: `RECTIFY_POSITIVEHALF`) -> Possible Options: `RECTIFY_FULL`, `RECTIFY_NEGATIVEHALF`, `RECTIFY_POSITIVEHALF`

### `Limiter`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`
- **Options (Integer / List / Toggle)**:
  - `Type` (Default: `LIMITER_CUBIC`) -> Possible Options: `LIMITER_CUBIC`, `LIMITER_HARD`, `LIMITER_INVSQRT`

### `SnapToZero`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`

### `GridQuantizer`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Pre-Scale` (Default: `1.0f`)
  - `Levels` (Default: `12.0f`)
  - `Post-Scale` (Default: `1.0f`)

## Mixing

### `Mixer`
- **Inlets (Audio/CV)**: Dynamic (N inputs)
- **Outlets (Audio/CV)**: `Out`

### `CrossFade`
- **Inlets (Audio/CV)**: `A`, `B`, `Fade`
- **Outlets (Audio/CV)**: `Out`

### `MonoPanner`
- **Inlets (Audio/CV)**: `In`, `Pan`
- **Outlets (Audio/CV)**: `Left`, `Right`

### `StereoPanner`
- **Inlets (Audio/CV)**: `Left In`, `Right In`, `Pan`
- **Outlets (Audio/CV)**: `Left Out`, `Right Out`

### `StereoCrossFade`
- **Inlets (Audio/CV)**: `Left A`, `Left B`, `Right A`, `Right B`, `Fade`
- **Outlets (Audio/CV)**: `Left Out`, `Right Out`

### `StereoToMono`
- **Inlets (Audio/CV)**: `Left In`, `Right In`
- **Outlets (Audio/CV)**: `Out`
- **Options (Integer / List / Toggle)**:
  - `Routing` (Default: `CHOICE_LEFT`) -> Possible Options: `CHOICE_LEFT`, `CHOICE_BOTH`, `CHOICE_RIGHT`

## Envelopes

### `ADSR`
- **Inlets (Audio/CV)**: `Gate`, `Attack`, `Decay`, `Sustain`, `Release`
- **Outlets (Audio/CV)**: `Out`

### `SkewedSineEnvelope`
- **Inlets (Audio/CV)**: `Trigger`, `Level`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Skew` (Default: `0.0f`)
  - `Duration` (Default: `0.1f`)

### `EnvelopeFollower`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Attack Time` (Default: `0.001f`)
  - `Release Time` (Default: `0.010f`)

### `PopReducer`
- **Inlets (Audio/CV)**: `In`, `Trigger`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Time` (Default: `0.0f`)

## Oscillators

### `SineOscillator`
- **Inlets (Audio/CV)**: `V/Oct`, `Sync`, `Fundamental`, `Phase`, `Feedback`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Internal Phase` (Default: `0.0f`)

### `TriangleOscillator`
- **Inlets (Audio/CV)**: `V/Oct`, `Sync`, `Fundamental`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Phase` (Default: `0.0f`)

### `SawtoothOscillator`
- **Inlets (Audio/CV)**: `V/Oct`, `Sync`, `Fundamental`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Phase` (Default: `0.0f`)

### `SingleCycle`
- **Inlets (Audio/CV)**: `V/Oct`, `Sync`, `Fundamental`, `Phase`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Internal Phase` (Default: `0.0f`)

## Filters

### `LadderFilter`
- **Inlets (Audio/CV)**: `In`, `V/Oct`, `Resonance`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Fundamental` (Default: `880.0f`)

### `StereoLadderFilter`
- **Inlets (Audio/CV)**: `Left In`, `Right In`, `V/Oct`, `Resonance`, `Fundamental`
- **Outlets (Audio/CV)**: `Left Out`, `Right Out`

### `StereoLadderHPF`
- **Inlets (Audio/CV)**: `Left In`, `Right In`, `V/Oct`, `Resonance`, `Fundamental`
- **Outlets (Audio/CV)**: `Left Out`, `Right Out`

### `StereoFixedHPF`
- **Inlets (Audio/CV)**: `Left In`, `Right In`
- **Outlets (Audio/CV)**: `Left Out`, `Right Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Cutoff` (Default: `27.5`)

### `FixedFilter`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`

### `DeadbandFilter`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Threshold` (Default: `0.1f`)

### `AllPassFilter`
- **Inlets (Audio/CV)**: `In`, `Frequency`, `Feedback`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Order` (Default: varies)

### `Equalizer3`
- **Inlets (Audio/CV)**: `In`, `Low Gain`, `Mid Gain`, `High Gain`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Low Freq` (Default: `880.0f`)
  - `High Freq` (Default: `5000.0f`)

### `SlewLimiter`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`
- **Options (Integer / List / Toggle)**:
  - `Direction` (Default: `CHOICE_BOTH`) -> Possible Options: `CHOICE_BOTH`, `CHOICE_DOWN`, `CHOICE_UP`
- **Parameters (Float / GainBias / Pitch)**:
  - `Time` (Default: `0.0f`)

### `Energy`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`

## Delays

### `Delay`
- **Inlets (Audio/CV)**: `Left In`, `Right In`, `Feedback`
- **Outlets (Audio/CV)**: `Left Out`, `Right Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Left Delay` (Default: `0.0f`)
  - `Right Delay` (Default: `0.0f`)
  - `Spread` (Default: `0`)

### `MicroDelay`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Delay` (Default: `0.0f`)

### `DopplerDelay`
- **Inlets (Audio/CV)**: `In`, `Feedback`, `Delay`
- **Outlets (Audio/CV)**: `Out`

## Reverb / Convolution

### `Freeverb`
- **Inlets (Audio/CV)**: `Left In`, `Right In`
- **Outlets (Audio/CV)**: `Left Out`, `Right Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Size` (Default: `initialroom`)
  - `Damp` (Default: `initialdamp`)
  - `Width` (Default: `initialwidth`)

### `MonoConvolution`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`

### `StereoConvolution`
- **Inlets (Audio/CV)**: `Left In`, `Right In`
- **Outlets (Audio/CV)**: `Left Out`, `Right Out`

### `OverlapAddTest`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Hop` (Default: `1`)

## Timing / Clock

### `Clock`
- **Inlets (Audio/CV)**: `Sync`
- **Outlets (Audio/CV)**: `Out`
- **Options (Integer / List / Toggle)**:
  - `Rational` (Default: `1`)
- **Parameters (Float / GainBias / Pitch)**:
  - `Divider` (Default: `1`)
  - `Multiplier` (Default: `1`)
  - `Pulse Width` (Default: `0.5`)
  - `Period` (Default: `0.5`)
  - `Tempo` (Default: `120`)
  - `Frequency` (Default: `2`)

### `TapTempo`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: (none - parameters only)
- **Parameters (Float / GainBias / Pitch)**:
  - `Base Period`
  - `Base Frequency`
  - `Derived Period`
  - `Derived Frequency`
  - `Multiplier` (Default: `1`)
  - `Divider` (Default: `1`)
- **Options (Integer / List / Toggle)**:
  - `Rational` (Default: `1`)

### `Counter`
- **Inlets (Audio/CV)**: `In`, `Reset`
- **Outlets (Audio/CV)**: `Out`
- **Options (Integer / List / Toggle)**:
  - `Wrap` (Default: `CHOICE_YES`) -> Possible Options: `CHOICE_BOTH`, `CHOICE_DOWN`, `CHOICE_LEFT`, `CHOICE_NO`, `CHOICE_RIGHT`, `CHOICE_UNKNOWN`, `CHOICE_UP`, `CHOICE_YES`
  - `Processing Rate` (Default: `PER_FRAME`) -> Possible Options: `PER_FRAME`, `PER_SAMPLE`
- **Parameters (Float / GainBias / Pitch)**:
  - `Step Size` (Default: `0.0f`)
  - `Start` (Default: `0.0f`)
  - `Finish` (Default: `0.0f`)
  - `Gain` (Default: `1.0f`)
  - `Bias` (Default: `0.0f`)
  - `Value` (Default: `0.0f`)

### `RoundRobin`
- **Inlets (Audio/CV)**: `Next`, `In`
- **Outlets (Audio/CV)**: (sequential outputs based on mode)
- **Options (Integer / List / Toggle)**:
  - `Processing Rate` (Default: `PER_FRAME`) -> Possible Options: `PER_FRAME`, `PER_SAMPLE`
  - `Active Output` (Default: `0`)

### `QuantizeToClock`
- **Inlets (Audio/CV)**: `In`, `Clock`
- **Outlets (Audio/CV)**: `Out`

### `ZeroCrossingDetector`
- **Inlets (Audio/CV)**: `Gate In`, `Audio In`
- **Outlets (Audio/CV)**: `Gate Out`

### `TrackAndHold`
- **Inlets (Audio/CV)**: `In`, `Track`
- **Outlets (Audio/CV)**: `Out`
- **Options (Integer / List / Toggle)**:
  - `Flavor` (Default: `TRACKNHOLD_CHOICE_HIGH`) -> Possible Options: `TRACKNHOLD_CHOICE_HIGH`, `TRACKNHOLD_CHOICE_LOW`, `TRACKNHOLD_CHOICE_MINMAX`
- **Parameters (Float / GainBias / Pitch)**:
  - `Value` (Default: `0.0f`)

### `BumpMap`
- **Inlets (Audio/CV)**: `In`, `Phase`
- **Outlets (Audio/CV)**: `Out`
- **Options (Integer / List / Toggle)**:
  - `Interpolation` (Default: `INTERPOLATION_LINEAR`) -> Possible Options: `INTERPOLATION_LINEAR`, `INTERPOLATION_NONE`, `INTERPOLATION_QUADRATIC`
- **Parameters (Float / GainBias / Pitch)**:
  - `Center` (Default: `0.0f`)
  - `Width` (Default: `0.1f`)
  - `Height` (Default: `0.5f`)
  - `Fade` (Default: `0.25f`)

## Noise

### `WhiteNoise`
- **Outlets (Audio/CV)**: `Out`

### `PinkNoise`
- **Outlets (Audio/CV)**: `Out`

### `VelvetNoise`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Rate` (Default: `1000.0f`)

## Looping / Tape

### `Looper`
- **Inlets (Audio/CV)**: `In`
- **Options (Integer / List / Toggle)**:
  - `Rational` (Default: `1`)
- **Parameters (Float / GainBias / Pitch)**:
  - `Base Period`
  - `Base Frequency`
  - `Derived Period`
  - `Derived Frequency`
  - `Multiplier` (Default: `1`)
  - `Divider` (Default: `1`)

### `RawHead`
- **Inlets (Audio/CV)**: `Trigger`
- **Outlets (Audio/CV)**: `Left Out`, `Right Out`
- **Options (Integer / List / Toggle)**:
  - `Routing` (Default: `CHOICE_LEFT`) -> Possible Options: `CHOICE_BOTH`, `CHOICE_DOWN`, `CHOICE_LEFT`, `CHOICE_RIGHT`, `CHOICE_UP`, `CHOICE_YES`
- **Parameters (Float / GainBias / Pitch)**:
  - `Speed` (Default: `1`)

### `LoopHead`
- **Inlets (Audio/CV)**: `Sync`, `Speed`
- **Outlets (Audio/CV)**: `Left Out`, `Right Out`
- **Options (Integer / List / Toggle)**:
  - `Interpolation` (Default: `INTERPOLATION_QUADRATIC`) -> Possible Options: `INTERPOLATION_LINEAR`, `INTERPOLATION_NONE`, `INTERPOLATION_QUADRATIC`
  - `Routing` (Default: `CHOICE_LEFT`) -> Possible Options: `CHOICE_BOTH`, `CHOICE_DOWN`, `CHOICE_LEFT`, `CHOICE_RIGHT`, `CHOICE_UP`, `CHOICE_YES`
- **Parameters (Float / GainBias / Pitch)**:
  - `Loop Start` (Default: `0.0f`)
  - `Loop Length` (Default: `1.0f`)
  - `Fade Time` (Default: `0.01f`)
  - `Maximum Loop Duration` (Default: `5.0f`)
  - `Actual Loop Duration` (Default: `0.0f`)

### `VariSpeedHead`
- **Inlets (Audio/CV)**: `Trigger`, `Speed`
- **Outlets (Audio/CV)**: `Left Out`, `Right Out`
- **Options (Integer / List / Toggle)**:
  - `Routing` (Default: `CHOICE_LEFT`) -> Possible Options: `CHOICE_BOTH`, `CHOICE_DOWN`, `CHOICE_LEFT`, `CHOICE_RIGHT`, `CHOICE_UP`, `CHOICE_YES`
  - `Interpolation` (Default: `INTERPOLATION_QUADRATIC`) -> Possible Options: `INTERPOLATION_LINEAR`, `INTERPOLATION_NONE`, `INTERPOLATION_QUADRATIC`

### `RecordHead`
- (Used internally for recording)

### `PedalLooper`
- **Inlets (Audio/CV)**: `Left In`, `Right In`, `Record`, `Stop`, `Undo`
- **Outlets (Audio/CV)**: `Left Out`, `Right Out`
- **Options (Integer / List / Toggle)**:
  - `After Record` (Default: `0`)
  - `Pass Input` (Default: `0b11111`)
- **Parameters (Float / GainBias / Pitch)**:
  - `Feedback` (Default: `1.0f`)

### `FeedbackLooper`
- **Inlets (Audio/CV)**: `Reset`, `Punch`, `Engage`, `Feedback`, `Left In`, `Right In`
- **Outlets (Audio/CV)**: `Left Out`, `Right Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Start` (Default: `0.0f`)
  - `Fade Time` (Default: `0.005f`)

### `DubLooper`
- **Inlets (Audio/CV)**: `Reset`, `Punch`, `Engage`, `Dub`, `Left In`, `Right In`
- **Outlets (Audio/CV)**: `Left Out`, `Right Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Start` (Default: `0.0f`)
  - `Fade Time` (Default: `0.005f`)

## Granular

### `GranularHead`
- **Inlets (Audio/CV)**: `Trigger`, `Speed`
- **Outlets (Audio/CV)**: `Left Out`, `Right Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Duration` (Default: `0.0f`)
  - `Start` (Default: `0.0f`)
  - `Gain` (Default: `0.0f`)
  - `Pan` (Default: `0.0f`)
  - `Squash` (Default: `0.0f`)

### `GrainStretch`
- **Inlets (Audio/CV)**: `Trigger`, `Speed`
- **Outlets (Audio/CV)**: `Left Out`, `Right Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Grain Duration` (Default: `0.1f`)
  - `Grain Pitch` (Default: `0.0f`)
  - `Grain Jitter` (Default: `0.0f`)

### `MonoGrainDelay`
- **Inlets (Audio/CV)**: `In`, `Delay`, `Speed`
- **Outlets (Audio/CV)**: `Out`

## Pitch

### `VoltPerOctave`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`

### `PitchCircle`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Pre-Transpose` (Default: `0.0f`)
  - `Post-Transpose` (Default: `0.0f`)

### `ScaleQuantizer`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Pre-Transpose` (Default: `0.0f`) - in cents
  - `Post-Transpose` (Default: `0.0f`) - in cents

## Control / Logic

### `Comparator`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Threshold` (Default: `0.1f`)
  - `Hysteresis` (Default: `0.03f`)
  - `Last Edge Count` (Default: `0`)
  - `Last Level Count` (Default: `0`)
- **Options (Integer / List / Toggle)**:
  - `State` (Default: `0`)
  - `Mode` (Default: `COMPARATOR_TRIGGER_ON_RISE`) -> Possible Options: `COMPARATOR_TOGGLE`, `COMPARATOR_GATE`, `COMPARATOR_TRIGGER_ON_RISE`, `COMPARATOR_TRIGGER_ON_FALL`

### `ParameterAdapter`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Out` (Default: `0.0f`)
  - `Gain` (Default: `1.0f`)
  - `Bias` (Default: `0.0f`)
  - `Min` (Default: varies)
  - `Max` (Default: varies)
  - `Center` (Default: varies)

### `ParamSetMorph`
- **Parameters (Float / GainBias / Pitch)**:
  - `Weight` (Default: `0.0f`)

## Measurement / Monitoring

### `Probe`
- **Inlets (Audio/CV)**: `In`
- (No outlets - purely for inspection/debugging)

### `Monitor`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`
- (Pass-through with measurement capability)

### `MinMax`
- **Inlets (Audio/CV)**: `In`
- **Parameters (Float / GainBias / Pitch)**:
  - `Min` (Default: varies)
  - `Max` (Default: varies)
  - `Center` (Default: varies)

### `LoudnessProbe`
- **Inlets (Audio/CV)**: `In`
- (RMS loudness measurement)

### `TriggerProbe`
- **Inlets (Audio/CV)**: `In`
- **Parameters (Float / GainBias / Pitch)**:
  - `Threshold` (Default: `0.1f`)
- **Options (Integer / List / Toggle)**:
  - `State` (Default: `0`)

### `Stress`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Load` (Default: varies)

### `FifoProbe`
- **Inlets (Audio/CV)**: `In`
- (FIFO buffer probe for debugging)

### `CaptureProbe`
- **Inlets (Audio/CV)**: `In`
- (Sample capture probe for debugging)

## Common Constants / Choices

These are used with `Option` parameters:
- `CHOICE_LEFT`, `CHOICE_RIGHT`, `CHOICE_BOTH`, `CHOICE_UP`, `CHOICE_DOWN`
- `CHOICE_YES`, `CHOICE_NO`, `CHOICE_UNKNOWN`
- `PER_FRAME`, `PER_SAMPLE`
- `INTERPOLATION_LINEAR`, `INTERPOLATION_QUADRATIC`, `INTERPOLATION_NONE`
- `RECTIFY_FULL`, `RECTIFY_POSITIVEHALF`, `RECTIFY_NEGATIVEHALF`
- `LIMITER_CUBIC`, `LIMITER_HARD`, `LIMITER_INVSQRT`
