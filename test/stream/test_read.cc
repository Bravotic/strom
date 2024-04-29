#include <catch2/catch_test_macros.hpp>
#include "dummyStream.hh"

extern "C" {
#include <strom/stream.h>
}

TEST_CASE("[Stream] S_read works properly", "[StreamTests]") {
    ISTREAM *sd;
    size_t charReadSize, shortReadSize, restReadSize;
    char charBuffer[10];
    short shortBuffer[1];

    sd = S_openTestIStream(10, TYPE_NUMBER);
    charReadSize = S_read(charBuffer, sizeof(char), 5, sd);

    CHECK(charReadSize == 5);
    CHECK(strncmp(charBuffer, "01234", 5) == 0);

    /* Check reading in a short instead of a char */
    shortReadSize = S_read(shortBuffer, sizeof(short), 1, sd);
    CHECK(shortReadSize == 1);

    CHECK(shortBuffer[0] == 0x3635); /* 56 */

    /* Here, we will read more than is in the buffer, make sure it actually
     * stops when it has too much */
    restReadSize = S_read(charBuffer, sizeof(char), 5, sd);
    CHECK(restReadSize == 3);
    /* 234 */
    CHECK(strncmp(charBuffer, "789", 3) == 0);

    S_closeTestIStream(sd);
}

TEST_CASE("[Stream] S_read fills buffer", "[StreamTests]") {
    ISTREAM *sd;
    size_t len;
    char buffer[5];

    sd = S_openTestIStream(15, TYPE_NUMBER);

    /* Buffer should start with no refreshes */
    CHECK(TIMES_REFRESHED(sd) == 0);

    /* After we call read for the first time, it should refresh our buffer since
     * the buffer starts empty */
    len = S_read(buffer, sizeof(char), 2, sd);
    REQUIRE(len == 2);
    CHECK(TIMES_REFRESHED(sd) == 1);

    /* When we call read here, we will get right to the edge of the buffer, but
     * without the need to refresh yet... so it shouldn't refresh */
    len = S_read(buffer, sizeof(char), 3, sd);
    REQUIRE(len == 3);
    CHECK(TIMES_REFRESHED(sd) == 1);

    /* Once we call read again, we should refresh once more */
    len = S_read(buffer, sizeof(char), 1, sd);
    REQUIRE(len == 1);
    CHECK(TIMES_REFRESHED(sd) == 2);

    /* If we read more than the buffer, we expect it to refresh in the middle of
     * reading */
    len = S_read(buffer, sizeof(char), 7, sd);
    REQUIRE(len == 7);
    CHECK(TIMES_REFRESHED(sd) == 3);

    S_closeTestIStream(sd);
}

TEST_CASE("[Stream] S_read() read from write-only stream", "[StreamTests]") {
    OSTREAM *sd;
    char buffer[5];
    size_t len;
    sd = S_openTestOStream();

    len = S_read(buffer, sizeof(char), 5, sd);
    CHECK(len == 0);

    S_closeTestIStream(sd);
}