#include <iostream>
#include <fstream>
#include "Image/image.h"
#include "PBM/pbm.h"
#include "PGM/pgm.h"
#include "PPM/ppm.h"

int main()
{
    std::ifstream file("Files/TestImg.ppm",  std::ios::binary);

    char form[3];

    //file >> form[0] >> form[1];
    //form[2] = '\0';
    //file.getline(form, 3);

    std::string format; //= form;
    file >> format;

    //PBM pic(form, file, "Files/text1.pbm");
    //PGM pic(form, file, "Files/text1.pgm");
    PPM pic(format, file, "Files/text7.ppm");

    //pic.print();

    std::cout << "--------------------------------" << std::endl;
    
    //pic.crop(130,120, 180, 170);
    //std::cout << pic.getPixelGrayscale(0, 6);

    //pic.resize(600,600, true);

    //pic.errorDiffusion();
    //pic.twoDimErrorDiffusion();
    pic.floydDithering();
    pic.write();

    //pic.print();


    file.close();

    return 0;
}