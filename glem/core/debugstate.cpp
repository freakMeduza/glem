#include "debugstate.hpp"

#include "window.hpp"
#include "application.hpp"
#include "inputmanager.hpp"
#include "statemanager.hpp"

#include <render/font.hpp>
#include <render/camera.hpp>
#include <render/texture.hpp>
#include <render/renderer.hpp>
#include <render/drawable.hpp>
#include <render/shaderprogram.hpp>

#include <util/log.hpp>

#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <fstream>
#include <map>

namespace {
    const std::string TAG = "Debug";

    const auto& vs = R"glsl(
                     #version 450 core

                     layout(location = 0) in vec3  vertex_position;
                     layout(location = 1) in vec4  vertex_color;
                     layout(location = 2) in vec2  vertex_uv;
                     layout(location = 3) in float vertex_unit;

                     uniform mat4 uniform_view_projection;

                     out vec3  fragment_position;
                     out vec4  fragment_color;
                     out vec2  fragment_uv;
                     out float fragment_unit;

                     void main() {
                         gl_Position = uniform_view_projection * vec4(vertex_position, 1.0f);

                         fragment_position = vertex_position;
                         fragment_color    = vertex_color;
                         fragment_uv       = vertex_uv;
                         fragment_unit     = vertex_unit;
                     }
                     )glsl";

    const auto& ps = R"glsl(
                     #version 450 core

                     layout(location = 0) out vec4 out_color;

                     uniform vec2 uniform_light_position;

                     uniform sampler2D uniform_textures[32];

                     in vec3  fragment_position;
                     in vec4  fragment_color;
                     in vec2  fragment_uv;
                     in float fragment_unit;

                     void main() {
                         float intensity = 1.0 / length(fragment_position.xy - uniform_light_position);

                         intensity = clamp(intensity, 0.0f, 1.0f);
                         intensity = sqrt(intensity);

                         intensity = intensity * 2.5f;

                         vec4 color = fragment_color;

                         if(fragment_unit >= 0.0f) {
                            color = texture2D(uniform_textures[int(fragment_unit)], fragment_uv);
                         }

                         out_color = color * intensity;
                     }
                     )glsl";

    const auto& WIDTH  = static_cast<float>(glem::core::Application::instance().window()->width());
    const auto& HEIGHT = static_cast<float>(glem::core::Application::instance().window()->height());

    struct Glyph {
        glm::vec2 coord0;
        glm::vec2 coord1;
        glm::vec2 offset;

        float advance;
    };

    const std::string FONT = "OCRAEXT.TTF";

    /**** Font atlas ****/
    bool save(const std::string& path, const std::map<uint8_t, Glyph>& data) noexcept {
        using namespace glem::util;

        std::ofstream file{path, std::ios::out | std::ios::binary};

        if(file.is_open()) {
            file << "# ascii_code prop_x0 prop_y0 prop_x1 prop_y1 prop_x_offset prop_y_offset prop_advance"
                 << std::endl;

            for(auto it = data.begin(); it != data.end(); ++it)
                file << it->first << " "
                     << it->second.coord0.x << " "
                     << it->second.coord0.y << " "
                     << it->second.coord1.x << " "
                     << it->second.coord1.y << " "
                     << it->second.offset.x << " "
                     << it->second.offset.y << " "
                     << it->second.advance
                     << std::endl;

            file.flush();
            file.close();

            return true;
        }

        Log::e(TAG, "Failed to open file for writing.");

        return false;
    }

    void make(const std::string& font) noexcept {
        using namespace glem::render;
        using namespace glem::util;

        Log::d(TAG, "Loading font ", font);

        FT_Library ft;

        if(FT_Init_FreeType(&ft)) {
            Log::e(TAG, "Failed to init FT.");
            return;
        }

        FT_Face face;

        if(FT_New_Face(ft, font.data(), 0, &face)) {
            Log::e(TAG, "Failed to load font.");
            FT_Done_FreeType(ft);
            return;
        }

        if(FT_Set_Pixel_Sizes(face, 0, 96)) {
            Log::e(TAG, "Failed to set font pixel size.");
            FT_Done_Face(face);
            FT_Done_FreeType(ft);
            return;
        }

        std::map<uint8_t, Glyph> glyphs;

        const auto& ATLAS_DIMENSION = 1024u;

        /**** Load glyphs ****/
        std::vector<uint8_t> buffer;
        buffer.resize(ATLAS_DIMENSION * ATLAS_DIMENSION);

        uint32_t pen_x = 0;
        uint32_t pen_y = 0;

        for(int ascii = 33; ascii < 256; ++ascii) {
            if(FT_Load_Char(face, ascii, FT_LOAD_RENDER)) {
                Log::w(TAG, "Failed to load character ", ascii);
                continue;
            }

            const auto& bitmap = face->glyph->bitmap;

            if(pen_x + bitmap.width >= ATLAS_DIMENSION) {
                pen_x = 0;
                pen_y += (face->size->metrics.height >> 6) + 1;
            }

            for(uint32_t row = 0; row < bitmap.rows; ++row) {
                for(uint32_t column = 0; column < bitmap.width; ++column) {
                    const auto& x = pen_x + column;
                    const auto& y = pen_y + row;

                    buffer[y * ATLAS_DIMENSION + x] = bitmap.buffer[row * bitmap.pitch + column];
                }
            }

            Glyph glyph;
            glyph.coord0  = glm::vec2{static_cast<float>(pen_x),
                                      static_cast<float>(pen_y)};
            glyph.coord1  = glm::vec2{static_cast<float>(pen_x + bitmap.width),
                                      static_cast<float>(pen_y + bitmap.rows)};
            glyph.offset  = glm::vec2{static_cast<float>(face->glyph->bitmap_left),
                                      static_cast<float>(face->glyph->bitmap_top)};
            glyph.advance = (face->glyph->advance.x >> 6);

            glyphs.insert({ascii, glyph});

            pen_x += bitmap.width + 1;
        }

        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        std::vector<uint8_t> atlas;
        atlas.resize(ATLAS_DIMENSION * ATLAS_DIMENSION * 4);

        for(uint32_t i = 0; i < buffer.size(); ++i) {
            atlas[i * 4 + 0] |= buffer[i];
            atlas[i * 4 + 1] |= buffer[i];
            atlas[i * 4 + 2] |= buffer[i];
            atlas[i * 4 + 3] = 0xff;
        }

        glem::render::Surface image{atlas, ATLAS_DIMENSION, ATLAS_DIMENSION, glem::render::Format::RGBA};
        image.save("atlas.png");

        save("atlas.meta", glyphs);
    }
}

namespace glem::core {

    void DebugState::onAttach() noexcept
    {
        font_.reset(new render::Font{});

        const auto& FONT = "OCRAEXT.TTF";

        util::Log::d(TAG, "Loading font ", FONT);
        if(font_->load(FONT))
            util::Log::d(TAG, FONT, " loaded.");
        else
            util::Log::e(TAG, "Failed to load ", FONT);

        camera_.reset(new render::Camera{glm::ortho(0.0f, WIDTH, 0.0f, HEIGHT)});

        program_.reset(new render::ShaderProgram{});
        program_->append(render::Shader::fromSource(vs, render::VS));
        program_->append(render::Shader::fromSource(ps, render::PS));

        if(!program_->link()) {
            util::Log::e(TAG, "Failed to link shader program.");
            StateManager::pop();
        }
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
    }

    void DebugState::onDraw() noexcept
    {
        util::Log::d(TAG, __FUNCTION__);

        render::Renderer::begin();

//        for(const auto& s : sprites_)
//            render::Renderer::submit(s);
//        render::Renderer::submitText("Hello!!!", glm::vec2{100.0f, 100.0f}, font_);
        render::Renderer::submit(std::make_shared<render::Drawable>(glm::vec3{100.0f, 100.0f, 0.0f}, glm::vec2{500.0f, 500.0f}, glm::vec4{1.0f, 0.0f, 1.0f, 1.0f}));

        render::Renderer::end();
        render::Renderer::present();
    }

}
