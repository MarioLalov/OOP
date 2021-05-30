#ifndef IMAGE_EDITOR_H
#define IMAGE_EDITOR_H

#include "../Image/image.h"

class ImageEditor
{
public:
    static void crop(Image *image, std::size_t upper_x, std::size_t upper_y, std::size_t lower_x, std::size_t lower_y);
    static void resize(Image *image, int widthInput, int heightInput, bool percentage);
    static void errorDiffusion(Image *image);
    static void twoDimErrorDiffusion(Image *image);
    static void floydDithering(Image *image);
    static void jarvisDithering(Image *image);
    static void stuckiDithering(Image *image);
    static void atkinsonDithering(Image *image);
    static void burkesDithering(Image *image);
    static void sierraDithering(Image *image);
    static void twoRowSierra(Image *image);
    static void sierraLite(Image *image);
    static void orderedDithering(Image* image, int n);
};

#endif