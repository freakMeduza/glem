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

    class Box {
    public:
        Box() {
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

        void render(glem::Context& context) const noexcept {
            vao_->bind();

            context.drawIndexed(vao_->indexCount());
        }

    private:
        std::unique_ptr<glem::VertexArray> vao_ {nullptr};

    };

    class SolidSphere {
    public:
        SolidSphere() {

        }

        ~SolidSphere() {

        }

        void render(glem::Context& context) const noexcept {
            vao_->bind();

            context.drawIndexed(vao_->indexCount());
        }

    private:
        std::unique_ptr<glem::VertexArray> vao_ {nullptr};

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
        auto program = std::make_unique<Program>();

        program->append(std::make_unique<Shader>(vs, ShaderType::VS));
        program->append(std::make_unique<Shader>(ps, ShaderType::PS));

        if(!program->link()) {
            Log::e(TAG, "Failed to link program.");
            return -1;
        }

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);

        glEnable(GL_BLEND);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        auto model = std::make_unique<Box>();

        auto camera = std::make_unique<MayaCamera>();
        camera->setPosition({0.0f, 0.0f, 10.0f});
        camera->setProjection(glm::perspective(glm::radians(45.0f), static_cast<float>(window_->width()) / static_cast<float>(window_->height()), 0.1f, 100.0f));

        program->bind();
        program->setUniform("uProjectionMatrix", camera->projection());

        while(true) {
            if(auto ret = window_->pollEvents()) {
                /**** shitdown ****/

                return *ret;
            }

            auto deltaTime = timer.mark();

            if(Keyboard::isKeyPressed(Keyboard::Key::Escape))
                window_->close();

            camera->update(deltaTime);

            program->setUniform("uViewMatrix", camera->view());

            /**** draw call ****/
            {
                context_->beginFrame({0.1f, 0.1f, 0.1f, 1.0f});

                model->render(*context_);

                context_->endFrame();
            }
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
