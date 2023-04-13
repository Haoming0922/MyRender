#ifndef SHADER_H
#define SHADER_H

#include "tgaimage.h"
#include <vector>
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
    
    Eigen::Vector3f color;
    Eigen::Vector3f position;
    Eigen::Vector3f normal;
    
    TGAImage* texture;
    Eigen::Vector2f texPosition;
};


class PhongShader{
    public:
        PhongShader(struct ShaderPayload &payload) : payload(payload) {};
        Eigen::Vector3f performShading();
        struct ShaderPayload& payload;        
};


class TextureShader : public PhongShader{
    public:
        TextureShader(struct ShaderPayload &payload);
};


class BumpShader : public PhongShader{
    public:
        BumpShader(struct ShaderPayload &payload);
        Eigen::Vector3f performShading() {return payload.normal * 255.0;};
};


class DisplacementShader : public PhongShader{
    public:
        DisplacementShader(struct ShaderPayload &payload);
};


#endif