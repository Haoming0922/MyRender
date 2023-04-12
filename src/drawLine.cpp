# include "line.h"
# include "model.h"
# include "geometry.h"
# include <iostream>

void draw_obj(const char *filename, int width, int height){
    const TGAColor white = TGAColor(255, 255, 255, 255);

    TGAImage image(width, height, TGAImage::RGB);
    Model m(filename);
    
    for(int i=0;i<m.nfaces();i++){
        for(int j=0; j<3; j++){
            Vec3f v0 = m.vert(m.face(i)[j]);
            Vec3f v1 = m.vert(m.face(i)[(j+1)%3]);
            int x0 = (v0.x+1.)*width/2.; 
            int y0 = (v0.y+1.)*height/2.; 
            int x1 = (v1.x+1.)*width/2.; 
            int y1 = (v1.y+1.)*height/2.; 
            lineWu(x0, y0, x1, y1, image, white); 
        }
    }
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("../img/africanHead_Wu.tga");
}



int main(){
    const TGAColor white = TGAColor(255, 255, 255, 255);
    const TGAColor red = TGAColor(255, 0, 0, 255);
    const TGAColor green = TGAColor(0, 255, 0, 255);
    const TGAColor blue = TGAColor(0, 0, 255, 255);
    
    TGAImage image_Wu(100, 100, TGAImage::RGB);
    lineWu(10, 50, 90, 50, image_Wu, white);
    lineWu(50, 10, 50, 90, image_Wu, white);
    lineWu(30, 20, 50, 90, image_Wu, red); 
    lineWu(10, 20, 90, 50, image_Wu, red); 
    lineWu(50, 90, 70, 20, image_Wu, green); 
    lineWu(10, 50, 90, 20, image_Wu, blue);
    
    TGAImage image_Bresenham(100, 100, TGAImage::RGB);
    lineBresenham(10, 50, 90, 50, image_Bresenham, white);
    lineBresenham(50, 10, 50, 90, image_Bresenham, white);
    lineBresenham(30, 20, 50, 90, image_Bresenham, red); 
    lineBresenham(10, 20, 90, 50, image_Bresenham, red); 
    lineBresenham(50, 90, 70, 20, image_Bresenham, green); 
    lineBresenham(10, 50, 90, 20, image_Bresenham, blue);
    
    image_Wu.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image_Bresenham.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image_Wu.write_tga_file("../img/line_Wu.tga");
    image_Bresenham.write_tga_file("../img/line_Bresenham.tga");
    
    draw_obj("../obj/africanHead.obj",800,800);
    
    return 0;
}