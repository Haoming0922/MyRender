#ifndef RASTERIZER_H
#define RASTERIZER_H

#include "triangle.h"
#include "tgaimage.h"
#include "shader.h"
#include <algorithm>
#include <vector>
#include <Eigen/Dense>

class Rasterizer{
    public:
        Rasterizer(int w, int h, PhongShader& s);
        void rasterizeWorld(std::vector<Triangle*> &triangleList);
        void rasterizeTriangle(Triangle& t, std::vector<Eigen::Vector4f> worldSpacePosition);

        void setModelView(const Eigen::Matrix4f& m);
        void setProjection(const Eigen::Matrix4f& p);
        void setViewport(const Eigen::Matrix4f& v);
        
        void saveResult(std::string path);
        
    private:
        int width, height;
        Eigen::Matrix4f modelView;
        Eigen::Matrix4f projection;
        Eigen::Matrix4f viewport;
        
        TGAImage image;
        std::vector<float> zBuf;
        
        PhongShader& shader;
        
        int getIdx(int x, int y);
};

#endif