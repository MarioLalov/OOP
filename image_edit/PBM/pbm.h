#ifndef PBM_H
#define PBM_H
#include "../Image/image.h"

class PBM : public Image
{
private:
    int **picture;
    int **editingPicture = nullptr;

    //needed because with PBM grayscale values are flipped
    int flipPixel(std::size_t x, std::size_t y) const;
    int **allocateNew(std::size_t widht, std::size_t height);
    void deleteArr(int **arr, std::size_t curHeight);
    bool validate(std::ifstream &file);

public:
    PBM(std::string in_format, int in_width, int in_height, int color);
    PBM(std::string in_format, std::ifstream &file);
    Rgb getPixelRgb(std::size_t x, std::size_t y) const;
    int getPixelGrayscale(std::size_t x, std::size_t y) const;
    void setPixel(std::size_t x, std::size_t y, Rgb value);
    void startDimensionEditing(std::size_t new_width, std::size_t new_height);
    void endDimensionEditing(std::size_t new_width, std::size_t new_height);
    void copyToEditing(std::size_t srcX, std::size_t srcY, std::size_t destX, std::size_t destY);
    void writePixel(std::size_t x, std::size_t y, std::ofstream &file, std::string extension);
    void writeFormatInfo(std::ofstream &file, std::string extension);
    void print();
    ~PBM();
};

#endif