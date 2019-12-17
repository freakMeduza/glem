#include <glem.hpp>

#include <iostream>

class Sandbox : public glem::core::Application {
public:
    Sandbox() = default;
    ~Sandbox() = default;
};

//class Layer : public glem::core::Layer {
//public:
//    Layer() = default;
//    ~Layer() override = default;

//    // Layer interface
//    void onAttach() noexcept override {
//        const float vertices[] {
//            // positions    // texture coords
//               0.5f,  0.5f,   1.0f, 1.0f, // top right
//               0.5f, -0.5f,   1.0f, 0.0f, // bottom right
//              -0.5f, -0.5f,   0.0f, 0.0f, // bottom left
//              -0.5f,  0.5f,   0.0f, 1.0f  // top left
//        };

//        const uint32_t indices[] { 0, 1, 3, 1, 2, 3 };

//        const auto& vao     = std::make_shared<glem::render::VertexArray>();
//        const auto& program = std::make_shared<glem::render::ShaderProgram>();

//        const auto& vbo = std::make_shared<glem::render::VertexBuffer>(glem::render::InputLayout{{glem::render::Float4, "vertex"}}, vertices, sizeof (vertices));
//        const auto& ibo = std::make_shared<glem::render::IndexBuffer>(indices, sizeof (indices));
//        const auto& tex = glem::render::Texture2D::fromFile("download.jpeg");

//        vao->append(vbo);

//        const auto& vs = R"glsl(
//                         #version 450 core
//                         layout(location = 0) in vec4 vertex;

//                         out vec2 uv;

//                         void main() {
//                            uv = vertex.zw;
//                            gl_Position = vec4(vertex.xy, 0.0f, 1.0f);
//                         }
//                         )glsl";

//        const auto& ps = R"glsl(
//                         #version 450 core
//                         layout(location = 0) out vec4 color;

//                         in vec2 uv;

//                         uniform sampler2D s;

//                         void main() {
//                            color = texture(s, uv);
//                         }
//                         )glsl";

//        program->append(glem::render::Shader::fromSource(vs, glem::render::ShaderType::VS));
//        program->append(glem::render::Shader::fromSource(ps, glem::render::ShaderType::PS));

//        if(!program->link())
//            std::cerr << "Unable to link shader program." << std::endl;

//        drawable_.reset(new glem::render::Drawable{});

//        drawable_->append(vao);
//        drawable_->append(ibo);
//        drawable_->append(tex);
//        drawable_->append(program);
//    }

//    void onDetach() noexcept override {
//        drawable_.reset();
//    }

//    void onUpdate(float dt) noexcept override {
//        static_cast<void>(dt);
//    }

//    void onDraw() noexcept override {
//        drawable_->draw();
//    }

//private:
//    std::unique_ptr<glem::render::Drawable> drawable_ {nullptr};

//};

int main() {
    Sandbox sandbox;

//    sandbox.attach(std::make_shared<Layer>());

    return sandbox.exec();
}
