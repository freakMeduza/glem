#include "font.hpp"

#include "texture.hpp"
#include "surface.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <util/log.hpp>

#include <algorithm>

namespace {
    const std::string TAG = "Atlas";

    const uint32_t ATLAS_DIMENSION = 1024u;
    const uint32_t ATLAS_PADDING   = 1u;
}

namespace glem::render {

    bool Font::load(const std::string &font, uint32_t size) noexcept
    {
        FT_Library ft;

        if(FT_Init_FreeType(&ft)) {
            util::Log::e(TAG, "Init FT failed.");
            return false;
        }

        FT_Face face;

        if(FT_New_Face(ft, font.data(), 0, &face)) {
            util::Log::e(TAG, "Load FT face failed.");
            return false;
        }

        FT_Set_Pixel_Sizes(face, 0, size);

        auto&& g = face->glyph;

        uint32_t offset_x {0};
        uint32_t offset_y {0};

        uint32_t width  {0};
        uint32_t height {0};

        /**** compute atlas size ****/
        for(uint8_t ascii = 32; ascii < 128; ++ ascii) {
            if(FT_Load_Char(face, ascii, FT_LOAD_RENDER)) {
                util::Log::e(TAG, "Failed to load character ", ascii);
                continue;
            }

            if(offset_x + g->bitmap.width + ATLAS_PADDING >= ATLAS_DIMENSION) {
                width = std::max(width, offset_x);
                height += offset_y;

                offset_x = 0;
                offset_y = 0;
            }

            offset_x += g->bitmap.width + ATLAS_PADDING;
            offset_y = std::max(offset_y, g->bitmap.rows);
        }

        width = std::max(width, offset_x);
        height += offset_y;

        util::Log::d(TAG, "Atlas size: ", width, "x ", height);

        render::Properties p;
        p.wrap      = render::Wrap::ClampToEdge;
        p.minFilter = render::Filter::Linear;
        p.magFilter = render::Filter::Linear;
        p.format    = render::Format::R;

        atlas_.reset(new Texture{font, width, height, p});

        uint32_t pen_x {0};
        uint32_t pen_y {0};

        offset_y = 0;

        /**** load glyphs ****/
        for(uint8_t ascii = 32; ascii < 128; ++ascii) {
            if(FT_Load_Char(face, ascii, FT_LOAD_RENDER)) {
                util::Log::e(TAG, "Loading character ", ascii, "failed.");
                continue;
            }

            if(pen_x + g->bitmap.width + ATLAS_PADDING >= ATLAS_DIMENSION) {
                pen_y += offset_y;
                pen_x = 0;
                offset_y = 0;
            }

            atlas_->setSubSurface(g->bitmap.buffer, {g->bitmap.width, g->bitmap.rows}, {pen_x, pen_y});

            Glyph glyph;
            glyph.ascii   = ascii;
            glyph.size    = {g->bitmap.width,   g->bitmap.rows};
            glyph.advance = {g->advance.x >> 6, g->advance.y >> 6};
            glyph.bearing = {g->bitmap_left,    g->bitmap_top};
            glyph.uv      = {pen_x / static_cast<float>(width),
                             pen_y / static_cast<float>(height)};

            glyphs_.emplace_back(glyph);

            offset_y = std::max(offset_y, g->bitmap.rows);

            pen_x += g->bitmap.width + ATLAS_PADDING;
        }

        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        return true;
    }

    std::shared_ptr<Texture> Font::atlas() const noexcept
    {
        return atlas_;
    }

    const std::vector<Glyph>& Font::glyphs() const noexcept
    {
        return glyphs_;
    }

}
