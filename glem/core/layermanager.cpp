#include "layermanager.hpp"

#include <algorithm>

namespace glem::core {

    void LayerManager::attach(const std::shared_ptr<Layer> &layer) noexcept
    {
        layer->onAttach();

        layers_.emplace_back(layer);
    }

    void LayerManager::detach(const std::shared_ptr<Layer> &layer) noexcept
    {
        auto it = std::find(layers_.begin(), layers_.end(), layer);

        if(it != layers_.end()) {
            (*it)->onDetach();

            layers_.erase(it);
        }
    }

    void LayerManager::onUpdate(float dt) noexcept
    {
        for(auto&& l : layers_)
            l->onUpdate(dt);
    }

    void LayerManager::onDraw() noexcept
    {
        for(auto&& l : layers_)
            l->onDraw();
    }

    LayerManager::LayerManager()
    {

    }

    LayerManager::~LayerManager()
    {
//        for(auto l : layers_)
//            detach(l);
    }

}
