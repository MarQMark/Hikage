#ifndef HIKAGE_CONFIG_H
#define HIKAGE_CONFIG_H

#include <string>

//TOOD: Fix PATH
#define CONFIG_PATH "/home/mark/.hikage/"
#define CONFIG_FILE_NAME "hikage.conf"
#define CONFIG_FILE_PATH CONFIG_PATH CONFIG_FILE_NAME

#define HIKAGE_VERSION "0.1.0"

class Config {
private:
    Config() = default;

    static Config* s_instance;

public:
    static Config* get(){
        return s_instance;
    }

    void save();
    static int open();

private:

    void create_blank();

    std::string _version{};
};


#endif //HIKAGE_CONFIG_H
