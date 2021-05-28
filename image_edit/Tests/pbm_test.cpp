#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../PBM/pbm.h"
#include <fstream>
#include <string>

TEST_CASE("regular files")
{
    std::ifstream file("C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testPBM.pbm", std::ios::binary);
    std::string format;
    file >> format;

    PBM pic(format, file, "C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testPBM.pbm");

    REQUIRE(pic.getWidth() == 6);
    REQUIRE(pic.getHeight() == 10);
    //REQUIRE(pic.getPixelRgb(1,2).red == 3);
    REQUIRE(!pic.getPixelGrayscale(2, 1) == 1);
}

TEST_CASE("files with comments")
{
    std::ifstream file("C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testCommentPBM.pbm", std::ios::binary);
    std::string format;
    file >> format;

    PBM pic(format, file, "C:\\uni\\Sem2\\OOP\\image_edit\\Tests\\testCommentPBM.pbm");

    REQUIRE(pic.getWidth() == 6);
    REQUIRE(pic.getHeight() == 10);
    REQUIRE(!pic.getPixelGrayscale(2, 1) == 1);
    //REQUIRE(pic.getPixelRgb(1,2).red == 3);
}
