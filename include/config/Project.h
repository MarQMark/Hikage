#ifndef HIKAGE_PROJECT_H
#define HIKAGE_PROJECT_H


#include <string>
#include <filesystem>

class Project {
private:
    Project() = default;

    static Project* s_instance;

public:
    static Project* get(){
        return s_instance;
    }

    static int open(char* path);

    void create(char* path);
    void save();

    bool changed();

private:
    std::string _path{};

    std::filesystem::file_time_type _last_time;

};


#endif //HIKAGE_PROJECT_H
