#include <catch2/catch_test_macros.hpp>
#include "dummyStream.hh"

extern "C" {
#include <strom/stream.h>
}

TEST_CASE("[Stream] S_write works properly and flushes properly", "[StreamTests]") {
    OSTREAM *sd;
    char *buffer;
    size_t len;

    sd = S_openTestOStream();

    buffer = ((dummyBufferData_t*)(sd->handle.pointer))->buffer;

    // Test writing a standard char buffer
    len = S_write("123", sizeof(char), 3, sd);
    CHECK(len == 3);

    S_flush(sd);
    CHECK(strncmp(buffer, "123", 3) == 0);

    // Test writing a char buffer masquerading as a short buffer :)
    len = S_write("45", sizeof(short), 1, sd);
    CHECK(len == 1);

    S_flush(sd);
    CHECK(strncmp(buffer, "12345", 5) == 0);

    S_closeTestOStream(sd);
}

TEST_CASE("[Stream] S_write flushes the buffer", "[StreamTests]") {
    OSTREAM *sd;
    char *buffer;
    size_t len;

    sd = S_openTestOStream();

    buffer = ((dummyBufferData_t*)(sd->handle.pointer))->buffer;

    REQUIRE(S_BUFFERSIZE == 5);

    // Writing exactly 5 bytes should NOT flush a buffer of size 5
    len = S_write("12345", sizeof(char), 5, sd);
    CHECK(len == 5);
    CHECK(((dummyBufferData_t*)(sd->handle.pointer))->timesFlushed == 0);

    // Writing anymore will cause the buffer to flush
    len = S_write("67", sizeof(char), 2, sd);
    CHECK(len == 2);
    CHECK(((dummyBufferData_t*)(sd->handle.pointer))->timesFlushed == 1);

    // Make sure that a flush will happen mid write
    len = S_write("890123", sizeof(char), 6, sd);
    CHECK(len == 6);
    CHECK(((dummyBufferData_t*)(sd->handle.pointer))->timesFlushed == 2);

    // Now make sure the flushes add together correctly in the buffer
    S_flush(sd);

    CHECK(strncmp(buffer, "1234567890123", 13) == 0);

    S_closeTestOStream(sd);
}