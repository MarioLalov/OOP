#include "commands.h"

ditheringAlg Commands::dithering[11] = {ImageEditor::errorDiffusion, ImageEditor::twoDimErrorDiffusion,
                                        ImageEditor::floydDithering, ImageEditor::jarvisDithering,
                                        ImageEditor::stuckiDithering, ImageEditor::atkinsonDithering,
                                        ImageEditor::burkesDithering, ImageEditor::sierraDithering,
                                        ImageEditor::twoRowSierra, ImageEditor::sierraLite,
                                        ImageEditor::orderedDithering8x8};

std::string Commands::curFilePath;

void Commands::open(std::string file_path, Image *&image)
{
    //if another image is open colse it
    if (image != nullptr)
    {
        deleteImage(image);
    }

    std::cout << "Opening..." << std::endl;

    //set current file path
    curFilePath = file_path;
    std::string extension = curFilePath.substr(curFilePath.find_last_of(".") + 1);

    //open file for reading
    std::ifstream file(curFilePath, std::ios::binary);

    //read format
    std::string format;
    file >> format;

    //create image in the correct format
    if ((format == "P1" || format == "P4") && extension == "pbm")
    {
        image = new PBM(format, file);
    }
    else if ((format == "P2" || format == "P5") && extension == "pgm")
    {
        image = new PGM(format, file);
    }
    else if ((format == "P3" || format == "P6") && extension == "ppm")
    {
        image = new PPM(format, file);
    }
    else
    {
        throw std::invalid_argument("Unsupported file format.");
    }

    file.close();
    std::cout << "Successfully opened " << curFilePath << std::endl;
}

void Commands::deleteImage(Image *&image)
{
    if (image == nullptr)
    {
        throw std::logic_error("Cannot close - no document loaded");
    }

    //delte image and mark as deleted
    delete image;
    image = nullptr;

    std::cout << "Successfully closed " << curFilePath << std::endl;

    //clear file path
    curFilePath.clear();
}

void Commands::save(Image *&image)
{
    //check if an image is loaded
    if (curFilePath.empty() || image == nullptr)
    {
        throw std::logic_error("Cannot save - no document loaded");
    }

    std::cout << "Saving..." << std::endl;

    //get extension of current file and save
    std::string extension = curFilePath.substr(curFilePath.find_last_of(".") + 1);
    image->write(curFilePath, extension);

    std::cout << "Successfully saved " << curFilePath << std::endl;
}

void Commands::saveas(std::string file_path, Image *&image)
{
    //check if an image is loaded
    if (image == nullptr)
    {
        throw std::logic_error("Cannot save - no document loaded");
    }

    std::cout << "Saving..." << std::endl;

    //get extension and save if formats are compatible
    std::string extension = file_path.substr(file_path.find_last_of(".") + 1);
    image->write(file_path, extension);

    std::cout << "Successfully saved another " << file_path << std::endl;
}

void Commands::dither(Image *&image)
{
    //check if an image is loaded
    if (image == nullptr)
    {
        throw std::logic_error("Cannot edit - no document loaded");
    }

    //call dithering algorithm
    dithering[getDitheringIndex()](image);

    std::cout << "Dithering successful" << std::endl;
}

void Commands::crop(Image *&image, std::vector<std::string> &params)
{
    if (curFilePath.empty() || image == nullptr)
    {
        throw std::logic_error("Cannot edit - no document loaded");
    }

    int upper_x, upper_y, lower_x, lower_y;

    //get crop parameters and validate
    try
    {
        upper_x = std::stoi(params[0]);
        upper_y = std::stoi(params[1]);
        lower_x = std::stoi(params[2]);
        lower_y = std::stoi(params[3]);
    }
    catch (const std::invalid_argument &err)
    {
        throw std::invalid_argument("Invalid parameters crop");
    }

    if (upper_x < 0 || upper_y < 0 || lower_x < 0 || lower_y < 0)
    {
        throw std::invalid_argument("Invalid parameters for crop");
    }

    ImageEditor::crop(image, upper_x, upper_y, lower_x, lower_y);

    std::cout << "Cropping successful" << std::endl;
}

void Commands::resize(Image *&image, std::vector<std::string> &params)
{
    if (curFilePath.empty() || image == nullptr)
    {
        throw std::logic_error("Cannot edit - no document loaded");
    }

    int new_width, new_height;

    //check if it's percentage resize or pixel resize
    if (params.size() == 1 && params[0].back() == '%')
    {
        int percentage;

        try
        {
            percentage = std::stoi(params[0].substr(0, params[0].size() - 1));
        }
        catch (const std::invalid_argument &err)
        {
            throw std::invalid_argument("Invalid parameters resize");
        }

        //call resize for percentage
        ImageEditor::resize(image, percentage, percentage, true);
    }
    else if (params.size() == 2)
    {
        try
        {
            new_width = std::stoi(params[0]);
            new_height = std::stoi(params[1]);
        }
        catch (const std::invalid_argument &err)
        {
            throw std::invalid_argument("Invalid parameters resize");
        }

        //call resize for pixels
        ImageEditor::resize(image, new_width, new_height, false);
    }
    else
    {
        throw std::invalid_argument("Invalid parameters resize");
    }

    std::cout << "Resizing successful" << std::endl;
}

void Commands::createNew(Image *&image, std::vector<std::string> &params)
{
    //if another image is loaded close it
    if (image != nullptr)
    {
        deleteImage(image);
    }

    int height, width;
    std::string colorValue;

    //get values and validate
    try
    {
        width = std::stoi(params[0]);
        height = std::stoi(params[1]);
    }
    catch (const std::invalid_argument &err)
    {
        throw std::invalid_argument("Invalid parameters for new");
    }

    if (width < 0 || height < 0)
    {
        throw std::invalid_argument("Invalid parameters for new");
    }

    image = Image::createImageByColor(width, height, params[2]);

    std::cout << "Created new document with size " << width << "x" << height 
              << ", format " << image->getFormat()
              << " and filled with " << params[2] << std::endl;
}

void Commands::exit(Image *&image)
{
    //close image before exiting
    if (image != nullptr)
    {
        deleteImage(image);
    }

    std::cout << "Exiting..." << std::endl;
}

void Commands::initiateCommand(std::string fullCommand, Image *&image, bool &end)
{
    std::string command;
    std::string param;
    std::vector<std::string> params;

    std::size_t endPos;
    std::size_t startPos;

    //get command
    endPos = fullCommand.find(' ');
    command = fullCommand.substr(0, endPos);

    //make command in upper case
    toUpper(command);

    //get parameters
    while (endPos < fullCommand.length())
    {
        while (fullCommand[endPos + 1] == ' ')
        {
            endPos++;
        }

        startPos = endPos + 1;

        if (fullCommand[startPos] == '"')
        {
            startPos += 1;
            endPos = fullCommand.find('"', startPos);

            param = fullCommand.substr(startPos, endPos - startPos);

            endPos++;
        }
        else
        {
            endPos = fullCommand.find(' ', startPos);

            param = fullCommand.substr(startPos, endPos - startPos);
        }

        params.push_back(param);
    }

    if (command == "OPEN" && params.size() == 1)
    {
        try
        {
            open(params[0], image);
        }
        catch (const std::invalid_argument &err)
        {
            //if file path is invalid clear curFilePath
            curFilePath.clear();

            throw err;
        }
    }
    else if (command == "SAVE" && params.size() == 0)
    {
        save(image);
    }
    else if (command == "SAVEAS" && params.size() == 1)
    {
        saveas(params[0], image);
    }
    else if (command == "CLOSE" && params.size() == 0)
    {
        deleteImage(image);
    }
    else if (command == "DITHER" && params.size() == 0)
    {
        dither(image);
    }
    else if (command == "NEW" && (params.size() == 5 || params.size() == 3))
    {
        createNew(image, params);
    }
    else if (command == "CROP" && params.size() == 4)
    {
        crop(image, params);
    }
    else if (command == "RESIZE" && (params.size() == 2 || params.size() == 1))
    {
        resize(image, params);
    }
    else if (command == "EXIT" && params.size() == 0)
    {
        exit(image);
        end = true;
    }
    else if (command == "HELP" && params.size() == 0)
    {
        printHelpMenu();
    }
    else
    {
        throw std::invalid_argument("Unknown command: " + command);
    }
}

void toUpper(std::string &input) //make whole string to upper
{
    for (std::size_t i = 0; i < input.length(); i++)
    {
        input[i] = std::toupper(input[i]);
    }
}

std::size_t getDitheringIndex()
{
    //print dithering menu
    std::cout << "Please enter the index of the dithering algorithm you'd like to use: " << std::endl;
    std::cout << "---------------------------------------------------------------------" << std::endl;
    std::cout << "(1) Error Diffusion Dithering " << std::endl;
    std::cout << "(2) Two-Dimensional Error Diffusion Dithering " << std::endl;
    std::cout << "(3) Floyd-Steinberg Dithering " << std::endl;
    std::cout << "(4) Jarvis, Judice, and Ninke Dithering " << std::endl;
    std::cout << "(5) Stucki Dithering " << std::endl;
    std::cout << "(6) Atkinson Dithering " << std::endl;
    std::cout << "(7) Burkes Dithering " << std::endl;
    std::cout << "(8) Sierra Dithering " << std::endl;
    std::cout << "(9) Two-Row Sierra Diffusion Dithering " << std::endl;
    std::cout << "(10) Sierra Dithering " << std::endl;
    std::cout << "(11) Ordered Dithering with 8x8 Bayer matrix " << std::endl;

    //get dithering algorithm index
    std::string s_index;
    std::size_t index;
    do
    {
        std::cout << "Enter dithering index: ";
        std::getline(std::cin, s_index);

        try
        {
            index = std::stoi(s_index);
        }
        catch (const std::invalid_argument err)
        {
            std::cout << "Please enter a number" << std::endl;
        }
    } while (index > 11 || index == 0);

    return index - 1;
}

void printHelpMenu()
{
    std::cout << "Possible commands:" << std::endl;
    std::cout << "---------------------------------------------------------------------" << std::endl;
    std::cout << "Create new image: NEW <width> <height> <color>" << std::endl;
    std::cout << "Open existing image: OPEN <file path>" << std::endl;
    std::cout << "Save changes to current image: SAVE" << std::endl;
    std::cout << "Save new copy of current image: SAVEAS <file path>" << std::endl;
    std::cout << "Crop image: CROP <upper x> <upper y> <lower x> <lower y>" << std::endl;
    std::cout << "Resize image: RESIZE <new width> <new height> or RESIZE <new size in percentage>" << std::endl;
    std::cout << "Dither current image: Dither" << std::endl;
    std::cout << "Close current image: CLOSE" << std::endl;
    std::cout << "Exit program: EXIT" << std::endl;
    std::cout << "---------------------------------------------------------------------" << std::endl;
}