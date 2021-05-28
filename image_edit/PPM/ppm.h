#ifndef PPM_H
#define PPM_H
#include "../Image/image.h"

class PPM : public Image
{
private:
    int maxRgbValue;
    Rgb **picture;
    Rgb **editingPicture = nullptr;

    Rgb **allocateNew(std::size_t curWidth, std::size_t curHeight);
    void deleteArr(Rgb **arr, std::size_t curHeight, std::size_t curWidht);
    void startEditing(std::size_t new_width, std::size_t new_height);
    void endEditing();

public:
    PPM(std::string in_format, std::ifstream &file, std::string output_location);
    //void crop(std::size_t upper_x, std::size_t upper_y, std::size_t lower_x, std::size_t lower_y);
    Rgb getPixelRgb(std::size_t x, std::size_t y) const;
    int getPixelGrayscale(std::size_t x, std::size_t y) const;
    void setPixel(std::size_t x, std::size_t y, Rgb value);
    void copyToEditing(std::size_t srcX, std::size_t srcY, std::size_t destX, std::size_t destY);
    void print();
    //void createCropped(std::size_t upper_x, std::size_t upper_y, std::size_t lower_x, std::size_t lower_y);
    //void createResized(std::size_t newWidth, std::size_t newHeight);
    void writePixel(std::size_t x, std::size_t y, std::ofstream &file);
    void writeFormatInfo(std::ofstream &file);
    ~PPM();
};

#endif