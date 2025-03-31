#ifndef HIKAGE_MENUBAR_H
#define HIKAGE_MENUBAR_H

#include "Viewable.h"
#include "config/Project.h"

class Menubar : public Viewable{
public:
    Menubar() = default;

    void render() override;

private:
    static void open_project(Project* project);

    bool _delete_script = false;
    bool _info_const = false;
    bool _info_func = false;

    void render_delete_script();
    void render_info_const();
    void render_info_func();
};


#endif //HIKAGE_MENUBAR_H
