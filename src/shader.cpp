#include "shader.h"
#include <algorithm>
#include <iostream>


Eigen::Vector3f PhongShader::performShading(){
    Eigen::Vector3f result = {0.0, 0.0, 0.0};
    for(auto& light : payload.lights){
        float rSquare = (light.position - payload.position).dot(light.position - payload.position);
        Eigen::Vector3f l = (light.position - payload.position).normalized();
        Eigen::Vector3f n = payload.normal.normalized();
        Eigen::Vector3f h = (l + (payload.eyePosition - payload.position).normalized()).normalized();
        
        Eigen::Vector3f Ld = payload.kd.cwiseProduct(light.intensity / rSquare * std::max(0.0f, static_cast<float>(n.dot(l)) ) );
        Eigen::Vector3f Ls = payload.ks.cwiseProduct(light.intensity / rSquare * std::pow(std::max(0.0f, static_cast<float>(n.dot(h))) , payload.p) );
        Eigen::Vector3f La = payload.ka.cwiseProduct(payload.ambientLightIntensity);
        
        result += Ld + Ls + La;
    }
    return result * 255.0;
}


Eigen::Vector3f TextureShader::performShading(){
    if(payload.texture){
        Eigen::Vector3f texColor = {0.0, 0.0, 0.0};
        TGAColor c = payload.texture->get(payload.texPosition[0], payload.texPosition[1]);
        texColor << c.r, c.g, c.b;
        payload.kd = texColor / 255.0;
    }
    
    Eigen::Vector3f result = {0.0, 0.0, 0.0};
    for(auto& light : payload.lights){
        float rSquare = (light.position - payload.position).dot(light.position - payload.position);
        Eigen::Vector3f l = (light.position - payload.position).normalized();
        Eigen::Vector3f n = payload.normal.normalized();
        Eigen::Vector3f h = (l + (payload.eyePosition - payload.position).normalized()).normalized();
        
        Eigen::Vector3f Ld = payload.kd.cwiseProduct(light.intensity / rSquare * std::max(0.0f, static_cast<float>(n.dot(l)) ) );
        Eigen::Vector3f Ls = payload.ks.cwiseProduct(light.intensity / rSquare * std::pow(std::max(0.0f, static_cast<float>(n.dot(h))) , payload.p) );
        Eigen::Vector3f La = payload.ka.cwiseProduct(payload.ambientLightIntensity);
        
        result += Ld + Ls + La;
    }
    return result * 255.0;   
}



Eigen::Vector3f BumpShader::performShading(){
    float kh = 0.2, kn = 0.1; // scaling
    
    // update normal vector after bump at tangent-bitangent-normal coord space
    Eigen::Vector3f normalAtTBN;
    float u = payload.texPosition[0];
    float v = payload.texPosition[1];
    float dU = kh * kn * (payload.texture->get(u+1,v).TGAnorm() - payload.texture->get(u,v).TGAnorm());
    float dV = kh * kn * (payload.texture->get(u,v+1).TGAnorm() - payload.texture->get(u,v).TGAnorm());
    normalAtTBN << -dU, -dV, 1;
    
    // get the transform matrix from tangent-bitangent-normal coord space to world space
    Eigen::Vector3f tangent, bitangent;
    float x = payload.normal[0];
    float y = payload.normal[1];
    float z = payload.normal[2];
    tangent << x*y/sqrt(x*x + z*z), -sqrt(x*x + z*z), y*z/sqrt(x*x + z*z);
    bitangent = tangent.cross(payload.normal);
    Eigen::Matrix3f TBN;
    TBN.col(0) << tangent;
    TBN.col(1) << bitangent;
    TBN.col(2) << payload.normal;
    
    // transform the normal vector at tangent-bitangent-normal coord space to world space
    payload.normal = (TBN * normalAtTBN).normalized();
    return payload.normal * 255.0;
}




Eigen::Vector3f DisplacementShader::performShading(){
    float kh = 0.2, kn = 0.1; // scaling

    // update position after displacement (displace along normal vector)
    float u = payload.texPosition[0];
    float v = payload.texPosition[1];
    payload.position += kn * payload.texture->get(u,v).TGAnorm() * payload.normal;
    
    // update normal vector after displacement at tangent-bitangent-normal coord space
    Eigen::Vector3f normalAtTBN;
    float dU = kh * kn * (payload.texture->get(u+1,v).TGAnorm() - payload.texture->get(u,v).TGAnorm());
    float dV = kh * kn * (payload.texture->get(u,v+1).TGAnorm() - payload.texture->get(u,v).TGAnorm());
    normalAtTBN << -dU, -dV, 1;
    
    // get the transform matrix from tangent-bitangent-normal coord space to world space
    Eigen::Vector3f tangent, bitangent;
    float x = payload.normal[0];
    float y = payload.normal[1];
    float z = payload.normal[2];
    tangent << x*y/sqrt(x*x + z*z), -sqrt(x*x + z*z), y*z/sqrt(x*x + z*z);
    bitangent = tangent.cross(payload.normal);
    Eigen::Matrix3f TBN;
    TBN.col(0) << tangent;
    TBN.col(1) << bitangent;
    TBN.col(2) << payload.normal;
    
    // transform the normal vector at tangent-bitangent-normal coord space to world space
    payload.normal = (TBN * normalAtTBN).normalized();
    
    Eigen::Vector3f result = {0.0, 0.0, 0.0};
    for(auto& light : payload.lights){
        float rSquare = (light.position - payload.position).dot(light.position - payload.position);
        Eigen::Vector3f l = (light.position - payload.position).normalized();
        Eigen::Vector3f n = payload.normal.normalized();
        Eigen::Vector3f h = (l + (payload.eyePosition - payload.position).normalized()).normalized();
        
        Eigen::Vector3f Ld = payload.kd.cwiseProduct(light.intensity / rSquare * std::max(0.0f, static_cast<float>(n.dot(l)) ) );
        Eigen::Vector3f Ls = payload.ks.cwiseProduct(light.intensity / rSquare * std::pow(std::max(0.0f, static_cast<float>(n.dot(h))) , payload.p) );
        Eigen::Vector3f La = payload.ka.cwiseProduct(payload.ambientLightIntensity);
        
        result += Ld + Ls + La;
    }
    return result * 255.0;
}