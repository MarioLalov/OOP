#include "ppm.h"

void PPM::deleteArr(Rgb **arr, std::size_t curHeight)
{
    //delete everything that was successfully allocated
    if (arr != nullptr)
    {
        for (int i = 0; i < curHeight; i++)
        {
            if (arr[i] != nullptr)
            {
                delete[] arr[i];
            }
        }

        delete[] arr;
    }
}

Rgb **PPM::allocateNew(std::size_t curWidth, std::size_t curHeight)
{
    //allocate and if an error occurs delete the allocated memory
    Rgb **pic = nullptr;

    try
    {
        pic = new Rgb *[curHeight];

        for (int i = 0; i < curHeight; i++)
        {
            pic[i] = nullptr;
        }

        for (int i = 0; i < curHeight; i++)
        {
            pic[i] = new Rgb[curWidth];
        }

        return pic;
    }
    catch (const std::bad_alloc &err)
    {
        //delete everything that was allocated before the error occured
        deleteArr(pic, curHeight);

        throw err;
    }
}

PPM::PPM(std::string in_format, int in_width, int in_height, Rgb color)
{
    //validate width and height
    if (in_width < 0 || in_height < 0)
    {
        throw std::invalid_argument("Invalid file dimensions!");
    }

    if (color.red > 255 || color.red < 0 || color.green > 255 || color.green < 0 || color.blue > 255 || color.blue < 0)
    {
        throw std::invalid_argument("Unsupported color!");
    }

    //assign values
    format = in_format;
    width = in_width;
    height = in_height;
    maxRgbValue = 255;

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

PPM::PPM(std::string in_format, std::ifstream &file)
{
    //set format
    format = in_format;

    if (!file.is_open())
    {
        throw std::invalid_argument("File problem occured!");
    }

    (format == "P3") ? readText(file) : readBinary(file);
}

int PPM::getPixelGrayscale(std::size_t x, std::size_t y) const
{
    //throw if out of range
    if (x >= width || y >= height || x < 0 || y < 0)
    {
        throw std::out_of_range("Out of range");
    }

    //retrun grayscale value
    return (picture[y][x].red + picture[y][x].green + picture[y][x].blue) / 3;
}

Rgb PPM::getPixelRgb(std::size_t x, std::size_t y) const
{
    //throw if out of range
    if (x >= width || y >= height || x < 0 || y < 0)
    {
        throw std::out_of_range("Out of range");
    }

    Rgb to255(picture[y][x].red, picture[y][x].green, picture[y][x].blue);

    return to255;
}

void PPM::setPixel(std::size_t x, std::size_t y, Rgb value)
{
    picture[y][x] = value;
}

void PPM::startDimensionEditing(std::size_t new_width, std::size_t new_height)
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

void PPM::copyToEditing(std::size_t srcX, std::size_t srcY, std::size_t destX, std::size_t destY)
{
    editingPicture[destY][destX] = picture[srcY][srcX];
}

void PPM::endDimensionEditing()
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

void PPM::validateFormat(std::string extension)
{
    if (extension != "ppm")
    {
        throw std::invalid_argument("Incompatible file formats");
    }
}

void PPM::writePixel(std::size_t x, std::size_t y, std::ofstream &file, std::string extension, bool binary)
{
    binary ? writeBinaryPixel(x, y, file, extension) : writeTextPixel(x, y, file, extension);
}

void PPM::writeFormatInfo(std::ofstream &file, std::string extension, bool binary)
{
    binary ? writeBinaryFormat(file, extension) : writeTextFormat(file, extension);
}

void PPM::writeTextFormat(std::ofstream &file, std::string extension)
{
    file << "P3" << std::endl;
    file << width << " " << height << std::endl;
    file << maxRgbValue << std::endl;
}

void PPM::writeTextPixel(std::size_t x, std::size_t y, std::ofstream &file, std::string extension)
{
    Rgb value = getPixelRgb(x, y);
    file << value.red << " " << value.green << " " << value.blue << std::endl;
}

void PPM::writeBinaryFormat(std::ofstream &file, std::string extension)
{
    char empty = ' ';
    std::string newFormat = "P6";

    file.write((char *)&newFormat[0], 1);
    file.write((char *)&newFormat[1], 1);
    file.write((char *)&empty, 1);

    writeNumberBinary(convertToASCII(width), file);
    writeNumberBinary(convertToASCII(height), file);
    writeNumberBinary(convertToASCII(maxRgbValue), file);
}

void PPM::writeBinaryPixel(std::size_t x, std::size_t y, std::ofstream &file, std::string extension)
{
    Rgb value = getPixelRgb(x, y);

    writeRgbPixelBinary(value, file);
}

void PPM::readText(std::ifstream &file)
{
    int in_width, in_height;

    //get width, height and max RGB value and check for errors
    checkForComments(file);
    file >> in_width >> in_height;
    checkForComments(file);

    if (in_width < 0 || in_height < 0)
    {
        throw std::invalid_argument("Invalid picture dimensions!");
    }

    width = in_width;
    height = in_height;

    file >> maxRgbValue;
    checkForComments(file);

    if (maxRgbValue < 0 || maxRgbValue > 255)
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
            file >> picture[i][j].red >> picture[i][j].green >> picture[i][j].blue;

            if (picture[i][j].red < 0 || picture[i][j].green < 0 || picture[i][j].blue < 0 ||
                picture[i][j].red > maxRgbValue || picture[i][j].green > maxRgbValue || picture[i][j].blue > maxRgbValue)
            {
                //if pixel is invalid delete picture and mark as deleted
                deleteArr(picture, height);
                picture = nullptr;

                throw std::invalid_argument("Invalid pixel value!");
            }

            checkForComments(file);
        }
    }
}

void PPM::readBinary(std::ifstream &file)
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
        throw std::invalid_argument("Invalid picture dimensions!");
    }

    width = in_width;
    height = in_height;

    maxRgbValue = getNumberBinary(file);
    checkForCommentsBinary(file);

    if (maxRgbValue < 0 || maxRgbValue > 255)
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
            picture[i][j] = getRgbBinary(file);

            if (picture[i][j].red < 0 || picture[i][j].green < 0 || picture[i][j].blue < 0 ||
                picture[i][j].red > maxRgbValue || picture[i][j].green > maxRgbValue || picture[i][j].blue > maxRgbValue)
            {
                //if pixel is invalid delete picture and mark as deleted
                deleteArr(picture, height);
                picture = nullptr;

                throw std::invalid_argument("Invalid pixel value!");
            }
        }
    }
}

PPM::~PPM()
{
    deleteArr(picture, height);
}