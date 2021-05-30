#include <iostream>
#include <fstream>
#include "Image/image.h"
#include "PBM/pbm.h"
#include "PGM/pgm.h"
#include "PPM/ppm.h"
#include "Editor/image_editor.h"

int main()
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
    PPM *pic = new PPM(format, file);

    //pic.print();

    std::cout << "--------------------------------" << std::endl;

    //ImageEditor::crop(pic, 130,120, 180, 170);
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
    ImageEditor::orderedDithering(pic, 8);
    pic->write("Files/text7.ppm");

    //pic.print();

    file.close();

    delete pic;

    return 0;
}