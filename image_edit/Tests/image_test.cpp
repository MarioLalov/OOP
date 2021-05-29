#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../Image/image.h"
#include <fstream>
#include <string>

TEST_CASE("Rgb")
{
    Rgb obj1(1, 2, 3);
    Rgb obj2(4, 5, 6);

    SECTION("operator + for error")
    {
        SECTION("default +")
        {
            Rgb error(-1, 34, 5);
            obj2 = obj2 + error;

            REQUIRE(obj2.red == 3);
            REQUIRE(obj2.green == 39);
            REQUIRE(obj2.blue == 11);
        }

        SECTION("+ overflow")
        {
            Rgb error(-5, 254, 1);
            obj1 = obj1 + error;

            REQUIRE(obj1.red == 0);
            REQUIRE(obj1.green == 255);
            REQUIRE(obj1.blue == 4);
        }
    }

    SECTION("operator *")
    {
        SECTION("default *")
        {
            obj1 = obj1 * 5;

            REQUIRE(obj1.red == 5);
            REQUIRE(obj1.green == 10);
            REQUIRE(obj1.blue == 15);
        }

        SECTION("* overflow")
        {
            obj1 = obj1 * 128;

            REQUIRE(obj1.red == 128);
            REQUIRE(obj1.green == 255);
            REQUIRE(obj1.blue == 255);
        }
    }

    SECTION("operator /")
    {
        obj2 = obj2 / 2;

        REQUIRE(obj2.red == 2);
        REQUIRE(obj2.green == 2);
        REQUIRE(obj2.blue == 3);
    }
}