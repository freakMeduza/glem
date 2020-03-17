#include <memory>

#include <glem/Log.hpp>
#include <glem/Window.hpp>
#include <glem/Mouse.hpp>
#include <glem/Keyboard.hpp>
#include <glem/Bindable.hpp>
#include <glem/Context.hpp>
#include <glem/VertexArray.hpp>
#include <glem/Buffer.hpp>
#include <glem/Program.hpp>

#include <InputLayout.hpp>

#include <glad/glad.h>

#define UNUSED(x) { static_cast<void>((x)); }

const auto& vs = R"glsl(
                 #version 450

                 layout(location = 0) in vec3 vPosition;

                 void main() {
                    gl_Position = vec4(vPosition, 1.0f);
                 }

                 )glsl";

const auto& ps = R"glsl(
                 #version 450

                 layout(location = 0) out vec4 FragmentColor;

                 void main() {
                    FragmentColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
                 }

                 )glsl";


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

    struct Vertex {
        Vertex(const glm::vec2& p) : position{p} {

        }

        glm::vec2 position {glm::vec2{0.0f}};
    };

    std::vector<Vertex> vertices {
        glm::vec2{ -0.5f, -0.5f },
        glm::vec2{  0.0f,  0.5f },
        glm::vec2{  0.5f, -0.5f }
    };

    glem::InputLayout layout;
    layout.push<glem::ElementType::Position2D>("position", false);

    auto vbo = std::make_unique<glem::VertexBuffer>(vertices, layout);

    std::vector<uint32_t> indices {
        0, 1, 2
    };

    auto ibo = std::make_unique<glem::IndexBuffer>(indices);

    auto vao = std::make_unique<glem::VertexArray>();
    vao->append(std::move(vbo));
    vao->append(std::move(ibo));

    auto program = std::make_unique<glem::Program>();

    program->append(std::make_unique<glem::Shader>(vs, glem::ShaderType::VS));
    program->append(std::make_unique<glem::Shader>(ps, glem::ShaderType::PS));

    if(!program->link()) {
        glem::Log::e("Application", "Failed to link program.");

        return EXIT_FAILURE;
    }

    while(true) {
        if(auto ret = window->pollEvents()) {
            /**** shutdown here ****/

            return *ret;
        }

        if(glem::Keyboard::isKeyPressed(glem::Keyboard::Key::Escape))
            window->close();

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 0.1f);

        vao->bind();
        program->bind();

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vao->indexCount()), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window->handler());
    }

    return 0;
}
