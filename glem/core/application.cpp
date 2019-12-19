#include "application.hpp"

#include <glad/glad.h>

#include <iostream>

#include <render/bindable.hpp>
#include <render/drawable.hpp>
#include <render/shader.hpp>
#include <render/context.hpp>
#include <render/attribute.hpp>
#include <render/inputlayout.hpp>
#include <render/indexbuffer.hpp>
#include <render/vertexarray.hpp>
#include <render/vertexbuffer.hpp>
#include <render/shaderprogram.hpp>
#include <render/surface.hpp>
#include <render/texture.hpp>

namespace glem::core {

    Application::Application() :
        window_       {new Window{}},
        layerManager_ {new LayerManager{}}
    {

    }

    Application::~Application()
    {

    }

    void Application::attach(const std::shared_ptr<Layer> &layer) noexcept
    {
        layerManager_->attach(layer);
    }

    void Application::detach(const std::shared_ptr<Layer> &layer) noexcept
    {
        layerManager_->detach(layer);
    }

    int Application::exec() noexcept
    {
        const float vertices[] {
            -0.5f, -0.5f, 0.0f, 0.0f,
            -0.5f,  0.5f, 0.0f, 1.0f,
             0.5f, -0.5f, 1.0f, 0.0f,
             0.5f,  0.5f, 1.0f, 1.0f
        };

        const uint32_t indices[] { 0, 1, 2, 2, 1, 3 };

        const auto& vs = R"glsl(
                         #version 450 core
                         layout (location = 0) in vec4 vertex;

                         out vec2 uv;

                         void main()
                         {
                            uv          = vec2(vertex.z, vertex.w);
                            gl_Position = vec4(vertex.xy, 0.0f, 1.0f);
                         })glsl";

        const auto& ps = R"glsl(
                         #version 450 core
                         out vec4 color;

                         in vec2 uv;

                         uniform sampler2D sampler;

                         void main()
                         {
                            color = texture(sampler, uv);
                         })glsl";

        auto vao = std::make_shared<render::VertexArray>();
        vao->append(std::make_shared<render::VertexBuffer>(render::InputLayout{{render::Float4, "vertex"}}, vertices, sizeof (vertices)));
        vao->append(std::make_shared<render::IndexBuffer>(indices, sizeof (indices)));

        auto program = std::make_shared<render::ShaderProgram>();
        program->append(render::Shader::fromSource(vs, render::VS));
        program->append(render::Shader::fromSource(ps, render::PS));

        if(!program->link())
            return -1;

        auto tex = std::make_shared<render::Texture>("texture", "tex.jpg", render::Wrap::Repeat, render::Filter::Linear, render::Filter::Linear, 0);

        while(true) {
            if(auto ret = window_->pollEvents())
                return *ret;

            layerManager_->onUpdate(0.0f);

            window_->context().beginFrame();

            layerManager_->onDraw();

            vao->bind();
            tex->bind();
            program->bind();

            glDrawElements(GL_TRIANGLES, vao->indexBuffer()->count(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));

            window_->context().endFrame();
        }
    }

}
