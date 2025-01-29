#include "config/Config.h"
#include <filesystem>
#include <fstream>
#include "json/json.hpp"

Config* Config::s_instance = nullptr;

void Config::save() {

    nlohmann::json j;

    j["Version"] = _version;


    std::ofstream file(CONFIG_FILE_PATH);
    file << std::setw(4) << j << std::endl;
    file.close();
}

int Config::open() {
    s_instance = new Config;

    if(!std::filesystem::exists(CONFIG_FILE_PATH)){
        std::filesystem::create_directory(CONFIG_PATH);
        s_instance->create_blank();
        return 0;
    }

    std::ifstream f(CONFIG_FILE_PATH);
    nlohmann::json j;

    try {
        j = nlohmann::json::parse(f);
    } catch (const nlohmann::json::parse_error &e) {
        printf("[ERROR] opening " CONFIG_FILE_PATH " Invalid Json\n");
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


}

void Config::create_blank() {
    _version = HIKAGE_VERSION;

    save();
}
