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

#include "Cube.hpp"

class Box {
public:
    Box() {
        struct Vertex {
            glm::vec3 position {glm::vec3{0.0f}};
            glm::vec3 normal   {glm::vec3{0.0f}};
        };

        auto model = Cube<Vertex>::create();

        model.calculateNormals();

        glem::InputLayout layout;

        layout.push<glem::ElementType::Vector3f>("position");
        layout.push<glem::ElementType::Vector3f>("normal");

        vao_ = std::make_unique<glem::VertexArray>();

        vao_->append(std::make_unique<glem::VertexBuffer>(model.vertices, layout));
        vao_->append(std::make_unique<glem::IndexBuffer>(model.indices));
    }

    ~Box() {

    }

    void onRender() const noexcept {
        vao_->bind();

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vao_->indexCount()), GL_UNSIGNED_INT, 0);
    }

private:
    std::unique_ptr<glem::VertexArray> vao_ {nullptr};

};

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
    void onRender(glem::Camera& camera, std::shared_ptr<glem::Program> program) noexcept;

    void emitParticle(const Properties& value);

private:
    std::vector<Particle> pool_;

    size_t index_ {0u};

    glm::mat4 projection_ {glm::mat4{1.0f}};

    std::unique_ptr<Box>           model_   {nullptr};
    //std::unique_ptr<glem::Program> program_ {nullptr};

};
