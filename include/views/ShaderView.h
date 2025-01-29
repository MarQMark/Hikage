#ifndef HIKAGE_SHADERVIEW_H
#define HIKAGE_SHADERVIEW_H

#include "Viewable.h"
#include "rendering/Viewport.h"

class ShaderView : public Viewable{
public:
    ShaderView() = default;

    void render() override;

    void setViewport(Viewport* viewport);
private:

    Viewport* _viewport = nullptr;
};


#endif //HIKAGE_SHADERVIEW_H
