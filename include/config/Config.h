#ifndef HIKAGE_CONFIG_H
#define HIKAGE_CONFIG_H

#include <string>
#include <vector>

#define CONFIG_PATH ".hikage/"
#define CONFIG_FILE_NAME "hikage.conf"
#define PROJECT_PATH "projects/"
#define PROJECT_FILE_NAME "hikage.proj"
#define PROJECT_TXT_PATH "txt/"

#define HIKAGE_VERSION "0.1.0"

class Config {
private:
    Config() {
#ifdef _WIN32
        //snprintf(homedir, MAX_PATH, "%s%s", getenv("HOMEDRIVE"), getenv("HOMEPATH"));
        printf("[ERROR] Config Path not implemented for Windows\n")
#elif __linux__
        char* path = getenv("HOME");
        _path = std::string(path);
        _path += "/" CONFIG_PATH;
#else
        printf("[ERROR] Config Path not implemented for OS\n")
#endif
        printf("[VERBOSE] Config path: %s\n", _path.c_str());
    };

    static Config* s_instance;

public:
    static Config* get(){
        return s_instance;
    }

    void save();
    static int open();

    std::string configPath();
    static std::string configFileName();
    std::string configFilePath();
    std::string projectPath();

    void addRecentProject(const std::string& project);
    std::vector<std::string> getRecentProjects();

private:

    void create_blank();

    std::string _path;
    std::string _version{};

    std::vector<std::string> _resent_projects;
};


#endif //HIKAGE_CONFIG_H
