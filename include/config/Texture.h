#ifndef TEXTURE_H
#define TEXTURE_H
#include <string>


class Texture {
public:
    Texture(const std::string &pathName, bool onlyName = false);
    ~Texture();

    int create(const std::string &pathName);

    std::string getName();
    std::string getSampler();
    void setSampler(std::basic_string<char> name);
    std::string* getRSampler();

    int getWidth() const;
    int getHeight() const;

    uint32_t getID() const;

private:
    std::string _name;
    std::string _sampler;

    uint32_t _id;
    int _width;
    int _height;

    void gen_txt();
};



#endif //TEXTURE_H
