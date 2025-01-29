#include <fstream>
#include <utility>
#include "config/Project.h"
#include "config/Config.h"
#include "json/json.hpp"

Project* Project::s_instance = nullptr;
bool Project::_opened = false;

Project::Project(std::string name, const std::string& path) : _name(std::move(name)) {
    if(path[path.size() - 1] == '/')
        _path = path + _name + "/";
    else
        _path = path + "/" + _name + "/";
}

Project::Project(const std::string& projPath) {
    size_t pos = projPath.rfind('/');

    if (pos != std::string::npos) {
        _path = projPath.substr(0, pos);

        pos = _path.rfind('/');
        _name = _path.substr(pos + 1);

        _path += "/";
    } else {
        printf("[ERROR] Loading path of project %s\n", _path.c_str());
    }
}

void Project::close() {
    save();
    delete this;
    s_instance = nullptr;
}

int Project::open(Project* project) {
    if(s_instance)
        s_instance->close();

    s_instance = project;
    Config::get()->addRecentProject(project->_path);

    _opened = true;

    return 1;
}

int Project::load() {

    std::ifstream f(_path  + PROJECT_FILE_NAME);
    nlohmann::json data;

    try {
        data = nlohmann::json::parse(f);
    } catch (const nlohmann::json::parse_error& e) {
        printf("Error opening %s Invalid Json\n", _name.c_str());
        return -1;
    }

    if(!data.contains("Version")){
        printf("Error opening %s Version not found\n", _name.c_str());
        return -1;
    }

    _version = data["Version"];
    if(_version != HIKAGE_VERSION){
        printf("Error incompatible Version %s, needed %s\n", _name.c_str(), HIKAGE_VERSION);
        return -1;
    }

    if(data.contains("Project Files")){
        for(const auto & i : data["Project Files"]){
            addFile(i);
        }
    }

    if(data.contains("Width"))
        _width = data["Width"];
    if(data.contains("Height"))
        _height = data["Height"];

    return 1;
}

void Project::save() {
    nlohmann::json j;
    j["Version"] = _version;

    j["Width"] = _width;
    j["Height"] = _height;

    for(size_t i = 0; i < _files.size(); i++){
        j["Project Files"][i] = _files[i]->path;
    }

    std::ofstream file(_path + PROJECT_FILE_NAME);
    file << std::setw(4) << j << std::endl;
    file.close();
}

void Project::create() {

    _version = HIKAGE_VERSION;

    if(!std::filesystem::exists(_path.c_str())){
        std::filesystem::create_directory(_path.c_str());
    }

    std::ofstream file(_path + "default.frag");
    file.close();
    addFile(_path + "default.frag");

    save();
}

std::string Project::getName() {
    return _name;
}

std::string Project::getPath() {
    return _path;
}

bool Project::changed() {

    for(auto* file: _files){
        std::filesystem::file_time_type time = std::filesystem::last_write_time(file->path);
        if(file->last_time != time){
            file->last_time = time;
            return true;
        }
    }

    return false;
}

void Project::addFile(std::string path) {
    auto* file = new File;
    file->path = std::move(path);
    file->last_time = std::filesystem::last_write_time(_path.c_str());
    _files.push_back(file);

    save();
}

std::vector<Project::File *> Project::getFiles() {
    return _files;
}

bool Project::opened() {
    if(_opened){
        _opened = false;
        return true;
    }

    return false;
}

void Project::setDimensions(int w, int h) {
    _width = w;
    _height = h;

    save();
}

int Project::getWidth() const {
    return _width;
}

int Project::getHeight() const {
    return _height;
}

