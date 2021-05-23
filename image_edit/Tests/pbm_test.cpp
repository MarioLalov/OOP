#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../PBM/pbm.h"
#include <fstream>
#include <string>

TEST_CASE("Constructors and allocators")
{
    std::ifstream file("../text.pbm");
    std::string format;
    file >> format[0] >> format[1];

    PBM pic(format, file, "../text1.pbm");

    //REQUIRE
}
