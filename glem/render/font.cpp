#include "font.hpp"
#include "texture.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <util/log.hpp>

#include <fstream>

namespace {
    const std::string TAG = "Font";

    bool save(const std::string& path, const std::map<uint8_t, glem::render::Glyph>& data) noexcept {
        using namespace glem::util;

        std::ofstream file{path, std::ios::out | std::ios::binary};

        if(file.is_open()) {
            file << "# ascii_code prop_x0 prop_y0 prop_x1 prop_y1 prop_x_offset prop_y_offset prop_advance"
                 << std::endl;

            for(auto it = data.begin(); it != data.end(); ++it)
                file << it->first << " "
                     << it->second.uv[0].x << " "
                     << it->second.uv[0].y << " "
                     << it->second.uv[1].x << " "
                     << it->second.uv[1].y << " "
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
}

namespace glem::render {

    bool Font::load(const std::string &font) noexcept
    {
        FT_Library ft;

        if(FT_Init_FreeType(&ft)) {
            util::Log::e(TAG, "Failed to init FT.");
            return false;
        }

        FT_Face face;

        if(FT_New_Face(ft, font.data(), 0, &face)) {
            util::Log::e(TAG, "Failed to load font.");
            FT_Done_FreeType(ft);
            return false;
        }

        if(FT_Set_Pixel_Sizes(face, 0, 96)) {
            util::Log::e(TAG, "Failed to set font pixel size.");
            FT_Done_Face(face);
            FT_Done_FreeType(ft);
            return false;
        }

        const auto& ATLAS_DIMENSION = 1024u;

        /**** Load glyphs ****/
        std::vector<uint8_t> buffer;
        buffer.resize(ATLAS_DIMENSION * ATLAS_DIMENSION);

        uint32_t pen_x = 0;
        uint32_t pen_y = 0;

        for(int ascii = 33; ascii < 256; ++ascii) {
            if(FT_Load_Char(face, ascii, FT_LOAD_RENDER)) {
                util::Log::w(TAG, "Failed to load character ", ascii);
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
            glyph.uv.push_back(glm::vec2{static_cast<float>(pen_x),
                                         static_cast<float>(pen_y)});
            glyph.uv.push_back(glm::vec2{static_cast<float>(pen_x + bitmap.width),
                                         static_cast<float>(pen_y + bitmap.rows)});
            glyph.offset  = glm::vec2{static_cast<float>(face->glyph->bitmap_left),
                                      static_cast<float>(face->glyph->bitmap_top)};
            glyph.size    = glm::vec2{static_cast<float>(face->glyph->bitmap.width),
                                      static_cast<float>(face->glyph->bitmap.rows)};
            glyph.advance = (face->glyph->advance.x >> 6);

            if(ascii == 33) {
                util::Log::d(TAG, "u0v0: ", glyph.uv[0].x, glyph.uv[0].y);
            }

            font_.insert({ascii, glyph});

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

        render::Properties p;
        p.wrap = render::Wrap::ClampToEdge;
        p.minFilter = render::Filter::Linear;
        p.magFilter = render::Filter::Linear;

        atlas_.reset(new Texture{"ATLAS", ATLAS_DIMENSION, ATLAS_DIMENSION, p});
        atlas_->setSurface(std::make_shared<Surface>(atlas, ATLAS_DIMENSION, ATLAS_DIMENSION, Format::RGBA));

        return true;
    }

    std::shared_ptr<Texture> Font::atlas() const noexcept
    {
        return atlas_;
    }

    std::map<uint8_t, Glyph> Font::font() const noexcept
    {
        return font_;
    }

}
