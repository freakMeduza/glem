#pragma once

#include <string>
#include <memory>

namespace glem::render {

    class Atlas;

    class Font {
    public:
        Font();
        ~Font();

        bool load(const std::string& font, uint32_t size) noexcept;

        std::shared_ptr<Atlas> atlas() const noexcept;

    private:
        std::shared_ptr<Atlas> atlas_;

    };

}
