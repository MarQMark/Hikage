#ifndef HIKAGE_PROJECT_H
#define HIKAGE_PROJECT_H


#include <string>
#include <filesystem>
#include <vector>

class Project {
private:
    static Project* s_instance;

public:
    Project(std::string name, const std::string& path);
    explicit Project(const std::string& projPath);

    static Project* get(){
        return s_instance;
    }

    struct File{
        std::filesystem::file_time_type last_time;
        std::string path;
    };

    static int open(Project* project);
    static bool opened();
    void close();

    void create();
    int load();
    void save();

    std::string getName();
    std::string getPath();

    void addFile(std::string path);
    std::vector<Project::File*> getFiles();

    bool changed();

    void setDimensions(int w, int h);
    int getWidth() const;
    int getHeight() const;

private:
    static bool _opened;

    std::string _path{};
    std::string _name{};
    std::string _version{};

    int _width = 1280;
    int _height = 720;

    std::vector<File*> _files;
};


#endif //HIKAGE_PROJECT_H
