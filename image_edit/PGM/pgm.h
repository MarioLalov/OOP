#ifndef PGM_H
#define PGM_H
#include "../Image/image.h"

class PGM : public Image
{
private:
    int maxGrayscaleValue;
    int **picture;
    int **editingPicture = nullptr;

    int** allocateNew(std::size_t widht, std::size_t height);
    void deleteArr(int** arr, std::size_t curHeight);

public:
    PGM(std::string in_format, std::ifstream &file);
    //void crop(std::size_t upper_x, std::size_t upper_y, std::size_t lower_x, std::size_t lower_y);
    Rgb getPixelRgb(std::size_t x, std::size_t y) const;
    int getPixelGrayscale(std::size_t x, std::size_t y) const;
    void setPixel(std::size_t x, std::size_t y, Rgb value);
    void startDimensionEditing(std::size_t new_width, std::size_t new_height);
    void endDimensionEditing(std::size_t new_width, std::size_t new_height);
    void copyToEditing(std::size_t srcX, std::size_t srcY, std::size_t destX, std::size_t destY);
    void print();
    void createCropped(std::size_t upper_x, std::size_t upper_y, std::size_t lower_x, std::size_t lower_y);
    void createResized(std::size_t newWidth, std::size_t newHeight);
    void writePixel(std::size_t x, std::size_t y, std::ofstream& file);
    void writeFormatInfo(std::ofstream& file);
    ~PGM();
};

#endif