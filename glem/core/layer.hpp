#pragma once

namespace glem::core {

    /**
     * @brief The Layer class
     *
     * Interface for render layer
     */
    class Layer {
    public:
        Layer() = default;
        virtual ~Layer() = default;

        /**
         * @brief onAttach
         */
        virtual void onAttach()         noexcept = 0;

        /**
         * @brief onDetach
         */
        virtual void onDetach()         noexcept = 0;

        /**
         * @brief onUpdate
         * @param dt
         */
        virtual void onUpdate(float dt) noexcept = 0;

        /**
         * @brief onDraw
         */
        virtual void onDraw()           noexcept = 0;

    };

}
