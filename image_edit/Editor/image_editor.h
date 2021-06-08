#ifndef IMAGE_EDITOR_H
#define IMAGE_EDITOR_H

#include "../Image/image.h"

class ImageEditor
{
public:
    /*!
    *crop image
    *@param[in, out] image image to be cropped
    */
    static void crop(Image *image, std::size_t upper_x, std::size_t upper_y, std::size_t lower_x, std::size_t lower_y);
    /*!
    *resize image
    *@param[in, out] image image to be resized
    *@param[in] widthInput width after resize
    *@param[in] heightInput height after resize
    *@param[in] percentage if true interpret widthInpt and heightInput as percentage
    */
    static void resize(Image *image, int widthInput, int heightInput, bool percentage);
    //dithering algorithms
    /*!
    *dither image
    *@param[in, out] image image to be dithered
    */
    static void errorDiffusion(Image *image);
    /*!
    *dither image
    *@param[in, out] image image to be dithered
    */
    static void twoDimErrorDiffusion(Image *image);
    /*!
    *dither image
    *@param[in, out] image image to be dithered
    */
    static void floydDithering(Image *image);
    /*!
    *dither image
    *@param[in, out] image image to be dithered
    */
    static void jarvisDithering(Image *image);
    /*!
    *dither image
    *@param[in, out] image image to be dithered
    */
    static void stuckiDithering(Image *image);
    /*!
    *dither image
    *@param[in, out] image image to be dithered
    */
    static void atkinsonDithering(Image *image);
    /*!
    *dither image
    *@param[in, out] image image to be dithered
    */
    static void burkesDithering(Image *image);
    /*!
    *dither image
    *@param[in, out] image image to be dithered
    */
    static void sierraDithering(Image *image);
    /*!
    *dither image
    *@param[in, out] image image to be dithered
    */
    static void twoRowSierra(Image *image);
    /*!
    *dither image
    *@param[in, out] image image to be dithered
    */
    static void sierraLite(Image *image);
    /*!
    *dither image
    *@param[in, out] image image to be dithered
    */
    static void orderedDithering8x8(Image *image);
};

//helpers
/*!
*calculate pixel using (NxN) treshold map
*/
double thresholdMap(std::size_t i, std::size_t j, int n);

#endif