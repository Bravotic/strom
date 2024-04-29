#include <catch2/catch_test_macros.hpp>
#include "dummyStream.hh"

extern "C" {
#include <strom/stream.h>
}

TEST_CASE("[Stream] ungetch works in full buffer", "[StreamTests]") {
    ISTREAM *sd;

    sd = S_openTestIStream(2 * S_BUFFERSIZE, TYPE_NUMBER);

    CHECK(S_getch(sd) == '0');

    CHECK(S_ungetch(sd, '0') == '0');

    CHECK(S_getch(sd) == '0');

    CHECK(S_ungetch(sd, 'X') == 'X');

    CHECK(S_getch(sd) == 'X');

    S_closeTestIStream(sd);
}

TEST_CASE("[Stream] ungetch works in empty buffer", "[StreamTests]") {
    ISTREAM *sd;

    sd = S_openTestIStream(2 * S_BUFFERSIZE, TYPE_NUMBER);

    CHECK(S_ungetch(sd, 'X') == 'X');
    CHECK(S_getch(sd) == 'X');
}

TEST_CASE("[Stream] ungetch works at beginning of full buffer", "[StreamTests]") {
    ISTREAM *sd;

    sd = S_openTestIStream(2 * S_BUFFERSIZE, TYPE_NUMBER);

    CHECK(S_getch(sd) == '0');

    CHECK(S_ungetch(sd, 'X') == 'X');
    CHECK(S_ungetch(sd, 'X') == 'X');

    CHECK(S_getch(sd) == 'X');
    CHECK(S_getch(sd) == 'X');
}