#include "image.h"
#include <limits>

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
    Rgb result;
    result.red = red / num;
    result.green = green / num;
    result.blue = blue / num;

    return result;
}

Rgb grayscaleToRgb(int value, int max_value)
{
    Rgb Rgb_value;
    int to255 = value * (255 / max_value);
    //int to255 = value*(max_value/255);

    /*for (int i = 0; i < 3; i++)
    {
        Rgb_value[i] = to255;
    }*/
    Rgb_value.red = to255;
    Rgb_value.green = to255;
    Rgb_value.blue = to255;

    return Rgb_value;
}

int RgbToGrayscale(Rgb Rgb_value, int max_value)
{
    //double grayscale = ((Rgb_value[0] + Rgb_value[1] + Rgb_value[2]) / 3) * (max_value / 255);
    //double grayscale = ((Rgb_value.red + Rgb_value.green + Rgb_value.blue) / 3) * (max_value / 255);
    int grayscale = (Rgb_value.green*max_value)/255;

    //std::cout << Rgb_value.red << ": ";
    //std::cout << grayscale << " ";

    return grayscale;
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

std::size_t Image::getWidth() const
{
    return width;
}

std::size_t Image::getHeight() const
{
    return height;
}

//template <class T>
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

    std::size_t newWidth = (lower_x - upper_x + 1);
    std::size_t newHeight = (lower_y - upper_y + 1);
    startDimensionEditing(newWidth, newHeight);

    for (int i = upper_y, k = 0; i <= lower_y; i++, k++)
    {
        for (int j = upper_x, l = 0; j <= lower_x; j++, l++)
        {
            copyToEditing(j, i, l, k);
        }
    }

    endDimensionEditing();

    width = newWidth;
    height = newHeight;
}

void Image::resize(int widthInput, int heightInput, bool percentage)
{
    std::size_t newWidth;
    std::size_t newHeight;

    newWidth = (percentage) ? roundToInt(((double)widthInput / 100) * ((double)width)) : widthInput;
    newHeight = (percentage) ? roundToInt(((double)heightInput / 100) * ((double)height)) : heightInput;

    //createResized(newWidth, newHeight);
    startDimensionEditing(newWidth, newHeight);

    for (int i = 0; i < newHeight; i++)
    {
        for (int j = 0; j < newWidth; j++)
        {
            std::size_t srcX = roundToInt((((double)j) / ((double)newWidth)) * ((double)width));
            srcX = std::min(srcX, width - 1);
            std::size_t srcY = roundToInt((((double)i) / ((double)newHeight)) * ((double)height));
            srcY = std::min(srcY, height - 1);

            //Rgb curValue = getPixelRgb(srcX, srcY);
            /*newPicture[i][j][0] = curValue[0];
            newPicture[i][j][1] = curValue[1];
            newPicture[i][j][2] = curValue[2];*/
            //newPicture[i][j] = curValue;
            copyToEditing(srcX, srcY, j, i);

            //delete[] curValue;
        }
    }

    endDimensionEditing();

    width = newWidth;
    height = newHeight;
}

void checkForComments(std::ifstream &file)
{
    std::streampos currentPos = file.tellg();

    std::string comment;

    while (true)
    {
        file >> comment;
        if (comment[0] == '#')
        {
            file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            currentPos = file.tellg();
        }
        else
        {
            file.seekg(currentPos, std::ios::beg);
            break;
        }
    }
}

int roundToInt(double num)
{
    return (int)(num + 0.5);
}

void assignNewValues(int value, int &destValue, int &errorValue)
{
    if ((255 - value) > value)
    {
        errorValue = value;
        destValue = 0;
    }
    else
    {
        errorValue = value - 255;
        destValue = 255;
    }
}

void Image::errorDiffusion()
{
    Rgb curValue;
    /*int errorRed = 0;
    int errorGreen = 0;
    int errorBlue = 0;*/
    Rgb pixel;

    for (std::size_t y = 0; y < height; y++)
    {
        Rgb error(0, 0, 0);

        for (std::size_t x = 0; x < width; x++)
        {
            curValue = getPixelRgb(x, y);
            //std::cout << curValue.red << " " << curValue.green << " " << curValue.blue << std::endl;
            //std::cout << errorRed << " " << errorGreen << " " << errorBlue << std::endl;

            //red
            /*if (curValue.red + errorRed < 0)
            {
                pixel.red = 0;
            }
            else if (curValue.red + errorRed > 255)
            {
                pixel.red = 255;
            }
            else
            {
                pixel.red = curValue.red + errorRed;
            }

            if ((255 - pixel.red) > pixel.red)
            {
                error.red = pixel.red;
                curValue.red = 0;
            }
            else
            {
                error.red = pixel.red - 255;
                curValue.red = 255;
            }*/

            pixel = curValue + error;
            assignNewValues(pixel.red, curValue.red, error.red);
            assignNewValues(pixel.green, curValue.green, error.green);
            assignNewValues(pixel.blue, curValue.blue, error.blue);

            //green
            /*if (curValue.green + errorGreen < 0)
            {
                pixel.green = 0;
            }
            else if (curValue.green + errorGreen > 255)
            {
                pixel.green = 255;
            }
            else
            {
                pixel.green = curValue.green + errorGreen;
            }

            if ((255 - pixel.green) > pixel.green)
            {
                errorGreen = pixel.green;
                curValue.green = 0;
            }
            else
            {
                errorGreen = pixel.green - 255;
                curValue.green = 255;
            }

            //blue
            if (curValue.blue + errorBlue < 0)
            {
                pixel.blue = 0;
            }
            else if (curValue.blue + errorBlue > 255)
            {
                pixel.blue = 255;
            }
            else
            {
                pixel.blue = curValue.blue + errorBlue;
            }

            if ((255 - pixel.blue) > pixel.blue)
            {
                errorBlue = pixel.blue;
                curValue.blue = 0;
            }
            else
            {
                errorBlue = pixel.blue - 255;
                curValue.blue = 255;
            }*/

            //std::cout << "new" << curValue.red << " " << curValue.green << " " << curValue.blue << std::endl;
            setPixel(x, y, curValue);
        }
    }
}

void Image::twoDimErrorDiffusion()
{
    Rgb curValue;
    int errorRed = 0;
    int errorGreen = 0;
    int errorBlue = 0;
    Rgb pixel;

    //create previous error row
    Rgb *rowError = new Rgb[width];
    /*int **rowError = new int *[width];
    for (int i = 0; i < width; i++)
    {
        rowError[i] = new int[3];
    }*/

    for (std::size_t y = 0; y < height; y++)
    {
        Rgb error(0, 0, 0);

        for (std::size_t x = 0; x < width; x++)
        {
            curValue = getPixelRgb(x, y);
            //std::cout << curValue.red << " " << curValue.green << " " << curValue.blue << std::endl;
            //std::cout << errorRed << " " << errorGreen << " " << errorBlue << std::endl;
            pixel = curValue + error + rowError[x] / 2;

            assignNewValues(pixel.red, curValue.red, error.red);
            assignNewValues(pixel.red, curValue.red, rowError[x].red);
            assignNewValues(pixel.green, curValue.green, error.green);
            assignNewValues(pixel.green, curValue.green, rowError[x].green);
            assignNewValues(pixel.blue, curValue.blue, error.blue);
            assignNewValues(pixel.blue, curValue.blue, rowError[x].red);
            //red
            /*if ((255 - pixel.red) > pixel.red)
            {
                error.red = pixel.red;
                rowError[x].red = pixel.red;
                curValue.red = 0;
            }
            else
            {
                error.red = pixel.red - 255;
                rowError[x].red = pixel.red - 255;
                curValue.red = 255;
            }

            //green
            if ((255 - pixel.green) > pixel.green)
            {
                error.green = pixel.green;
                rowError[x].green = pixel.green;
                curValue.green = 0;
            }
            else
            {
                error.green = pixel.green - 255;
                rowError[x].green = pixel.green - 255;
                curValue.green = 255;
            }

            //blue
            if ((255 - pixel.blue) > pixel.blue)
            {
                error.blue = pixel.blue;
                rowError[x].blue = pixel.blue;
                curValue.blue = 0;
            }
            else
            {
                error.blue = pixel.blue - 255;
                rowError[x].blue = pixel.blue - 255;
                curValue.blue = 255;
            }*/

            /*if (curValue.red + (errorRed + rowError[x][0]) / 2 < 0)
            {
                pixel.red = 0;
            }
            else if (curValue.red + (errorRed + rowError[x][0]) / 2 > 255)
            {
                pixel.red = 255;
            }
            else
            {
                pixel.red = curValue.red + (errorRed + rowError[x][0]) / 2;
            }

            if ((255 - pixel.red) > pixel.red)
            {
                errorRed = pixel.red;
                rowError[x][0] = pixel.red;
                curValue.red = 0;
            }
            else
            {
                errorRed = pixel.red - 255;
                rowError[x][0] = pixel.red - 255;
                curValue.red = 255;
            }

            //green
            if (curValue.green + (errorGreen + rowError[x][1]) / 2 < 0)
            {
                pixel.green = 0;
            }
            else if (curValue.green + (errorGreen + rowError[x][1]) / 2 > 255)
            {
                pixel.green = 255;
            }
            else
            {
                pixel.green = curValue.green + (errorGreen + rowError[x][1]) / 2;
            }

            if ((255 - pixel.green) > pixel.green)
            {
                errorGreen = pixel.green;
                rowError[x][1] = pixel.green;
                curValue.green = 0;
            }
            else
            {
                errorGreen = pixel.green - 255;
                rowError[x][1] = pixel.green - 255;
                curValue.green = 255;
            }

            //blue
            if (curValue.blue + (errorBlue + rowError[x][2]) / 2 < 0)
            {
                pixel.blue = 0;
            }
            else if (curValue.blue + (errorBlue + rowError[x][2]) / 2 > 255)
            {
                pixel.blue = 255;
            }
            else
            {
                pixel.blue = curValue.blue + (errorBlue + rowError[x][2]) / 2;
            }

            if ((255 - pixel.blue) > pixel.blue)
            {
                errorBlue = pixel.blue;
                rowError[x][2] = pixel.blue;
                curValue.blue = 0;
            }
            else
            {
                errorBlue = pixel.blue - 255;
                rowError[x][2] = pixel.blue - 255;
                curValue.blue = 255;
            }*/

            //std::cout << "new" << curValue.red << " " << curValue.green << " " << curValue.blue << std::endl;
            setPixel(x, y, curValue);
        }
    }

    /*for (int i = 0; i < width; i++)
    {
        delete[] rowError[i];
    }*/

    delete[] rowError;
}

void Image::floydDithering()
{
    Rgb curValue;
    //int errorRed, errorGreen, errorBlue;

    for (std::size_t y = 0; y < height; y++)
    {
        Rgb error(0, 0, 0);

        for (std::size_t x = 0; x < width; x++)
        {
            curValue = getPixelRgb(x, y);
            //std::cout << curValue.red << " " << curValue.green << " " << curValue.blue << std::endl;
            //std::cout << errorRed << " " << errorGreen << " " << errorBlue << std::endl;

            assignNewValues(curValue.red, curValue.red, error.red);
            assignNewValues(curValue.green, curValue.green, error.green);
            assignNewValues(curValue.blue, curValue.blue, error.blue);

            /*//red
            if ((255 - curValue.red) > curValue.red)
            {
                errorRed = curValue.red;
                curValue.red = 0;
            }
            else
            {
                errorRed = curValue.red - 255;
                curValue.red = 255;
            }

            //green
            if ((255 - curValue.green) > curValue.green)
            {
                errorGreen = curValue.green;
                curValue.green = 0;
            }
            else
            {
                errorGreen = curValue.green - 255;
                curValue.green = 255;
            }

            //blue
            if ((255 - curValue.blue) > curValue.blue)
            {
                errorBlue = curValue.blue;
                curValue.blue = 0;
            }
            else
            {
                errorBlue = curValue.blue - 255;
                curValue.blue = 255;
            }*/

            setPixel(x, y, curValue);

            Rgb neighbour;

            //spread error
            if (y + 1 < height)
            {

                neighbour = getPixelRgb(x, y + 1);
                /*curValue.red = neighbour.red + 5 * errorRed / 16;
                if (curValue.red < 0)
                    curValue.red = 0;
                if (curValue.red > 255)
                    curValue.red = 255;

                curValue.green = neighbour.green + 5 * errorGreen / 16;
                if (curValue.green < 0)
                    curValue.green = 0;
                if (curValue.green > 255)
                    curValue.green = 255;

                curValue.blue = neighbour.blue + 5 * errorBlue / 16;
                if (curValue.blue < 0)
                    curValue.blue = 0;
                if (curValue.blue > 255)
                    curValue.blue = 255;*/
                curValue = neighbour + ((error / 16) * 5);

                setPixel(x, y + 1, curValue);

                if (x + 1 < width)
                {
                    neighbour = getPixelRgb(x + 1, y + 1);
                    /*curValue.red = neighbour.red + errorRed / 16;
                    if (curValue.red < 0)
                        curValue.red = 0;
                    if (curValue.red > 255)
                        curValue.red = 255;

                    curValue.green = neighbour.green + errorGreen / 16;
                    if (curValue.green < 0)
                        curValue.green = 0;
                    if (curValue.green > 255)
                        curValue.green = 255;

                    curValue.blue = neighbour.blue + errorBlue / 16;
                    if (curValue.blue < 0)
                        curValue.blue = 0;
                    if (curValue.blue > 255)
                        curValue.blue = 255;*/

                    curValue = neighbour + (error / 16);

                    setPixel(x + 1, y + 1, curValue);
                }

                if (x - 1 >= 0)
                {
                    neighbour = getPixelRgb(x - 1, y + 1);
                    /*curValue.red = neighbour.red + 3 * errorRed / 16;
                    if (curValue.red < 0)
                        curValue.red = 0;
                    if (curValue.red > 255)
                        curValue.red = 255;

                    curValue.green = neighbour.green + 3 * errorGreen / 16;
                    if (curValue.green < 0)
                        curValue.green = 0;
                    if (curValue.green > 255)
                        curValue.green = 255;

                    curValue.blue = neighbour.blue + 3 * errorBlue / 16;
                    if (curValue.blue < 0)
                        curValue.blue = 0;
                    if (curValue.blue > 255)
                        curValue.blue = 255;*/

                    curValue = neighbour + ((error / 16) * 3);

                    setPixel(x - 1, y + 1, curValue);
                }
            }

            if (x + 1 < width)
            {
                neighbour = getPixelRgb(x + 1, y);
                /*curValue.red = neighbour.red + 7 * errorRed / 16;
                if (curValue.red < 0)
                    curValue.red = 0;
                if (curValue.red > 255)
                    curValue.red = 255;

                curValue.green = neighbour.green + 7 * errorGreen / 16;
                if (curValue.green < 0)
                    curValue.green = 0;
                if (curValue.green > 255)
                    curValue.green = 255;

                curValue.blue = neighbour.blue + 7 * errorBlue / 16;
                if (curValue.blue < 0)
                    curValue.blue = 0;
                if (curValue.blue > 255)
                    curValue.blue = 255;*/

                curValue = neighbour + ((error / 16) * 7);

                setPixel(x + 1, y, curValue);
            }
        }
    }
}

void Image::jarvisDithering()
{
    Rgb curValue;

    for (std::size_t y = 0; y < height; y++)
    {
        Rgb error(0, 0, 0);

        for (std::size_t x = 0; x < width; x++)
        {
            curValue = getPixelRgb(x, y);
            assignNewValues(curValue.red, curValue.red, error.red);
            assignNewValues(curValue.green, curValue.green, error.green);
            assignNewValues(curValue.blue, curValue.blue, error.blue);

            setPixel(x, y, curValue);

            Rgb neighbour;

            try
            {
                neighbour = getPixelRgb(x + 1, y);
                curValue = neighbour + ((error / 48) * 7);
                setPixel(x + 1, y, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x + 2, y);
                curValue = neighbour + ((error / 48) * 5);
                setPixel(x + 2, y, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x - 2, y + 1);
                curValue = neighbour + ((error / 48) * 3);
                setPixel(x - 2, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x - 1, y + 1);
                curValue = neighbour + ((error / 48) * 5);
                setPixel(x - 1, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x, y + 1);
                curValue = neighbour + ((error / 48) * 7);
                setPixel(x, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x + 1, y + 1);
                curValue = neighbour + ((error / 48) * 5);
                setPixel(x + 1, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x + 2, y + 1);
                curValue = neighbour + ((error / 48) * 3);
                setPixel(x + 2, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x - 2, y + 2);
                curValue = neighbour + (error / 48);
                setPixel(x - 2, y + 2, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x - 1, y + 2);
                curValue = neighbour + ((error / 48) * 3);
                setPixel(x - 1, y + 2, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x, y + 2);
                curValue = neighbour + ((error / 48) * 5);
                setPixel(x, y + 2, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x + 1, y + 2);
                curValue = neighbour + ((error / 48) * 3);
                setPixel(x + 1, y + 2, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x + 2, y + 2);
                curValue = neighbour + (error / 48);
                setPixel(x + 2, y + 2, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }
        }
    }
}

void Image::stuckiDithering()
{
    Rgb curValue;

    for (std::size_t y = 0; y < height; y++)
    {
        Rgb error(0, 0, 0);

        for (std::size_t x = 0; x < width; x++)
        {
            curValue = getPixelRgb(x, y);
            assignNewValues(curValue.red, curValue.red, error.red);
            assignNewValues(curValue.green, curValue.green, error.green);
            assignNewValues(curValue.blue, curValue.blue, error.blue);

            setPixel(x, y, curValue);

            Rgb neighbour;

            try
            {
                neighbour = getPixelRgb(x + 1, y);
                curValue = neighbour + ((error / 42) * 8);
                setPixel(x + 1, y, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x + 2, y);
                curValue = neighbour + ((error / 42) * 4);
                setPixel(x + 2, y, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x - 2, y + 1);
                curValue = neighbour + ((error / 42) * 2);
                setPixel(x - 2, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x - 1, y + 1);
                curValue = neighbour + ((error / 42) * 4);
                setPixel(x - 1, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x, y + 1);
                curValue = neighbour + ((error / 42) * 8);
                setPixel(x, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x + 1, y + 1);
                curValue = neighbour + ((error / 42) * 4);
                setPixel(x + 1, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x + 2, y + 1);
                curValue = neighbour + ((error / 42) * 2);
                setPixel(x + 2, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x - 2, y + 2);
                curValue = neighbour + (error / 42);
                setPixel(x - 2, y + 2, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x - 1, y + 2);
                curValue = neighbour + ((error / 42) * 2);
                setPixel(x - 1, y + 2, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x, y + 2);
                curValue = neighbour + ((error / 42) * 4);
                setPixel(x, y + 2, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x + 1, y + 2);
                curValue = neighbour + ((error / 42) * 2);
                setPixel(x + 1, y + 2, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x + 2, y + 2);
                curValue = neighbour + (error / 42);
                setPixel(x + 2, y + 2, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }
        }
    }
}

void Image::atkinsonDithering()
{
    Rgb curValue;

    for (std::size_t y = 0; y < height; y++)
    {
        Rgb error(0, 0, 0);

        for (std::size_t x = 0; x < width; x++)
        {
            curValue = getPixelRgb(x, y);
            assignNewValues(curValue.red, curValue.red, error.red);
            assignNewValues(curValue.green, curValue.green, error.green);
            assignNewValues(curValue.blue, curValue.blue, error.blue);

            setPixel(x, y, curValue);

            Rgb neighbour;

            try
            {
                neighbour = getPixelRgb(x + 1, y);
                curValue = neighbour + (error / 8);
                setPixel(x + 1, y, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x + 2, y);
                curValue = neighbour + (error / 8);
                setPixel(x + 2, y, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x - 1, y + 1);
                curValue = neighbour + (error / 8);
                setPixel(x - 1, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x, y + 1);
                curValue = neighbour + (error / 8);
                setPixel(x, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x + 1, y + 1);
                curValue = neighbour + (error / 8);
                setPixel(x + 1, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x, y + 2);
                curValue = neighbour + (error / 8);
                setPixel(x, y + 2, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }
        }
    }
}

void Image::burkesDithering()
{
    Rgb curValue;

    for (std::size_t y = 0; y < height; y++)
    {
        Rgb error(0, 0, 0);

        for (std::size_t x = 0; x < width; x++)
        {
            curValue = getPixelRgb(x, y);
            assignNewValues(curValue.red, curValue.red, error.red);
            assignNewValues(curValue.green, curValue.green, error.green);
            assignNewValues(curValue.blue, curValue.blue, error.blue);

            setPixel(x, y, curValue);

            Rgb neighbour;

            try
            {
                neighbour = getPixelRgb(x + 1, y);
                curValue = neighbour + ((error / 32) * 8);
                setPixel(x + 1, y, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x + 2, y);
                curValue = neighbour + ((error / 32) * 4);
                setPixel(x + 2, y, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x - 2, y + 1);
                curValue = neighbour + ((error / 32) * 2);
                setPixel(x - 2, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x - 1, y + 1);
                curValue = neighbour + ((error / 32) * 4);
                setPixel(x - 1, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x, y + 1);
                curValue = neighbour + ((error / 32) * 8);
                setPixel(x, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x + 1, y + 1);
                curValue = neighbour + ((error / 32) * 4);
                setPixel(x + 1, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x + 2, y + 1);
                curValue = neighbour + ((error / 32) * 2);
                setPixel(x + 2, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }
        }
    }
}

void Image::sierraDithering()
{
    Rgb curValue;

    for (std::size_t y = 0; y < height; y++)
    {
        Rgb error(0, 0, 0);

        for (std::size_t x = 0; x < width; x++)
        {
            curValue = getPixelRgb(x, y);
            assignNewValues(curValue.red, curValue.red, error.red);
            assignNewValues(curValue.green, curValue.green, error.green);
            assignNewValues(curValue.blue, curValue.blue, error.blue);

            setPixel(x, y, curValue);

            Rgb neighbour;

            try
            {
                neighbour = getPixelRgb(x + 1, y);
                curValue = neighbour + ((error / 32) * 5);
                setPixel(x + 1, y, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x + 2, y);
                curValue = neighbour + ((error / 32) * 3);
                setPixel(x + 2, y, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x - 2, y + 1);
                curValue = neighbour + ((error / 32) * 2);
                setPixel(x - 2, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x - 1, y + 1);
                curValue = neighbour + ((error / 32) * 4);
                setPixel(x - 1, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x, y + 1);
                curValue = neighbour + ((error / 32) * 5);
                setPixel(x, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x + 1, y + 1);
                curValue = neighbour + ((error / 32) * 4);
                setPixel(x + 1, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x + 2, y + 1);
                curValue = neighbour + ((error / 32) * 2);
                setPixel(x + 2, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x - 1, y + 2);
                curValue = neighbour + ((error / 32) * 2);
                setPixel(x + 2, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x, y + 2);
                curValue = neighbour + ((error / 32) * 3);
                setPixel(x + 2, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x + 1, y + 2);
                curValue = neighbour + ((error / 32) * 2);
                setPixel(x + 2, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }
        }
    }
}

void Image::twoRowSierra()
{
    Rgb curValue;

    for (std::size_t y = 0; y < height; y++)
    {
        Rgb error(0, 0, 0);

        for (std::size_t x = 0; x < width; x++)
        {
            curValue = getPixelRgb(x, y);
            assignNewValues(curValue.red, curValue.red, error.red);
            assignNewValues(curValue.green, curValue.green, error.green);
            assignNewValues(curValue.blue, curValue.blue, error.blue);

            setPixel(x, y, curValue);

            Rgb neighbour;

            try
            {
                neighbour = getPixelRgb(x + 1, y);
                curValue = neighbour + ((error / 16) * 4);
                setPixel(x + 1, y, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x + 2, y);
                curValue = neighbour + ((error / 16) * 3);
                setPixel(x + 2, y, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x - 2, y + 1);
                curValue = neighbour + (error / 16);
                setPixel(x - 2, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x - 1, y + 1);
                curValue = neighbour + ((error / 16) * 2);
                setPixel(x - 1, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x, y + 1);
                curValue = neighbour + ((error / 16) * 3);
                setPixel(x, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x + 1, y + 1);
                curValue = neighbour + ((error / 16) * 2);
                setPixel(x + 1, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x + 2, y + 1);
                curValue = neighbour + (error / 16);
                setPixel(x + 2, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }
        }
    }
}

void Image::sierraLite()
{
    Rgb curValue;

    for (std::size_t y = 0; y < height; y++)
    {
        Rgb error(0, 0, 0);

        for (std::size_t x = 0; x < width; x++)
        {
            curValue = getPixelRgb(x, y);
            assignNewValues(curValue.red, curValue.red, error.red);
            assignNewValues(curValue.green, curValue.green, error.green);
            assignNewValues(curValue.blue, curValue.blue, error.blue);

            setPixel(x, y, curValue);

            Rgb neighbour;

            try
            {
                neighbour = getPixelRgb(x + 1, y);
                curValue = neighbour + ((error / 4) * 2);
                setPixel(x + 1, y, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x - 1, y + 1);
                curValue = neighbour + (error / 4);
                setPixel(x - 1, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = getPixelRgb(x, y + 1);
                curValue = neighbour + (error / 4);
                setPixel(x, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }
        }
    }
}
// grayscale 17 = (17,17,17)
// (0.3 * R) + (0.59 * G) + (0.11 * B) = 17
// 17 + 17 + 17  <=>  0.3*