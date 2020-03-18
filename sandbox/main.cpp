#include <memory>

#include <glem/Log.hpp>
#include <glem/Window.hpp>
#include <glem/Mouse.hpp>
#include <glem/Keyboard.hpp>
#include <glem/Bindable.hpp>
#include <glem/Context.hpp>
#include <glem/VertexArray.hpp>
#include <glem/Buffer.hpp>
#include <glem/Camera.hpp>
#include <glem/Program.hpp>
#include <glem/Timer.hpp>
#include <glem/InputLayout.hpp>

#include <sstream>

#include "Cube.hpp"
#include "Sphere.hpp"

#include "ParticleSystem.hpp"

#include <glad/glad.h>

#define UNUSED(x) { static_cast<void>((x)); }
namespace {
    const auto& vs = R"glsl(
                     #version 450

                     layout(location = 0) in vec3 vPosition;
                     layout(location = 1) in vec3 vNormal;

                     uniform mat4 projectionMatrix;
                     uniform mat4 viewMatrix;
                     uniform mat4 modelMatrix = mat4(1.0f);

                     void main() {
                        gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vPosition, 1.0f);
                     }

                     )glsl";

    const auto& ps = R"glsl(
                     #version 450

                     layout(location = 0) out vec4 FragmentColor;

                     uniform vec4 color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

                     void main() {
                        FragmentColor = color;
                     }
                     )glsl";
}

int main(int argc, char** argv) {
    UNUSED(argc)
    UNUSED(argv)

    auto glfw = std::shared_ptr<void>(reinterpret_cast<void*>(glfwInit()), [](void*){ glfwTerminate(); });

    if(!glfw)
        return EXIT_FAILURE;

    auto window = std::make_unique<glem::Window>();

    if(!window)
        return EXIT_FAILURE;

    auto context = std::make_unique<glem::Context>(window->handler());

    glem::Mouse::setParent(window->handler());
    glem::Keyboard::setParent(window->handler());

    auto camera = std::make_unique<glem::Camera>(glm::vec3{0.0f, 0.0f, 100.0f});
    camera->speed = 8.0f;

    auto projection = glm::perspective(glm::radians(45.0f), static_cast<float>(window->width()) / static_cast<float>(window->height()), 0.1f, 1000.0f);

    auto program = std::make_shared<glem::Program>();

    program->append(std::make_unique<glem::Shader>(vs, glem::ShaderType::VS));
    program->append(std::make_unique<glem::Shader>(ps, glem::ShaderType::PS));

    if(!program->link()) {
        glem::Log::e("Application", "Failed to link program.");
        return EXIT_FAILURE;
    }

    /**** particle system ****/
    ParticleSystem particleSystem{projection, 10000};

    Properties properties;
    properties.position  = {0.0f, 0.0f, 0.0f};
    properties.velocity  = {0.0f, 0.0f, 0.0f};
    properties.colorBegin = {1.0f, 0.0f, 1.0f, 1.0f};
    properties.colorEnd   = {0.5f, 0.0f, 0.5f, 1.0f};
    properties.variation = {15.0f, 15.0f, 15.0f};
    properties.sizeBegin = 1.0f;
    properties.sizeEnd   = 0.0f;
    properties.life      = 10.0f;
    /*************************/

    float last_x = window->width()  / 2.0f;
    float last_y = window->height() / 2.0f;
    bool first = true;

    glem::Timer timer;

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /**** main loop ****/
    while(true) {
        if(auto ret = window->pollEvents()) {
            /**** shutdown here ****/

            return *ret;
        }

        auto deltaTime = timer.mark();

        if(glem::Keyboard::isKeyPressed(glem::Keyboard::Key::Escape))
            window->close();

        /**** camera ****/
        {
            if (glem::Keyboard::isKeyPressed(glem::Keyboard::Key::W))
                camera->processKeyboard(glem::Forward,  deltaTime);
            if (glem::Keyboard::isKeyPressed(glem::Keyboard::Key::S))
                camera->processKeyboard(glem::Backward, deltaTime);
            if (glem::Keyboard::isKeyPressed(glem::Keyboard::Key::A))
                camera->processKeyboard(glem::Left,     deltaTime);
            if (glem::Keyboard::isKeyPressed(glem::Keyboard::Key::D))
                camera->processKeyboard(glem::Right,    deltaTime);

            auto position = glem::Mouse::position();

            auto x = position.x;
            auto y = position.y;

            if (first) {
                last_x = static_cast<float>(x);
                last_y = static_cast<float>(y);
                first = false;
            }

            float x_offset = static_cast<float>(x) - last_x;
            float y_offset = last_y - static_cast<float>(y);

            last_x = static_cast<float>(x);
            last_y = static_cast<float>(y);

            camera->processMouse(x_offset, y_offset);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 0.1f);


        //if(glem::Keyboard::isKeyPressed(glem::Keyboard::Key::Space)) {
            for(size_t i = 0; i < 5; ++i)
                particleSystem.emitParticle(properties);
        //}

        particleSystem.onUpdate(deltaTime);
        particleSystem.onRender(*camera, program);

        glfwSwapBuffers(window->handler());
    }

    return 0;
}
