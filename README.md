# MyRender

based on [GAMES101](https://sites.cs.ucsb.edu/~lingqi/teaching/games101.html) and [tinyrenderer](https://github.com/ssloy/tinyrenderer)

<p float="left">
<img width='160' height='180' src="img/2023-04-20%20173702.png">
<img width='180' height='180' src="img/2023-04-20%20173806.png">
<img width='150' height='180' src="img/2023-04-20%20173737.png">
</p>

---

## Feature
- use TGA format image
- implment Bresenham and Xiaolin Wu'line drawing algorithm.
- implented blinn phong shader, texture shader, bump shader and displacement shader.

## Command
`./drawTriangle <OBJ_PATH> -<SHADER_MODE> <OPTINAL_TEXTURE_PATH>`
- SHADER_MODE: `phong`, `texture`, `bump`, `displacement`
- example: `./drawTriangle ../obj/spot/spot_triangulated_good.obj -texture ../obj/spot/spot_texture.tga`
