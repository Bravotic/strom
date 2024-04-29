#include <catch2/catch_test_macros.hpp>

extern "C" {
#include <strom/stream.h>
#include <strom/string.h>
}

TEST_CASE("[String] String refresh works", "[StringTests]") {
    ISTREAM *sd;
    char buffer[5];
    size_t len;

    sd = S_openStringInput("Hello world");

    len = S_read(buffer, sizeof(char), 5, sd);
    REQUIRE(len == 5);
    CHECK(strncmp(buffer, "Hello", 5) == 0);

    CHECK(S_getch(sd) == ' ');

    len = S_read(buffer, sizeof(char), 5, sd);
    REQUIRE(len == 5);
    CHECK(strncmp(buffer, "world", 5) == 0);

    S_closeStringInput(sd);
}