#pragma once

#include <glm/glm.hpp>

#include <array>
#include <memory>

#include <glad/glad.h>

namespace glem {

    class Context;
    class Program;
    class VertexArray;

    struct Particle {
        glm::vec3 position {glm::vec3{0.0f}};
        glm::vec3 velocity {glm::vec3{0.0f}};
        glm::vec3 rotation {glm::vec3{0.0f}};

        std::array<float, 2> size;

        std::array<glm::vec4, 2> color;

        float life   {0.0f};
        float remain {0.0f};

        bool alive {false};
    };

    class ParticleEmitter {
    public:
        ParticleEmitter();
        ~ParticleEmitter();

        ParticleEmitter(ParticleEmitter&&) = delete;
        ParticleEmitter(const ParticleEmitter&) = delete;

        ParticleEmitter& operator=(ParticleEmitter&&) = delete;
        ParticleEmitter& operator=(const ParticleEmitter&) = delete;

        /**
         * @brief Update
         * @param deltaTime
         */
        void update(float deltaTime) noexcept;

        /**
         * @brief Render
         * @param context
         */
        void render(Context& context, std::shared_ptr<Program> program, std::shared_ptr<VertexArray> vertexArray, GLint topology = GL_TRIANGLES) noexcept;

        /**
         * @brief Spawn
         */
        void spawn() noexcept;

    private:
        std::array<Particle, 10000> pool_;

    };

}
