#include <catch2/catch_test_macros.hpp>
#include "dummyStream.hh"

extern "C" {
#include <strom/stream.h>
}

TEST_CASE("[Stream] S_readUntil works properly", "[StreamTests]") {
    ISTREAM *sd;
    char *word;

    sd = S_openTestIStream(15, TYPE_TEXT);
    word = S_readUntil(sd, ' ');
    CHECK(strcmp(word, "Hi") == 0);
    free(word);

    S_closeTestIStream(sd);
}

TEST_CASE("[Stream] S_readUntil works properly with refresh", "[StreamTests]") {
    ISTREAM *sd;
    char *word;

    sd = S_openTestIStream(15, TYPE_TEXT);
    word = S_readUntil(sd, '!');
    CHECK(strcmp(word, "Hi World") == 0);
    free(word);

    S_closeTestIStream(sd);
}

TEST_CASE("[Stream] S_readUntil refuses write-only stream", "[StreamTests]") {
    OSTREAM *sd;
    char *word;
    sd = S_openTestOStream();

    word = S_readUntil(sd, ' ');

    CHECK(word == NULL);

    S_closeTestOStream(sd);
}