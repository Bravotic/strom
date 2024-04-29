#include <catch2/catch_test_macros.hpp>

extern "C" {
#include <strom/file.h>
#include <strom/stream.h>
}

TEST_CASE("[File] Input stream accepts Input mode types", "[FileTests]") {
    ISTREAM *sd;

    sd = S_openFileInput(ROOT "/test/file/helloWorld.txt", "r");
    REQUIRE(sd != NULL);
    S_closeFileInput(sd);

    sd = S_openFileInput(ROOT "/test/file/helloWorld.txt", "rb");
    REQUIRE(sd != NULL);
    S_closeFileInput(sd);
}

TEST_CASE("[File] Input stream rejects Output mode types", "[FileTests]") {
    ISTREAM *sd;

    // write
    sd = S_openFileInput(ROOT "/test/file/helloWorld.txt", "w");
    CHECK(sd == NULL);

    // write
    sd = S_openFileInput(ROOT "/test/file/helloWorld.txt", "wb");
    CHECK(sd == NULL);

    // write
    sd = S_openFileInput(ROOT "/test/file/helloWorld.txt", "a");
    CHECK(sd == NULL);

    // write
    sd = S_openFileInput(ROOT "/test/file/helloWorld.txt", "ab");
    CHECK(sd == NULL);

    // read/write
    sd = S_openFileInput(ROOT "/test/file/helloWorld.txt", "w+");
    CHECK(sd == NULL);

    // read/write
    sd = S_openFileInput(ROOT "/test/file/helloWorld.txt", "wb+");
    CHECK(sd == NULL);

    // read/write
    sd = S_openFileInput(ROOT "/test/file/helloWorld.txt", "a+");
    CHECK(sd == NULL);

    // read/write
    sd = S_openFileInput(ROOT "/test/file/helloWorld.txt", "ab+");
    CHECK(sd == NULL);

    // read/write
    sd = S_openFileInput(ROOT "/test/file/helloWorld.txt", "r+");
    CHECK(sd == NULL);

    // read/write
    sd = S_openFileInput(ROOT "/test/file/helloWorld.txt", "rb+");
    CHECK(sd == NULL);
}

TEST_CASE("[File] Error file returns NULL stream", "[FileTests]") {
    ISTREAM *sd;

    // File does not exist
    sd = S_openFileInput("Fake file", "r");
    CHECK(sd == NULL);
}

TEST_CASE("[File] File refresh works", "[FileTests]") {
    ISTREAM *sd;
    char buffer[5];
    size_t len;

    sd = S_openFileInput(ROOT "/test/file/helloWorld.txt", "r");

    REQUIRE(sd != NULL);

    len = S_read(buffer, sizeof(char), 5, sd);
    REQUIRE(len == 5);
    CHECK(strncmp(buffer, "Hello", 5) == 0);

    CHECK(S_getch(sd) == '\n');

    len = S_read(buffer, sizeof(char), 5, sd);
    REQUIRE(len == 5);
    CHECK(strncmp(buffer, "world", 5) == 0);

    S_closeFileInput(sd);
}
