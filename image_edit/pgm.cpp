#include "pgm.h"

PGM::PGM(std::string in_format, std::ifstream &file, std::string in_output_location)
{
    format = in_format;
    output_location = in_output_location;

    if (!file.is_open())
    {
        throw std::invalid_argument("File problem occured!");
    }

    file >> width >> height;
    file >> maxGrayscaleValue;

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

void PGM::print()
{
    std::cout << format << std::endl;
    std::cout << width << " " << height << std::endl;
    std::cout << maxGrayscaleValue << std::endl;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            std::cout << picture[i][j] << " ";
        }

        std::cout << std::endl;
    }
}

RGB PGM::getPixelRGB(std::size_t x, std::size_t y) const
{
    return grayscaleToRGB(picture[y][x], maxGrayscaleValue);
}

int PGM::getPixelGrayscale(std::size_t x, std::size_t y) const
{
    return picture[y][x];
}

void PGM::setPixel(std::size_t x, std::size_t y, RGB value)
{
    picture[y][x] = RGBToGrayscale(value, maxGrayscaleValue);
}

void PGM::createResized(std::size_t newWidth, std::size_t newHeight)
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
            srcX = std::min(srcX, width-1);
            std::size_t srcY = roundToInt((((double)i) / ((double)newHeight)) * ((double)height));
            srcY = std::min(srcY, height-1);

            newPicture[i][j] = getPixelGrayscale(srcX, srcY);
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

void PGM::writePixel(std::size_t x, std::size_t y, std::ofstream &file)
{
    file << getPixelGrayscale(y, x);
}

void PGM::writeFormatInfo(std::size_t curWidth, std::size_t curHeight, std::ofstream& file)
{
    file << format << std::endl;
    file << curWidth << " " << curHeight << std::endl;
    file << maxGrayscaleValue << std::endl;
}


PGM::~PGM()
{
    for (int i = 0; i < height; i++)
    {

        delete[] picture[i];
    }

    delete[] picture;
    std::cout << "all clear";
}
