#include "image.h"
#include <limits>
#include <algorithm>

Rgb::Rgb()
{
    red = 0;
    green = 0;
    blue = 0;
}

Rgb::Rgb(int in_red, int in_green, int in_blue)
{
    red = in_red;
    green = in_green;
    blue = in_blue;
}

Rgb &Rgb::operator=(const Rgb &other)
{
    red = other.red;
    green = other.green;
    blue = other.blue;

    return *this;
}

Rgb Rgb::operator+(Rgb const &other)
{
    //add all color components
    //if the result exceeds 255 or is less than 0 assign min or max value
    Rgb sum;

    if (red + other.red > 255)
        sum.red = 255;
    else if (red + other.red < 0)
        sum.red = 0;
    else
        sum.red = red + other.red;

    if (green + other.green > 255)
        sum.green = 255;
    else if (green + other.green < 0)
        sum.green = 0;
    else
        sum.green = green + other.green;

    if (blue + other.blue > 255)
        sum.blue = 255;
    else if (blue + other.blue < 0)
        sum.blue = 0;
    else
        sum.blue = blue + other.blue;

    return sum;
}

Rgb Rgb::operator*(int const &num)
{
    //multiply all color values by num
    //if the result exceeds 255 or is less than 0 assign min or max value
    Rgb result;

    if (red * num > 255)
        result.red = 255;
    else
        result.red = red * num;

    if (green * num > 255)
        result.green = 255;
    else
        result.green = green * num;

    if (blue * num > 255)
        result.blue = 255;
    else
        result.blue = blue * num;

    return result;
}

Rgb Rgb::operator/(int const &num)
{
    //divide all color values by num
    Rgb result;
    result.red = red / num;
    result.green = green / num;
    result.blue = blue / num;

    return result;
}

//pack grayscale in Rgb value
Rgb grayscaleToRgb(int value, int max_value)
{
    Rgb Rgb_value;
    int to255 = value * (255 / max_value);
    Rgb_value.red = to255;
    Rgb_value.green = to255;
    Rgb_value.blue = to255;

    return Rgb_value;
}

//transfer grayscale value in Rgb to grayscale in int
int RgbToGrayscale(Rgb Rgb_value, int max_value)
{
    int grayscale = (Rgb_value.green * max_value) / 255;

    return grayscale;
}

bool getSaveFormat()
{
    std::string input;
    int n;

    std::cout << "How do you want to save the image? " << std::endl
              << "(1)ASCII" << std::endl
              << "(2)Raw" << std::endl;

    while (n != 1 && n != 2)
    {
        std::cout << "Enter index: ";
        std::getline(std::cin, input);

        try
        {
            n = std::stoi(input);
        }
        catch (const std::invalid_argument &err)
        {
            std::cout << "Please enter a valid index" << std::endl;
        }
    }

    return n - 1;
}

void Image::write(std::string output_location, std::string extension)
{
    //check if formats are compatible
    validateFormat(extension);

    //get desired save format
    bool binary = getSaveFormat();

    //check if save is possible
    if(binary && extension == "pbm")
    {
        std::cout << "Raw format not supported for PBM. Saving in ASCII instead." << std::endl;
        binary = false;
    }

    //write format info in file
    std::ofstream ofile(output_location, std::ofstream::trunc);
    writeFormatInfo(ofile, extension, binary);

    //write all pixels
    for (std::size_t i = 0; i < height; i++)
    {
        for (std::size_t j = 0; j < width; j++)
        {
            writePixel(j, i, ofile, extension, binary);
        }

        if (format != "P3" && format != "P4" && format != "P5" && format != "P6" && extension != "ppm")
        {
            ofile << std::endl;
        }
    }

    ofile.close();
}

std::size_t Image::getWidth() const
{
    return width;
}

std::size_t Image::getHeight() const
{
    return height;
}

void checkForComments(std::ifstream &file)
{
    //get current position
    std::streampos currentPos = file.tellg();
    std::string comment;

    while (true)
    {
        //get next string in file strem
        file >> comment;
        if (comment[0] == '#')
        {
            //string is comment so ignore till the end of the line
            file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            currentPos = file.tellg();
        }
        else
        {
            //string is not a comment so return to begining of string
            file.seekg(currentPos, std::ios::beg);
            break;
        }
    }
}

int roundToInt(double num)
{
    return (int)(num + 0.5);
}

std::string convertToASCII(int num)
{
    std::string number;

    //get all digits and transfer them to ASCII
    while (num > 0)
    {
        char n = num % 10 + '0';

        number += n;
        num = num / 10;
    }

    //reverse string
    std::reverse(number.begin(), number.end());

    return number;
}

void writeNumberBinary(std::string number, std::ofstream &file)
{
    char empty = ' ';

    //write each digit in ASCII separately
    for (std::size_t i = 0; i < number.length(); i++)
    {
        file.write((char *)&number[i], 1);
    }

    //write empty space
    file.write((char *)&empty, 1);
}

void writeRgbPixelBinary(Rgb color, std::ofstream &file)
{
    //write each value in ASCII
    file.write((char *)&color.red, 1);
    file.write((char *)&color.green, 1);
    file.write((char *)&color.blue, 1);
}

void writeGrayscalePixelBinary(int color, std::ofstream &file)
{
    file.write((char *)&color, 1);
}

void checkForCommentsBinary(std::ifstream &file)
{
    //check if next chracter is #
    int character = file.peek();

    //ignore everything till the end of the comment
    if (character == '#')
    {
        while (file.peek() != '\n')
        {
            character = file.get();
        }
    }
}

int getNumberBinary(std::ifstream &file)
{
    //get each digitn in ASCII then transform to int
    std::string number;
    char current;

    file.get();

    while (file.peek() != ' ' && file.peek() != '\n')
    {
        current = file.get();
        number += current;
    }

    return std::stoi(number);
}

int getGrayscaleBinary(std::ifstream &file)
{
    //read value then transfer to int
    int value;
    unsigned char buffer;

    file.read((char *)&buffer, 1);
    if (file)
    {
        value = buffer;
    }

    return value;
}

Rgb getRgbBinary(std::ifstream &file)
{
    //read each component then transfer to Rgb value
    Rgb value;
    unsigned char buffer;

    file.read((char *)&buffer, 1);
    if (file)
    {
        value.red = buffer;
    }

    file.read((char *)&buffer, 1);
    if (file)
    {
        value.green = buffer;
    }

    file.read((char *)&buffer, 1);
    if (file)
    {
        value.blue = buffer;
    }

    return value;
}