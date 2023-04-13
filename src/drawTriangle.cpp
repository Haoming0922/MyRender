#include <iostream>
#include "triangle.h"
#include "rasterizer.h"
#include "shader.h"
#include "global.h"
#include "tgaimage.h"
#include "OBJ_Loader.h"


Eigen::Matrix4f getMoelView(Eigen::Vector3f eyePosition, float angle_){
    Eigen::Matrix4f view;
    view << 1,0,0,-eyePosition[0],
            0,1,0,-eyePosition[1],
            0,0,1,-eyePosition[2],
            0,0,0,1;
                 
    Eigen::Matrix4f rotation;
    float angle = angle_ * MY_PI / 180.f;
    rotation << cos(angle), 0, sin(angle), 0,
                0, 1, 0, 0,
                -sin(angle), 0, cos(angle), 0,
                0, 0, 0, 1;

    Eigen::Matrix4f scale;
    scale << 2.5, 0, 0, 0,
             0, 2.5, 0, 0,
             0, 0, 2.5, 0,
             0, 0, 0, 1;

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, 0,
                 0, 1, 0, 0,
                 0, 0, 1, 0,
                 0, 0, 0, 1;
            
    return view * translate * rotation * scale;
}


Eigen::Matrix4f getProjection(){
    float eye_fov = 45.0 , aspect_ratio = 1, zNear = 0.1, zFar = 50, xRight, yTop;
    yTop = tan(eye_fov/180.0f*MY_PI)*zNear;
    xRight = aspect_ratio * yTop;
    
    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();
    projection << -zNear/xRight, 0.f, 0.f, 0.f,
                  0.f, -zNear/yTop, 0.f, 0.f,
                  0.f, 0.f, (zNear+zFar)/(zFar-zNear), 2.f*zFar*zNear/(zNear-zFar),
                  0.f, 0.f, 1.f, 0.f;
    return projection;
}


Eigen::Matrix4f getViewport(int width, int height){
    float f1 = (50 - 0.1) / 2.0;
    float f2 = (50 + 0.1) / 2.0;
    
    Eigen::Matrix4f viewport = Eigen::Matrix4f::Identity();
    viewport << 0.5*width, 0.0f, 0.0f, 0.5*width,
                0.0f, 0.5*height, 0.0f, 0.5*height,
                0.0f, 0.0f, f1, f2,
                0.0f, 0.0f, 0.0f, 1;
    return viewport;
}


int main(int argc, const char** argv){
    if(argc>=2){
        // Load .obj file into triangle lists
        std::vector<Triangle*> TriangleList;
        objl::Loader loader;
        std::string path = std::string(argv[1]);
        loader.LoadFile(path); 
        for(auto mesh : loader.LoadedMeshes){
            for(int i=0; i<mesh.Vertices.size(); i+=3){
                Triangle* t = new Triangle();
                for(int j=0; j<3; j++){
                    t->setPosition(j, Eigen::Vector4f(mesh.Vertices[i+j].Position.X, mesh.Vertices[i+j].Position.Y, mesh.Vertices[i+j].Position.Z, 1.0) );
                    t->setTex(j, Eigen::Vector2f(mesh.Vertices[i+j].TextureCoordinate.X, mesh.Vertices[i+j].TextureCoordinate.Y) );
                    t->setNormal(j, Eigen::Vector4f(mesh.Vertices[i+j].Normal.X, mesh.Vertices[i+j].Normal.Y, mesh.Vertices[i+j].Normal.Z, 0.0) );
                    t->setColor(j, Eigen::Vector3f(148.0, 121.0, 92.0));
                }
                TriangleList.push_back(t);
            }
        }
        
        // set up shader and (texture)
        ShaderPayload payload;
        Shader* s;
        if(argc == 4 && strcmp(argv[2], "-texture") == 0 ){
            // load texture 
            TGAImage* t = new TGAImage();
            t->read_tga_file(argv[3]);   
            // set up shader
            payload.texture = t;
            s = new TextureShader(payload);
        }
        if(argc == 4 && strcmp(argv[2], "-bump") == 0 ){
            // load texture 
            TGAImage* t = new TGAImage();
            t->read_tga_file(argv[3]);   
            // set up shader
            payload.texture = t;
            s = new BumpShader(payload);
        }
        if(argc == 4 && strcmp(argv[2], "-displacement") == 0 ){
            // load texture 
            TGAImage* t = new TGAImage();
            t->read_tga_file(argv[3]);   
            // set up shader
            payload.texture = t;
            s = new DisplacementShader(payload);
        }
        if(argc >= 3 && strcmp(argv[2], "-phong") == 0 ){
            // set up shader
            s = new PhongShader(payload);
        }
        
        // set up rasterizer
        std::cout << "setting " << s->getName() << ", begin rasterizing" << std::endl;
        int width = 700, height = 700;
        Rasterizer r(width, height, s);
        r.setModelView(getMoelView(payload.eyePosition, payload.angle));
        r.setProjection(getProjection());
        r.setViewport(getViewport(width,height));

        // rasterize
        r.rasterizeWorld(TriangleList);
        
        // save result
        std::string resultName = path.substr(path.find_last_of("/"), path.find_last_of(".")-path.find_last_of("/"));
        r.saveResult(("../img" + resultName + argv[2] + ".tga").c_str());
        std::cout << "result save in: " << ("../img" + resultName + argv[2] + ".tga") << std::endl;
    }
    else{
        throw std::runtime_error("invalid input");
    }
    
    return 0;
}