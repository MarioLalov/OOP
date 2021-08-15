#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#ifndef IMAGE_H
#define IMAGE_H

struct Rgb
{
    int red;
    int green;
    int blue;

    //constructors
    /*!
    *default constructor
    *sets all values to 0
    */
    Rgb();
    /*!
    *parametric construtor
    *sets values to coresponding input
    */
    Rgb(int in_red, int in_green, int in_blue);
    /*!
    *parametric construtor
    *sets values from hex
    */
    Rgb(std::string &hex);

    //operators
    /*!
    *assignment operator
    */
    Rgb &operator=(const Rgb &other);
    /*!
    *Rgb values addition operator
    */
    Rgb operator+(Rgb const &other);
    /*!
    *multiply all color values by num
    */
    Rgb operator*(int const &num);
    /*!
    *divide all color values by num
    */
    Rgb operator/(int const &num);
};

class Image
{
protected:
    std::string format;
    std::size_t width;
    std::size_t height;

    //output in file virtuals
    virtual void validateFormat(std::string extension) = 0;
    virtual void writePixel(std::size_t x, std::size_t y, std::ofstream &file, std::string extension, bool binary) = 0;
    virtual void writeFormatInfo(std::ofstream &file, std::string extension, bool binary) = 0;

public:
    //factory
    /*!
    *factory method for image with background color
    *@param[in] in_width width of image to be created
    *@param[in] in_height height of image to be created
    *@param[in] color color in hex
    */
    static Image *createImageByColor(int in_width, int in_height, std::string color);
    /*!
    *factory method for image with format
    *@param[in] format format of image to be created
    *@param[in] in_width width of image to be created
    *@param[in] in_height height of image to be created
    */
    static Image *createImageByFormat(std::string format, int in_width, int in_height);

    //getters
    std::string getFormat() const;
    std::size_t getWidth() const;
    std::size_t getHeight() const;

    //virtual pixel getters
    virtual int getPixelGrayscale(std::size_t x, std::size_t y) const = 0;
    virtual Rgb getPixelRgb(std::size_t x, std::size_t y) const = 0;
    virtual void setPixel(std::size_t x, std::size_t y, Rgb value) = 0;

    //write in output file
    /*!
    *write in output file
    *
    *throws when attempting to write in an incomatible file format
    *@param[in] output_location output file path
    *@param[in] extension extension of output file
    */
    void write(std::string output_location, std::string extension);
    //virtual ~Image();
};

//helpers
/*!
*checks for comments from current position
*@param file file to read from
*/
void checkForComments(std::ifstream &file);
int roundToInt(double num);
bool getSaveFormat();

/*!
*pack grayscale in Rgb
*/
Rgb grayscaleToRgb(int value, int max_value);
/*!
*get grayscale value from Rgb
*/
int RgbToGrayscale(Rgb, int max_value);

//binary files
/*!
*convert int to ASCII
*@param num number to be converted
*/
std::string convertToASCII(int num);
/*!
*write number in binary file
*@param number number to be written
*@param file file to write in
*/
void writeNumberBinary(std::string number, std::ofstream &file);
/*!
*write Rgb value in binary file
*@param color Rgb value to be written
*@param file file to write in
*/
void writeRgbPixelBinary(Rgb color, std::ofstream &file);
/*!
*write grayscale value in binary file
*@param color grayscale value to be written
*@param file file to write in
*/
void writeGrayscalePixelBinary(int color, std::ofstream &file);
/*!
*check for comments from current position in binary
*@param file file to read from
*/
void checkForCommentsBinary(std::ifstream &file);
/*!
*get number from binary
*@param file file to read from
*/
int getNumberBinary(std::ifstream &file);
/*!
*get grayscale from binary
*@param file file to read from
*/
int getGrayscaleBinary(std::ifstream &file);
/*!
*get Rgb value from binary
*@param file file to read from
*/
Rgb getRgbBinary(std::ifstream &file);

#endif
