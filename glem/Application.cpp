#include "Application.hpp"
#include "Context.hpp"
#include "Window.hpp"
#include "Input.hpp"

#include "Log.hpp"
#include "Timer.hpp"
#include "Camera.hpp"

#include "Buffer.hpp"
#include "Shader.hpp"
#include "Program.hpp"
#include "VertexArray.hpp"

#include <glad/glad.h>

#include <Primitives.hpp>

#include "Particle.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/compatibility.hpp>

namespace {
    const std::string TAG = "Application";

    const auto& vs = R"glsl(
                     #version 450

                     layout(location = 0) in vec3 vPosition;
                     layout(location = 1) in vec3 vNormal;

                     uniform mat4 uProjectionMatrix;
                     uniform mat4 uViewMatrix;
                     uniform mat4 uModelMatrix = mat4(1.0f);

                     void main() {
                        gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(vPosition, 1.0f);
                     }

                     )glsl";

    const auto& ps = R"glsl(
                     #version 450

                     layout(location = 0) out vec4 FragmentColor;

                     uniform vec4 uColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

                     void main() {
                        FragmentColor = uColor;
                     }
                     )glsl";

    const auto& phong_vs = R"glsl(
                     #version 450

                     layout(location = 0) in vec3 vPosition;
                     layout(location = 1) in vec3 vNormal;

                     uniform mat4 uProjectionMatrix;
                     uniform mat4 uViewMatrix;
                     uniform mat4 uModelMatrix = mat4(1.0f);

                     out vec3 fPosition;
                     out vec3 fNormal;

                     void main() {
                        gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(vPosition, 1.0f);

                        fPosition = vec3(uModelMatrix * vec4(vPosition, 1.0f));
                        fNormal   = mat3(transpose(inverse(uModelMatrix))) * vNormal;
                     }

                     )glsl";

    const auto& phong_ps = R"glsl(
                     #version 450

                     layout(location = 0) out vec4 FragmentColor;

                     uniform vec3 uViewPosition;

                     struct Material {
                        vec4 color;
                        float shininess;
                     };

                     struct Light {
                        vec3 position;

                        vec3 ambient;
                        vec3 diffuse;
                        vec3 specular;

                        float constant;
                        float linear;
                        float quadratic;
                     };

                     uniform Light uLight;
                     uniform Material uMaterial;

                     in vec3 fPosition;
                     in vec3 fNormal;

                     void main() {
                        vec3 ambient = uLight.ambient;

                        vec3 N = normalize(fNormal);
                        vec3 L = normalize(uLight.position - fPosition);

                        vec3 diffuse = uLight.diffuse * max(dot(N, L), 0.0f);

                        vec3 V = normalize(uViewPosition - fPosition);
                        vec3 R = reflect(-L, N);

                        vec3 specular = uLight.specular * pow(max(dot(V, R), 0.0f), uMaterial.shininess);

                        float D = length(uLight.position - fPosition);

                        float attenuation = 1.0f / (uLight.constant + uLight.linear * D + uLight.quadratic * D * D);

                        ambient  *= attenuation;
                        diffuse  *= attenuation;
                        specular *= attenuation;

                        FragmentColor = vec4(ambient + diffuse + specular, 1.0f) * uMaterial.color;
                     }
                     )glsl";

    class Renderable {
    public:
        Renderable() = default;
        ~Renderable() = default;

        GLint topology() const noexcept {
            return topology_;
        }

    protected:
        GLint topology_ {GL_TRIANGLES};
    };

    class Box : public Renderable {
    public:
        Box() {
            topology_ = GL_TRIANGLES;

            struct Vertex {
                glm::vec3 position {glm::vec3{0.0f}};
                glm::vec3 normal   {glm::vec3{0.0f}};
            };

            auto model = primitives::Cube<Vertex>::create();

            model.setNormal();

            glem::InputLayout layout;

            layout.push<glem::ElementType::Vector3f>("position");
            layout.push<glem::ElementType::Vector3f>("normal");

            vao_ = std::make_unique<glem::VertexArray>();

            vao_->append(std::make_unique<glem::VertexBuffer>(model.vertices, layout));
            vao_->append(std::make_unique<glem::IndexBuffer>(model.indices));
        }

        ~Box() {

        }

        std::shared_ptr<glem::VertexArray> vertexArray() const noexcept {
            return vao_;
        }

    private:
        std::shared_ptr<glem::VertexArray> vao_ {nullptr};

    };

    class Sphere : public Renderable {
    public:
        Sphere() {
            topology_  = GL_TRIANGLE_STRIP;

            struct Vertex {
                glm::vec3 position {glm::vec3{0.0f}};
                glm::vec3 normal   {glm::vec3{0.0f}};
            };

            auto model = primitives::Sphere<Vertex>::create({64, 64});

            model.transform(glm::scale(glm::mat4{1.0f}, glm::vec3{0.3f}));

            for(auto& v : model.vertices)
                v.normal = v.position;

            glem::InputLayout layout;

            layout.push<glem::ElementType::Vector3f>("position");
            layout.push<glem::ElementType::Vector3f>("normal");

            vao_ = std::make_unique<glem::VertexArray>();

            vao_->append(std::make_unique<glem::VertexBuffer>(model.vertices, layout));
            vao_->append(std::make_unique<glem::IndexBuffer>(model.indices));
        }

        ~Sphere() {

        }

        std::shared_ptr<glem::VertexArray> vertexArray() const noexcept {
            return vao_;
        }

    private:
        std::shared_ptr<glem::VertexArray> vao_ {nullptr};

    };

}

namespace glem {

    Window &Application::window() const noexcept
    {
        return *window_;
    }

    Context &Application::context() const noexcept
    {
        return *context_;
    }

    int Application::exec() noexcept
    {
        Timer timer;

        /**** render shit ****/
        auto program = std::make_shared<Program>();

        program->append(std::make_unique<Shader>(vs, ShaderType::VS));
        program->append(std::make_unique<Shader>(ps, ShaderType::PS));

        if(!program->link()) {
            Log::e(TAG, "Failed to link program.");
            return -1;
        }

        auto phongProgram = std::make_shared<Program>();

        phongProgram->append(std::make_unique<Shader>(phong_vs, ShaderType::VS));
        phongProgram->append(std::make_unique<Shader>(phong_ps, ShaderType::PS));

        if(!phongProgram->link()) {
            Log::e(TAG, "Failed to link program.");
            return -1;
        }

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);

        glEnable(GL_BLEND);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        auto box = std::make_shared<Box>();

        /**** camera ****/
        std::unique_ptr<Camera> camera {nullptr};

        auto projection = glm::perspective(glm::radians(45.0f), static_cast<float>(window_->width()) / static_cast<float>(window_->height()), 0.1f, 1000.0f);

        camera.reset(new MayaCamera{});
        camera->setProjection(projection);
        camera->setPosition({0.0f, 0.0f, 50.0f});

        phongProgram->bind();
        /**** material setup ****/
        phongProgram->setUniform("uMaterial.color",     glm::vec4{1.0f, 0.0f, 1.0f, 1.0f});
        phongProgram->setUniform("uMaterial.shininess", 32.0f);

        auto sphere = std::make_shared<Sphere>();

        glm::vec3 lightPosition{glm::vec3{0.0f}};

        /**** light setup ****/
        phongProgram->setUniform("uLight.position",  lightPosition);
        phongProgram->setUniform("uLight.ambient",   glm::vec3{0.2f, 0.2f, 0.2f});
        phongProgram->setUniform("uLight.diffuse",   glm::vec3{0.5f, 0.5f, 0.5f});
        phongProgram->setUniform("uLight.specular",  glm::vec3{1.0f, 1.0f, 1.0f});
        phongProgram->setUniform("uLight.constant",  1.0f);
        phongProgram->setUniform("uLight.linear",    0.09f);
        phongProgram->setUniform("uLight.quadratic", 0.032f);

        ParticleEmitter emitter;

        while(true) {
            if(auto ret = window_->pollEvents()) {
                /**** shitdown ****/

                return *ret;
            }

            auto deltaTime = timer.mark();

            if(Keyboard::isKeyPressed(Keyboard::Key::Escape))
                window_->close();

            if(Keyboard::isKeyPressed(Keyboard::Key::F1)) {
                camera.reset(new FreeCamera{});
                camera->setProjection(projection);
                camera->setPosition({0.0f, 0.0f, 50.0f});
            }

            if(Keyboard::isKeyPressed(Keyboard::Key::F2)) {
                camera.reset(new MayaCamera{});
                camera->setProjection(projection);
                camera->setPosition({0.0f, 0.0f, 50.0f});
            }

            auto speed = 10.0f;

            if(Keyboard::isKeyPressed(Keyboard::Key::Up))
                lightPosition.y += speed * deltaTime;
            if(Keyboard::isKeyPressed(Keyboard::Key::Down))
                lightPosition.y -= speed * deltaTime;
            if(Keyboard::isKeyPressed(Keyboard::Key::Right))
                lightPosition.x += speed * deltaTime;
            if(Keyboard::isKeyPressed(Keyboard::Key::Left))
                lightPosition.x -= speed * deltaTime;
            if(Keyboard::isKeyPressed(Keyboard::Key::PageUp))
                lightPosition.z += speed * deltaTime;
            if(Keyboard::isKeyPressed(Keyboard::Key::PageDown))
                lightPosition.z -= speed * deltaTime;
            if(Keyboard::isKeyPressed(Keyboard::Key::Space))
                lightPosition = glm::vec3{0.0f};

            emitter.update(deltaTime);
            camera->update(deltaTime);

            /**** emitter ****/
            for(auto i = 0; i < 1; ++i)
                emitter.spawn();

            /**** particle rendering ****/
            phongProgram->bind();
            phongProgram->setUniform("uProjectionMatrix", camera->projection());
            phongProgram->setUniform("uViewMatrix", camera->view());

            phongProgram->setUniform("uViewPosition", camera->position());
            phongProgram->setUniform("uLight.position",  lightPosition);

            context_->beginFrame({0.1f, 0.1f, 0.1f, 1.0f});

            box->vertexArray()->bind();
            emitter.render(*context_, phongProgram, box->vertexArray(), box->topology());

            program->bind();
            program->setUniform("uProjectionMatrix", camera->projection());
            program->setUniform("uViewMatrix", camera->view());

            auto transform = glm::translate(glm::mat4{1.0f}, lightPosition);

            program->setUniform("uModelMatrix", transform);

            sphere->vertexArray()->bind();
            context_->renderIndexed(sphere->vertexArray()->indexCount(), sphere->topology());

            context_->endFrame();
        }
    }

    Application::Application()
    {
        if(!glfwInit()) {
            Log::e(TAG, "Failed to initialize GLFW.");
            abort();
        }

        window_ = std::make_unique<Window>();

        if(!window_) {
            Log::e(TAG, "Failed to create window.");
            abort();
        }

        context_ = std::make_unique<Context>(window_->handler());

        if(!context_) {
            Log::e(TAG, "Failed to create context.");
            abort();
        }

        Mouse::setParent(window_->handler());
        Keyboard::setParent(window_->handler());
    }

    Application::~Application()
    {

    }

}
