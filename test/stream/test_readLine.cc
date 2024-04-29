#include <catch2/catch_test_macros.hpp>
#include "dummyStream.hh"

extern "C" {
#include <strom/stream.h>
}

TEST_CASE("[Stream] S_readLine works properly", "[StreamTests]") {
    ISTREAM *sd;
    char *word;

    sd = S_openTestIStream(15, TYPE_TEXT);
    word = S_readLine(sd);
    CHECK(strcmp(word, "Hi World!\n") != 0);
    CHECK(strcmp(word, "Hi World!") == 0);
    free(word);

    S_closeTestIStream(sd);
}

TEST_CASE("[Stream] S_readLine truncates CR from CRLF", "[StreamTests]") {
    ISTREAM *sd;
    char *word;

    sd = S_openTestIStream(15, TYPE_WINTEXT);
    word = S_readLine(sd);
    CHECK(strcmp(word, "Hi World!\r") != 0);
    CHECK(strcmp(word, "Hi World!") == 0);
    free(word);

    S_closeTestIStream(sd);
}