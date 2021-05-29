#include "ppm.h"

void PPM::deleteArr(Rgb **arr, std::size_t curHeight, std::size_t curWidth)
{
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
        deleteArr(pic, curHeight, curWidth);

        throw err;
    }
}

PPM::PPM(std::string in_format, std::ifstream &file, std::string in_output_location)
{
    format = in_format;
    output_location = in_output_location;

    if (!file.is_open())
    {
        throw std::invalid_argument("File problem occured!");
    }

    checkForComments(file);
    file >> width >> height;
    checkForComments(file);
    file >> maxRgbValue;
    checkForComments(file);

    //allocate memory
    try
    {
        picture = allocateNew(width, height);
    }
    catch (const std::bad_alloc &err)
    {
        throw err;
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            file >> picture[i][j].red >> picture[i][j].green >> picture[i][j].blue;
            checkForComments(file);
        }
    }
}

void PPM::print()
{
    std::cout << format << std::endl;
    std::cout << width << " " << height << std::endl;
    std::cout << maxRgbValue << std::endl;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            std::cout << picture[i][j].red << " " << picture[i][j].green << " " << picture[i][j].blue << " ";

            std::cout << " | ";
        }
        std::cout << std::endl;
    }
}

int PPM::getPixelGrayscale(std::size_t x, std::size_t y) const
{
    //return (picture[y][x][0] + picture[y][x][1] + picture[y][x][2]) / 3;
    return (picture[y][x].red + picture[y][x].green + picture[y][x].blue) / 3;
}

Rgb PPM::getPixelRgb(std::size_t x, std::size_t y) const
{
    if(x >= width || y >= height || x < 0 || y < 0)
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
    if(editingPicture)
    {
        throw std::logic_error("Operation already started!");
    }

    editingPicture = allocateNew(new_width, new_height);
}

void PPM::endDimensionEditing()
{
    if(!editingPicture)
    {
        throw std::logic_error("Operation is not started!");
    }

    deleteArr(picture, height, width);
    picture = editingPicture;

    editingPicture = nullptr;
}

void PPM::copyToEditing(std::size_t srcX, std::size_t srcY, std::size_t destX, std::size_t destY)
{
    editingPicture[destY][destX] = picture[srcY][srcX];
}

void PPM::writePixel(std::size_t x, std::size_t y, std::ofstream &file)
{
    Rgb value = getPixelRgb(y, x);
    file << value.red << " " << value.green << " " << value.blue << std::endl;
}

void PPM::writeFormatInfo(std::ofstream &file)
{
    file << format << std::endl;
    file << width << " " << height << std::endl;
    file << maxRgbValue << std::endl;
}

PPM::~PPM()
{
    deleteArr(picture, height, width);

    std::cout << "all clear";
}