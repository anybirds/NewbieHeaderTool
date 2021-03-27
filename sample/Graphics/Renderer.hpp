#pragma once

#include <Component.hpp>

namespace Engine {
    class ENGINE_EXPORT [[Serialize]] Renderer : public Component {
        TYPE(Renderer)
    
    private:
        unsigned order;
    
    public:
        Renderer() : order(0U) {}
        virtual void Render() = 0;

        unsigned GetOrder() const { return order; }
        void SetOrder(unsigned order);
    };
    
    class RendererComparer {
    public:
        bool operator()(Renderer *l, Renderer *r) const {
            return l->GetOrder() < r->GetOrder();
        }
    };
}