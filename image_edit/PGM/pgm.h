#ifndef PGM_H
#define PGM_H
#include "../Image/image.h"

class PGM : public Image
{
private:
    int maxGrayscaleValue;
    int **picture;

    //memory management functions
    /*!
    *memory allocator
    *@param[in] curWidth width of array
    *@param[in] curHeight height of array
    */
    int **allocateNew(std::size_t widht, std::size_t height);
    /*!
    *memory deletion
    *@param[in] curHeight height of array
    */
    void deleteArr(int **arr, std::size_t curHeight);

    //output in file functions
    /*!
    *check if format is compatible with the given extension
    */
    void validateFormat(std::string extension);
    /*!
    *write pixel on position depending on the format of the output file
    */
    void writePixel(std::size_t x, std::size_t y, std::ofstream &file, std::string extension, bool binary);
    /*!
    *write format info depending on the format of the output file
    */
    void writeFormatInfo(std::ofstream &file, std::string extension, bool binary);

    /*!
    *write format info in ASCII
    */
    void writeTextFormat(std::ofstream &file, std::string extension);
    /*!
    *write pixel info in ASCII
    */
    void writeTextPixel(std::size_t x, std::size_t y, std::ofstream &file, std::string extension);
    /*!
    *write format info in binary
    */
    void writeBinaryFormat(std::ofstream &file, std::string extension);
    /*!
    *write pixel info in binary
    */
    void writeBinaryPixel(std::size_t x, std::size_t y, std::ofstream &file, std::string extension);

    /*!
    *read image info from binary
    */
    void readBinary(std::ifstream &file);
    /*!
    *read image info from ASCII
    */
    void readText(std::ifstream &file);

public:
    //constructors
    /*!
    *default construtor
    *
    *creates new picture with background color
    *@param[in] in_format format of picture
    *@param[in] in_width width of picture
    *@param[in] in_width height of picture
    *@param[in] in_width background of picture
    */
    PGM(std::string in_format, int in_width, int in_height, int color);
    /*!
    *parametric construtor
    *
    *creates new picture reading from the file input
    *
    *throws when a problem with the file occurs
    *@param[in] in_format format of picture
    *@param[in] file file to read from
    */
    PGM(std::string in_format, std::ifstream &file);

    //pixel getters and setters
    /*!
    *return pixel value at certain position packed in an Rgb value
    *
    *first the grayscle value is transformed to 0-255
    *
    *then set into Rgb value in the following manner a->(a,a,a)
    *@param[in] x x coordiante of pixel
    *@param[in] y y coordinate of pixel
    */
    Rgb getPixelRgb(std::size_t x, std::size_t y) const;
    /*!
    *return grayscale value at certain position
    *
    *throws when out of range
    *@param[in] x x coordiante of pixel
    *@param[in] y y coordinate of pixel
    */
    int getPixelGrayscale(std::size_t x, std::size_t y) const;
    /*!
    *set pixel value at certain position
    *
    *throws when out of range
    *@param[in] x x coordiante of pixel
    *@param[in] y y coordinate of pixel
    *@param[in] value Rgb value to be assigned that is transformed to grayscale before setting
    */
    void setPixel(std::size_t x, std::size_t y, Rgb value);
    
    /*!
    *erase picture
    */
    ~PGM();
};

#endif