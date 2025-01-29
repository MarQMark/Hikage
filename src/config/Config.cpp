#include "config/Config.h"
#include <filesystem>
#include <fstream>
#include "json/json.hpp"

Config* Config::s_instance = nullptr;

void Config::save() {

    nlohmann::json j;

    j["Version"] = _version;

    for(int i = 0; i < (int)_resent_projects.size(); i++)
        j["Resent Projects"][i] = _resent_projects[i];

    std::ofstream file(s_instance->configFilePath().c_str());
    file << std::setw(4) << j << std::endl;
    file.close();
}

int Config::open() {
    s_instance = new Config;

    if(!std::filesystem::exists(s_instance->configFilePath().c_str())){
        std::filesystem::create_directory(s_instance->configPath().c_str());
        s_instance->create_blank();
        return 0;
    }

    if(!std::filesystem::exists(s_instance->projectPath().c_str())){
        std::filesystem::create_directory(s_instance->projectPath().c_str());
    }

    std::ifstream f(s_instance->configFilePath().c_str());
    nlohmann::json j;

    try {
        j = nlohmann::json::parse(f);
    } catch (const nlohmann::json::parse_error &e) {
        printf("[ERROR] opening %s Invalid Json\n", s_instance->configFilePath().c_str());
        return -1;
    }

    if(j.contains("Version")){
        s_instance->_version = j["Version"];
        if(s_instance->_version != HIKAGE_VERSION){
            printf("[ERROR] Incompatible Version %s - %s \n", HIKAGE_VERSION, s_instance->_version.c_str());
            return -1;
        }

    }
    else{
        printf("[ERROR] No Version in config. Unable to verify compatibility\n");
        return -1;
    }

    if(j.contains("Resent Projects"))
        for(const auto & i : j["Resent Projects"])
            s_instance->_resent_projects.push_back(i);

}

void Config::create_blank() {
    _version = HIKAGE_VERSION;

    save();
}

std::string Config::configPath() {
    return _path;
}

std::string Config::configFileName() {
    return CONFIG_FILE_NAME;
}

std::string Config::configFilePath() {
    return std::string(configPath() + configFileName());
}

std::string Config::projectPath() {
    return std::string(configPath() + PROJECT_PATH);
}

void Config::addRecentProject(const std::string& project) {
    auto it = std::find(_resent_projects.begin(), _resent_projects.end(), project);
    if (it != _resent_projects.end())
        _resent_projects.erase(it);

    _resent_projects.insert(_resent_projects.begin(), project);

    save();
}

std::vector<std::string> Config::getRecentProjects() {
    return _resent_projects;
}
