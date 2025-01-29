#ifndef HIKAGE_STARTMENU_H
#define HIKAGE_STARTMENU_H

#include "Viewable.h"
#include "config/Project.h"

class StartMenu : public Viewable{
public:
    StartMenu() = default;

    void render() override;

private:
    static void open_project(Project* project);
};


#endif //HIKAGE_STARTMENU_H
