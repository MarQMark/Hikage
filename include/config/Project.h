#ifndef HIKAGE_PROJECT_H
#define HIKAGE_PROJECT_H


#include <string>
#include <filesystem>
#include <vector>

#include "Texture.h"
#include "script/Script.h"

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
        std::string name;
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

    void addTexture(Texture* txt, const std::string& path);
    std::vector<Texture*> getTextures();
    void delTexture(size_t idx);

    void createScript();
    Script* getScript();

    bool isPaused() const;
    void pause(bool pause);

private:
    static bool _opened;

    bool _paused = false;

    std::string _path{};
    std::string _name{};
    std::string _version{};

    int _width = 1280;
    int _height = 720;

    std::vector<File*> _files;
    std::vector<Texture*> _textures;
    Script* _script{};

    std::string _default_frag =
            "#version 430\n"
            "\n"
            "uniform vec2  u_resolution; // Screen Resolution (width, height)\n"
            "uniform vec2  u_mouse;      // Mouse Position (x, y)\n"
            "uniform float u_time;       // Time in Seconds\n"
            "uniform float u_delta_time; // Time per Frame\n"
            "\n"
            "layout(location = 0) out vec4 fragColor;\n"
            "\n"
            "void main() {\n"
            "    fragColor = vec4(.2, .3, .8, 1.0);\n"
            "}";

    std::string _default_script =
            "import hikage\n"
            "\n"
            "def init():\n"
            "    pass\n"
            "    \n"
            "def update():\n"
            "    pass";
};


#endif //HIKAGE_PROJECT_H
