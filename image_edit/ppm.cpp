#include "ppm.h"

PPM::PPM(std::string in_format, std::ifstream &file, std::string in_output_location)
{
    format = in_format;
    output_location = in_output_location;

    if (!file.is_open())
    {
        throw std::invalid_argument("File problem occured!");
    }

    file >> width >> height;
    file >> maxRGBValue;

    //allocate memory
    picture = nullptr;

    try
    {
        picture = new RGB *[height];

        for (int i = 0; i < height; i++)
        {
            picture[i] = nullptr;
        }

        for (int i = 0; i < height; i++)
        {
            picture[i] = new RGB[width];

            for (int k = 0; k < width; k++)
            {
                picture[i][k] = new int[3];
            }
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
                    for (int k = 0; k < width; k++)
                    {
                        if (picture[i][k] != nullptr)
                        {
                            delete[] picture[i][k];
                        }
                    }

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
            for (int k = 0; k < 3; k++)
            {
                file >> picture[i][j][k];
            }
        }
    }
}

void PPM::print()
{
    std::cout << format << std::endl;
    std::cout << width << " " << height << std::endl;
    std::cout << maxRGBValue << std::endl;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                std::cout << picture[i][j][k] << " ";
            }

            std::cout << " | ";
        }
        std::cout << std::endl;
    }
}

int PPM::getPixelGrayscale(std::size_t x, std::size_t y) const
{
    return (picture[y][x][0] + picture[y][x][1] + picture[y][x][2]) / 3;
}

RGB PPM::getPixelRGB(std::size_t x, std::size_t y) const
{
    RGB to255 = new int[3];
    for (int i = 0; i < 3; i++)
    {
        to255[i] = picture[y][x][i] * (255 / maxRGBValue);
    }

    return to255;
}

void PPM::setPixel(std::size_t x, std::size_t y, RGB value)
{
    for (int i = 0; i < 3; i++)
    {
        picture[y][x][i] = value[i];
    }
}

PPM::~PPM()
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            delete[] picture[i][j];
        }

        delete[] picture[i];
    }

    delete[] picture;
    std::cout << "all clear";
}