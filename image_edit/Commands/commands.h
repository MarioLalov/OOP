#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "../Image/image.h"
#include "../PBM/pbm.h"
#include "../PGM/pgm.h"
#include "../PPM/ppm.h"
#include "../Editor/image_editor.h"

typedef void(*ditheringAlg)(Image*);

class Commands
{
    static ditheringAlg dithering[10];
    static void initiateCommand(std::string command, Image* image);
    static void open(std::string file_path, Image* image);
};

#endif