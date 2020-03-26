#pragma once

#include <memory>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/compatibility.hpp>

namespace glem {

    class Scene {
    public:
        virtual ~Scene() = default;

        inline void setVisible(bool value) noexcept {
            visible_ = value;
        }

        inline bool visible() const noexcept {
            return visible_;
        }

        /**
         * @brief Attach scene
         */
        virtual void attach() noexcept = 0;

        /**
         * @brief Detach scene
         */
        virtual void detach() noexcept = 0;

        /**
         * @brief Update scene
         * @param deltaTime
         */
        virtual void update(float deltaTime) noexcept = 0;

        /**
         * @brief Render scene
         */
        virtual void render() noexcept = 0;

    private:
        bool visible_ {false};

    };

    class Camera;
    class Program;
    class VertexArray;
    class ParticleEmitter;

    class ParticleScene : public Scene {
    public:
        ParticleScene();
        ~ParticleScene() override;

        // Scene interface
        void attach() noexcept override;
        void detach() noexcept override;
        void update(float deltaTime) noexcept override;
        void render() noexcept override;

    private:
        std::unique_ptr<Camera>          camera_  {nullptr};
        std::unique_ptr<ParticleEmitter> emitter_ {nullptr};

        std::shared_ptr<Program> lightProgram_ {nullptr};
        std::shared_ptr<Program> modelProgram_ {nullptr};

        std::shared_ptr<VertexArray> lightVertexArray_ {nullptr};
        std::shared_ptr<VertexArray> modelVertexArray_ {nullptr};

        glm::vec3 lightPosition_ {0.0f, 0.0f, 0.0f};
    };

    class Texture;

    class TextureMapScene : public Scene {
    public:
        TextureMapScene();
        ~TextureMapScene() override;

        // Scene interface
        void attach() noexcept override;
        void detach() noexcept override;
        void update(float deltaTime) noexcept override;
        void render() noexcept override;

    private:
        std::unique_ptr<Camera> camera_ {nullptr};

        std::shared_ptr<Program> lightProgram_ {nullptr};
        std::shared_ptr<Program> modelProgram_ {nullptr};

        std::shared_ptr<VertexArray> lightVertexArray_ {nullptr};
        std::shared_ptr<VertexArray> modelVertexArray_ {nullptr};

        std::unique_ptr<Texture> diffuseMap_  {nullptr};
        std::unique_ptr<Texture> specularMap_ {nullptr};

        glm::vec3 lightPosition_ {1.2f, 1.0f, 2.0f};
    };

    class SkyboxScene : public Scene {
    public:
        SkyboxScene();
        ~SkyboxScene() override;

        // Scene interface
        void attach() noexcept override;
        void detach() noexcept override;
        void update(float deltaTime) noexcept override;
        void render() noexcept override;

    private:
        std::unique_ptr<Camera>  camera_ {nullptr};

        std::shared_ptr<Program>     program_     {nullptr};
        std::unique_ptr<Texture>     texture_     {nullptr};
        std::shared_ptr<VertexArray> vertexArray_ {nullptr};
    };

}
