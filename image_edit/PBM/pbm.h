#ifndef PBM_H
#define PBM_H
#include "../Image/image.h"

class PBM : public Image
{
private:
    int **picture;
    //! buffer
    int **editingPicture = nullptr;

    //needed because with PBM monochrome values are flipped
    /*!
    *flip pixel value because in pbm the black and white are reversed
    */
    int flipPixel(std::size_t x, std::size_t y) const;

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
    *@param[in] in_height height of picture
    *@param[in] color background of picture
    */
    PBM(std::string in_format, int in_width, int in_height, int color);
    /*!
    *parametric construtor
    *
    *creates new picture reading from the file input
    *
    *throws when a problem with the file occurs
    *@param[in] in_format format of picture
    *@param[in] file file to read from
    */
    PBM(std::string in_format, std::ifstream &file);

    //pixel getters and setters
    /*!
    *return pixel value at certain position packed in an Rgb value
    *
    *the monochrome value is packed in an Rgb value 
    *
    *first the monochrome value is transformed to 0-255
    *
    *then set into an Rgb value in the following manner a->(a,a,a)
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
    *@param[in] value Rgb value to be assigned that is transformed to monochrome before setting
    */
    void setPixel(std::size_t x, std::size_t y, Rgb value);

    //editing with changes in dimensions
    /*!
    *open buffer for editing
    *
    *throws if buufer is already being used
    *@param[in] new_width width of new picture
    *@param[in] new_height height of new picture
    */
    void startDimensionEditing(std::size_t new_width, std::size_t new_height);
    /*!
    *copy from original picture to buffer
    *@param[in] srcX x coordiante of source pixel
    *@param[in] srcY y coordinate of source pixel
    *@param[in] destX x coordiante of destination pixel
    *@param[in] destY y coordinate of destination pixel
    */
    void copyToEditing(std::size_t srcX, std::size_t srcY, std::size_t destX, std::size_t destY);
    /*!
    *close buffer and copy to original picture
    *
    *throws if buffer is already set to nullptr
    */
    void endDimensionEditing();

    /*!
    *erase picture
    */
    ~PBM();
};

#endif