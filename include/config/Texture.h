#ifndef TEXTURE_H
#define TEXTURE_H
#include <string>


class Texture {
public:
    Texture(const std::string &pathName);
    Texture(std::string path, std::string name);
    ~Texture();

    int create();

    std::string getName();
    std::string getPath();
    std::string getSampler();
    void setSampler(std::string& name);

    uint32_t getID() const;

private:
    std::string _name;
    std::string _path;
    std::string _sampler;

    uint32_t _id;
    int _width;
    int _height;

    void gen_txt();
};



#endif //TEXTURE_H
