#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include "../Image/image.h"
#include "../PBM/pbm.h"
#include "../PGM/pgm.h"
#include "../PPM/ppm.h"
#include "../Editor/image_editor.h"

typedef void (*ditheringAlg)(Image *);

class Commands
{

private:
    static std::string curFilePath;

public:
    static ditheringAlg dithering[10];
    static void initiateCommand(std::string command, Image *&image);
    static void open(std::string file_path, Image *&image);
    static void save(Image *&image);
    static void saveas(std::string file_path, Image *&image);
    static void dither(Image *&image);
    static void close(Image *&image);
};

//helpers
std::size_t printgetDitheringIndex();
void toUpper(std::string &input); //make whole string to upper

#endif