#pragma once

#include "layer.hpp"

#include <vector>
#include <memory>

namespace glem::core {

    class LayerManager {
    public:
        LayerManager() = delete;
        ~LayerManager() = delete;

        LayerManager(LayerManager&&) = delete;
        LayerManager(const LayerManager&) = delete;

        LayerManager& operator=(LayerManager&&) = delete;
        LayerManager& operator=(const LayerManager&) = delete;

        static void attach(const std::shared_ptr<Layer>& layer) noexcept;
        static void detach(const std::shared_ptr<Layer>& layer) noexcept;

        static void onUpdate(float dt) noexcept;
        static void onDraw() noexcept;

    private:
        static std::vector<std::shared_ptr<Layer>> layers_;

    };

}
