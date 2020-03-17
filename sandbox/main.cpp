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

#include <glad/glad.h>

#define UNUSED(x) { static_cast<void>((x)); }

const auto& vs = R"glsl(
                 #version 450

                 layout(location = 0) in vec3 vPosition;
                 layout(location = 1) in vec3 vNormal;

                 uniform mat4 projectionMatrix;
                 uniform mat4 viewMatrix;
                 uniform mat4 modelMatrix = mat4(1.0f);

                 out vec3 fPosition;
                 out vec3 fNormal;

                 void main() {
                    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vPosition, 1.0f);

                    fPosition = vec3(modelMatrix * vec4(vPosition, 1.0f));
                    fNormal   = mat3(transpose(inverse(modelMatrix))) * vNormal;
                 }

                 )glsl";

const auto& ps = R"glsl(
                 #version 450

                 layout(location = 0) out vec4 FragmentColor;

                 // Material
                 const vec3 cube_color = vec3(1.0f, 0.0f, 1.0f);

                 // Light
                 const vec3 light_color    = vec3(1.0f);
                 const vec3 light_position = vec3(1.0f, 3.0f, 2.0f);

                 const float light_constant  = 1.0f;
                 const float light_linear    = 0.09f;
                 const float light_quadratic = 0.032f;

                 uniform vec3 viewPosition;

                 in vec3 fPosition;
                 in vec3 fNormal;

                 void main() {
                     // ambient
                     vec3 ambient = 0.1f * light_color;

                     // diffuse
                     vec3 light_direction = normalize(light_position - fPosition);
                     vec3 diffuse         = max(dot(normalize(fNormal), light_direction), 0.0f) * light_color;

                     // specular
                     const float specular_strength = 0.5f;

                     vec3 view_direction    = normalize(viewPosition - fPosition);
                     vec3 reflect_direction = reflect(-light_direction, normalize(fNormal));
                     vec3 specular          = specular_strength * pow(max(dot(view_direction, reflect_direction), 0.0f), 32.0f) * light_color;

                     // attenuation
                     float distance    = length(light_position - fPosition);
                     float attenuation = 1.0 / (light_constant + light_linear * distance + light_quadratic * (distance * distance));

                     ambient  *= attenuation;
                     diffuse  *= attenuation;
                     specular *= attenuation;

                     FragmentColor = vec4((ambient + diffuse + specular) * cube_color, 1.0f);
                 }

                 )glsl";

template<typename T>
[[maybe_unused]] inline static void calculate_mesh_normals(std::vector<T>& vertices, const std::vector<uint32_t>& indices) noexcept {
    assert(indices.size() % 3 == 0 && indices.size() > 0);

    for(uint32_t i = 0; i < indices.size(); i += 3) {
        auto& v0 = vertices[indices[  i  ]];
        auto& v1 = vertices[indices[i + 1]];
        auto& v2 = vertices[indices[i + 2]];

        const auto p0 = v0.position;
        const auto p1 = v1.position;
        const auto p2 = v2.position;

        auto normal = glm::normalize(glm::cross((p1 - p0), (p2 - p0)));

        v0.normal = normal;
        v1.normal = normal;
        v2.normal = normal;
    }
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

    auto camera = std::make_unique<glem::Camera>(glm::vec3{0.0f, 0.0f, 3.0f});

    struct Vertex {
        glm::vec3 position {glm::vec3{0.0f}};
        glm::vec3 normal   {glm::vec3{0.0f}};
    };

//    std::vector<Vertex> vertices {
//        {{ -1.0f, -1.0f, -1.0f }, {}},
//        {{  1.0f, -1.0f, -1.0f }, {}},
//        {{ -1.0f,  1.0f, -1.0f }, {}},
//        {{  1.0f,  1.0f, -1.0f }, {}},
//        {{ -1.0f, -1.0f,  1.0f }, {}},
//        {{  1.0f, -1.0f,  1.0f }, {}},
//        {{ -1.0f,  1.0f,  1.0f }, {}},
//        {{  1.0f,  1.0f,  1.0f }, {}}
//    };

//    std::vector<uint32_t> indices {
//        0,2,1, 2,3,1,
//        1,3,5, 3,7,5,
//        2,6,3, 3,6,7,
//        4,5,7, 4,7,6,
//        0,4,2, 2,4,6,
//        0,1,4, 1,5,4
//    };

    std::vector<Vertex> vertices {
        {{ -0.5f, -0.5f,  0.5f }, {}},
        {{  0.5f, -0.5f,  0.5f }, {}},
        {{  0.5f,  0.5f,  0.5f }, {}},
        {{ -0.5f,  0.5f,  0.5f }, {}},

        {{ -0.5f, -0.5f, -0.5f }, {}},
        {{ -0.5f,  0.5f, -0.5f }, {}},
        {{  0.5f,  0.5f, -0.5f }, {}},
        {{  0.5f, -0.5f, -0.5f }, {}},

        {{ -0.5f,  0.5f, -0.5f }, {}},
        {{ -0.5f,  0.5f,  0.5f }, {}},
        {{  0.5f,  0.5f,  0.5f }, {}},
        {{  0.5f,  0.5f, -0.5f }, {}},

        {{ -0.5f, -0.5f, -0.5f }, {}},
        {{  0.5f, -0.5f, -0.5f }, {}},
        {{  0.5f, -0.5f,  0.5f }, {}},
        {{ -0.5f, -0.5f,  0.5f }, {}},

        {{  0.5f, -0.5f, -0.5f }, {}},
        {{  0.5f,  0.5f, -0.5f }, {}},
        {{  0.5f,  0.5f,  0.5f }, {}},
        {{  0.5f, -0.5f,  0.5f }, {}},

        {{ -0.5f, -0.5f, -0.5f }, {}},
        {{ -0.5f, -0.5f,  0.5f }, {}},
        {{ -0.5f,  0.5f,  0.5f }, {}},
        {{ -0.5f,  0.5f, -0.5f }, {}}
    };

    std::vector<uint32_t> indices {
        0,  1,  2,      0,  2,  3,
        4,  5,  6,      4,  6,  7,
        8,  9,  10,     8,  10, 11,
        12, 13, 14,     12, 14, 15,
        16, 17, 18,     16, 18, 19,
        20, 21, 22,     20, 22, 23,
    };

    calculate_mesh_normals(vertices, indices);

    glem::InputLayout layout;

    layout.push<glem::ElementType::Vector3f>("position", false);
    layout.push<glem::ElementType::Vector3f>("normal",   false);

    auto vbo = std::make_unique<glem::VertexBuffer>(vertices, layout);
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

    auto projection = glm::perspective(glm::radians(45.0f), static_cast<float>(window->width()) / static_cast<float>(window->height()), 0.1f, 100.0f);

    program->bind();
    program->setUniform("projectionMatrix", projection);

    float last_x = window->width()  / 2.0f;
    float last_y = window->height() / 2.0f;
    bool first = true;

    glem::Timer timer;

    using frame = std::chrono::duration<int32_t, std::ratio<1, 60>>;
    using ms    = std::chrono::duration<float, std::milli>;

    auto last = std::chrono::steady_clock::now();
    frame FPS{};

    while(true) {
        if(auto ret = window->pollEvents()) {
            /**** shutdown here ****/

            return *ret;
        }

        auto deltaTime = timer.mark();

        if(glem::Keyboard::isKeyPressed(glem::Keyboard::Key::Escape))
            window->close();

        /**** FPS ****/
        {
            FPS = std::chrono::duration_cast<frame>(std::chrono::steady_clock::now() - last);

            if (FPS.count() >= 1.0) {
                last = std::chrono::steady_clock::now();

                if(glem::Keyboard::isKeyPressed(glem::Keyboard::Key::Space))
                    glem::Log::d("Application", "LastFrame: ", std::chrono::duration_cast<ms>(FPS).count(), "ms  |  FPS: ", FPS.count() * 60);
            }
        }

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

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 0.1f);

        vao->bind();

        program->bind();
        program->setUniform("viewMatrix",   camera->viewMatrix());
        program->setUniform("viewPosition", camera->position);

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vao->indexCount()), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window->handler());
    }

    return 0;
}
