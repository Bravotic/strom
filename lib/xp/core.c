#include <strom/stream.h>
#include <stdlib.h>

STREAM *S_createStream(void) {
    STREAM *sd;

    sd = (STREAM*)malloc(sizeof(STREAM));
    
    /* There was an error creating our stream memory allocation */
    if (sd == NULL) {
      return NULL;
    }

    sd->buffer = (char*)malloc(S_BUFFERSIZE * sizeof(char));

    /* There was an error creating our buffer */
    if (sd->buffer == NULL) {
      return NULL;
    }

    sd->ptr = 0;
    sd->size = 0;
    sd->flags = 0;

    return sd;
}

void S_destroyStream(STREAM *sd) {
    /* Flush output buffers when destroyed */
    if (sd->flags & S_WRITE) {
        S_flush(sd);
    }

    free(sd->buffer);
    free(sd);
}
