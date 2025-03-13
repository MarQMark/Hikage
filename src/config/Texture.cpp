#include "config/Texture.h"

#include <GL/glew.h>
#include <stb_image/stb_image.h>

Texture::Texture(const std::string &pathName, bool onlyName) {
    if(onlyName){
        _name = pathName;
    }
    else{
        size_t pos = pathName.rfind('/');

        if (pos != std::string::npos) {
            _name = pathName.substr(pos + 1, pathName.length() - 1);
        } else {
            printf("[ERROR] Failed to load path of image: %s\n", pathName.c_str());
        }
    }
}


Texture::~Texture() {
    glDeleteTextures(1, &_id);
}

int Texture::create(const std::string &pathName) {
    int channels;
    unsigned char* imageData = stbi_load(pathName.c_str(), &_width, &_height, &channels, 4);
    if (!imageData) {
        printf("[ERROR] Failed to load texture image: %s\n", pathName.c_str());
        return -1;
    }

    glGenTextures(1, &_id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(imageData);

    return 1;
}

std::string Texture::getName() {
    return _name;
}

std::string Texture::getSampler() {
    return _sampler;
}

void Texture::setSampler(std::basic_string<char> name) {
    _sampler = name;
}

uint32_t Texture::getID() const {
    return _id;
}

std::string *Texture::getRSampler() {
    return &_sampler;
}

int Texture::getWidth() const {
    return _width;
}

int Texture::getHeight() const {
    return _height;
}
