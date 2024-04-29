#include <stdlib.h>
#include "dummyStream.hh"

char numbers[] = "0123456789";
char text[] = "Hi World!\n";
char textWindows[] = "Hi World!\r\n";

void S_testIStreamFillBuffer(STREAM *sd) {
    dummyData_t *td;

    /* Get our string data handle */
    td = (struct dummyData_t*)sd->handle.pointer;

    td->timesRefreshed++;

    /* If our position is past the length of the string, we hit the end */
    if (td->count == 0) {
        sd->flags |= S_EOF;
        return;
    }

    /* Otherwise we are filling the buffer */
    else {
        const char *stringpos;
        size_t readSize;
        int x;

        /* If at our position, reading S_BUFFERSIZE would read more characters
         * than we have, then we would only want to read the remaining chars,
         * so if that is the case, our size is len - pos, otherwise its just
         * S_BUFFERSIZE */
        if (S_BUFFERSIZE > td->count) {
            readSize = td->count;
        }
        else {
            readSize = S_BUFFERSIZE;
        }

        for (x = 0; x < readSize; x++, td->numPtr++) {
            if (td->type == TYPE_NUMBER) {
                sd->buffer[x] = numbers[td->numPtr % 10];
            }
            else if(td->type == TYPE_TEXT) {
                sd->buffer[x] = text[td->numPtr % 10];
            }
            else if(td->type == TYPE_WINTEXT) {
                sd->buffer[x] = textWindows[td->numPtr % 11];
            }

        }

        td->count -= readSize;

        /* Reset our read pointer and read size, and increment our refresh
         * counter */
        sd->ptr = 0;
        sd->size = readSize;
    }
}

ISTREAM *S_openTestIStream(int count, dummyType_t type) {
    STREAM *sd;
    dummyData_t *dat;

    sd = S_createStream();

    dat = (dummyData_t*)malloc( sizeof(dummyData_t));
    dat->count = count;
    dat->numPtr = 0;
    dat->timesRefreshed = 0;
    dat->type = type;

    sd->bufferFunction.fill = S_testIStreamFillBuffer;

    sd->handle.pointer = (void*)dat;
    sd->flags |= S_READ;

    return sd;
}

void S_closeTestIStream(ISTREAM *sd) {
    free(sd->handle.pointer);
    S_destroyStream(sd);
}

/* STUB: OSTREAM is not yet properly implemented */
OSTREAM *S_openTestOStream() {
    STREAM *sd;
    dummyData_t *dat;

    sd = S_createStream();

    sd->bufferFunction.flush = S_testIStreamFillBuffer;

    sd->handle.pointer = NULL;
    sd->flags |= S_WRITE;

    return sd;
}