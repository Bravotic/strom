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
    size_t count;
    dummyType_t type;
};

struct dummyBufferData_t {
    char *buffer;
    size_t ptr;
    size_t size;
    int timesFlushed;
};

#define TIMES_REFRESHED(sd) ((struct dummyData_t*)sd->handle.pointer)->timesRefreshed

extern ISTREAM *S_openTestIStream(size_t count, dummyType_t type);

extern void S_closeTestIStream(ISTREAM *sd);

extern OSTREAM *S_openTestOStream();
extern void S_closeTestOStream(OSTREAM *sd);

#endif //STROM_TEST_DUMMYSTREAM_HH
