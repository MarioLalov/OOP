#ifndef PPM_H
#define PPM_H
#include "../Image/image.h"

class PPM : public Image
{
private:
    int maxRGBValue;
    RGB **picture;

    RGB **allocateNew(std::size_t widht, std::size_t height);
    void deleteArr(RGB **arr, std::size_t curHeight, std::size_t curWidht);

public:
    PPM(std::string in_format, std::ifstream &file, std::string output_location);
    //void crop(std::size_t upper_x, std::size_t upper_y, std::size_t lower_x, std::size_t lower_y);
    RGB getPixelRGB(std::size_t x, std::size_t y) const;
    int getPixelGrayscale(std::size_t x, std::size_t y) const;
    void setPixel(std::size_t x, std::size_t y, RGB value);
    void print();
    void createCropped(std::size_t upper_x, std::size_t upper_y, std::size_t lower_x, std::size_t lower_y);
    void createResized(std::size_t newWidth, std::size_t newHeight);
    void writePixel(std::size_t x, std::size_t y, std::ofstream &file);
    void writeFormatInfo(std::ofstream &file);
    ~PPM();
};

#endif