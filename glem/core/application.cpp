#include "application.hpp"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <render/shader.hpp>
#include <render/shaderprogram.hpp>

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
            0.5f,  0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 1.0f, 0.0f,
           -0.5f, -0.5f, 0.0f, 0.0f,
           -0.5f,  0.5f, 0.0f, 1.0f
        };

//        const float vertices[] {
//            -0.5f, -0.5f, 0.0f,
//            -0.5f,  0.5f, 0.0f,
//             0.5f,  0.5f, 0.0f,
//             0.5f, -0.5f, 0.0f,
//        };

        const uint32_t indices[] { 0, 1, 3, 1, 2, 3 };

        const auto& vs = R"glsl(
                         #version 450 core
                         layout(location = 0) in vec2 in_pos;
                         layout(location = 1) in vec2 in_uv;

                         out vec2 uv;

                         void main() {
                            uv = in_uv;

                            gl_Position = vec4(in_pos.xy, 0.0f, 1.0f);
                         }
                         )glsl";

        const auto& ps = R"glsl(
                         #version 450 core
                         layout(location = 0) out vec4 color;

                         in vec2 uv;

                         uniform sampler2D s;

                         void main() {
                            color = texture(s, uv);
                         }
                         )glsl";

        GLuint vbo {0};
        GLuint ibo {0};
        GLuint vao {0};
        GLuint tex {0};

        render::ShaderProgram p;
        p.append(render::Shader::fromSource(vs, render::VS));
        p.append(render::Shader::fromSource(ps, render::PS));

        if(!p.link())
            return -1;

        glCreateVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glCreateBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW);

        glCreateBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (indices), indices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof (GLfloat), reinterpret_cast<const void*>(0));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof (GLfloat), reinterpret_cast<const void*>(2 * sizeof (GLfloat)));

        int w, h, c;

        unsigned char* data = stbi_load("download.jpeg", &w, &h, &c, 0);

        if(!data)
            return -1;

        GLenum internalFormat = 0;
        GLenum format = 0;

        if(c == 3) {
            internalFormat = GL_RGB8;
            format = GL_RGB;
        }
        else if(c == 4)
            internalFormat = GL_RGBA8;
            format = GL_RGBA;

        glCreateTextures(GL_TEXTURE_2D, 1, &tex);

        glTextureStorage2D(tex, 1, internalFormat, w, h);

        glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(tex, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(tex, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(tex, 0, 0, 0, w, h, format, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);

        while(true) {
            if(auto ret = window_->pollEvents())
                return *ret;

            layerManager_->onUpdate(0.0f);

            window_->context().beginFrame();

            layerManager_->onDraw();

            glBindVertexArray(vao);
            glBindTextureUnit(0u, tex);
            p.bind();

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

            window_->context().endFrame();
        }

        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);
        glDeleteVertexArrays(1, &vao);
    }

}
