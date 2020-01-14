#pragma once

#include <memory>
#include <stdint.h>

namespace glem::render {

    /**
     * @brief The Bindable class
     *
     * Interface of any bindable object
     */
    class Bindable {
    public:
        Bindable() = default;
        virtual ~Bindable() = default;

        /**
         * @brief bind
         * @param unit
         */
        virtual void bind(uint32_t unit = 0) noexcept = 0;

        /**
         * @brief id        Object id
         * @return
         */
        inline uint32_t id() const noexcept { return id_; }

    protected:
        uint32_t id_ {0};

    };

}
