#ifndef PBM_H
#define PBM_H
#include "image.h"

class PBM : public Image
{
private:
    int **picture;

    //needed because with PBM grayscale values are flipped
    int flipPixel(std::size_t x, std::size_t y) const;
    int** allocateNew(std::size_t widht, std::size_t height);
    void deleteArr(int** arr, std::size_t curHeight);

public:
    PBM(std::string in_format, std::ifstream &file, std::string output_location);
    //void crop(std::size_t upper_x, std::size_t upper_y, std::size_t lower_x, std::size_t lower_y);
    RGB getPixelRGB(std::size_t x, std::size_t y) const;
    int getPixelGrayscale(std::size_t x, std::size_t y) const;
    void setPixel(std::size_t x, std::size_t y, RGB value);
    void createResized(std::size_t newWidth, std::size_t newHeight);
    void createCropped(std::size_t upper_x, std::size_t upper_y, std::size_t lower_x, std::size_t lower_y);
    void writePixel(std::size_t x, std::size_t y, std::ofstream& file);
    void writeFormatInfo(std::ofstream& file);
    void print();
    ~PBM();
};

#endif