#include "Particle.hpp"

#include <random>

#define _USE_MATH_DEFINES
#include <math.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/compatibility.hpp>

#include "Buffer.hpp"
#include "Context.hpp"
#include "Program.hpp"

namespace {
    const std::string TAG = "Particle";

    static uint32_t index {0u};

    /**** random ****/
    static std::mt19937 device;

    template<typename T>
    static T random(T&& v1, T&& v2) noexcept {
        if constexpr (std::is_same_v<float, std::decay_t<T>>) {
            std::uniform_real_distribution d(std::forward<T>(v1), std::forward<T>(v2));

            return static_cast<float>(d(device));
        }
        else {
            std::uniform_int_distribution d(std::forward<T>(v1), std::forward<T>(v2));

            return d(device);
        }
    }
}

namespace glem {

    ParticleEmitter::ParticleEmitter()
    {
        device.seed(std::random_device()());
    }

    ParticleEmitter::~ParticleEmitter()
    {

    }

    void ParticleEmitter::update(float deltaTime) noexcept
    {
        for(auto& p : pool_) {
            if(!p.alive)
                continue;

            if(p.remain <= 0.0f) {
                p.alive = false;
                continue;
            }

            p.remain -= deltaTime;

            p.rotation += p.velocity * deltaTime;
            p.position += p.velocity * deltaTime;
        }
    }

    void ParticleEmitter::render(Context &context, std::shared_ptr<Program> program, std::shared_ptr<VertexArray> vertexArray, GLint topology) noexcept
    {
        for(const auto& p : pool_) {
            if(!p.alive)
                continue;

            auto life = p.remain / p.life;

            auto size  = glm::lerp(p.size[1],  p.size[0],  life);
            auto color = glm::lerp(p.color[1], p.color[0], life);

            auto transform = glm::translate(glm::mat4{1.0f}, p.position) *
                             glm::rotate(glm::mat4{1.0f}, p.rotation.x, {1.0f, 0.0f, 0.0f}) *
                             glm::rotate(glm::mat4{1.0f}, p.rotation.y, {0.0f, 1.0f, 0.0f}) *
                             glm::rotate(glm::mat4{1.0f}, p.rotation.z, {0.0f, 0.0f, 1.0f}) *
                             glm::scale(glm::mat4{1.0f}, {size, size, size});

            program->bind();
            program->setUniform("uModelMatrix", transform);
            program->setUniform("uMaterial.color", color);

            context.renderIndexed(vertexArray->indexCount(), topology);
        }
    }

    void ParticleEmitter::spawn() noexcept
    {
        auto& p = pool_[index];

        p.alive = true;

        p.position = {0.0f, 0.0f, 0.0f};

        p.velocity.x = random<float>(1.0f, 10.0f) * (random<float>(0.0f, 1.0f) - 0.5f);
        p.velocity.y = random<float>(1.0f, 10.0f) * (random<float>(0.0f, 1.0f) - 0.5f);
        p.velocity.z = random<float>(1.0f, 10.0f) * (random<float>(0.0f, 1.0f) - 0.5f);

        p.rotation.x = random<float>(2.5f, 2 * M_PI) * (random<float>(0.0f, 1.0f) - 0.5f);
        p.rotation.y = random<float>(2.5f, 2 * M_PI) * (random<float>(0.0f, 1.0f) - 0.5f);
        p.rotation.z = random<float>(2.5f, 2 * M_PI) * (random<float>(0.0f, 1.0f) - 0.5f);

        p.size[1] = {0.0f};
        p.size[0] = {1.0f};

        p.color[0] = {1.0f, 0.0f, 1.0f, 1.0f};
        p.color[1] = {0.0f, 0.0f, 0.0f, 0.0f};

        p.life   = 8.0f;
        p.remain = 8.0f;

        index = ++index % pool_.size();
    }

}
