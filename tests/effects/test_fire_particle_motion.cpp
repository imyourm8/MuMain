#include "doctest.h"
#include "Render/Effects/FireParticleMotion.h"

#include <algorithm>
#include <cmath>
#include <initializer_list>

TEST_CASE("Fire particle acceleration preserves the 25 FPS trajectory [effects][motion]")
{
    constexpr float referenceFps = 25.f;
    constexpr float lifetime = 12.f;
    constexpr float initialSpeed = 9.6f;
    const float referenceDistance = initialSpeed * (std::pow(1.05f, lifetime) - 1.f) / 0.05f;
    for (const int fps : {25, 50, 60, 144})
    {
        CAPTURE(fps);
        float velocity[3] = {0.f, -initialSpeed, 0.f};
        float distance = 0.f;
        float elapsed = 0.f;
        while (elapsed < lifetime)
        {
            const float remaining = lifetime - elapsed;
            const float step = std::min(referenceFps / fps, remaining);
            distance -= velocity[1] * step;
            Render::Effects::AccelerateFireParticle(velocity, step);
            elapsed += step;
        }
        CHECK(velocity[1] == doctest::Approx(-initialSpeed * std::pow(1.05f, lifetime)).epsilon(0.0001));
        // Movement still uses the engine's discrete integration; allow its small step-size error.
        CHECK(distance == doctest::Approx(referenceDistance).epsilon(0.025));
        CHECK(velocity[0] == 0.f);
        CHECK(velocity[2] == 0.f);
    }
}

TEST_CASE("A zero duration leaves fire particle velocity unchanged [effects][motion]")
{
    float velocity[3] = {1.f, -2.f, 3.f};
    Render::Effects::AccelerateFireParticle(velocity, 0.f);
    CHECK(velocity[0] == 1.f);
    CHECK(velocity[1] == -2.f);
    CHECK(velocity[2] == 3.f);
}
