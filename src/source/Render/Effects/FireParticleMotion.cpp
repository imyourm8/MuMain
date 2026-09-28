#include "FireParticleMotion.h"

#include <cmath>

namespace Render::Effects
{
    void AccelerateFireParticle(float* velocity, float frameFactor)
    {
        constexpr float accelerationPerReferenceFrame = 1.05f;
        const float acceleration = std::pow(accelerationPerReferenceFrame, frameFactor);
        for (int axis = 0; axis < 3; ++axis)
        {
            velocity[axis] *= acceleration;
        }
    }
}
