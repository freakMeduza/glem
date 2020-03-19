#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/compatibility.hpp>

#include <vector>
#include <memory>

#include <glem/Log.hpp>
#include <glem/Camera.hpp>
#include <glem/Buffer.hpp>
#include <glem/Program.hpp>
#include <glem/InputLayout.hpp>
#include <glem/VertexArray.hpp>

#include "primitives/Cube.hpp"

struct Particle {
    glm::vec3 position {glm::vec3{0.0f}};
    glm::vec3 velocity {glm::vec3{0.0f}};

    glm::vec4 colorBegin {glm::vec4{0.0f}};
    glm::vec4 colorEnd   {glm::vec4{0.0f}};

    float sizeBegin {0.0f};
    float sizeEnd   {0.0f};

    float pitch {0.0f};
    float roll  {0.0f};
    float yaw   {0.0f};

    float life   {0.0f};
    float remain {0.0f};

    bool alive {false};
};

struct Properties {
    glm::vec3 position  {glm::vec3{0.0f}};
    glm::vec3 velocity  {glm::vec3{0.0f}};
    glm::vec3 variation {glm::vec3{0.0f}};

    glm::vec4 colorBegin {glm::vec4{0.0f}};
    glm::vec4 colorEnd   {glm::vec4{0.0f}};

    float sizeBegin {0.0f};
    float sizeEnd   {0.0f};

    float life   {0.0f};
};

class ParticleSystem {
public:
    ParticleSystem(const glm::mat4& projection, size_t size = 1000u);
    ~ParticleSystem();

    ParticleSystem(ParticleSystem&&) = delete;
    ParticleSystem(const ParticleSystem&) = delete;

    ParticleSystem& operator=(ParticleSystem&&) = delete;
    ParticleSystem& operator=(const ParticleSystem&) = delete;

    void onUpdate(float deltaTime) noexcept;
    void onRender(glem::OldCamera& camera, std::shared_ptr<glem::Program> program) noexcept;

    void emitParticle(const Properties& value);

private:
    std::vector<Particle> pool_;

    size_t index_ {0u};

    glm::mat4 projection_ {glm::mat4{1.0f}};

};
