#include "image.h"

RGB::RGB()
{
    red = 0;
    green = 0;
    blue = 0;
}

RGB::RGB(unsigned int in_red, unsigned int in_green, unsigned int in_blue)
{
    red = in_red;
    green = in_green;
    blue = in_blue;
}

RGB Image::grayscaleToRGB(int value, int max_value)
{
    RGB rgb_value;
    int to255 = value * (255 / max_value);

    /*for (int i = 0; i < 3; i++)
    {
        rgb_value[i] = to255;
    }*/
    rgb_value.red = to255;
    rgb_value.green = to255;
    rgb_value.blue = to255;

    return rgb_value;
}

int Image::RGBToGrayscale(RGB rgb_value, int max_value)
{
    //double grayscale = ((rgb_value[0] + rgb_value[1] + rgb_value[2]) / 3) * (max_value / 255);
    double grayscale = ((rgb_value.red + rgb_value.green + rgb_value.blue) / 3) * (max_value / 255);
    std::cout << grayscale;

    return (int)grayscale;
}

void Image::write()
{
    std::ofstream ofile(output_location, std::ofstream::trunc);

    writeFormatInfo(ofile);

    for (std::size_t i = 0; i < height; i++)
    {
        for (std::size_t j = 0; j < width; j++)
        {
            writePixel(i, j, ofile);
        }

        if (format != "P3")
        {
            ofile << std::endl;
        }
    }

    ofile.close();
    std::cout << "out";
}

void Image::crop(std::size_t upper_x, std::size_t upper_y, std::size_t lower_x, std::size_t lower_y)
{
    if (upper_x > lower_x || upper_y > lower_y)
    {
        throw std::invalid_argument("Reversed coordinates!");
    }

    if (upper_x >= width)
    {
        upper_x = width - 1;
    }

    if (upper_x < 0)
    {
        upper_x = 0;
    }

    if (lower_x >= width)
    {
        lower_x = width - 1;
    }

    if (lower_x < 0)
    {
        lower_x = 0;
    }

    if (upper_y >= height)
    {
        upper_y = height - 1;
    }

    if (upper_y < 0)
    {
        upper_y = 0;
    }

    if (lower_y >= height)
    {
        lower_y = height - 1;
    }

    if (lower_y < 0)
    {
        lower_y = 0;
    }

    createCropped(upper_x, upper_y, lower_x, lower_y);

    width = (lower_x - upper_x + 1);
    height = (lower_y - upper_y + 1);
}

void Image::resize(int widthInput, int heightInput, bool percentage)
{
    std::size_t newWidth;
    std::size_t newHeight;

    newWidth = (percentage) ? roundToInt(((double)widthInput / 100) * ((double)width)) : widthInput;
    newHeight = (percentage) ? roundToInt(((double)heightInput / 100) * ((double)height)) : heightInput;

    createResized(newWidth, newHeight);
}

int roundToInt(double num)
{
    return (int)(num + 0.5);
}
// grayscale 17 = (17,17,17)
// (0.3 * R) + (0.59 * G) + (0.11 * B) = 17
// 17 + 17 + 17  <=>  0.3*