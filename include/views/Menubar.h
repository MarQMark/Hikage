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
};


#endif //HIKAGE_MENUBAR_H
