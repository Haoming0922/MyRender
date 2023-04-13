#include "rasterizer.h"
#include <iostream>

Rasterizer::Rasterizer(int w, int h, PhongShader& s): width(w), height(h), shader(s), image() {
    TGAImage image(width, height, TGAImage::RGB);
}

bool insideTriangle(float x, float y, Eigen::Vector4f position[]){
    Eigen::Vector3f p = {x, y, 1.0};
    
    Eigen::Vector3f v[3]; // a pyramid
    for(int i=0; i<3; i++) v[i] = {position[i][0], position[i][1], 1.0};
    
    Eigen::Vector3f n0, n1, n2; // normal of pyramids' three planes
    n0 = v[0].cross(v[1]);
    n1 = v[1].cross(v[2]);
    n2 = v[2].cross(v[0]);
    
    // determine if p is within pyramids' three planes (cos are of same sign)
    if( (n0.dot(v[2]) * n0.dot(p) > 0) && (n1.dot(v[0]) * n1.dot(p) > 0) && (n2.dot(v[1]) * n2.dot(p) > 0) )
        return true;
    return false;
}


std::tuple<float,float,float> getBarycentricCoord(float x, float y, Eigen::Vector4f position[]){
    float c1 = ( x*(position[1][1]-position[2][1]) + y*(position[2][0]-position[1][0]) + position[1][0]*position[2][1] - position[2][0]*position[1][1] ) 
               / ( position[0][0]*(position[1][1]-position[2][1]) + position[0][1]*(position[2][0]-position[1][0]) + position[1][0]*position[2][1] - position[2][0]*position[1][1]);
   
    float c2 = ( x*(position[2][1]-position[0][1]) + y*(position[0][0]-position[2][0]) + position[2][0]*position[0][1] - position[0][0]*position[2][1] ) 
           / ( position[1][0]*(position[2][1]-position[0][1]) + position[1][1]*(position[0][0]-position[2][0]) + position[2][0]*position[0][1] - position[0][0]*position[2][1]);
    
    float c3 = ( x*(position[0][1]-position[1][1]) + y*(position[1][0]-position[0][0]) + position[0][0]*position[1][1] - position[1][0]*position[0][1] ) 
           / ( position[2][0]*(position[0][1]-position[1][1]) + position[2][1]*(position[1][0]-position[0][0]) + position[0][0]*position[1][1] - position[1][0]*position[0][1]);
    
    return {c1,c2,c3};
}


template<typename t> t interpolate(float alpha, float beta, float gamma, t &attribute1, t &attribute2, t &attribute3){
    return alpha*attribute1 + beta*attribute2 + gamma*attribute3; 
}


Eigen::Vector3f toVector3(Eigen::Vector4f v){
    return Eigen::Vector3f(v[0], v[1], v[2]);
}



// Eigen::Vector3f interpolate(float alpha, float beta, float gamma, Eigen::Vector3f &attribute1, Eigen::Vector3f &attribute2, Eigen::Vector3f &attribute3){
//     return alpha*attribute1 + beta*attribute2 + gamma*attribute3; 
// }

// Eigen::Vector2f interpolate(float alpha, float beta, float gamma, Eigen::Vector2f &attribute1, Eigen::Vector2f &attribute2, Eigen::Vector2f &attribute3){
//     return alpha*attribute1 + beta*attribute2 + gamma*attribute3; 
// }


void Rasterizer::rasterizeWorld(std::vector<Triangle*> &triangleList){
    Eigen::Matrix4f PositionTransform = viewport * projection * modelView;
    Eigen::Matrix4f NormalTransform = modelView.inverse().transpose();
    // Eigen::Vector3f color = {148.0, 121.0, 92.0};
    for(const auto& t:triangleList){
        Triangle tt = *t; // t is a const reference, cannot be modified
        std::vector<Eigen::Vector4f> worldSpacePosition(3); 
        
        // for all 3 vertexes in one triangle
        for(int i = 0; i < 3; i++){
            // position transform
            tt.Position[i] = PositionTransform * tt.Position[i];
            tt.Position[i] /= tt.Position[i][3]; // homogeneous division
            
            // normal vector transform
            tt.Normal[i] = NormalTransform * tt.Normal[i];
            tt.Normal[i] /= tt.Normal[i][3];
            
            // save world space coord 
            worldSpacePosition[i] = modelView * t->Position[i];
            worldSpacePosition[i] /= worldSpacePosition[i][3];
            
            // set color
            // tt.Color[i] = color;
        }
        
        // rasterize triangle
        rasterizeTriangle(tt, worldSpacePosition);
    }
}



void Rasterizer::rasterizeTriangle(Triangle& t, std::vector<Eigen::Vector4f> worldSpacePosition){
    /*
        traverse all possible pixels:
        -> if inside triangle 
        -> compute Barycentric coord 
        -> z buffer 
        -> if visible
        -> interpolate attributes
        -> get color from shader
        -> set pixel
    */
    
    int minX = std::floor(std::min(t.Position[0][0], std::min(t.Position[1][0], t.Position[2][0])));
    int maxX = std::ceil(std::max(t.Position[0][0], std::max(t.Position[1][0], t.Position[2][0])));
    int minY = std::floor(std::min(t.Position[0][1], std::min(t.Position[1][1], t.Position[2][1])));
    int maxY = std::ceil(std::max(t.Position[0][1], std::max(t.Position[1][1], t.Position[2][1])));
    
    for(int x = minX; x <= maxX; x++){
        for(int y = minY; y <= maxY; y++){
            if(insideTriangle(x+0.5f, y+0.5f, t.Position)){
                float alpha, beta, gamma;
                std::tie(alpha, beta, gamma) = getBarycentricCoord(x+0.5f, y+0.5f, t.Position);
                
                float z_screen = (alpha*t.Position[0][2]/t.Position[0][3] + beta*t.Position[1][2]/t.Position[1][3] + gamma*t.Position[2][2]/t.Position[2][3]) / (alpha/t.Position[0][3] + beta/t.Position[1][3] + gamma/t.Position[2][3]);
                if(z_screen < zBuf[getIdx(x,y)]){
                    std::cout << "ethan 2" << std::endl;
                    zBuf[getIdx(x,y)] = z_screen;
                    
                    auto colorInterpolated = interpolate(alpha, beta, gamma, t.Color[0], t.Color[1], t.Color[2]);
                    auto normalInterpolated = interpolate(alpha, beta, gamma, t.Normal[0], t.Normal[1], t.Normal[2]).normalized();
                    auto texInterpolated = interpolate(alpha, beta, gamma, t.Tex[0], t.Tex[1], t.Tex[2]);
                    auto worldSpacePositionInterpolated = interpolate(alpha, beta, gamma, worldSpacePosition[0], worldSpacePosition[1], worldSpacePosition[2]);
                    
                    shader.payload.kd = colorInterpolated;
                    shader.payload.color = colorInterpolated;
                    shader.payload.normal = toVector3(normalInterpolated);
                    shader.payload.position = toVector3(worldSpacePositionInterpolated);
                    shader.payload.texPosition = texInterpolated;
                    
                    auto pixelColor = shader.performShading();
                    TGAColor c(static_cast<unsigned char>(pixelColor[0]), 
                               static_cast<unsigned char>(pixelColor[0]), 
                               static_cast<unsigned char>(pixelColor[0]), 255);
                    image.set(x, y, c, 1.0);
                }
            }
        }
    }
}


void Rasterizer::setModelView(const Eigen::Matrix4f& m){
    modelView = m;
}


void Rasterizer::setProjection(const Eigen::Matrix4f& p){
    projection = p;
}


void Rasterizer::setViewport(const Eigen::Matrix4f& v){
    viewport = v;
}

void Rasterizer::saveResult(std::string path){
    image.write_tga_file(path.c_str());
}


int Rasterizer::getIdx(int x, int y){
    return (height-y)*width + x;
}