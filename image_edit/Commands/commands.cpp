#include "commands.h"

ditheringAlg Commands::dithering[10] = {ImageEditor::errorDiffusion, ImageEditor::twoDimErrorDiffusion,
                                        ImageEditor::floydDithering, ImageEditor::jarvisDithering,
                                        ImageEditor::stuckiDithering, ImageEditor::atkinsonDithering,
                                        ImageEditor::burkesDithering, ImageEditor::sierraDithering,
                                        ImageEditor::twoRowSierra, ImageEditor::sierraLite};

std::string Commands::curFilePath;

void toUpper(std::string &input) //make whole string to upper
{
    for (std::size_t i = 0; i < input.length(); i++)
    {
        input[i] = std::toupper(input[i]);
    }
}

std::size_t getDitheringIndex()
{
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
    
    std::size_t index;
    do
    {
        std::cout << "Enter dithering index: " << std::endl;
        std::cin >> index;
    } while (index > 10 || index == 0);

    return index-1;
}

void Commands::open(std::string file_path, Image *&image)
{
    curFilePath = file_path;
    std::ifstream file(curFilePath, std::ios::binary);

    //read format
    std::string format;
    file >> format;

    if (format == "P1" /*&& std::filesystem::path(curFilePath).extension() == ".pbm"*/)
    {
        //try
        image = new PBM(format, file);
    }
    else if (format == "P2" /*&& std::filesystem::path(curFilePath).extension() == ".pgm"*/)
    {
        image = new PGM(format, file);
    }
    else if (format == "P3" /*&& std::filesystem::path(curFilePath).extension() == ".ppm"*/)
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

void Commands::close(Image *&image)
{
    delete image;

    std::cout << "Successfully closed " << curFilePath << std::endl;

    curFilePath.clear();
}

void Commands::save(Image *&image)
{
    if(curFilePath.empty())
    {
        throw std::logic_error("Cannot save - no document loaded");
    }

    image->write(curFilePath);
    std::cout << "Successfully saved " << curFilePath << std::endl;
}

void Commands::saveas(std::string file_path, Image *&image)
{
    image->write(file_path);
    std::cout << "Successfully saved another " << file_path << std::endl;
}

void Commands::dither(Image *& image)
{
    if(curFilePath.empty())
    {
        throw std::logic_error("Cannot edit - no document loaded");
    }

    dithering[getDitheringIndex()](image);

    std::cout << "Dithering successfull" << std::endl;

}

void Commands::initiateCommand(std::string fullCommand, Image *&image)
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

    if(command == "OPEN" && params.size() == 1)
    {
        try
        {
            open(params[0], image);
        }
        catch(const std::invalid_argument& err)
        {
            curFilePath.clear();

            throw err;
        }
    }
    else if(command == "SAVE" && params.size() == 0)
    {
        save(image);
    }
    else if(command == "SAVEAS" && params.size() == 1)
    {
        saveas(params[0], image);
    }
    else if(command == "CLOSE" && params.size() == 0)
    {
        close(image);
    }
    else if(command == "DITHER" && params.size() == 0)
    {
        dither(image);
    }
}