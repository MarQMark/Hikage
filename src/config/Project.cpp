#include "config/Project.h"

Project* Project::s_instance = nullptr;

int Project::open(char *path) {
    s_instance = new Project;
    s_instance->_path = std::string(path);
    s_instance->_last_time = std::filesystem::last_write_time(path);

    return 1;
}

void Project::save() {

}

void Project::create(char *path) {

}

bool Project::changed() {
    std::filesystem::file_time_type time = std::filesystem::last_write_time(_path.c_str());
    if(_last_time != time){
        _last_time = time;
        return true;
    }

    return false;
}
