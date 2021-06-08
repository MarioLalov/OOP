#include <iostream>
#include <fstream>
#include "Image/image.h"
#include "PBM/pbm.h"
#include "PGM/pgm.h"
#include "PPM/ppm.h"
#include "Editor/image_editor.h"
#include "Commands/commands.h"

/*! \mainpage Main
 *
 * \section Run commands
 *
 * Image tests: g++ Image/image.cpp Tests/image_test.cpp
 * 
 * PBM tests: g++ Image/image.cpp PBM/pbm.cpp Tests/pbm_test.cpp Editor/image_editor.cpp
 * 
 * PGM tests: g++ Image/image.cpp PGM/pgm.cpp Tests/pgm_test.cpp Editor/image_editor.cpp
 * 
 * PPM tests: g++ Image/image.cpp PPM/ppm.cpp Tests/ppm_test.cpp Editor/image_editor.cpp
 * 
 * Full compilation: g++ Image/image.cpp PBM/pbm.cpp PGM/pgm.cpp PPM/ppm.cpp Editor/image_editor.cpp main.cpp Commands/commands.cpp
 *
 * \section Program commands
 *
 * open <file_path>
 * 
 * new <width> <height> <color>
 * 
 * close
 * 
 * exit
 * 
 * save
 * 
 * saveas <file_path>
 * 
 * crop <upper_x> <upper_y> <lower_x> <lower_y>
 * 
 * resize <width> <height> || <percentage>
 * 
 * dither
 * 
 */
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
        catch (const std::bad_alloc &err)
        {
            std::cout << err.what() << '\n';
            
            return 0;
        }

        command.clear();
    }

    return 0;
}