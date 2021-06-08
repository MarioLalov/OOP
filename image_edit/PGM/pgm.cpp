#include "pgm.h"

void PGM::deleteArr(int **arr, std::size_t curHeight)
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

int **PGM::allocateNew(std::size_t curWidth, std::size_t curHeight)
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

PGM::PGM(std::string in_format, int in_width, int in_height, int color)
{
    //validate width and height
    if (in_width < 0 || in_height < 0)
    {
        throw std::invalid_argument("Invalid file dimensions!");
    }

    if (color > 255 || color < 0)
    {
        throw std::invalid_argument("Unsupported color!");
    }

    //assign values
    format = in_format;
    width = in_width;
    height = in_height;
    maxGrayscaleValue = 255;

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

PGM::PGM(std::string in_format, std::ifstream &file)
{
    //set format
    format = in_format;

    if (!file.is_open())
    {
        throw std::invalid_argument("File problem occured!");
    }

    (format == "P2") ? readText(file) : readBinary(file);
}

int PGM::getPixelGrayscale(std::size_t x, std::size_t y) const
{
    //throw if out of range
    if (x >= width || y >= height || x < 0 || y < 0)
    {
        throw std::out_of_range("Out of range");
    }

    //return grayscale value
    return picture[y][x];
}

Rgb PGM::getPixelRgb(std::size_t x, std::size_t y) const
{
    //throw if out of range
    if (x >= width || y >= height || x < 0 || y < 0)
    {
        throw std::out_of_range("Out of range");
    }

    //return value in RGB
    return grayscaleToRgb(picture[y][x], maxGrayscaleValue);
}

void PGM::setPixel(std::size_t x, std::size_t y, Rgb value)
{
    picture[y][x] = RgbToGrayscale(value, maxGrayscaleValue);
}

void PGM::startDimensionEditing(std::size_t new_width, std::size_t new_height)
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

void PGM::copyToEditing(std::size_t srcX, std::size_t srcY, std::size_t destX, std::size_t destY)
{
    editingPicture[destY][destX] = picture[srcY][srcX];
}

void PGM::endDimensionEditing()
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

void PGM::validateFormat(std::string extension)
{
    if (extension != "pgm" && extension != "ppm")
    {
        throw std::invalid_argument("Incompatible file formats");
    }
}

void PGM::writePixel(std::size_t x, std::size_t y, std::ofstream &file, std::string extension, bool binary)
{
    binary ? writeBinaryPixel(x, y, file, extension) : writeTextPixel(x, y, file, extension);
}

void PGM::writeFormatInfo(std::ofstream &file, std::string extension, bool binary)
{
    binary ? writeBinaryFormat(file, extension) : writeTextFormat(file, extension);
}

void PGM::writeTextFormat(std::ofstream &file, std::string extension)
{
    if (extension == "ppm")
    {
        file << "P3" << std::endl;
        file << width << " " << height << std::endl;
        file << 255 << std::endl;
    }
    else
    {
        file << "P2" << std::endl;
        file << width << " " << height << std::endl;
        file << maxGrayscaleValue << std::endl;
    }
}

void PGM::writeTextPixel(std::size_t x, std::size_t y, std::ofstream &file, std::string extension)
{
    if (extension == "pgm")
    {
        file << getPixelGrayscale(x, y) << " ";
    }
    else if (extension == "ppm")
    {
        Rgb value = getPixelRgb(x, y);

        file << value.red << " " << value.green << " " << value.blue << std::endl;
    }
}

void PGM::writeBinaryFormat(std::ofstream &file, std::string extension)
{
    char empty = ' ';

    if (extension == "ppm")
    {
        std::string newFormat = "P6";
        int maxValue = 255;

        file.write((char *)&newFormat[0], 1);
        file.write((char *)&newFormat[1], 1);
        file.write((char *)&empty, 1);

        writeNumberBinary(convertToASCII(width), file);
        writeNumberBinary(convertToASCII(height), file);
        writeNumberBinary(convertToASCII(maxValue), file);
    }
    else
    {
        std::string newFormat = "P5";

        file.write((char *)&newFormat[0], 1);
        file.write((char *)&newFormat[1], 1);
        file.write((char *)&empty, 1);

        writeNumberBinary(convertToASCII(width), file);
        writeNumberBinary(convertToASCII(height), file);
        writeNumberBinary(convertToASCII(maxGrayscaleValue), file);
    }
}

void PGM::readText(std::ifstream &file)
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

    file >> maxGrayscaleValue;
    checkForComments(file);

    if (maxGrayscaleValue < 0 || maxGrayscaleValue > 255)
    {
        throw std::invalid_argument("Unsupported color!");
    }

    //allocate memory
    picture = allocateNew(width, height);

    //assign pixel values and check for potential errors
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            file >> picture[i][j];
            checkForComments(file);

            if (picture[i][j] < 0 || picture[i][j] > maxGrayscaleValue)
            {
                //if pixel is invalid delete picture and mark as deleted
                deleteArr(picture, height);
                picture = nullptr;

                throw std::invalid_argument("Invalid pixel value!");
            }
        }
    }
}

void PGM::writeBinaryPixel(std::size_t x, std::size_t y, std::ofstream &file, std::string extension)
{
    if (extension == "pgm")
    {
        int value;
        value = getPixelGrayscale(x, y);

        writeGrayscalePixelBinary(value, file);
    }
    else if (extension == "ppm")
    {
        Rgb value = getPixelRgb(x, y);
        writeRgbPixelBinary(value, file);
    }
}

void PGM::readBinary(std::ifstream &file)
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

    maxGrayscaleValue = getNumberBinary(file);
    checkForCommentsBinary(file);

    if (maxGrayscaleValue < 0 || maxGrayscaleValue > 255)
    {
        throw std::invalid_argument("Unsupported color!");
    }

    //allocate memory
    picture = allocateNew(width, height);

    //assign pixel values
    file.get();
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            picture[i][j] = getGrayscaleBinary(file);

            if (picture[i][j] < 0 || picture[i][j] > maxGrayscaleValue)
            {
                //if pixel is invalid delete picture and mark as deleted
                deleteArr(picture, height);
                picture = nullptr;

                throw std::invalid_argument("Invalid pixel value!");
            }
        }
    }
}

PGM::~PGM()
{
    deleteArr(picture, height);
}
