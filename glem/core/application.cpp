#include "application.hpp"

#include <glad/glad.h>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <render/visitor.hpp>
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
                         #version 330 core
                         layout (location = 0) in vec4 vertex;

                         out vec2 TexCoord;

                         void main()
                         {
                             gl_Position = vec4(vertex.xy, 0.0f, 1.0f);
                             TexCoord    = vec2(vertex.z, vertex.w);
                         })glsl";

        const auto& ps = R"glsl(
                         #version 330 core
                         out vec4 FragColor;

                         in vec2 TexCoord;

                         // texture sampler
                         uniform sampler2D texture1;

                         void main()
                         {
                             FragColor = texture(texture1, TexCoord);
                         })glsl";


        GLuint vbo;
        GLuint ibo;
        GLuint vao;

        glCreateVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glCreateBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW);

        glCreateBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (indices), indices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof (vertices[0]) * 4, reinterpret_cast<const void*>(0));
        glBindVertexArray(0);

        auto program = std::make_shared<render::ShaderProgram>();
        program->append(render::Shader::fromSource(vs, render::VS));
        program->append(render::Shader::fromSource(ps, render::PS));

        if(!program->link())
            return -1;

        int w, h, c;

        stbi_set_flip_vertically_on_load(1);

        uint8_t* data = stbi_load("texture.jpg", &w, &h, &c, STBI_rgb_alpha);

        if(!data)
            return -1;

        std::cout << "texture: " << w << "x" << h << " ("<< c << ")" << std::endl;

        GLenum interrnalFormat {0}, imageFormat {0};

        if(c == 4) {
            std::cout << "Format RGBA" << std::endl;
            interrnalFormat = GL_RGBA8;
            imageFormat     = GL_RGBA;
        }
        else if(c == 3) {
            std::cout << "Format RGB" << std::endl;
            interrnalFormat = GL_RGB8;
            imageFormat     = GL_RGB;
        }

        assert(interrnalFormat & imageFormat && "Format doesn't supported.");

        GLuint t{0};

        glCreateTextures(GL_TEXTURE_2D, 1, &t);
        glTextureStorage2D(t, 1, interrnalFormat, w, h);
        glTextureParameteri(t, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(t, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(t, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(t, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureSubImage2D(t, 0, 0, 0, w, h, imageFormat, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);

        while(true) {
            if(auto ret = window_->pollEvents()) {
                return *ret;
            }

//            layerManager_->onUpdate(0.0f);

            window_->context().beginFrame();

//            layerManager_->onDraw();

            glBindTexture(GL_TEXTURE_2D, t);

            program->bind();
            glBindVertexArray(vao);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            window_->context().endFrame();
        }
    }

}
