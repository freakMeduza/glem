#include <glem.hpp>

#include <iostream>

class Sandbox : public glem::core::Application {
public:
    Sandbox() = default;
    ~Sandbox() = default;
};

class Layer : public glem::core::Layer {
public:
    Layer() = default;
    ~Layer() override = default;

    // Layer interface
    void onAttach() noexcept override {
        const float vertices[] {
               0.5f,  0.5f,
               0.5f, -0.5f,
              -0.5f, -0.5f,
              -0.5f,  0.5f,
        };

        const uint32_t indices[] { 0, 1, 3, 1, 2, 3 };

        const auto& vao     = std::make_shared<glem::render::VertexArray>();
        const auto& program = std::make_shared<glem::render::ShaderProgram>();

        const auto& vbo = std::make_shared<glem::render::VertexBuffer>(glem::render::InputLayout{{glem::render::Float2, "vertex"}}, vertices, sizeof (vertices));
        const auto& ibo = std::make_shared<glem::render::IndexBuffer>(indices, sizeof (indices));

        vao->append(vbo);
        vao->append(ibo);

        const auto& vs = R"glsl(
                         #version 450 core

                         layout(location = 0) in vec4 vertex;

                         out vec2 uv;

                         void main() {
                            gl_Position = vec4(vertex.xy, 0.0f, 1.0f);
                         }
                         )glsl";

        const auto& ps = R"glsl(
                         #version 450 core

                         layout(location = 0) out vec4 color;

                         void main() {
                            color = vec4(0.5f, 1.0f, 1.0f, 1.0f);
                         }
                         )glsl";

        program->append(glem::render::Shader::fromSource(vs, glem::render::ShaderType::VS));
        program->append(glem::render::Shader::fromSource(ps, glem::render::ShaderType::PS));

        if(!program->link())
            std::cerr << "Unable to link shader program." << std::endl;

        drawable_.reset(new glem::render::Drawable{});

        drawable_->append(vao);
        drawable_->append(program);
    }

    void onDetach() noexcept override {
        drawable_.reset();
    }

    void onUpdate(float dt) noexcept override {
        static_cast<void>(dt);
    }

    void onDraw() noexcept override {
        drawable_->draw();
    }

private:
    std::unique_ptr<glem::render::Drawable> drawable_ {nullptr};

};

int main() {
    Sandbox sandbox;

    sandbox.attach(std::make_shared<Layer>());

    return sandbox.exec();
}
