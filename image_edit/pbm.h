#ifndef PBM_H
#define PBM_H
#include "image.h"

class PBM : public Image
{
private:
    int **picture;

    //needed because with PBM grayscale values are flipped
    int flipPixel(std::size_t x, std::size_t y) const;

public:
    PBM(std::string in_format, std::ifstream &file, std::string output_location);
    //void crop(std::size_t upper_x, std::size_t upper_y, std::size_t lower_x, std::size_t lower_y);
    RGB getPixelRGB(std::size_t x, std::size_t y) const;
    int getPixelGrayscale(std::size_t x, std::size_t y) const;
    void setPixel(std::size_t x, std::size_t y, RGB value);
    void createResized(std::size_t newWidth, std::size_t newHeight);
    void print();
    ~PBM();
};

#endif