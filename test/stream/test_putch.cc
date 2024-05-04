#include <catch2/catch_test_macros.hpp>
#include "dummyStream.hh"

extern "C" {
#include <strom/stream.h>
}

TEST_CASE("[Stream] S_putch works correctly", "[StreamTests]") {
    int charOne, charTwo;
    ISTREAM *sd;

    sd = S_openTestOStream();
    REQUIRE(sd->bufferFunction.flush != NULL);

    // We should start at ptr = 0. Basically make sure wehn we put data we
    // increase the pointer and don't flush the data.
    CHECK(sd->ptr == 0);
    charOne = S_putch(sd, '1');
    CHECK(sd->ptr == 1);
    CHECK(((dummyBufferData_t*)(sd->handle.pointer))->timesFlushed == 0);

    // Make sure that the function correctly returns what it should, in this
    // case the character that was just pushed to the output buffer. And make
    // sure the character is placed in the correct part of the buffer
    CHECK(charOne == '1');
    CHECK(sd->buffer[0] == '1');

    CHECK(sd->ptr == 1);
    charTwo = S_putch(sd, '2');
    CHECK(sd->ptr == 2);
    CHECK(((dummyBufferData_t*)(sd->handle.pointer))->timesFlushed == 0);

    // Make sure we are not overwriting our original value we pushed before
    CHECK(charTwo == '2');
    CHECK(sd->buffer[0] == '1');
    CHECK(sd->buffer[1] == '2');

    // Make sure we are actually flushing the buffer when we force a flush
    S_flush(sd);
    CHECK(((dummyBufferData_t*)(sd->handle.pointer))->timesFlushed == 1);

    // Verify that when flushed, our data is added to the flush buffer
    CHECK(((dummyBufferData_t*)(sd->handle.pointer))->buffer[0] == '1');
    CHECK(((dummyBufferData_t*)(sd->handle.pointer))->buffer[1] == '2');

    S_closeTestOStream(sd);
}

TEST_CASE("[Stream] S_putch flushes buffer when full", "[StreamTests]") {
    ISTREAM *sd;

    sd = S_openTestOStream();
    REQUIRE(sd->bufferFunction.flush != NULL);
    REQUIRE(S_BUFFERSIZE == 5);

    CHECK(S_putch(sd, '1') == '1');
    CHECK(((dummyBufferData_t*)(sd->handle.pointer))->timesFlushed == 0);
    CHECK(sd->ptr == 1);

    CHECK(S_putch(sd, '2') == '2');
    CHECK(((dummyBufferData_t*)(sd->handle.pointer))->timesFlushed == 0);
    CHECK(sd->ptr == 2);

    CHECK(S_putch(sd, '3') == '3');
    CHECK(((dummyBufferData_t*)(sd->handle.pointer))->timesFlushed == 0);
    CHECK(sd->ptr == 3);

    CHECK(S_putch(sd, '4') == '4');
    CHECK(((dummyBufferData_t*)(sd->handle.pointer))->timesFlushed == 0);
    CHECK(sd->ptr == 4);

    CHECK(S_putch(sd, '5') == '5');
    CHECK(((dummyBufferData_t*)(sd->handle.pointer))->timesFlushed == 0);
    CHECK(sd->ptr == 5);

    CHECK(S_putch(sd, '6') == '6');
    CHECK(((dummyBufferData_t*)(sd->handle.pointer))->timesFlushed == 1);
    CHECK(sd->ptr == 1);

    S_closeTestOStream(sd);
}

TEST_CASE("[Stream] OutputBuffer incomplete flush appends data", "[StreamTests]") {
    ISTREAM *sd;

    sd = S_openTestOStream();

    CHECK(S_putch(sd, '1') == '1');
    CHECK(((dummyBufferData_t*)(sd->handle.pointer))->timesFlushed == 0);
    CHECK(sd->ptr == 1);

    S_flush(sd);
    CHECK(((dummyBufferData_t*)(sd->handle.pointer))->timesFlushed == 1);

    CHECK(S_putch(sd, '2') == '2');
    CHECK(((dummyBufferData_t*)(sd->handle.pointer))->timesFlushed == 1);
    CHECK(sd->ptr == 1);

    S_flush(sd);
    CHECK(((dummyBufferData_t*)(sd->handle.pointer))->timesFlushed == 2);

    CHECK(((dummyBufferData_t*)(sd->handle.pointer))->buffer[0] == '1');
    CHECK(((dummyBufferData_t*)(sd->handle.pointer))->buffer[1] == '2');

    S_closeTestOStream(sd);
}