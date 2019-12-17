#pragma once

#include "context.hpp"
#include "visitor.hpp"
#include "bindable.hpp"

#include "indexbuffer.hpp"

#include <vector>
#include <memory>

namespace glem::render {

    /**
     * @brief The Drawable class
     *
     * Interface of any drawable object
     */
    class Drawable : public Visitor {
    public:
        Drawable() = default;
        virtual ~Drawable() override = default;

        Drawable(Drawable&&) = delete;
        Drawable(const Drawable&) = delete;

        Drawable& operator=(Drawable&&) = delete;
        Drawable& operator=(const Drawable&) = delete;

        // Visitor interface
        void onAppend(IndexBuffer   &value) noexcept override;
        void onAppend(VertexBuffer  &value) noexcept override;
        void onAppend(VertexArray   &value) noexcept override;
        void onAppend(ShaderProgram &value) noexcept override;

        /**
         * @brief append  Append bindable
         * @param value
         */
        void append(const std::shared_ptr<Bindable>& value) noexcept;

        /**
         * @brief draw   Draw object
         * @param context
         */
        void draw(Context& context) noexcept;

    private:
        std::vector<std::shared_ptr<Bindable>> bindables_;

        IndexBuffer* indexBuffer_ {nullptr};


    };

}
