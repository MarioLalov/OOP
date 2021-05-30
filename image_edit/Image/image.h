#include <iostream>
#include <string>
#include <fstream>

#ifndef IMAGE_H
#define IMAGE_H

struct Rgb
{
    int red;
    int green;
    int blue;

    Rgb();
    Rgb(int in_red, int in_green, int in_blue);
    Rgb &operator=(const Rgb &other);
    Rgb operator+(Rgb const &other);
    Rgb operator*(int const &num);
    Rgb operator/(int const &num);
};

//template<class T>
class Image
{
protected:
    //std::string output_location;
    std::string format;
    std::size_t width;
    std::size_t height;

    virtual void writePixel(std::size_t x, std::size_t y, std::ofstream &file) = 0;
    virtual void writeFormatInfo(std::ofstream &file) = 0;

public:
    void crop(std::size_t upper_x, std::size_t upper_y, std::size_t lower_x, std::size_t lower_y);
    void resize(int widthInput, int heightInput, bool percentage);
    std::size_t getWidth() const;
    std::size_t getHeight() const;
    virtual void startDimensionEditing(std::size_t width, std::size_t height) = 0;
    virtual void copyToEditing(std::size_t srcX, std::size_t srcY, std::size_t destX, std::size_t destY) = 0;
    virtual void endDimensionEditing(std::size_t width, std::size_t height) = 0;
    virtual Rgb getPixelRgb(std::size_t x, std::size_t y) const = 0;
    virtual void setPixel(std::size_t x, std::size_t y, Rgb value) = 0;
    void write(std::string output_location);
    void errorDiffusion();
    void twoDimErrorDiffusion();
    void floydDithering();
    void jarvisDithering();
    void stuckiDithering();
    void atkinsonDithering();
    void burkesDithering();
    void sierraDithering();
    void twoRowSierra();
    void sierraLite();
    //virtual ~Image();
};
//helpers
void checkForComments(std::ifstream &file);
int roundToInt(double num);
Rgb grayscaleToRgb(int value, int max_value);
int RgbToGrayscale(Rgb, int max_value);

#endif

//120 - 600x800 -> 720x960