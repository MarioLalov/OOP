#include "pgm.h"

void PGM::deleteArr(int **arr, std::size_t curHeight)
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

int **PGM::allocateNew(std::size_t curWidth, std::size_t curHeight)
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

PGM::PGM(std::string in_format, int in_width, int in_height, int color)
{
    if(in_width < 0 || in_height < 0)
    {
        throw std::invalid_argument("Invalid file dimensions!");
    }

    if(color > 255 || color < 0)
    {
        throw std::invalid_argument("Unsupported color!");
    }

    format = in_format;
    width = in_width;
    height = in_height;
    maxGrayscaleValue = 255;

    picture = allocateNew(width, height);
    for(std::size_t i = 0; i < height; i++)
    {
        for(std::size_t j = 0; j < width; j++)
        {
            picture[i][j] = color;
        }
    }
}

PGM::PGM(std::string in_format, std::ifstream &file)
{
    format = in_format;

    if (!file.is_open())
    {
        throw std::invalid_argument("File problem occured!");
    }

    int in_width, in_height;
    
    checkForComments(file);
    file >> in_width >> in_height;
    checkForComments(file);

    if(in_width < 0 || in_height < 0)
    {
        throw std::invalid_argument("Invalid file dimensions!");
    }

    width = in_width;
    height = in_height;

    file >> maxGrayscaleValue;
    checkForComments(file);

    if(maxGrayscaleValue < 0 || maxGrayscaleValue > 255)
    {
        throw std::invalid_argument("Unsupported color!");
    }

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
            file >> picture[i][j];
            checkForComments(file);

            if(picture[i][j] < 0 || picture[i][j] > maxGrayscaleValue)
            {
                deleteArr(picture, height);
                picture = nullptr;

                throw std::invalid_argument("Invalid pixel value!");
            }
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

Rgb PGM::getPixelRgb(std::size_t x, std::size_t y) const
{
    if(x >= width || y >= height || x < 0 || y < 0)
    {
        throw std::out_of_range("Out of range");
    }

    return grayscaleToRgb(picture[y][x], maxGrayscaleValue);
}

int PGM::getPixelGrayscale(std::size_t x, std::size_t y) const
{
    return picture[y][x];
}

void PGM::setPixel(std::size_t x, std::size_t y, Rgb value)
{
    picture[y][x] = RgbToGrayscale(value, maxGrayscaleValue);
}

void PGM::startDimensionEditing(std::size_t new_width, std::size_t new_height)
{
    if(editingPicture)
    {
        throw std::logic_error("Operation already started!");
    }

    editingPicture = allocateNew(new_width, new_height);
}

void PGM::endDimensionEditing(std::size_t new_width, std::size_t new_height)
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

void PGM::copyToEditing(std::size_t srcX, std::size_t srcY, std::size_t destX, std::size_t destY)
{
    editingPicture[destY][destX] = picture[srcY][srcX];
}

void PGM::createResized(std::size_t newWidth, std::size_t newHeight)
{

    int **newPicture = nullptr;

    //allocate
    try
    {
        newPicture = allocateNew(newWidth, newHeight);
    }
    catch (const std::bad_alloc &err)
    {
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

            newPicture[i][j] = getPixelGrayscale(srcX, srcY);
        }
    }

    deleteArr(picture, height);

    picture = newPicture;
    height = newHeight;
    width = newWidth;
}

void PGM::createCropped(std::size_t upper_x, std::size_t upper_y, std::size_t lower_x, std::size_t lower_y)
{
    int **newPicture = nullptr;

    try
    {
        newPicture = allocateNew((lower_x - upper_x + 1), (lower_y - upper_y + 1));
    }
    catch (const std::bad_alloc &err)
    {
        throw err;
    }

    for (int i = upper_y, k = 0; i <= lower_y; i++, k++)
    {
        for (int j = upper_x, l = 0; j <= lower_x; j++, l++)
        {
            newPicture[k][l] = picture[i][j];
        }
    }

    deleteArr(picture, height);

    picture = newPicture;
}

void PGM::writePixel(std::size_t x, std::size_t y, std::ofstream &file)
{
    file << getPixelGrayscale(y, x) << " ";
}

void PGM::writeFormatInfo(std::ofstream &file)
{
    file << format << std::endl;
    file << width << " " << height << std::endl;
    file << maxGrayscaleValue << std::endl;
}

PGM::~PGM()
{
    deleteArr(picture, height);
    std::cout << "all clear";
}
