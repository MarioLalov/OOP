#include "image_editor.h"

void ImageEditor::crop(Image *&image, std::size_t upper_x, std::size_t upper_y, std::size_t lower_x, std::size_t lower_y)
{
    //validate input
    if (upper_x > lower_x || upper_y > lower_y)
    {
        throw std::invalid_argument("Reversed coordinates!");
    }

    //assign coordinates if out of range
    if (upper_x >= image->getWidth())
    {
        upper_x = image->getWidth() - 1;
    }

    if (upper_x < 0)
    {
        upper_x = 0;
    }

    if (lower_x >= image->getWidth())
    {
        lower_x = image->getWidth() - 1;
    }

    if (lower_x < 0)
    {
        lower_x = 0;
    }

    if (upper_y >= image->getHeight())
    {
        upper_y = image->getHeight() - 1;
    }

    if (upper_y < 0)
    {
        upper_y = 0;
    }

    if (lower_y >= image->getHeight())
    {
        lower_y = image->getHeight() - 1;
    }

    if (lower_y < 0)
    {
        lower_y = 0;
    }

    //get new dimensions
    std::size_t newWidth = (lower_x - upper_x + 1);
    std::size_t newHeight = (lower_y - upper_y + 1);

    //start editing
    Image *newImg = Image::createImageByFormat(image->getFormat(), newWidth, newHeight);

    //copy values to cropped
    for (int i = upper_y, k = 0; i <= lower_y; i++, k++)
    {
        for (int j = upper_x, l = 0; j <= lower_x; j++, l++)
        {
            newImg->setPixel(l, k, image->getPixelRgb(j, i));
        }
    }

    delete image;
    image = newImg;
}

void ImageEditor::resize(Image *&image, int widthInput, int heightInput, bool percentage)
{
    //get new dimensions
    std::size_t newWidth;
    std::size_t newHeight;
    newWidth = (percentage) ? roundToInt(((double)widthInput / 100) * ((double)image->getWidth())) : widthInput;
    newHeight = (percentage) ? roundToInt(((double)heightInput / 100) * ((double)image->getHeight())) : heightInput;

    //start editing
    Image *newImg = Image::createImageByFormat(image->getFormat(), newWidth, newHeight);

    //assign pixel values to resized picture
    for (int i = 0; i < newHeight; i++)
    {
        for (int j = 0; j < newWidth; j++)
        {
            //calculate source pixel coordinates
            std::size_t srcX = roundToInt((((double)j) / ((double)newWidth)) * ((double)image->getWidth()));
            srcX = std::min(srcX, image->getWidth() - 1);
            std::size_t srcY = roundToInt((((double)i) / ((double)newHeight)) * ((double)image->getHeight()));
            srcY = std::min(srcY, image->getHeight() - 1);

            newImg->setPixel(j, i, image->getPixelRgb(srcX, srcY));
        }
    }

    delete image;
    image = newImg;
}

void ImageEditor::errorDiffusion(Image *image)
{
    Rgb curValue;
    Rgb pixel;

    for (std::size_t y = 0; y < image->getHeight(); y++)
    {
        //set error to zero
        Rgb error(0, 0, 0);

        for (std::size_t x = 0; x < image->getWidth(); x++)
        {
            //get value of pixel
            curValue = image->getPixelRgb(x, y);

            //calculate value after pusing the error
            pixel = curValue + error;
            assignNewValues(pixel, curValue, error);

            //set pixel value
            image->setPixel(x, y, curValue);
        }
    }
}

void ImageEditor::twoDimErrorDiffusion(Image *image)
{
    Rgb curValue;
    Rgb pixel;

    //hold previous row errors
    Rgb *rowError = new Rgb[image->getWidth()];

    for (std::size_t y = 0; y < image->getHeight(); y++)
    {
        //set error to zero
        Rgb error(0, 0, 0);

        for (std::size_t x = 0; x < image->getWidth(); x++)
        {
            //get value of pixel
            curValue = image->getPixelRgb(x, y);

            //calculate value after pusing the error
            pixel = curValue + error + rowError[x] / 2;
            assignNewValues(pixel, curValue, error);       //first for default error
            assignNewValues(pixel, curValue, rowError[x]); //second for row error

            //set pixel value
            image->setPixel(x, y, curValue);
        }
    }

    delete[] rowError;
}

void ImageEditor::floydDithering(Image *image)
{
    Rgb curValue;

    for (std::size_t y = 0; y < image->getHeight(); y++)
    {
        //set error to zero
        Rgb error(0, 0, 0);

        for (std::size_t x = 0; x < image->getWidth(); x++)
        {
            curValue = image->getPixelRgb(x, y);
            assignNewValues(curValue, curValue, error);
            image->setPixel(x, y, curValue);

            Rgb neighbour;

            //push error
            try
            {
                neighbour = image->getPixelRgb(x, y + 1);
                curValue = neighbour + ((error / 16) * 5);
                image->setPixel(x, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x + 1, y + 1);
                curValue = neighbour + (error / 16);
                image->setPixel(x + 1, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x - 1, y + 1);
                curValue = neighbour + ((error / 16) * 3);
                image->setPixel(x - 1, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x + 1, y);
                curValue = neighbour + ((error / 16) * 7);
                image->setPixel(x + 1, y, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }
        }
    }
}

void ImageEditor::jarvisDithering(Image *image)
{
    Rgb curValue;

    for (std::size_t y = 0; y < image->getHeight(); y++)
    {
        //set error to zero
        Rgb error(0, 0, 0);

        for (std::size_t x = 0; x < image->getWidth(); x++)
        {
            curValue = image->getPixelRgb(x, y);
            assignNewValues(curValue, curValue, error);
            image->setPixel(x, y, curValue);

            Rgb neighbour;

            //push error
            try
            {
                neighbour = image->getPixelRgb(x + 1, y);
                curValue = neighbour + ((error / 48) * 7);
                image->setPixel(x + 1, y, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x + 2, y);
                curValue = neighbour + ((error / 48) * 5);
                image->setPixel(x + 2, y, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x - 2, y + 1);
                curValue = neighbour + ((error / 48) * 3);
                image->setPixel(x - 2, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x - 1, y + 1);
                curValue = neighbour + ((error / 48) * 5);
                image->setPixel(x - 1, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x, y + 1);
                curValue = neighbour + ((error / 48) * 7);
                image->setPixel(x, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x + 1, y + 1);
                curValue = neighbour + ((error / 48) * 5);
                image->setPixel(x + 1, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x + 2, y + 1);
                curValue = neighbour + ((error / 48) * 3);
                image->setPixel(x + 2, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x - 2, y + 2);
                curValue = neighbour + (error / 48);
                image->setPixel(x - 2, y + 2, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x - 1, y + 2);
                curValue = neighbour + ((error / 48) * 3);
                image->setPixel(x - 1, y + 2, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x, y + 2);
                curValue = neighbour + ((error / 48) * 5);
                image->setPixel(x, y + 2, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x + 1, y + 2);
                curValue = neighbour + ((error / 48) * 3);
                image->setPixel(x + 1, y + 2, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x + 2, y + 2);
                curValue = neighbour + (error / 48);
                image->setPixel(x + 2, y + 2, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }
        }
    }
}

void ImageEditor::stuckiDithering(Image *image)
{
    Rgb curValue;

    for (std::size_t y = 0; y < image->getHeight(); y++)
    {
        //set error to zero
        Rgb error(0, 0, 0);

        for (std::size_t x = 0; x < image->getWidth(); x++)
        {
            curValue = image->getPixelRgb(x, y);
            assignNewValues(curValue, curValue, error);
            image->setPixel(x, y, curValue);

            Rgb neighbour;

            //push error
            try
            {
                neighbour = image->getPixelRgb(x + 1, y);
                curValue = neighbour + ((error / 42) * 8);
                image->setPixel(x + 1, y, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x + 2, y);
                curValue = neighbour + ((error / 42) * 4);
                image->setPixel(x + 2, y, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x - 2, y + 1);
                curValue = neighbour + ((error / 42) * 2);
                image->setPixel(x - 2, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x - 1, y + 1);
                curValue = neighbour + ((error / 42) * 4);
                image->setPixel(x - 1, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x, y + 1);
                curValue = neighbour + ((error / 42) * 8);
                image->setPixel(x, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x + 1, y + 1);
                curValue = neighbour + ((error / 42) * 4);
                image->setPixel(x + 1, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x + 2, y + 1);
                curValue = neighbour + ((error / 42) * 2);
                image->setPixel(x + 2, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x - 2, y + 2);
                curValue = neighbour + (error / 42);
                image->setPixel(x - 2, y + 2, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x - 1, y + 2);
                curValue = neighbour + ((error / 42) * 2);
                image->setPixel(x - 1, y + 2, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x, y + 2);
                curValue = neighbour + ((error / 42) * 4);
                image->setPixel(x, y + 2, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x + 1, y + 2);
                curValue = neighbour + ((error / 42) * 2);
                image->setPixel(x + 1, y + 2, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x + 2, y + 2);
                curValue = neighbour + (error / 42);
                image->setPixel(x + 2, y + 2, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }
        }
    }
}

void ImageEditor::atkinsonDithering(Image *image)
{
    Rgb curValue;

    for (std::size_t y = 0; y < image->getHeight(); y++)
    {
        //set error to zero
        Rgb error(0, 0, 0);

        for (std::size_t x = 0; x < image->getWidth(); x++)
        {
            curValue = image->getPixelRgb(x, y);
            assignNewValues(curValue, curValue, error);
            image->setPixel(x, y, curValue);

            Rgb neighbour;

            //push error
            try
            {
                neighbour = image->getPixelRgb(x + 1, y);
                curValue = neighbour + (error / 8);
                image->setPixel(x + 1, y, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x + 2, y);
                curValue = neighbour + (error / 8);
                image->setPixel(x + 2, y, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x - 1, y + 1);
                curValue = neighbour + (error / 8);
                image->setPixel(x - 1, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x, y + 1);
                curValue = neighbour + (error / 8);
                image->setPixel(x, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x + 1, y + 1);
                curValue = neighbour + (error / 8);
                image->setPixel(x + 1, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x, y + 2);
                curValue = neighbour + (error / 8);
                image->setPixel(x, y + 2, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }
        }
    }
}

void ImageEditor::burkesDithering(Image *image)
{
    Rgb curValue;

    for (std::size_t y = 0; y < image->getHeight(); y++)
    {
        //set error to zero
        Rgb error(0, 0, 0);

        for (std::size_t x = 0; x < image->getWidth(); x++)
        {
            curValue = image->getPixelRgb(x, y);
            assignNewValues(curValue, curValue, error);
            image->setPixel(x, y, curValue);

            Rgb neighbour;

            //push error
            try
            {
                neighbour = image->getPixelRgb(x + 1, y);
                curValue = neighbour + ((error / 32) * 8);
                image->setPixel(x + 1, y, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x + 2, y);
                curValue = neighbour + ((error / 32) * 4);
                image->setPixel(x + 2, y, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x - 2, y + 1);
                curValue = neighbour + ((error / 32) * 2);
                image->setPixel(x - 2, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x - 1, y + 1);
                curValue = neighbour + ((error / 32) * 4);
                image->setPixel(x - 1, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x, y + 1);
                curValue = neighbour + ((error / 32) * 8);
                image->setPixel(x, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x + 1, y + 1);
                curValue = neighbour + ((error / 32) * 4);
                image->setPixel(x + 1, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x + 2, y + 1);
                curValue = neighbour + ((error / 32) * 2);
                image->setPixel(x + 2, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }
        }
    }
}

void ImageEditor::sierraDithering(Image *image)
{
    Rgb curValue;

    for (std::size_t y = 0; y < image->getHeight(); y++)
    {
        //set error to zero
        Rgb error(0, 0, 0);

        for (std::size_t x = 0; x < image->getWidth(); x++)
        {
            curValue = image->getPixelRgb(x, y);
            assignNewValues(curValue, curValue, error);
            image->setPixel(x, y, curValue);

            Rgb neighbour;

            //push error
            try
            {
                neighbour = image->getPixelRgb(x + 1, y);
                curValue = neighbour + ((error / 32) * 5);
                image->setPixel(x + 1, y, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x + 2, y);
                curValue = neighbour + ((error / 32) * 3);
                image->setPixel(x + 2, y, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x - 2, y + 1);
                curValue = neighbour + ((error / 32) * 2);
                image->setPixel(x - 2, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x - 1, y + 1);
                curValue = neighbour + ((error / 32) * 4);
                image->setPixel(x - 1, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x, y + 1);
                curValue = neighbour + ((error / 32) * 5);
                image->setPixel(x, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x + 1, y + 1);
                curValue = neighbour + ((error / 32) * 4);
                image->setPixel(x + 1, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x + 2, y + 1);
                curValue = neighbour + ((error / 32) * 2);
                image->setPixel(x + 2, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x - 1, y + 2);
                curValue = neighbour + ((error / 32) * 2);
                image->setPixel(x + 2, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x, y + 2);
                curValue = neighbour + ((error / 32) * 3);
                image->setPixel(x + 2, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x + 1, y + 2);
                curValue = neighbour + ((error / 32) * 2);
                image->setPixel(x + 2, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }
        }
    }
}

void ImageEditor::twoRowSierra(Image *image)
{
    Rgb curValue;

    for (std::size_t y = 0; y < image->getHeight(); y++)
    {
        //set error to zero
        Rgb error(0, 0, 0);

        for (std::size_t x = 0; x < image->getWidth(); x++)
        {
            curValue = image->getPixelRgb(x, y);
            assignNewValues(curValue, curValue, error);
            image->setPixel(x, y, curValue);

            Rgb neighbour;

            //push error
            try
            {
                neighbour = image->getPixelRgb(x + 1, y);
                curValue = neighbour + ((error / 16) * 4);
                image->setPixel(x + 1, y, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x + 2, y);
                curValue = neighbour + ((error / 16) * 3);
                image->setPixel(x + 2, y, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x - 2, y + 1);
                curValue = neighbour + (error / 16);
                image->setPixel(x - 2, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x - 1, y + 1);
                curValue = neighbour + ((error / 16) * 2);
                image->setPixel(x - 1, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x, y + 1);
                curValue = neighbour + ((error / 16) * 3);
                image->setPixel(x, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x + 1, y + 1);
                curValue = neighbour + ((error / 16) * 2);
                image->setPixel(x + 1, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x + 2, y + 1);
                curValue = neighbour + (error / 16);
                image->setPixel(x + 2, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }
        }
    }
}

void ImageEditor::sierraLite(Image *image)
{
    Rgb curValue;

    for (std::size_t y = 0; y < image->getHeight(); y++)
    {
        //set error to zero
        Rgb error(0, 0, 0);

        for (std::size_t x = 0; x < image->getWidth(); x++)
        {
            curValue = image->getPixelRgb(x, y);
            assignNewValues(curValue, curValue, error);
            image->setPixel(x, y, curValue);

            Rgb neighbour;

            //push error
            try
            {
                neighbour = image->getPixelRgb(x + 1, y);
                curValue = neighbour + ((error / 4) * 2);
                image->setPixel(x + 1, y, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x - 1, y + 1);
                curValue = neighbour + (error / 4);
                image->setPixel(x - 1, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }

            try
            {
                neighbour = image->getPixelRgb(x, y + 1);
                curValue = neighbour + (error / 4);
                image->setPixel(x, y + 1, curValue);
            }
            catch (std::out_of_range)
            {
                neighbour = Rgb(0, 0, 0);
            }
        }
    }
}

double thresholdMap(std::size_t i, std::size_t j, int n)
{
    //calculate pixel using (NxN) treshold map
    return (double)(~(i << (i ^ j))) / (n * n);
}

void ImageEditor::orderedDithering8x8(Image *image)
{
    int n = 8;
    Rgb curValue(0, 0, 0);
    Rgb newValue(0, 0, 0);

    for (std::size_t x = 0; x < image->getWidth(); x++)
    {
        for (std::size_t y = 0; y < image->getHeight(); y++)
        {
            curValue = image->getPixelRgb(x, y);

            newValue.red = (int)((double)curValue.red + (thresholdMap(x % n, y % n, n) - 0.5));
            newValue.green = (int)((double)curValue.green + (thresholdMap(x % n, y % n, n) - 0.5));
            newValue.blue = (int)((double)curValue.blue + (thresholdMap(x % n, y % n, n) - 0.5));

            image->setPixel(x, y, newValue);
        }
    }
}

void assignIntValues(int &value, int &destValue, int &errorValue)
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

void assignNewValues(Rgb value, Rgb &destValue, Rgb &errorValue)
{
    assignIntValues(value.red, destValue.red, errorValue.red);
    assignIntValues(value.green, destValue.green, errorValue.green);
    assignIntValues(value.blue, destValue.blue, errorValue.blue);
    /*if ((255 - value.red) > value.red)
    {
        errorValue.red = value.red;
        destValue.red = 0;
    }
    else
    {
        errorValue.red = value.red - 255;
        destValue.red = 255;
    }

    if ((255 - value.green) > value.green)
    {
        errorValue.green = value.green;
        destValue.green = 0;
    }
    else
    {
        errorValue.green = value.green - 255;
        destValue.green = 255;
    }

    if ((255 - value.blue) > value.blue)
    {
        errorValue.blue = value.blue;
        destValue.blue = 0;
    }
    else
    {
        errorValue.blue = value.blue - 255;
        destValue.blue = 255;
    }*/
}