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

void PPM::createResized(std::size_t newWidth, std::size_t newHeight)
{

    RGB **newPicture = nullptr;

    //allocate
    try
    {
        newPicture = new RGB *[newHeight];

        for (int i = 0; i < newHeight; i++)
        {
            newPicture[i] = nullptr;
        }

        for (int i = 0; i < newHeight; i++)
        {
            newPicture[i] = new RGB[newWidth];

            for (int k = 0; k < newWidth; k++)
            {
                newPicture[i][k] = new int[3];
            }
        }
    }
    catch (const std::bad_alloc &err)
    {
        if (newPicture != nullptr)
        {
            for (int i = 0; i < newHeight; i++)
            {
                if (newPicture[i])
                {
                    for (int k = 0; k < newWidth; k++)
                    {
                        if (newPicture[i][k] != nullptr)
                        {
                            delete[] newPicture[i][k];
                        }
                    }

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
            srcY = std::min(srcY, height - 1);

            RGB curValue = getPixelRGB(srcX, srcY);
            newPicture[i][j][0] = curValue[0];
            newPicture[i][j][1] = curValue[1];
            newPicture[i][j][2] = curValue[2];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            delete[] picture[i][j];
        }

        delete[] picture[i];
    }

    delete[] picture;

    picture = newPicture;
    height = newHeight;
    width = newWidth;
}

void PPM::writePixel(std::size_t x, std::size_t y, std::ofstream &file)
{
    RGB value = getPixelRGB(y, x);
    file << value[0] << " " << value[1] << " " << value[2] << std::endl;

    delete[] value;
}

void PPM::writeFormatInfo(std::size_t curWidth, std::size_t curHeight, std::ofstream &file)
{
    file << format << std::endl;
    file << curWidth << " " << curHeight << std::endl;
    file << maxRGBValue << std::endl;
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