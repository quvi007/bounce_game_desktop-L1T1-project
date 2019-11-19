#include <stdlib.h>
const int image_count = 49;
char image_dir[image_count][1000] = {
        "resource/dyn_thorn.fle",
        "resource/life.fle",
        "resource/right_triangle.fle",
        "resource/ring.fle",
        "resource/ring_back.fle",
        "resource/ring_back_checked.fle",
        "resource/ring_front.fle",
        "resource/ring_front_checked.fle",
        "resource/thorn.fle",
        "resource/up_left_triangle.fle",
        "resource/up_right_triangle.fle",
        "resource/xgrey_ring_front.fle",
        "resource/xring_back.fle",
        "resource/xring_back_checked.fle",
        "resource/xring_front.fle",
        "resource/ball.fle",
        "resource/checkpoint.fle",
        "resource/chkpoint.fle",
        "resource/grey_ring_front.fle",
        "resource/left_triangle.fle",
        "resource/block.fle",
        "resource/blue_block.fle",
        "resource/power_speed.fle",
        "resource/deflater.fle",
        "resource/pumper.fle",
        "resource/gate_close.fle",
        "resource/gate_open.fle",
        "digits/cursive/digit(0).fle",
        "digits/cursive/digit(1).fle",
        "digits/cursive/digit(2).fle",
        "digits/cursive/digit(3).fle",
        "digits/cursive/digit(4).fle",
        "digits/cursive/digit(5).fle",
        "digits/cursive/digit(6).fle",
        "digits/cursive/digit(7).fle",
        "digits/cursive/digit(8).fle",
        "digits/cursive/digit(9).fle",
        "digits/purple/digit(0).fle",
        "digits/purple/digit(1).fle",
        "digits/purple/digit(2).fle",
        "digits/purple/digit(3).fle",
        "digits/purple/digit(4).fle",
        "digits/purple/digit(5).fle",
        "digits/purple/digit(6).fle",
        "digits/purple/digit(7).fle",
        "digits/purple/digit(8).fle",
        "digits/purple/digit(9).fle",
        "resource/green_ball.fle",
        "resource/inverse_thorn.fle"

};

int **images;
int widths[image_count], heights[image_count];
void image_init() {
    images = new int*[image_count];
}
void iLoadImg(int idx,  int ignoreColor) {

    AUX_RGBImageRec *TextureImage;
    TextureImage = auxDIBImageLoad(image_dir[idx]);
    int i,j,k;
    int width = TextureImage->sizeX;
    int height = TextureImage->sizeY;
    int nPixels = width * height;
    widths[idx] =  width;
    heights[idx] = height;

    images[idx] = new int[nPixels];
    for (i = 0, j=0; i < nPixels; i++, j += 3)
    {
        int rgb = 0;
        for(int k = 2; k >= 0; k--)
        {
            rgb = ((rgb << 8) | TextureImage->data[j+k]);
        }

        images[idx][i] = (rgb == ignoreColor) ? 0 : 255;
        images[idx][i] = ((images[idx][i] << 24) | rgb);
    }
}

void iShowImg(int x, int y, int idx) {
    glRasterPos2f(x, y);
    glDrawPixels(widths[idx], heights[idx], GL_RGBA, GL_UNSIGNED_BYTE, images[idx]);
}

