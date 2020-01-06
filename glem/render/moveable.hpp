#pragma once

#include <glm/glm.hpp>

namespace glem::render {

    /**
     * @brief The Moveable class
     *
     * Interface of any moveable object
     */
    class Moveable {
    public:
        enum Movement {
            Up,
            Down,
            Left,
            Right
        };

        Moveable() = default;
        virtual ~Moveable() = default;

        /**
         * @brief move
         * @param movement
         * @param dt
         */
        virtual void move(Movement movement, float dt) noexcept = 0;

    };

}
