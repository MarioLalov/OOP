#include "commands.h"

ditheringAlg Commands::dithering[10] = {ImageEditor::errorDiffusion, ImageEditor::twoDimErrorDiffusion, 
                                        ImageEditor::floydDithering, ImageEditor::jarvisDithering,
                                        ImageEditor::stuckiDithering, ImageEditor::atkinsonDithering,
                                        ImageEditor::burkesDithering, ImageEditor::sierraDithering,
                                        ImageEditor::twoRowSierra, ImageEditor::sierraLite};


void toUpper(std::string &input) //make whole string to upper
{
    for (std::size_t i = 0; i < input.length(); i++)
    {
        input[i] = std::toupper(input[i]);
    }
}

void Commands::open(std::string file_path, Image *image)
{
    std::ifstream file(file_path, std::ios::binary);

    //read format
    std::string format;
    file >> format;

    if (format == "P1")
    {
        //try
        image = new PBM(format, file);
    }
    else if (format == "P2")
    {
        image = new PGM(format, file);
    }
    else if (format == "P3")
    {
        image = new PPM(format, file);
    }

    file.close();
}

void Commands::initiateCommand(std::string fullCommand, Image *image)
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
}