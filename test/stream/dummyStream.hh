#ifndef STROM_TEST_DUMMYSTREAM_HH
#define STROM_TEST_DUMMYSTREAM_HH 1

extern "C" {
#include <strom/stream.h>
}

enum dummyType_t {
    TYPE_NUMBER,
    TYPE_TEXT,
    TYPE_WINTEXT
};

struct dummyData_t {
    char numPtr;
    int timesRefreshed;
    int count;
    dummyType_t type;
};

#define TIMES_REFRESHED(sd) ((struct dummyData_t*)sd->handle.pointer)->timesRefreshed

extern ISTREAM *S_openTestIStream(int count, dummyType_t type);

extern void S_closeTestIStream(ISTREAM *sd);

extern OSTREAM *S_openTestOStream();

#endif //STROM_TEST_DUMMYSTREAM_HH
