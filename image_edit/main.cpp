#include <iostream>
#include <fstream>
#include "image.h"
#include "pbm.h"
#include "pgm.h"
#include "ppm.h"

int main()
{
    std::ifstream file("text2.pgm");

    char form[3];

    file >> form[0] >> form[1];
    form[2] = '\0';
    //file.getline(form, 3);

    std::string format = form;

    //PBM pic(form, file, "text1.pbm");
    PGM pic(form, file, "text1.pgm");
    //PPM pic(form, file, "text4.ppm");

    pic.crop(0,0, 11, 6);
    //std::cout << pic.getPixelGrayscale(0, 6);

    //pic.resize(10, 15, false);

    //pic.print();

    file.close();

    return 0;
}