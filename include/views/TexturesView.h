#ifndef HIKAGE_TEXTURESVIEW_H
#define HIKAGE_TEXTURESVIEW_H

#include <string>

#include "Viewable.h"

class TexturesView : public Viewable {
public:
    TexturesView();

    void render() override;

private:

    bool _render_new_txt = false;
    std::string _new_txt_path;
    void render_new_txt();
};

#endif //HIKAGE_TEXTURESVIEW_H
