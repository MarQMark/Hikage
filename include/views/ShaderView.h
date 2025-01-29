#ifndef HIKAGE_SHADERVIEW_H
#define HIKAGE_SHADERVIEW_H

#include "Viewable.h"

class ShaderView : public Viewable{
public:
    ShaderView() = default;

    void render() override;

private:
};


#endif //HIKAGE_SHADERVIEW_H
