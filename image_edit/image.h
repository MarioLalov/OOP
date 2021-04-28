#include <iostream>
#include <string>
#include <fstream>

#ifndef IMAGE_H
#define IMAGE_H

typedef int *RGB;

class Image
{
protected:
    std::string output_location;
    std::string format;
    std::size_t width;
    std::size_t height;

    static RGB grayscaleToRGB(int value, int max_value);
    static int RGBToGrayscale(RGB, int max_value);
    void write(std::size_t x, std::size_t y, std::ofstream& file);

public:
    void crop(std::size_t upper_x, std::size_t upper_y, std::size_t lower_x, std::size_t lower_y);
    void resize(int widthInput, int heightInput, bool percentage);
    virtual RGB getPixelRGB(std::size_t x, std::size_t y) const = 0;
    virtual int getPixelGrayscale(std::size_t x, std::size_t y) const = 0;
    virtual void setPixel(std::size_t x, std::size_t y, RGB value) = 0;
    //virtual ~Image();
};
//helpers
//int *getRGB(int value, int max);

#endif

//120 - 600x800 -> 720x960