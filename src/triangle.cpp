#include "Triangle.h"
#include <stdexcept>

Triangle::Triangle(){
    Position[0] << 0.0, 0.0, 0.0, 1.0;
    Position[1] << 0.0, 0.0, 0.0, 1.0;
    Position[2] << 0.0, 0.0, 0.0, 1.0;
    
    Color[0] << 0.0, 0.0, 0.0;
    Color[1] << 0.0, 0.0, 0.0;
    Color[2] << 0.0, 0.0, 0.0;
    
    Tex[0] << 0.0, 0.0, 0.0;
    Tex[1] << 0.0, 0.0, 0.0;

    Normal[0] << 0.0, 0.0, 0.0, 0.0;
    Normal[1] << 0.0, 0.0, 0.0, 0.0;
    Normal[2] << 0.0, 0.0, 0.0, 0.0;
}

void Triangle::setPosition(int idx, Eigen::Vector4f p){
    Position[idx] = c;
}

void Triangle::setColor(int idx, Eigen::Vector3f c){
    if( c[0]<0.0 || c[0]>255.0 || c[1]<0.0 || c[1]>255.0 || c[2]<0.0 || c[2]>255.0)
    throw std::runtime_error("invalid color values");
    Color[idx] = c;
}

void Triangle::setTex(int idx, Eigen::Vector2f t){
    Tex[idx] = t;
}

void Triangle::setNormal(int idx, Eigen::Vector3f n){
    Normal[idx] = n;
}