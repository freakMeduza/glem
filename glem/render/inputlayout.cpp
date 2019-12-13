#include "inputlayout.hpp"

namespace {

}

namespace glem::render {

    InputLayout::InputLayout(const std::initializer_list<Attribute> &attributes) :
        attributes_ {attributes}
    {
        size_t offset {0};

        for(auto&& attr : attributes_) {
            attr.offset_ = offset;

            offset += attr.size();
            stride_ += attr.size();
        }
    }

    size_t InputLayout::stride() const noexcept
    {
        return stride_;
    }

    const std::vector<Attribute> &InputLayout::attributes() const noexcept
    {
        return attributes_;
    }

}
