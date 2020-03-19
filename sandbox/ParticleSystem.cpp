#include "ParticleSystem.hpp"

#include <random>

#include <glad/glad.h>

namespace {
    static const std::string TAG = "ParticleSystem";

    static std::mt19937 gen;

    template<typename T>
    [[maybe_unused]] static T random (T&& v1, T&& v2) noexcept {
        if constexpr (std::is_same_v<std::decay_t<T>, int>) {
            std::uniform_int_distribution dis(std::forward<T>(v1), std::forward<T> (v2));

            return dis (gen);
        }
        else {
            std::uniform_real_distribution dis (std::forward<T> (v1), std::forward<T> (v2));

            return static_cast<float>(dis (gen));
        }
    }
}

ParticleSystem::ParticleSystem(const glm::mat4 &projection, size_t size) :
    index_{size - 1}, projection_{projection}
{
    pool_.resize(size);

    gen.seed(std::random_device()());
}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::onUpdate(float deltaTime) noexcept
{
    for(auto& p : pool_) {
        if(!p.alive)
            continue;

        if(p.remain <= 0.0f) {
            p.alive = false;
            continue;
        }

        p.pitch += 3.0f * deltaTime;
        p.roll  += 3.0f * deltaTime;
        p.yaw   += 3.0f * deltaTime;

        p.remain -= deltaTime;

        p.position += p.velocity * deltaTime;
    }
}

void ParticleSystem::onRender(glem::OldCamera& camera, std::shared_ptr<glem::Program> program) noexcept
{
    program->bind();
    program->setUniform("projectionMatrix", projection_);
    program->setUniform("viewMatrix", camera.viewMatrix());

    for(const auto& p : pool_) {
        if(!p.alive)
            continue;

        auto life = p.remain / p.life;

        auto size  = glm::lerp(p.sizeEnd,  p.sizeBegin,  life);
        auto color = glm::lerp(p.colorEnd, p.colorBegin, life);

        auto transform = glm::translate(glm::mat4{1.0f}, p.position)               *
                         glm::rotate(glm::mat4{1.0f}, p.pitch, {1.0f, 0.0f, 0.0f}) *
                         glm::rotate(glm::mat4{1.0f}, p.roll,  {0.0f, 1.0f, 0.0f}) *
                         glm::rotate(glm::mat4{1.0f}, p.yaw,   {0.0f, 0.0f, 1.0f}) *
                         glm::scale(glm::mat4{1.0f}, {size, size, size});

        program->bind();
        program->setUniform("modelMatrix", transform);
        program->setUniform("color", color);
    }
}

void ParticleSystem::emitParticle(const Properties &value)
{
    auto& p = pool_[index_];

    p.alive = true;

    p.position = value.position;
    p.velocity = value.velocity;

    p.colorBegin = value.colorBegin;
    p.colorEnd   = value.colorEnd;

    p.velocity.x += value.variation.x * (random(0.0f, 1.0f) - 0.5f);
    p.velocity.y += value.variation.y * (random(0.0f, 1.0f) - 0.5f);
    p.velocity.z += value.variation.z * (random(0.0f, 1.0f) - 0.5f);

    p.pitch = value.variation.x * (random(0.0f, 1.0f) - 0.5f);
    p.roll  = value.variation.y * (random(0.0f, 1.0f) - 0.5f);
    p.yaw   = value.variation.z * (random(0.0f, 1.0f) - 0.5f);

    p.sizeBegin = value.sizeBegin;
    p.sizeEnd   = value.sizeEnd;

    p.life   = value.life;
    p.remain = value.life;

    index_ = --index_ % pool_.size();
}
