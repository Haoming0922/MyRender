#include "shader.h"
#include <algorithm>


Eigen::Vector3f PhongShader::performShading(){
    Eigen::Vector3f result = {0.0, 0.0, 0.0};
    for(auto& light : ShaderPayload.lights){
        float rSquare = (light.position - ShaderPayload.position).dot(light.position - ShaderPayload.position);
        Eigen::Vector3f l = (light.position - ShaderPayload.position).normalized();
        Eigen::Vector3f n = ShaderPayload.normal.normalized();
        Eigen::Vector3f h = (l + (ShaderPayload.eyePosition - ShaderPayload.position).normalized()).normalized();
        
        Eigen::Vector3f Ld = ShaderPayload.kd.cwiseProduct(light.intensity / rSquare * std::max(0, n.dot(l)) );
        Eigen::Vector3f Ls = ShaderPayload.ks.cwiseProduct(light.intensity / rSquare * std::pow(std::max(0, n.dot(h)), ShaderPayload.p) );
        Eigen::Vector3f La = ShaderPayload.ka.cwiseProduct(ambientLightIntensity);
        
        result += Ld + Ls + La;
    }
    return result * 255.0;
}



TextureShader::TextureShader(struct& payload) : ShaderPayload(payload){
    Eigen::Vector3f texColor = {0.0, 0.0, 0.0};
    if(ShaderPayload.texture){
        TGAColor = texture.get(texPosition[0],texPosition[1]);
        texColor << TGAColor.r, TGAColor.g, TGAColor.b;
    }
    ShaderPayload.kd = texColor / 255.0;
}



BumpShader::BumpShader(struct& payload) : ShaderPayload(payload){
    float kh = 0.2, kn = 0.1; // scaling
    
    // update normal vector after bump at tangent-bitangent-normal coord space
    Eigen::Vector3f normalAtTBN;
    float dU = kh * kn * (ShaderPayload.texture.get(u+1,v) - ShaderPayload.texture.get(u,v));
    float dU = kh * kn * (ShaderPayload.texture.get(u,v+1) - ShaderPayload.texture.get(u,v));
    normalAtTBN << -dU, -dV, 1;
    
    // get the transform matrix from tangent-bitangent-normal coord space to world space
    Eigen::Vector3f tangent, bitangent;
    float x = ShaderPayload.normal[0];
    float y = ShaderPayload.normal[1];
    float z = ShaderPayload.normal[2];
    tangent << x*y/sqrt(x*x + z*z), -sqrt(x*x + z*z), y*z/sqrt(x*x + z*z);
    bitangent = tangent.cross(normal);
    Eigen::Matrix3f TBN;
    TBN.col(0) << tangent;
    TBN.col(1) << bitangent;
    TBN.col(2) << normal;
    
    // transform the normal vector at tangent-bitangent-normal coord space to world space
    ShaderPayload.normal = (TBN * normalAtTBN).normalized();
}




DisplacementShader::DisplacementShader(struct& payload) : ShaderPayload(payload){
    float kh = 0.2, kn = 0.1; // scaling

    // update position after displacement (displace along normal vector)
    float u = ShaderPayload.texPosition[0];
    float v = ShaderPayload.texPosition[1];
    ShaderPayload.Position += kn * ShaderPayload.texture.get(u,v).norm() * ShaderPayload.normal;
    
    // update normal vector after displacement at tangent-bitangent-normal coord space
    Eigen::Vector3f normalAtTBN;
    float dU = kh * kn * (ShaderPayload.texture.get(u+1,v) - ShaderPayload.texture.get(u,v));
    float dU = kh * kn * (ShaderPayload.texture.get(u,v+1) - ShaderPayload.texture.get(u,v));
    normalAtTBN << -dU, -dV, 1;
    
    // get the transform matrix from tangent-bitangent-normal coord space to world space
    Eigen::Vector3f tangent, bitangent;
    float x = ShaderPayload.normal[0];
    float y = ShaderPayload.normal[1];
    float z = ShaderPayload.normal[2];
    tangent << x*y/sqrt(x*x + z*z), -sqrt(x*x + z*z), y*z/sqrt(x*x + z*z);
    bitangent = tangent.cross(normal);
    Eigen::Matrix3f TBN;
    TBN.col(0) << tangent;
    TBN.col(1) << bitangent;
    TBN.col(2) << normal;
    
    // transform the normal vector at tangent-bitangent-normal coord space to world space
    ShaderPayload.normal = (TBN * normalAtTBN).normalized();
}