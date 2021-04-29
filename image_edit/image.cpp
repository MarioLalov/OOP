#include "image.h"

RGB Image::grayscaleToRGB(int value, int max_value)
{
    RGB rgb_value = nullptr;

    try
    {
        rgb_value = new int[3];
    }
    catch (const std::bad_alloc &err)
    {
        throw err;
    }

    int to255 = value * (255 / max_value);

    for (int i = 0; i < 3; i++)
    {
        rgb_value[i] = to255;
    }

    return rgb_value;
}

int Image::RGBToGrayscale(RGB rgb_value, int max_value)
{
    double grayscale = ((rgb_value[0] + rgb_value[1] + rgb_value[2]) / 3) * (max_value / 255);
    std::cout << grayscale;

    return (int)grayscale;
}

//TODO rework write function as virtual
/*void Image::write(std::size_t x, std::size_t y, std::ofstream &file)
{
    if (format == "P1")
    {
        file << !getPixelGrayscale(y, x);
    }
    else if (format == "P2")
    {
        file << getPixelGrayscale(y, x);
    }
    else if (format == "P3")
    {
        RGB value = getPixelRGB(y, x);
        file << value[0] << " " << value[1] << " " << value[2] << std::endl;

        delete[] value;
    }
}*/

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

    std::ofstream ofile(output_location);

    /*ofile << format << std::endl;
    ofile << (lower_x - upper_x + 1) << " " << (lower_y - upper_y + 1) << std::endl;*/

    writeFormatInfo((lower_x - upper_x + 1), (lower_y - upper_y + 1), ofile);

    for (int i = upper_y; i <= lower_y; i++)
    {
        for (int j = upper_x; j <= lower_x; j++)
        {
            //write(i, j, ofile);
            writePixel(i, j, ofile);
        }

        if (format != "P3")
        {
            ofile << std::endl;
        }
    }

    ofile.close();
}

//!!!TODO maybe create virtual for each type to skip RGB Grayscale conversions
void Image::resize(int widthInput, int heightInput, bool percentage)
{
    std::size_t newWidth;
    std::size_t newHeight;

    newWidth = (percentage) ? (widthInput / 100) * width : widthInput;
    newHeight = (percentage) ? (heightInput / 100) * height : heightInput;

    createResized(newWidth, newHeight);
}

int roundToInt(double num)
{
    return (int)(num + 0.5);
}
// grayscale 17 = (17,17,17)
// (0.3 * R) + (0.59 * G) + (0.11 * B) = 17
// 17 + 17 + 17  <=>  0.3*