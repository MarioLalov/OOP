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
    void deleteArr(Rgb **arr, std::size_t curHeight);

public:
    PPM(std::string in_format, int in_width, int in_height, Rgb color);
    PPM(std::string in_format, std::ifstream &file);
    Rgb getPixelRgb(std::size_t x, std::size_t y) const;
    int getPixelGrayscale(std::size_t x, std::size_t y) const;
    Image* createEditingPicture(std::size_t new_width, std::size_t new_height);
    void startDimensionEditing(std::size_t new_width, std::size_t new_height);
    void copyToEditing(std::size_t srcX, std::size_t srcY, std::size_t destX, std::size_t destY);
    void endDimensionEditing(std::size_t new_width, std::size_t new_height);
    void setPixel(std::size_t x, std::size_t y, Rgb value);
    void print();
    void writePixel(std::size_t x, std::size_t y, std::ofstream &file, std::string extension);
    void writeFormatInfo(std::ofstream &file, std::string extension);
    ~PPM();
};

#endif