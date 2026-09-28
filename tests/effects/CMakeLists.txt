mu_add_test(
    NAME test_particle_draw_order
    SOURCES
        test_particle_draw_order.cpp
        ${CMAKE_SOURCE_DIR}/src/source/Render/Effects/ParticleDrawOrder.cpp
)

mu_add_test(
    NAME test_fire_particle_motion
    SOURCES
        test_fire_particle_motion.cpp
        ${CMAKE_SOURCE_DIR}/src/source/Render/Effects/FireParticleMotion.cpp
)
