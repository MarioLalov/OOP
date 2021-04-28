#include "pbm.h"

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
    return flipPixel(x, y);
}

void PBM::setPixel(std::size_t x, std::size_t y, RGB value)
{
    picture[y][x] = RGBToGrayscale(value, 1);
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
