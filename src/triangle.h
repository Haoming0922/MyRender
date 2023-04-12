#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <eigen/Eigen/Eigen>

class Triangle{
    public:
        Eigen::Vector4f Position[3];
        Eigen::Vector3f Color[3];
        Eigen::Vector2f Tex[3];
        Eigen::Vector4f Normal[3];
        
        Triangle();
        
        void setPosition(int idx, Eigen::Vector4f p);
        void setColor(int idx, Eigen::Vector3f c);
        void setTex(int idx, Eigen::Vector2f t);
        void setNormal(int idx, Eigen::Vector3f n);
};

#endif