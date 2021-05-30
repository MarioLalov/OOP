#include "pbm.h"
#include <iostream>

void PBM::deleteArr(int **arr, std::size_t curHeight)
{
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

PBM::PBM(std::string in_format, std::ifstream &file)
{
    format = in_format;

    if (!file.is_open())
    {
        throw std::invalid_argument("File problem occured!");
    }

    checkForComments(file);
    file >> width >> height;
    checkForComments(file);

    //allocate memory
    try
    {
        picture = allocateNew(width, height);
        std::cout << "done";
    }
    catch (const std::bad_alloc &err)
    {
        throw err;
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            file >> picture[i][j];
            checkForComments(file);
        }
    }
}

void PBM::print()
{
    std::cout << format << std::endl;
    std::cout << width << " " << height << std::endl;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            std::cout << picture[i][j] << " ";
        }

        std::cout << std::endl;
    }
}

int PBM::flipPixel(std::size_t x, std::size_t y) const
{
    return !picture[y][x];
}

Rgb PBM::getPixelRgb(std::size_t x, std::size_t y) const
{
    return grayscaleToRgb(flipPixel(x, y), 1);
}

int PBM::getPixelGrayscale(std::size_t x, std::size_t y) const
{
    if (x >= width || y >= height || x < 0 || y < 0)
    {
        throw std::out_of_range("Out of range!");
    }

    return flipPixel(x, y);
}

void PBM::setPixel(std::size_t x, std::size_t y, Rgb value)
{
    picture[y][x] = RgbToGrayscale(value, 1);
}

void PBM::startDimensionEditing(std::size_t new_width, std::size_t new_height)
{
    if(editingPicture)
    {
        throw std::logic_error("Operation already started!");
    }

    editingPicture = allocateNew(new_width, new_height);
}

void PBM::endDimensionEditing(std::size_t new_width, std::size_t new_height)
{
    if(!editingPicture)
    {
        throw std::logic_error("Operation is not started!");
    }

    deleteArr(picture, height);
    picture = editingPicture;

    editingPicture = nullptr;

    width = new_width;
    height = new_height;
}

void PBM::copyToEditing(std::size_t srcX, std::size_t srcY, std::size_t destX, std::size_t destY)
{
    editingPicture[destY][destX] = picture[srcY][srcX];
}

void PBM::writePixel(std::size_t x, std::size_t y, std::ofstream &file)
{
    file << !getPixelGrayscale(y, x) << " ";
}

void PBM::writeFormatInfo(std::ofstream &file)
{
    file << format << std::endl;
    file << width << " " << height << std::endl;
}

PBM::~PBM()
{
    deleteArr(picture, height);

    std::cout << "all clear";
}