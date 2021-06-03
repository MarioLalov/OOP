#include <iostream>
#include <fstream>
#include "Image/image.h"
#include "PBM/pbm.h"
#include "PGM/pgm.h"
#include "PPM/ppm.h"
#include "Editor/image_editor.h"
#include "Commands/commands.h"

int main()
{
    Image *image = nullptr;
    std::string command;

    bool end = false;

    while (!end)
    {
        std::cout << "Enter command: ";
        std::getline(std::cin, command);

        try
        {
            Commands::initiateCommand(command, image, end);
        }
        catch (const std::invalid_argument &err)
        {
            std::cout << err.what() << '\n';
        }
        catch (const std::logic_error &err)
        {
            std::cout << err.what() << '\n';
        }

        command.clear();
    }

    return 0;
}

/*int main()
{
    std::ifstream file("Files/TestImg.ppm", std::ios::binary);

    char form[3];

    //file >> form[0] >> form[1];
    //form[2] = '\0';
    //file.getline(form, 3);

    std::string format; //= form;
    file >> format;
    std::cout << format;

    //PBM pic(format, file, "Files/text1.pbm");
    //PGM pic(format, file, "Files/textDith.pgm");
    Image *pic = new PPM(format, file);

    //pic.print();

    std::cout << "--------------------------------" << std::endl;

    ImageEditor::crop(pic, 130,120, 180, 170);
    //std::cout << pic.getPixelGrayscale(0, 6);

    //ImageEditor::resize(pic, 600,600, true);

    //ImageEditor::errorDiffusion(pic);
    //ImageEditor::twoDimErrorDiffusion(pic);
    //ImageEditor::floydDithering(pic);
    //ImageEditor::jarvisDithering(pic);
    //ImageEditor::stuckiDithering(pic);
    //ImageEditor::atkinsonDithering(pic);
    //ImageEditor::burkesDithering(pic);
    //ImageEditor::sierraDithering(pic);
    //ImageEditor::twoRowSierra(pic);
    //ImageEditor::sierraLite(pic);
    //ImageEditor::orderedDithering(pic, 8);
    pic->write("Files/text8.ppm");

    //pic.print();

    file.close();

    delete pic;

    return 0;
}*/