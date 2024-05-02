#include <strom/stream.h>
#include <stdlib.h>

STREAM *S_createStream() {
    STREAM *sd;

    sd = (STREAM*)malloc(sizeof(STREAM));
    
    sd->buffer = (char*)malloc(S_BUFFERSIZE * sizeof(char));

    sd->ptr = 0;
    sd->size = 0;
    sd->flags = 0;

    return sd;
}

void S_destroyStream(STREAM *sd) {
    free(sd->buffer);
    free(sd);
}