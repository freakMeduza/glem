#include "application.hpp"

#include <iostream>

#include <glad/glad.h>

#include <render/attribute.hpp>
#include <render/inputlayout.hpp>
#include <render/vertexarray.hpp>
#include <render/indexbuffer.hpp>
#include <render/vertexbuffer.hpp>
#include <render/shader.hpp>
#include <render/shaderprogram.hpp>
#include <render/drawable.hpp>

namespace glem::core {

    Application::Application() :
        window_ {new Window{}}
    {

    }

    Application::~Application()
    {

    }

    int Application::exec() noexcept
    {
        const GLfloat positions[] {
             0.5f,  0.5f,
             0.5f, -0.5f,
            -0.5f, -0.5f,
            -0.5f,  0.5f
        };

        const GLuint indices[] { 0, 1, 3, 1, 2, 3 };

        auto vao = std::make_shared<render::VertexArray>();
        vao->append(std::make_shared<render::VertexBuffer>(render::InputLayout{
                                                               { render::AttributeType::Float2, "a_Position" }
                                                           },
                                                           positions,
                                                           sizeof (positions)));

        auto indexBuffer = std::make_shared<render::IndexBuffer>(indices, sizeof (indices));

        const auto& vs = R"glsl(
                         #version 450 core

                         layout(location = 0) in vec2 i_Position;

                         void main() {
                            gl_Position = vec4(i_Position, 0.0f, 1.0f);
                         }
                         )glsl";

        const auto& ps = R"glsl(
                         #version 450 core

                         layout(location = 0) out vec4 o_Color;

                         void main() {
                            o_Color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
                         }
                         )glsl";

        auto program = std::make_shared<render::ShaderProgram>();
        program->append(render::Shader::fromSource(vs, render::ShaderType::VS));
        program->append(render::Shader::fromSource(ps, render::ShaderType::PS));

        if(!program->link())
            return -1;

        render::Drawable drawable;
        drawable.append(vao);
        drawable.append(indexBuffer);
        drawable.append(program);

        while(true) {
            if(auto ret = window_->pollEvents())
                return *ret;

            window_->context().beginFrame();

            drawable.draw(window_->context());

            window_->context().endFrame();
        }
    }

}
