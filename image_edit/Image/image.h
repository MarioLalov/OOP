#include <iostream>
#include <string>
#include <fstream>

#ifndef IMAGE_H
#define IMAGE_H

struct RGB
{
    unsigned int red;
    unsigned int green;
    unsigned int blue;

    RGB();
    RGB(unsigned int in_red, unsigned int in_green, unsigned int in_blue);
    RGB& operator=(const RGB& other);
};

class Image
{
protected:
    std::string output_location;
    std::string format;
    std::size_t width;
    std::size_t height;

    static RGB grayscaleToRGB(int value, int max_value);
    static int RGBToGrayscale(RGB, int max_value);
    virtual void writePixel(std::size_t x, std::size_t y, std::ofstream& file) = 0;
    virtual void writeFormatInfo(std::ofstream& file) = 0;

public:
    void crop(std::size_t upper_x, std::size_t upper_y, std::size_t lower_x, std::size_t lower_y);
    void resize(int widthInput, int heightInput, bool percentage);
    std::size_t getWidth() const;
    std::size_t getHeight() const;
    virtual RGB getPixelRGB(std::size_t x, std::size_t y) const = 0;
    virtual int getPixelGrayscale(std::size_t x, std::size_t y) const = 0;
    virtual void setPixel(std::size_t x, std::size_t y, RGB value) = 0;
    virtual void createCropped(std::size_t upper_x, std::size_t upper_y, std::size_t lower_x, std::size_t lower_y) = 0;
    virtual void createResized(std::size_t newWidth, std::size_t newHeight) = 0;
    void write();
    //virtual ~Image();
};
//helpers
void checkForComments(std::ifstream &file);
int roundToInt(double num);

#endif

//120 - 600x800 -> 720x960