#ifndef SHADER_H
#define SHADER_H

#include "tgaimage.h"
#include <vector>
#include <string>
#include <Eigen/Dense>


struct light{
    Eigen::Vector3f position;
    Eigen::Vector3f intensity;
};


struct ShaderPayload{
    Eigen::Vector3f ka = Eigen::Vector3f(0.005, 0.005, 0.005);
    Eigen::Vector3f kd;
    Eigen::Vector3f ks = Eigen::Vector3f(0.7937, 0.7937, 0.7937);
    std::vector<light> lights = {
        light{{20, 20, 20}, {500, 500, 500}},
        light{{-20, 20, 20}, {500, 500, 500}}
    };
    Eigen::Vector3f ambientLightIntensity {10, 10, 10};
    Eigen::Vector3f eyePosition {0, 0, 10};
    float p = 150;
    float angle = 80.0;
    
    Eigen::Vector3f color;
    Eigen::Vector3f position;
    Eigen::Vector3f normal;
    
    TGAImage* texture;
    Eigen::Vector2f texPosition;
};



class Shader{
    public:
        Shader(struct ShaderPayload &payload) : payload(payload) {};
        virtual Eigen::Vector3f performShading() = 0;
        virtual std::string getName() = 0;
        struct ShaderPayload& payload;        
};


class PhongShader : public Shader{
    public:
        PhongShader(struct ShaderPayload &payload) : Shader(payload) {};
        Eigen::Vector3f performShading();
        std::string getName() { return "phong shader";}
};


class TextureShader : public Shader{
    public:
        TextureShader(struct ShaderPayload &payload) : Shader(payload) {};
        Eigen::Vector3f performShading();
        std::string getName() { return "texture shader";}
};


class BumpShader : public Shader{
    public:
        BumpShader(struct ShaderPayload &payload) : Shader(payload) {};
        Eigen::Vector3f performShading();
        std::string getName() { return "bump shader";}
};


class DisplacementShader : public Shader{
    public:
        DisplacementShader(struct ShaderPayload &payload) : Shader(payload) {};
        Eigen::Vector3f performShading();
        std::string getName() { return "displacement shader";}
};



#endif