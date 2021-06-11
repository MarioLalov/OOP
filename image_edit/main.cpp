#include <iostream>
#include <fstream>
#include "Image/image.h"
#include "PBM/pbm.h"
#include "PGM/pgm.h"
#include "PPM/ppm.h"
#include "Editor/image_editor.h"
#include "Commands/commands.h"

/*! \mainpage Description
 *
 * \section Main
 *
 * The user enters a command
 * 
 * The command is then intepreted by the program which decides how to procced depending on the command
 * 
 * Invalid commands or commands with invalid parameters are discarded
 * 
 * The first command that the user must issue is OPEN
 * 
 * If an image isn't loaded the programm will inform the user that they can't perform other operations
 * 
 * After an image is loaded, the user can edit and save the edited image in the current or other file
 * 
 * Before exiting or upon opening another image, the current image is automatically closed by the program
 * 
 * The user can also use CLOSE to close the current image
 * 
 * Closed images are treated as such and thus can't be edited
 *
 * Saving in different format is possible if the two formats are compatible
 * 
 * \section abstract class Image 
 *
 * Class that holds all identical properities 
 * 
 * All editing functions work with pointers to Image    
 * 
 * \section PBM 
 *
 * Monochrome(0,1)
 * 
 * In PBM 0 == white and 1 == black in contrast to other formats
 * 
 * Can be created from both P1 and P4 file formats
 * 
 * Can also be created by the NEW command after setting the background color as a monchrome value (0 or 1)
 * 
 * (example) new 100 100 1
 * 
 * Can be saved to .pbm, .pgm and .ppm file (both raw and ASCII for .pgm and .ppm and only ASCII for .pbm)
 * 
 * \section PGM 
 *
 * Grayscale(0-255)
 * 
 * Can be created from both P2 and P5 file formats
 * 
 * Can also be created by the NEW command after setting the background color as a grayscale value (0,255)
 * 
 * ex. new 100 100 15
 * 
 * Can be saved to .pgm and .ppm file (both raw and ASCII)
 *
 * \section PPM 
 *
 * Rgb(0-255)
 * 
 * Can be created from both P3 and P4 file formats
 * 
 * Can also be created by the NEW command after setting the background color as an Rgb value 
 * 
 * ex. new 100 100 11 42 33 
 * 
 * Can be saved to .ppm file (both raw and ASCII)
 *
 * \section Image Editor
 *
 * Works with pointers to abstract calss Image
 * 
 * Implements crop, resize and all dithering algorithms
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