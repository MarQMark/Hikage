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
    void render_delete_script();
};


#endif //HIKAGE_MENUBAR_H
