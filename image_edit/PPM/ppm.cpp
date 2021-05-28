#include "ppm.h"

void PPM::deleteArr(Rgb **arr, std::size_t curHeight, std::size_t curWidth)
{
    if (arr != nullptr)
    {
        for (int i = 0; i < curHeight; i++)
        {
            if (arr[i] != nullptr)
            {
                /*for (int k = 0; k < curWidth; k++)
                {
                    if (arr[i][k] != nullptr)
                    {
                        delete[] arr[i][k];
                    }
                }*/

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

            /*for (int k = 0; k < curWidth; k++)
            {
                pic[i][k] = new int[3];
            }*/
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
    /*Rgb to255 = new int[3];
    for (int i = 0; i < 3; i++)
    {
        to255[i] = picture[y][x][i] * (255 / maxRgbValue);
    }*/
    Rgb to255(picture[y][x].red, picture[y][x].green, picture[y][x].blue);

    return to255;
}

void PPM::setPixel(std::size_t x, std::size_t y, Rgb value)
{
    /*for (int i = 0; i < 3; i++)
    {
        picture[y][x][i] = value[i];
    }*/
    picture[y][x] = value;
}

void PPM::startEditing(std::size_t new_width, std::size_t new_height)
{
    if(editingPicture)
    {
        throw std::logic_error("Operation already started!");
    }

    editingPicture = allocateNew(new_width, new_height);
}

void PPM::endEditing()
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

/*void PPM::createResized(std::size_t newWidth, std::size_t newHeight)
{
    //allocate
    Rgb **newPicture = nullptr;

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

            Rgb curValue = getPixelRgb(srcX, srcY);
            newPicture[i][j][0] = curValue[0];
            newPicture[i][j][1] = curValue[1];
            newPicture[i][j][2] = curValue[2];
            newPicture[i][j] = curValue;

            //delete[] curValue;
        }
    }

    deleteArr(picture, height, width);

    picture = newPicture;
    height = newHeight;
    width = newWidth;
}*/

/*void PPM::createCropped(std::size_t upper_x, std::size_t upper_y, std::size_t lower_x, std::size_t lower_y)
{
    Rgb **newPicture = nullptr;

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

    deleteArr(picture, height, width);

    picture = newPicture;
}*/

void PPM::writePixel(std::size_t x, std::size_t y, std::ofstream &file)
{
    Rgb value = getPixelRgb(y, x);
    file << value.red << " " << value.green << " " << value.blue << std::endl;

    //delete[] value;
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