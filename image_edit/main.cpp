#include <iostream>
#include <fstream>
#include "image.h"
#include "pbm.h"
#include "pgm.h"
#include "ppm.h"

int main()
{
    std::ifstream file("text3.ppm");

    char form[3];

    file >> form[0] >> form[1];
    form[2] = '\0';
    //file.getline(form, 3);

    std::string format = form;

    //PBM pic(form, file, "text1.pbm");
    //PGM pic(form, file, "text1.pgm");
    PPM pic(form, file, "text4.ppm");

    //pic.print();

    std::cout << "--------------------------------" << std::endl;
    
    //pic.crop(0,0, 1, 1);
    //std::cout << pic.getPixelGrayscale(0, 6);

    pic.resize(15,15, false);

    pic.write();

    //pic.print();

    file.close();

    return 0;
}