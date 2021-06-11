#include "pbm.h"
#include <iostream>

void PBM::deleteArr(int **arr, std::size_t curHeight)
{
    //delete everything that was successfully allocated
    if (arr != nullptr)
    {
        for (int i = 0; i < curHeight; i++)
        {
            if (arr[i])
            {
                delete[] arr[i];
            }
        }

        delete[] arr;
    }
}

int **PBM::allocateNew(std::size_t curWidth, std::size_t curHeight)
{
    //allocate and if an error occurs delete the allocated memory
    int **pic = nullptr;

    try
    {
        pic = new int *[curHeight];

        for (int i = 0; i < curHeight; i++)
        {
            pic[i] = nullptr;
        }

        for (int i = 0; i < curHeight; i++)
        {
            pic[i] = new int[curWidth];
        }

        return pic;
    }
    catch (const std::bad_alloc &err)
    {
        deleteArr(pic, curHeight);

        throw err;
    }
}

PBM::PBM(std::string in_format, int in_width, int in_height, int color)
{
    //validate width and height
    if (in_width < 0 || in_height < 0)
    {
        throw std::invalid_argument("Invalid file dimensions!");
    }

    if (color != 1 && color != 0)
    {
        throw std::invalid_argument("Unsupported color!");
    }

    //assign values
    format = in_format;
    width = in_width;
    height = in_height;

    //allocate picture
    picture = allocateNew(width, height);

    //set background color
    for (std::size_t i = 0; i < height; i++)
    {
        for (std::size_t j = 0; j < width; j++)
        {
            picture[i][j] = color;
        }
    }
}

PBM::PBM(std::string in_format, std::ifstream &file)
{
    //set format
    format = in_format;

    if (!file.is_open())
    {
        throw std::invalid_argument("File problem occured!");
    }

    (format == "P1") ? readText(file) : readBinary(file);
}

int PBM::flipPixel(std::size_t x, std::size_t y) const
{
    return !picture[y][x];
}

Rgb PBM::getPixelRgb(std::size_t x, std::size_t y) const
{
    //throw if out of range
    if (x >= width || y >= height || x < 0 || y < 0)
    {
        throw std::out_of_range("Out of range");
    }

    //return value in rgb
    return grayscaleToRgb(flipPixel(x, y), 1);
}

int PBM::getPixelGrayscale(std::size_t x, std::size_t y) const
{
    if (x >= width || y >= height || x < 0 || y < 0)
    {
        throw std::out_of_range("Out of range!");
    }

    //return pixe in grayscale
    return flipPixel(x, y);
}

void PBM::setPixel(std::size_t x, std::size_t y, Rgb value)
{
    picture[y][x] = RgbToGrayscale(value, 1);
}

void PBM::startDimensionEditing(std::size_t new_width, std::size_t new_height)
{
    //check if edinting has started
    if (editingPicture)
    {
        throw std::logic_error("Operation already started!");
    }

    //create editing picture
    editingPicture = allocateNew(new_width, new_height);

    //assign new picture dimensions
    editingWidth = new_width;
    editingHeight = new_height;
}

void PBM::copyToEditing(std::size_t srcX, std::size_t srcY, std::size_t destX, std::size_t destY)
{
    editingPicture[destY][destX] = picture[srcY][srcX];
}

void PBM::endDimensionEditing()
{
    //check if process is started
    if (!editingPicture)
    {
        throw std::logic_error("Operation is not started!");
    }

    //delete old picture and assign new one
    deleteArr(picture, height);
    picture = editingPicture;
    editingPicture = nullptr;

    //assign new dimensions
    width = editingWidth;
    height = editingHeight;
}

void PBM::validateFormat(std::string extension)
{
    if (extension != "pbm" && extension != "pgm" && extension != "ppm")
    {
        throw std::invalid_argument("Incompatible file formats");
    }
}

void PBM::writePixel(std::size_t x, std::size_t y, std::ofstream &file, std::string extension, bool binary)
{
    binary ? writeBinaryPixel(x, y, file, extension) : writeTextPixel(x, y, file, extension);
}

void PBM::writeFormatInfo(std::ofstream &file, std::string extension, bool binary)
{
    binary ? writeBinaryFormat(file, extension) : writeTextFormat(file, extension);
}

void PBM::writeTextFormat(std::ofstream &file, std::string extension)
{
    if (extension == "pgm")
    {
        file << "P2" << std::endl;
    }
    else if (extension == "ppm")
    {
        file << "P3" << std::endl;
    }
    else
    {
        file << "P1" << std::endl;
    }

    file << width << " " << height << std::endl;

    if (extension == "ppm" || extension == "pgm")
    {
        file << 255 << std::endl;
    }
}

void PBM::writeTextPixel(std::size_t x, std::size_t y, std::ofstream &file, std::string extension)
{
    if (extension == "pbm")
    {
        file << !getPixelGrayscale(x, y) << " ";
    }
    else if (extension == "pgm")
    {
        int value = (getPixelGrayscale(x, y) == 0) ? 0 : 255;
        file << value << " ";
    }
    else if (extension == "ppm")
    {
        Rgb value = getPixelRgb(x, y);
        file << value.red << " " << value.green << " " << value.blue << std::endl;
    }
}

void PBM::writeBinaryFormat(std::ofstream &file, std::string extension)
{
    char empty = ' ';

    if (extension == "pgm")
    {
        std::string newFormat = "P5";

        file.write((char *)&newFormat[0], 1);
        file.write((char *)&newFormat[1], 1);
        file.write((char *)&empty, 1);
    }
    else if (extension == "ppm")
    {
        std::string newFormat = "P6";

        file.write((char *)&newFormat[0], 1);
        file.write((char *)&newFormat[1], 1);
        file.write((char *)&empty, 1);
    }

    writeNumberBinary(convertToASCII(width), file);
    writeNumberBinary(convertToASCII(height), file);

    if (extension == "ppm" || extension == "pgm")
    {
        int maxValue = 255;
        writeNumberBinary(convertToASCII(maxValue), file);
    }
}

void PBM::writeBinaryPixel(std::size_t x, std::size_t y, std::ofstream &file, std::string extension)
{
    if (extension == "pgm")
    {
        int value = (getPixelGrayscale(x, y) == 0) ? 0 : 255;
        writeGrayscalePixelBinary(value, file);
    }
    else if (extension == "ppm")
    {
        Rgb value = getPixelRgb(x, y);
        writeRgbPixelBinary(value, file);
    }
}

void PBM::readText(std::ifstream &file)
{
    int in_width, in_height;

    //get width, height and max RGB value and check for errors
    checkForComments(file);
    file >> in_width >> in_height;
    checkForComments(file);

    if (in_width < 0 || in_height < 0)
    {
        throw std::invalid_argument("Invalid file dimensions!");
    }

    width = in_width;
    height = in_height;

    //allocate memory
    picture = allocateNew(width, height);

    //assign pixel values and check for potential errors
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            file >> picture[i][j];
            checkForComments(file);

            if (picture[i][j] != 1 && picture[i][j] != 0)
            {
                //if pixel is invalid delete picture and mark as deleted
                deleteArr(picture, height);
                picture = nullptr;

                throw std::invalid_argument("Invalid pixel value!");
            }
        }
    }
}

void PBM::readBinary(std::ifstream &file)
{
    int in_width, in_height;

    file.get();
    checkForCommentsBinary(file);
    in_width = getNumberBinary(file);
    checkForCommentsBinary(file);
    in_height = getNumberBinary(file);
    checkForCommentsBinary(file);

    if (in_width < 0 || in_height < 0)
    {
        throw std::invalid_argument("Invalid file dimensions!");
    }

    width = in_width;
    height = in_height;

    //allocate memory
    picture = allocateNew(width, height);

    //assign pixel values
    file.get();
    for (int i = 0; i < height; i++)
    {
        int j = 0;
        while (j < width)
        {
            int value = getGrayscaleBinary(file);

            for (int k = 7; k >= 0; k--)
            {
                int thebit = (value >> k) & 1;
                picture[i][j] = thebit;

                if (picture[i][j] != 1 && picture[i][j] != 0)
                {
                    //if pixel is invalid delete picture and mark as deleted
                    deleteArr(picture, height);
                    picture = nullptr;

                    throw std::invalid_argument("Invalid pixel value!");
                }

                j++;
                if (j >= width)
                {
                    break;
                }
            }
        }
    }
}

PBM::~PBM()
{
    deleteArr(picture, height);
}