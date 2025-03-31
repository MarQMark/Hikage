#include "rendering/Icon.h"
#include "glm/glm.hpp"
#include "rendering/Shader.h"

float hashStringToFloat(const std::string& input) {
    std::hash<std::string> hasher;
    size_t hashValue = hasher(input);
    return (float)(hashValue) / (float)(std::numeric_limits<size_t>::max()) * 642851;
}

Icon::Icon() {
    glGenFramebuffers(1, &_fbo);

    glGenBuffers(1, &_vb);
    glBindBuffer(GL_ARRAY_BUFFER, _vb);

    float v[] = {
            -1,  1, 0, 0,
            1,  1, 0, 0,
            1, -1, 0, 0,

            -1,  1, 0, 0,
            1, -1, 0, 0,
            -1, -1, 0, 0,
    };
    glBufferData(GL_ARRAY_BUFFER, 4*4 * 6, v, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4*4, nullptr);

    glGenTextures(1, &_txt);
    glActiveTexture(GL_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, _txt);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    _shader.compile(
            "#version 430\n"
            "\n"
            "uniform vec2  u_resolution; // Screen Resolution (width, height)\n"
            "uniform vec2  u_mouse;      // Mouse Position (x, y)\n"
            "uniform float u_time;       // Time in Seconds\n"
            "uniform float u_delta_time; // Time per Frame\n"
            "\n"
            "uniform float u_rand;\n"
            "\n"
            "layout(location = 0) out vec4 fragColor;\n"
            "\n"
            "vec2 rand2(vec2 p) {\n"
            "    return fract(sin(vec2(dot(p,vec2(127.1,311.7)),dot(p,vec2(269.5,183.3))))*43758.5453);\n"
            "}\n"
            "float rand1(float r){\n"
            "    return (sin(r)+1.0)/2;\n"
            "}\n"
            "\n"
            "void main() {\n"
            "    vec2 st = gl_FragCoord.xy/u_resolution.xy;\n"
            "    st.x *= u_resolution.x/u_resolution.y;\n"
            "    vec3 color = vec3(.0);\n"
            "\n"
            "    vec2 pos = gl_FragCoord.xy / min(u_resolution.x, u_resolution.y);\n"
            "    pos -= vec2(.5 * u_resolution.x / min(u_resolution.x, u_resolution.y), .5 * u_resolution.y / min(u_resolution.x, u_resolution.y));\n"
            "    if(pos.x * pos.x + pos.y * pos.y > .25)\n"
            "        discard;\n"
            "\n"
            "    st *= 4.;\n"
            "    vec2 cell = floor(st);\n"
            "    vec2 frac = fract(st);\n"
            "    float d = 1.;\n"
            "\n"
            "    for (int y = -1; y <= 1; y++) {\n"
            "        for (int x = -1; x <= 1; x++) {\n"
            "            vec2 p = rand2(cell + vec2(x, y) + rand1(u_rand));\n"
            "            float dist = length(vec2(x, y) + p - frac);\n"
            "            d = min(d, dist);\n"
            "        }\n"
            "    }\n"
            "\n"
            "    color = mix(vec3(gl_FragCoord.xy / u_resolution - gl_FragCoord.x / (2 * u_resolution.x), 1) * vec3(rand1(u_rand + 5181), rand1(u_rand + 1187), rand1(u_rand + 848)), vec3(.05,0,0), 1-d);\n"
            "\n"
            "    fragColor = vec4(color,1.0);\n"
            "}"
    );
}

Icon::~Icon() {
    glDeleteTextures(1, &_txt);
    glDeleteBuffers(1, &_vb);
    glDeleteFramebuffers(1, &_fbo);
}

void Icon::gen(GLFWwindow* window, const std::string& text) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, _txt);

    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _txt, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, 64, 64);

    _shader.bind();
    _shader.uniform2fv("u_resolution", glm::vec2(64));
    float f = hashStringToFloat(text);
    printf("%f\n",f);
    _shader.uniform1lf("u_rand", f);

    glDrawArrays(GL_TRIANGLES, 0 , 6);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    uint8_t pixels[64 * 64 * 4];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    GLFWimage icon;
    icon.width = 64;
    icon.height = 64;
    icon.pixels = pixels;
    glfwSetWindowIcon(window, 1, &icon);
}
