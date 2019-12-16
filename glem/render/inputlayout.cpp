#include "inputlayout.hpp"

namespace glem::render {

    InputLayout::InputLayout(const std::initializer_list<Attribute> &attributes) :
        attributes_ {attributes}
    {
        uint32_t offset {0};

        for(auto&& attr : attributes_) {
            attr.offset_ = offset;

            offset += attr.size();
            stride_ += attr.size();
        }
    }

    uint32_t InputLayout::stride() const noexcept
    {
        return stride_;
    }

    const std::vector<Attribute> &InputLayout::attributes() const noexcept
    {
        return attributes_;
    }

}
