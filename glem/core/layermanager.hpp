#pragma once

#include "layer.hpp"

#include <vector>
#include <memory>

namespace glem::core {

    class LayerManager {
    public:
        ~LayerManager();

        LayerManager(LayerManager&&) = delete;
        LayerManager(const LayerManager&) = delete;

        LayerManager& operator=(LayerManager&&) = delete;
        LayerManager& operator=(const LayerManager&) = delete;

        void attach(const std::shared_ptr<Layer>& layer) noexcept;
        void detach(const std::shared_ptr<Layer>& layer) noexcept;

        void onUpdate(float dt) noexcept;
        void onDraw() noexcept;

    private:
        friend class Application;

        LayerManager();

        std::vector<std::shared_ptr<Layer>> layers_;

    };

}
