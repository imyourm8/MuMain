# ADR 0001: Preserve 25 FPS Reference Timing

**Status:** Accepted

**Date:** 2026-09-02

## Context

MU animation speeds, movement offsets, and related effects were authored
against a 25 FPS reference rate. Runtime frame rates vary by platform, GPU,
VSync mode, and scene complexity.

Advancing those values once per rendered frame without scaling changes game
speed. Limiting frame rate may hide that defect, but does not make the logic
correct.

## Decision

- Keep `REFERENCE_FPS` at `25.0`.
- Compute `FPS_ANIMATION_FACTOR` from elapsed wall-clock time as
  `clamp(REFERENCE_FPS / FPS, 0, 1)`.
- Multiply additive per-frame animation, movement, physics, and effect changes
  by `FPS_ANIMATION_FACTOR`.
- Apply exponential changes as `pow(base, FPS_ANIMATION_FACTOR)`.
- Use actual elapsed milliseconds for wall-clock timers and timeouts.
- Do not rely on VSync or a frame cap for gameplay correctness.

At frame rates of at least 25 FPS:

```text
step per second
  = FPS * speed * (25 / FPS)
  = speed * 25
```

Below 25 FPS the factor remains capped at `1`. The simulation slows instead
of taking oversized steps that can skip animation keys or destabilize legacy
logic.

### Fire trails and flock steering

Fire Burst emits flame particles whose speed grows by 5% per reference frame.
Their acceleration is `1.05 ^ FPS_ANIMATION_FACTOR`: over their 12-reference-frame
lifetime, speed grows by about 1.8 times at every supported frame rate. Applying
5% on every rendered frame instead makes the same particles reach about 4.1 times
their initial speed at 60 FPS, stretching the trail away from its emitter.

Ambient flocks move along local +X, so their heading uses `atan2(dy, dx)`.
Character and projectile headings use local -Y and must retain their different
angle convention. Flock turn limits are degrees per reference frame, scaled by
the animation factor with fractional angles retained. Coincident neighbors do
not contribute a heading, avoiding division by zero in the flock average.

## Consequences

- Raw per-frame increments are defects unless the value is intentionally
  frame-count based.
- Changing `REFERENCE_FPS` changes global gameplay timing and requires
  asset calibration plus broad behavioral verification.
- Frame caps remain performance and presentation controls, not correctness
  controls.

## References

- [Reference rate and factor calculation](../../src/source/Engine/AI/ZzzAI.cpp)
- [Reference-rate declaration](../../src/source/Engine/AI/ZzzAI.h)
- [Canonical animation step](../../src/source/Render/Models/ZzzBMD.cpp)
