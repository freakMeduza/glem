#include "debugstate.hpp"

#include "window.hpp"
#include "application.hpp"
#include "inputmanager.hpp"
#include "statemanager.hpp"

#include <render/camera.hpp>
#include <render/renderer.hpp>
#include <render/drawable.hpp>
#include <render/shaderprogram.hpp>
#include <render/vertexbuffer.hpp>
#include <render/vertexarray.hpp>

#include <glad/glad.h>

#include <util/log.hpp>

#include <glm/glm.hpp>

#include "stb_image_write.h"

#include <fstream>
#include <map>

namespace {
    const std::string TAG = "Debug";

    const auto& vs = R"glsl(
                     #version 450 core

                     layout(location = 0) in vec3  vert_position;
                     layout(location = 1) in vec4  vert_color;
                     layout(location = 2) in vec2  vert_uv;
                     layout(location = 3) in float vert_unit;

                     uniform mat4 unif_view_projection;

                     out vec3  frag_position;
                     out vec4  frag_color;
                     out vec2  frag_uv;
                     out float frag_unit;

                     void main() {
                         gl_Position = unif_view_projection * vec4(vert_position, 1.0f);

                         frag_position = vert_position;
                         frag_color    = vert_color;
                         frag_uv       = vert_uv;
                         frag_unit     = vert_unit;
                     }
                     )glsl";

    const auto& ps = R"glsl(
                     #version 450 core

                     layout(location = 0) out vec4 out_color;

                     uniform vec2 unif_light_position;

                     uniform sampler2D unif_textures[32];

                     in vec3  frag_position;
                     in vec4  frag_color;
                     in vec2  frag_uv;
                     in float frag_unit;

                     void main() {
                         float intensity = 1.0 / length(frag_position.xy - unif_light_position);

                         intensity = clamp(intensity, 0.0f, 1.0f);
                         intensity = sqrt(intensity);

                         intensity = intensity * 2.5f;

                         vec4 color = frag_color;

                         if(frag_unit >= 0.0f) {
                            color = texture2D(unif_textures[int(frag_unit)], frag_uv);
                         }

                         out_color = color;// * intensity;
                     }
                     )glsl";

    const auto& vs_text = R"glsl(
                          #version 450 core

                          layout(location = 0) in vec4 vert_vertex;

                          out vec2 frag_uv;

                          uniform mat4 unif_view_projection;

                          void main() {
                            gl_Position = unif_view_projection * vec4(vert_vertex.xy, 0.0f, 1.0f);

                            frag_uv = vert_vertex.zw;
                          }
                          )glsl";

    const auto& ps_text = R"glsl(
                          #version 450 core

                          layout(location = 0) out vec4 out_color;

                          uniform sampler2D unif_texture;
                          uniform vec4      unif_color;

                          in vec2 frag_uv;

                          void main() {
                            out_color = vec4(1.0f, 1.0f, 1.0f, texture2D(unif_texture, frag_uv).r);
                          }
                          )glsl";

    const auto& WIDTH  = static_cast<float>(glem::core::Application::instance().window()->width());
    const auto& HEIGHT = static_cast<float>(glem::core::Application::instance().window()->height());

    const uint32_t ATLAS_DIMENSION = 1024;
    const char* FONT = "OCRAEXT.TTF";
}

namespace glem::core {

    void DebugState::onAttach() noexcept
    {
        init();

        FT_Library ft;

        if(FT_Init_FreeType(&ft)) {
            util::Log::e(TAG, "Init FT failed.");
            StateManager::pop();
        }

        FT_Face face;

        if(FT_New_Face(ft, FONT, 0, &face)) {
            util::Log::e(TAG, "Load FT face failed.");
            StateManager::pop();
        }

        atlas_.reset(new Atlas{face, 48});

        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }

    void DebugState::onDetach() noexcept
    {

    }

    void DebugState::onEvent(Event &event) noexcept
    {
        static_cast<void>(event);
    }

    void DebugState::onUpdate(float dt) noexcept
    {
        static_cast<void>(dt);

        if(InputManager::keyboard().isKeyPressed(Keyboard::Escape))
            Application::instance().window()->close();

//        auto position = InputManager::mouse().position();
//        position.y = HEIGHT - position.y;

//        program_->setUniform("unif_light_position", position);
    }

    void DebugState::onDraw() noexcept
    {
        renderText("Suck my fuck!", atlas_.get(), glm::vec2{100.0f, 100.0f}, glm::vec2{1.0f});
//        draw();
    }

    void DebugState::init() noexcept
    {
        camera_.reset(new render::Camera{glm::ortho(0.0f, WIDTH, 0.0f, HEIGHT)});

        program_.reset(new render::ShaderProgram{});
        program_->append(render::Shader::fromSource(vs_text, render::VS));
        program_->append(render::Shader::fromSource(ps_text, render::PS));

//        program_->append(render::Shader::fromSource(vs, render::VS));
//        program_->append(render::Shader::fromSource(ps, render::PS));

        if(!program_->link()) {
            util::Log::e(TAG, "Failed to link shader program.");
            StateManager::pop();
        }

        program_->bind();
        program_->setUniform("unif_view_projection", camera_->viewProjection());

        const auto& side = 500.0f;
        const auto& x = (WIDTH  - side) / 2;
        const auto& y = (HEIGHT - side) / 2;

        sprites_.emplace_back(std::make_shared<render::Drawable>(glm::vec3{x, y, 0.0f}, glm::vec2{side, side}, glm::vec4{0.5f, 0.7f, 0.8f, 1.0f}));
    }

    void DebugState::draw() noexcept
    {
        if(sprites_.empty()) {
            util::Log::w(TAG, "No available sprites for rendering.");
            return;
        }

        render::Renderer::begin();

        for(const auto& s : sprites_)
            render::Renderer::submit(s);

        render::Renderer::end();
        render::Renderer::present();
    }

    void DebugState::renderText(const std::string &text, Atlas *atlas, const glm::vec2 &position, const glm::vec2 &scale) noexcept
    {
        program_->bind();
        program_->setUniform("unif_texture", 0);

        glBindTextureUnit(0, atlas->texture);

        auto pen_x = position.x;
        auto pen_y = position.y;

        std::vector<Point> points;

        for(uint32_t i = 0; i < text.size(); ++i) {
            const auto& ascii = text[i];

            if(auto it = std::find_if(atlas->glyphs.begin(),
                                      atlas->glyphs.end(),
                                      [&ascii](const Atlas::Glyph& g){ return (ascii == g.ascii); });
                    it != atlas->glyphs.end()) {

                float x = pen_x + (*it).bearing.x * scale.x;
                float y = - pen_y - (*it).bearing.y * scale.y;

                float w = (*it).size.x * scale.x;
                float h = (*it).size.y * scale.y;

                pen_x += (*it).advance.x * scale.x;
                pen_y += (*it).advance.y * scale.y;

                if(!w || !h)
                    continue;

                points.emplace_back(Point{x, -y, (*it).offset.x, (*it).offset.y});
                points.emplace_back(Point{x + w, -y, (*it).offset.x + (*it).size.x / atlas->width, (*it).offset.y});
                points.emplace_back(Point{x, -y - h, (*it).offset.x, (*it).offset.y + (*it).size.y / atlas->height});
                points.emplace_back(Point{x + w, -y, (*it).offset.x + (*it).size.x / atlas->width, (*it).offset.y});
                points.emplace_back(Point{x, -y - h, (*it).offset.x, (*it).offset.y + (*it).size.y / atlas->height});
                points.emplace_back(Point{x + w, -y - h, (*it).offset.x + (*it).size.x / atlas->width, (*it).offset.y + (*it).size.y / atlas->height});
            }
        }

        uint32_t vao {0};
        uint32_t vbo {0};

        glCreateVertexArrays(1, &vao);
        glBindVertexArray(vao);
            glCreateBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof (Point) * points.size(), points.data(), GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof (float), reinterpret_cast<const void*>(0));
        glBindVertexArray(0);

        glBindVertexArray(vao);

        glDrawArrays(GL_TRIANGLES, 0, points.size());

        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }

    /**** Atlas ****/
    Atlas::Atlas(FT_Face face, int size)
    {
        FT_Set_Pixel_Sizes(face, 0, size);

        FT_GlyphSlot g = face->glyph;

        uint32_t roww = 0;
        uint32_t rowh = 0;

        for(uint8_t i = 32; i < 128; ++i) {
            if(FT_Load_Char(face, i, FT_LOAD_RENDER)) {
                util::Log::e(TAG, "Loading character ", i, "failed.");
                continue;
            }

            if(roww + g->bitmap.width + 1 >= ATLAS_DIMENSION) {
                width  = std::max(width, roww);
                height += rowh;
                roww = 0;
                rowh = 0;
            }

            roww += g->bitmap.width + 1;
            rowh = std::max(rowh, g->bitmap.rows);
        }

        width  = std::max(width, roww);
        height += rowh;

        util::Log::d(TAG, "Atlas size: ", width, "x " , height);

        glCreateTextures(GL_TEXTURE_2D, 1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTextureStorage2D(texture, 1, GL_R8, width, height);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        int offset_x = 0;
        int offset_y = 0;

        rowh = 0;

        for(int i = 32; i < 128; ++i) {
            if(FT_Load_Char(face, i, FT_LOAD_RENDER)) {
                util::Log::e(TAG, "Loading character ", i, "failed.");
                continue;
            }

            if(offset_x + g->bitmap.width + 1 >= ATLAS_DIMENSION) {
                offset_y += rowh;
                offset_x = 0;
                rowh = 0;
            }

            glTextureSubImage2D(texture, 0, offset_x, offset_y, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

            Glyph glyph;
            glyph.ascii = i;
            glyph.size.x = g->bitmap.width;
            glyph.size.y = g->bitmap.rows;
            glyph.advance.x = g->advance.x >> 6;
            glyph.advance.y = g->advance.y >> 6;
            glyph.bearing.x = g->bitmap_left;
            glyph.bearing.y = g->bitmap_top;
            glyph.offset.x = offset_x / static_cast<float>(width);
            glyph.offset.y = offset_y / static_cast<float>(height);

            glyphs.emplace_back(glyph);

            rowh = std::max(rowh, g->bitmap.rows);

            offset_x += g->bitmap.width + 1;
        }

        util::Log::d(TAG, "Atlas ", width, "x ", height, width * height / 1024);

        std::vector<uint8_t> pixels;
        pixels.resize(width * height);

        glGetTextureImage(texture, 0, GL_RED, GL_UNSIGNED_BYTE, pixels.size() * sizeof(uint8_t), pixels.data());

        stbi_write_jpg("test.jpg", width, height, 1, pixels.data(), 100);
    }

    Atlas::~Atlas()
    {
        glDeleteTextures(1, &texture);
    }

}
