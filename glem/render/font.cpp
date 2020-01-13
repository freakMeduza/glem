#include "font.hpp"
#include "surface.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftglyph.h>

#include <util/log.hpp>

#include <vector>
#include <sstream>
#include <fstream>
#include <complex>
#include <algorithm>

namespace {
    const std::string TAG = "Font";

    const uint32_t ATLAS_DIMENSION = 1024;
    const uint32_t ATLAS_COLUMNS   = 16;
    const uint32_t ATLAS_PADDING   = 6;
    const uint32_t ATLAS_SLOT      = 64;
    const uint32_t ATLAS_GLYPH     = ATLAS_SLOT - ATLAS_PADDING;

    const std::string ATLAS_OUTPUT_PNG  = "atlas.png";
    const std::string ATLAS_OUTPUT_META = "atlas.meta";
}

namespace glem::render {

    class FontPrivate final {
    public:
        FontPrivate() {
            if(FT_Init_FreeType(&ft_))
                util::Log::e(TAG, "Unable to init FT.");
        }

        ~FontPrivate() {
            FT_Done_FreeType(ft_);
        }

        FT_Library ft_ {nullptr};

    };

    Font::Font() : d_ptr_ {new FontPrivate{}}
    {

    }

    Font::~Font()
    {

    }

    bool Font::makeAtlas(const std::string &font) noexcept
    {
        FT_Face face;

        if(FT_New_Face(d_ptr_->ft_, font.data(), 0, &face)) {
            util::Log::d(TAG, "Failed to load font ", font);
            return false;
        }

        if(FT_Set_Pixel_Sizes(face, 0, ATLAS_GLYPH)) {
            util::Log::e(TAG, "Failed to set pixel size.");
            return false;
        }

        std::vector<uint8_t> atlas;
        atlas.resize(ATLAS_DIMENSION * ATLAS_DIMENSION * 4);

        struct Glyph {
            uint8_t character {0};

            glm::ivec2 size {glm::ivec2{0}};

            uint32_t pitch  {0};
            uint32_t offset {0};
        };

        std::vector<Glyph> glyphs;

        for(int i = 33; i < 256; ++i) {
            if(FT_Load_Char(face, i, FT_LOAD_RENDER)) {
                util::Log::e(TAG, "Failed to load character ", static_cast<char>(i));
                return false;
            }

            FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

            FT_Glyph ftGlyph;

            if(FT_Get_Glyph(face->glyph, &ftGlyph)) {
                util::Log::e(TAG, "Failed to get glyph.");
                return false;
            }

            FT_BBox bbox;

            FT_Glyph_Get_CBox(ftGlyph, FT_GLYPH_BBOX_TRUNCATE, &bbox);

            const auto& bitmap = face->glyph->bitmap;

            Glyph glyph;
            glyph.character = static_cast<uint8_t>(i);
            glyph.size      = glm::ivec2{bitmap.width, bitmap.rows};
            glyph.pitch     = bitmap.pitch;
            glyph.offset    = bbox.yMin;

            glyphs.emplace_back(glyph);
        }

        auto saveAtlas = [](){

        };

        auto saveMeta = [](const std::string& path, const std::vector<Glyph>& data){
            std::ofstream file{path, std::ios::out | std::ios::binary};

            if(file.is_open()) {
                file << "# ascii_code prop_xMin prop_width, prop_yMin prop_height prop_offset"
                     << std::endl;

                /**** unique line for 'space' character ****/
                file << "32 0 " << 0.5f << " 0 " << 1.0f << " " << 0 << std::endl;

                for(int i = 33; i < 256; ++i) {
                    const auto& order  = i - 32;
                    const auto& column = order / ATLAS_COLUMNS;
                    const auto& row    = order % ATLAS_COLUMNS;

                    const auto& x_min = static_cast<float>(column * ATLAS_SLOT) / static_cast<float>(ATLAS_DIMENSION);
                    const auto& y_min = static_cast<float>(row    * ATLAS_SLOT) / static_cast<float>(ATLAS_DIMENSION);

                    if(auto it = std::find_if(data.begin(), data.end(), [&i](const Glyph& g){ return (g.character == i); }); it != data.end()) {
                        file << i << " "
                             << x_min << " "
                             << static_cast<float>((*it).size.x + ATLAS_PADDING) / static_cast<float>(ATLAS_SLOT) << " "
                             << y_min << " "
                             << static_cast<float>((*it).size.y + ATLAS_PADDING) / static_cast<float>(ATLAS_SLOT) << " "
                             << -(static_cast<float>(ATLAS_PADDING) - static_cast<float>((*it).offset) / static_cast<float>(ATLAS_SLOT))
                             << std::endl;
                    }
                    else
                        util::Log::w(TAG, "Can't find glyph ", i);
                }

                file.flush();
                file.close();

                return true;
            }

            util::Log::e(TAG, "Unable to open file for writing ", path);

            return false;
        };

        return saveMeta(ATLAS_OUTPUT_META, glyphs);

//        /**** dirty maximum dimension ****/
//        uint32_t dimension = (1 + (face->size->metrics.height >> 6) * ceilf(sqrtf(CHARACTER_NUMBER)));

//        /**** texture side ****/
//        uint32_t side {1};

//        while(side < dimension)
//            side <<= 1;

//        util::Log::d(TAG, "Texture size: ", side, "x ", side);

//        std::vector<Glyph> glyphs;

//        std::vector<uint8_t> pixels;
//        pixels.resize(side * side);

//        uint32_t pen_x {0};
//        uint32_t pen_y {0};

//        for(int i = 33; i < 256; ++i) {
//            if(FT_Load_Char(face, i, FT_LOAD_RENDER)) {
//                util::Log::e(TAG, "Unable to load character ", static_cast<char>(i));
//                return false;
//            }

//            const auto& bitmap = &face->glyph->bitmap;

//            /*** new line in texture ****/
//            if(pen_x + bitmap->width >= side) {
//                pen_x = 0;
//                pen_y += ((face->size->metrics.height >> 6) + 1);
//            }

//            for(uint32_t row = 0; row < bitmap->rows; ++row) {
//                for(uint32_t columns = 0; columns < bitmap->width; ++columns) {
//                    int x = pen_x + columns;
//                    int y = pen_y + row;

//                    pixels[y * side + x] = bitmap->buffer[row * bitmap->pitch + columns];
//                }
//            }

//            pen_x += bitmap->width + 1;

//            glyphs.emplace_back(Glyph{static_cast<uint8_t>(i),
//                                      glm::ivec2{pen_x, pen_y},
//                                      glm::ivec2{pen_x + bitmap->width, pen_x + bitmap->rows},
//                                      glm::ivec2{face->glyph->bitmap.width, face->glyph->bitmap.rows},
//                                      glm::ivec2{face->glyph->bitmap_left, face->glyph->bitmap_top},
//                                      static_cast<uint32_t>((face->glyph->advance.x) >> 6)});
//        }

//        for(auto&& g : glyphs) {
//            util::Log::d(TAG,
//                         "Character ",
//                         static_cast<char>(g.character),
//                         " coord0: ",
//                         g.coord0.x,
//                         "x ",
//                         g.coord0.y,
//                         " coord1: ",
//                         g.coord1.x,
//                         "x ",
//                         g.coord1.y);
//        }

//        FT_Done_Face(face);

//        std::vector<uint8_t> png;
//        png.resize(side * side * 4);

//        for(uint32_t i = 0; i < pixels.size(); ++i) {
//            png[i * 4 + 0] |= pixels[i];
//            png[i * 4 + 1] |= pixels[i];
//            png[i * 4 + 2] |= pixels[i];
//            png[i * 4 + 3] = 0xff;
//        }

//        /**** save atlas ****/
//        Surface image{png, side, side, Format::RGBA};

//        image.save(ATLAS_OUTPUT_PNG);

//        /**** save gliphs ****/
//        auto save = [](const std::string& path, const std::vector<Glyph>& data){
//            std::ofstream file{path, std::ios::out | std::ios::binary};

//            if(file.is_open()) {
//                file << "ascii_code prop_x0 prop_y0 prop_x1 prop_y1 prop_width prop_height prop_left_offset prop_top_offset prop_advance"
//                     << std::endl
//                     << std::endl;

//                for(auto glyph : data) {
//                    file << glyph.character << " "
//                         << glyph.coord0.x  << " " << glyph.coord0.y  << " "
//                         << glyph.coord1.x  << " " << glyph.coord1.y  << " "
//                         << glyph.size.x    << " " << glyph.size.y    << " "
//                         << glyph.bearing.x << " " << glyph.bearing.y << " "
//                         << glyph.advance
//                         << std::endl;
//                }

//                file.flush();
//                file.close();

//                return true;
//            }

//            util::Log::e(TAG, "Unable to open file ", path, " for writing.");

//            return false;
//        };

//        save(ATLAS_OUTPUT_META, glyphs);

//        return true;
    }

}
