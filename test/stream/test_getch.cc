#include <catch2/catch_test_macros.hpp>
#include "dummyStream.hh"

extern "C" {
#include <strom/stream.h>
}

TEST_CASE("[Stream] S_getch works correctly", "[StreamTests]"){
    int charOne, charTwo, charThree;
    ISTREAM *sd;

    sd = S_openTestIStream(2, TYPE_NUMBER);
    REQUIRE(sd->bufferFunction.fill != NULL);

    charOne = S_getch(sd);

    CHECK(charOne == '0');

    charTwo = S_getch(sd);
    CHECK(charTwo == '1');

    CHECK(sd->ptr == sd->size);

    charThree = S_getch(sd);
    CHECK(charThree == -1);
    CHECK(S_eof(sd));

    S_closeTestIStream(sd);
}

TEST_CASE("[Stream] S_getch hits EOF", "[StreamTests]") {
    ISTREAM *sd;

    sd = S_openTestIStream(1, TYPE_NUMBER);

    CHECK_FALSE(S_eof(sd));

    S_getch(sd);

    CHECK(S_eof(sd));

    S_closeTestIStream(sd);
}

TEST_CASE("[Stream] S_getch returns -1 on EOF", "[StreamTests]") {
    ISTREAM *sd;

    sd = S_openTestIStream(1, TYPE_NUMBER);

    /* Make sure we are starting without EOF */
    CHECK_FALSE(S_eof(sd));

    /* Get our first character */
    CHECK(S_getch(sd) == '0');

    /* Our next character should be EOF */
    CHECK(S_getch(sd) == -1);

    /* Verify EOF */
    CHECK(S_eof(sd));

    /* Repeat check for EOF using getch() */
    CHECK(S_getch(sd) == -1);

    S_closeTestIStream(sd);
}

TEST_CASE("[Stream] S_getch properly refills buffer", "[StreamTests]") {
    ISTREAM *sd;
    int x;

    sd = S_openTestIStream(2 * S_BUFFERSIZE, TYPE_NUMBER);

    /* Make sure our buffersize is artificially small */
    REQUIRE(S_BUFFERSIZE == 5);

    /* Make sure buffer hasn't refreshed already */
    CHECK(TIMES_REFRESHED(sd) == 0);

    /* Since the buffer starts with nothing, it should refresh once after
     * first calling getch() */
    S_getch(sd);
    CHECK(TIMES_REFRESHED(sd) == 1);

    for (x = 0; x < 4; x++) {
        S_getch(sd);
    }

    /* We are at the end of our buffer */
    CHECK(sd->ptr == sd->size);

    /* Make sure we haven't refreshed until getch is called again */
    CHECK(TIMES_REFRESHED(sd) == 1);

    /* Make sure we don't mistakenly hit EOF */
    CHECK(S_getch(sd) != -1);

    /* Make sure the refresh was run */
    CHECK(TIMES_REFRESHED(sd) == 2);

    /* And make sure our readptr resets */
    CHECK(sd->ptr == 1);

    S_closeTestIStream(sd);
}

TEST_CASE("[Stream] getch read from write-only stream", "[StreamTests]") {
    OSTREAM *sd;
    sd = S_openTestOStream();

    CHECK(S_getch(sd) == -1);
}