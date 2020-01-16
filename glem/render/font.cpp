#include "font.hpp"
#include "atlas.hpp"

#include <util/log.hpp>

namespace {
    const std::string TAG = "Font";

    FT_Library ft;
}

namespace glem::render {

    Font::Font()
    {
        if(FT_Init_FreeType(&ft)) {
            util::Log::e(TAG, "Failed to init FT.");
            return;
        }
    }

    Font::~Font()
    {
        FT_Done_FreeType(ft);
    }

    bool Font::load(const std::string &font, uint32_t size) noexcept
    {
        FT_Face face;

        if(FT_New_Face(ft, font.data(), 0, &face)) {
            util::Log::e(TAG, "Failed to load font ", font);
            return false;
        }

        atlas_.reset(new Atlas{face, size});

        FT_Done_Face(face);

        return true;
    }

    std::shared_ptr<Atlas> Font::atlas() const noexcept
    {
        return atlas_;
    }

}
