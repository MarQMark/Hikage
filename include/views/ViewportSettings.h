#ifndef HIKAGE_VIEWPORTSETTINGS_H
#define HIKAGE_VIEWPORTSETTINGS_H

#include "Viewable.h"
#include "rendering/Viewport.h"

class ViewportSettings : public Viewable{
public:
    ViewportSettings();

    void render() override;
    void setVisible(bool visible) override;

    void setViewport(Viewport* viewport);

private:
    int _width = 0;
    int _height = 0;

    Viewport* _viewport = nullptr;
};


#endif //HIKAGE_VIEWPORTSETTINGS_H
