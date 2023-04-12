# include "line.h"

/* things to consider:
   - direction, x0 > x1 ?
   - steep line?
   - antialiasing
   - eliminate branching?
   - taking const out of loop ?
   - replace float by int ?
   - check for exception? divide by 0
*/

void lineWu(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color){
    // transpose if steep, make sure increment along the unsteep axis
    bool steep = std::abs(y1-y0) > (x1-x0) ? true : false;
    if(steep) {std::swap(x0,y0); std::swap(x1,y1);} 
    
    // make sure x0 < x1, must after transpose
    if(x0 > x1) { std::swap(x0,x1); std::swap(y0,y1); } 
    
    float slope = static_cast<float>(y1-y0) / (x1-x0);
    float y = y0 + slope;
    
    if(steep){
        for(int x=x0; x<=x1; x++){
            image.set(std::floor(y), x, color, 1.0 - (y - std::floor(y)));
            image.set(std::floor(y+1), x, color, y - std::floor(y));
            y += slope;
        }
    }
    else{
        for(int x=x0; x<=x1; x++){
            image.set(x, std::floor(y), color, 1.0 - (y - std::floor(y)));
            image.set(x, std::floor(y+1), color, y - std::floor(y));
            y += slope;
        }
    }
}



void lineBresenham(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color){        
    // transpose if steep, make sure increment along the unsteep axis
    bool steep = std::abs(y1-y0) > (x1-x0) ? true : false;
    if(steep) {std::swap(x0,y0); std::swap(x1,y1);} 
    
    // make sure x0 < x1, must after transpose
    if(x0 > x1) { std::swap(x0,x1); std::swap(y0,y1); } 
    
    int DX = x1 - x0;
    int DY = std::abs(y1 - y0);
    int D = 2*DY - DX;
    int dy = y1>y0 ? 1 : -1;
    int y = y0;
    
    if(steep){
        for(int x=x0; x<=x1; x++){
            image.set(y,x,color,1.0);
            if(D > 0){
                y += dy;
                D -= 2*DX;
            }
            D += 2*DY;
        }
    }
    else{
        for(int x=x0; x<=x1; x++){
            image.set(x,y,color,1.0);
            if(D > 0){
                y += dy;
                D -= 2*DX;
            }
            D += 2*DY;
        }
    }
}