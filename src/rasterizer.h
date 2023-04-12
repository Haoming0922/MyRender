#ifndef RASTERIZER_H
#define RASTERIZER_H

#include "triangle.h"
#include "tgaimage.h"
#include <algorithm>
#include <vector>
#include <eigen/Eigen/Eigen>

class Rasterizer{
    public:
        Rasterizer(int w, int h);
        void rasterizeWorld(std::vector<Triangle*> &triangleList);
        void rasterizeTriangle(Triangle t, std::vector<Eigen::Vector4f> worldSpacePosition);

        void setModelView(Eigen::Matrix4f& m);
        void setProjection(Eigen::Matrix4f& p);
        void setViewport(Eigen::Matrix4f& v);
        void setShader(PhongShader& s);
        
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