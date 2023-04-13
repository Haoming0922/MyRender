#include <iostream>
#include "triangle.h"
#include "rasterizer.h"
#include "shader.h"
#include "tgaimage.h"
#include "OBJ_Loader.h"


Eigen::Matrix4f getMoelView(){
    return Eigen::Matrix4f::Identity();
}


Eigen::Matrix4f getProjection(){
    return Eigen::Matrix4f::Identity();
}


Eigen::Matrix4f getViewport(){
    return Eigen::Matrix4f::Identity();
}


int main(int argc, const char** argv){
    if(argc>=2){
        // Load .obj file into triangle lists
        std::vector<Triangle*> TriangleList;
        objl::Loader loader;
        std::string path = std::string(argv[1]);
        loader.LoadFile(path);  // ../obj/tinyrender/african_head/african_head.obj
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
        PhongShader s(payload);
        if(argc == 4 && std::string(argv[2]) == "-texture"){
            // load texture 
            TGAImage texture;
            texture.read_tga_file(argv[3]); // ../obj/tinyrender/african_head/african_head_diffuse.tga   
            // set up shader
            ShaderPayload payload;
            payload.texture = &texture;
            TextureShader s(payload);
        }
        if(argc == 3 && std::string(argv[2]) == "-phong"){
            // set up shader
            ShaderPayload payload;
            PhongShader s(payload);
        }
        
        // set up rasterizer
        Rasterizer r(800,800, s);
        r.setModelView(getMoelView());
        r.setProjection(getProjection());
        r.setViewport(getViewport());

        // rasterize
        r.rasterizeWorld(TriangleList);
        
        // save result
        std::cout << "ethan" << std::endl;
        std::string resultName = path.substr(path.find_last_of("/"), path.find_last_of(".")-path.find_last_of("/"));
        r.saveResult(("../img" + resultName + ".tga").c_str());
    }
    else{
        throw std::runtime_error("invalid input");
    }
    
    return 0;
}