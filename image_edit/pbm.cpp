#include "pbm.h"
#include <iostream>

PBM::PBM(std::string in_format, std::ifstream &file, std::string in_output_location)
{
    format = in_format;
    output_location = in_output_location;

    if (!file.is_open())
    {
        throw std::invalid_argument("File problem occured!");
    }

    file >> width >> height;

    //allocate memory
    picture = nullptr;

    try
    {
        picture = new int *[height];

        for (int i = 0; i < height; i++)
        {
            picture[i] = nullptr;
        }

        for (int i = 0; i < height; i++)
        {
            picture[i] = new int[width];
        }
    }
    catch (const std::bad_alloc &err)
    {
        if (picture != nullptr)
        {
            for (int i = 0; i < height; i++)
            {
                if (picture[i])
                {
                    delete[] picture[i];
                }
            }

            delete[] picture;
        }

        throw err;
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            file >> picture[i][j];
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

RGB PBM::getPixelRGB(std::size_t x, std::size_t y) const
{
    return grayscaleToRGB(flipPixel(x, y), 1);
}

int PBM::getPixelGrayscale(std::size_t x, std::size_t y) const
{
    if (x >= width || y >= height || x < 0 || y < 0)
    {
        throw std::out_of_range("Out of range!");
    }

    return flipPixel(x, y);
}

void PBM::setPixel(std::size_t x, std::size_t y, RGB value)
{
    picture[y][x] = RGBToGrayscale(value, 1);
}

void PBM::createResized(std::size_t newWidth, std::size_t newHeight)
{

    int **newPicture = nullptr;

    //allocate
    try
    {
        newPicture = new int *[newHeight];

        for (int i = 0; i < newHeight; i++)
        {
            newPicture[i] = nullptr;
        }

        for (int i = 0; i < newHeight; i++)
        {
            newPicture[i] = new int[newWidth];
        }
    }
    catch (const std::bad_alloc &err)
    {
        if (newPicture)
        {
            for (int i = 0; i < newHeight; i++)
            {
                if (newPicture[i])
                {
                    delete[] newPicture[i];
                }
            }

            delete[] newPicture;
        }

        throw err;
    }

    //resize
    for (int i = 0; i < newHeight; i++)
    {
        for (int j = 0; j < newWidth; j++)
        {
            std::size_t srcX = roundToInt((((double)j) / ((double)newWidth)) * ((double)width));
            srcX = std::min(srcX, width - 1);
            std::size_t srcY = roundToInt((((double)i) / ((double)newHeight)) * ((double)height));
            srcX = std::min(srcX, height - 1);

            newPicture[i][j] = !getPixelGrayscale(srcX, srcY);
        }
    }

    for (int i = 0; i < height; i++)
    {
        delete[] picture[i];
    }

    delete[] picture;

    picture = newPicture;
    height = newHeight;
    width = newWidth;
}

void PBM::writePixel(std::size_t x, std::size_t y, std::ofstream &file)
{
    file << !getPixelGrayscale(y, x);
}

void PBM::writeFormatInfo(std::size_t curWidth, std::size_t curHeight, std::ofstream& file)
{
    file << format << std::endl;
    file << curWidth << " " << curHeight << std::endl;
}

PBM::~PBM()
{
    for (int i = 0; i < height; i++)
    {

        delete[] picture[i];
    }

    delete[] picture;
    std::cout << "all clear";
}