#ifndef HIKAGE_NEWPROJECT_H
#define HIKAGE_NEWPROJECT_H

#include <string>
#include <vector>
#include "Viewable.h"

class NewProject : public Viewable{
public:
    NewProject();

    void render() override;

    void setVisible(bool visible) override;

private:

    void create_project();
    void clean();

    bool _valid_name = true;
    bool _valid_path = true;
    std::string _name;
    std::string _path;
    bool validate_name();
    bool validate_path();
};


#endif //HIKAGE_NEWPROJECT_H
