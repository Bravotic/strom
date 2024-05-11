#include <catch2/catch_test_macros.hpp>
#include "dummyStream.hh"

extern "C" {
#include <strom/stream.h>
}


TEST_CASE("[Stream] S_puts works as intended", "[StreamTests]") {
    OSTREAM *sd;
    char *buffer;
    size_t len;

    sd = S_openTestOStream();

    buffer = ((dummyBufferData_t*)(sd->handle.pointer))->buffer;

    CHECK(S_puts("123", sd) == 0);
    S_flush(sd);

    CHECK(strncmp(buffer, "123", 3) == 0);

    CHECK(S_puts("4567", sd) == 0);
    S_flush(sd);

    CHECK(strncmp(buffer, "1234567", 7) == 0);

    S_closeTestOStream(sd);
}