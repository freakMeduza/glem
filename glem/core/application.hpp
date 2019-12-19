#pragma once

#include "window.hpp"
#include "layermanager.hpp"

namespace glem::core {

    class Application {
    public:
        Application();
        ~Application();

        Application(Application&&) = delete;
        Application(const Application&) = delete;

        Application& operator=(Application&&) = delete;
        Application& operator=(const Application&) = delete;

        /**
         * @brief attach
         * @param layer
         */
        void attach(const std::shared_ptr<Layer>& layer) noexcept;

        /**
         * @brief detach
         * @param layer
         */
        void detach(const std::shared_ptr<Layer>& layer) noexcept;

        /**
         * @brief exec
         * @return
         */
        int exec() noexcept;

    private:
        std::unique_ptr<Window>       window_ {nullptr};
        std::unique_ptr<LayerManager> layerManager_ {nullptr};

    };

}
