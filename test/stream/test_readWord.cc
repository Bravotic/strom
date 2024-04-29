#include <catch2/catch_test_macros.hpp>
#include "dummyStream.hh"

extern "C" {
#include <strom/stream.h>
}

TEST_CASE("[Stream] S_readWord() works properly without buffer refresh", "[StreamTests]") {
    ISTREAM *sd;
    char *word;

    sd = S_openTestIStream(15, TYPE_TEXT);
    word = S_readWord(sd);
    CHECK(strcmp(word, "Hi") == 0);

    free(word);
    S_closeTestIStream(sd);
}

TEST_CASE("[Stream] S_readWord() treats newline as space and refills buffer", "[StreamTests]") {
    ISTREAM *sd;
    char *word;

    sd = S_openTestIStream(15, TYPE_TEXT);
    word = S_readWord(sd);
    CHECK(strcmp(word, "Hi") == 0);
    free(word);

    word = S_readWord(sd);
    CHECK(strcmp(word, "World!") == 0);
    free(word);

    S_closeTestIStream(sd);
}

TEST_CASE("[Stream] S_readWord() properly increments pointer", "[StreamTests]") {
    ISTREAM *sd;
    char *word;

    sd = S_openTestIStream(15, TYPE_TEXT);
    word = S_readWord(sd);
    CHECK(strcmp(word, "Hi") == 0);
    free(word);

    CHECK(S_getch(sd) == 'W');

    S_closeTestIStream(sd);
}