#pragma once

#include "Shader.hpp"
#include "Bindable.hpp"

#include <memory>
#include <vector>

namespace glem {

    class Program : public Bindable {
    public:
        Program();
        ~Program();

        Program(Program&&) = delete;
        Program(const Program&) = delete;

        Program& operator=(Program&&) = delete;
        Program& operator=(const Program&) = delete;

        // Bindable interface
        void bind() const noexcept override;
        void unbind() const noexcept override;

        /**
         * @brief Append shader to program
         * @param value - shader
         */
        void append(std::unique_ptr<Shader> value) noexcept;

        /**
         * @brief Link program
         * @return
         */
        bool link() const noexcept;

    private:
        mutable std::vector<std::unique_ptr<Shader>> shaders_;

    };

}
