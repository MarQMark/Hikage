#ifndef HIKAGE_TEXTURESVIEW_H
#define HIKAGE_TEXTURESVIEW_H

#include <string>

#include "Viewable.h"

class TexturesView : public Viewable {
public:
    TexturesView();

    void render() override;

private:

    std::string _new_txt_path;
    bool _render_new_txt = false;
    void render_new_txt();

    int _del_txt_idx = -1;
    bool _render_del_txt = false;
    void render_del_txt();
};

#endif //HIKAGE_TEXTURESVIEW_H
