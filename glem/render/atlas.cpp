#include "atlas.hpp"

#include "surface.hpp"

#include <glad/glad.h>

#include <util/log.hpp>

namespace {
    const std::string TAG = "Atlas";

    const auto& ATLAS_MAX_DIMENSION = 1024u;
}

namespace glem::render {

    Atlas::Atlas(FT_Face face, uint32_t size)
    {
        FT_Set_Pixel_Sizes(face, 0, size);

        FT_GlyphSlot g = face->glyph;

        uint32_t width  {0};
        uint32_t height {0};

        uint32_t roww = 0;
        uint32_t rowh = 0;

        for(uint8_t i = 32; i < 128; ++i) {
            if(FT_Load_Char(face, i, FT_LOAD_RENDER)) {
                util::Log::e(TAG, "Loading character ", i, "failed.");
                continue;
            }

            if(roww + g->bitmap.width + 1 >= ATLAS_MAX_DIMENSION) {
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

        glCreateTextures(GL_TEXTURE_2D, 1, &id_);
        glBindTexture(GL_TEXTURE_2D, id_);
        glTextureStorage2D(id_, 1, GL_R8, width, height);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTextureParameteri(id_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(id_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(id_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(id_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        int offset_x = 0;
        int offset_y = 0;

        rowh = 0;

        for(int i = 32; i < 128; ++i) {
            if(FT_Load_Char(face, i, FT_LOAD_RENDER)) {
                util::Log::e(TAG, "Loading character ", i, "failed.");
                continue;
            }

            if(offset_x + g->bitmap.width + 1 >= ATLAS_MAX_DIMENSION) {
                offset_y += rowh;
                offset_x = 0;
                rowh = 0;
            }

            glTextureSubImage2D(id_, 0, offset_x, offset_y, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

            Glyph glyph;
            glyph.ascii = i;
            glyph.size.x = g->bitmap.width;
            glyph.size.y = g->bitmap.rows;
            glyph.advance.x = g->advance.x >> 6;
            glyph.advance.y = g->advance.y >> 6;
            glyph.bearing.x = g->bitmap_left;
            glyph.bearing.y = g->bitmap_top;
            glyph.uv.x = offset_x / static_cast<float>(width);
            glyph.uv.y = offset_y / static_cast<float>(height);

            glyphs_.emplace_back(glyph);

            rowh = std::max(rowh, g->bitmap.rows);

            offset_x += g->bitmap.width + 1;
        }

        util::Log::d(TAG, "Atlas ", width, "x ", height, width * height / 1024);

        std::vector<uint8_t> pixels;
        pixels.resize(width * height);

        glGetTextureImage(id_, 0, GL_RED, GL_UNSIGNED_BYTE, pixels.size() * sizeof(uint8_t), pixels.data());

        // TODO: add Format R;
//        surface_.reset(new Surface{pixels, width, height, Format::RGB});
//        surface_->save("test.jpg");
    }

    Atlas::~Atlas()
    {
        glDeleteTextures(1, &id_);
    }

    void Atlas::bind(uint32_t unit) noexcept
    {
        glBindTextureUnit(unit, id_);
    }

}
