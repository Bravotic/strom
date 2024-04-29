#include <strom/stream.h>
#include <strom/string.h>

#include <string.h>
#include <stdlib.h>

typedef struct stringdat_t {
    const char *str;
    size_t refreshed;
} stringdat_t;

void S_stringFillReadBuffer(STREAM *sd) {
    stringdat_t *dat;
    size_t len, pos;

    /* Get our string data handle */
    dat = (stringdat_t*)sd->handle.pointer;

    /* Get the length of our string, so we can make sure we are not reading
     * more than we can */
    len = strlen(dat->str);

    /* Our position increases by S_BUFFERSIZE every time we fill the buffer,
     * since we keep track of how many times we refresh in dat->refreshed, we
     * simply multiply the times refreshed by S_BUFFERSIZE. */
    pos = dat->refreshed * S_BUFFERSIZE;

    /* If our position is past the length of the string, we hit the end */
    if (pos >= len) {
        sd->flags |= S_EOF;
        return;
    }
    /* Otherwise we are filling the buffer */
    else {
        const char *stringpos;
        size_t readSize;

        /* If at our position, reading S_BUFFERSIZE would read more characters
         * than we have, then we would only want to read the remaining chars,
         * so if that is the case, our size is len - pos, otherwise its just 
         * S_BUFFERSIZE */
        if (pos + S_BUFFERSIZE > len) {
            readSize = len - pos;
        }
        else {
            readSize = S_BUFFERSIZE;
        }

        /* The position we are copying from is str[pos] */
        stringpos = &dat->str[pos];

        /* Do the copy */
        strncpy(sd->buffer, stringpos, readSize);
        
        /* Reset our read pointer and read size, and increment our refresh 
         * counter */
        sd->ptr = 0;
        sd->size = readSize;
        dat->refreshed++;
    }
}

ISTREAM *S_openStringInput(const char *str) {
    ISTREAM *sd;
    stringdat_t *dat;

    dat = malloc(sizeof(stringdat_t));

    sd = S_createStream();
    sd->bufferFunction.fill = S_stringFillReadBuffer;

    dat->str = str;
    dat->refreshed = 0;

    sd->handle.pointer = (void*)dat;

    sd->flags |= S_READ;
    
    return sd;
}

void S_closeStringInput(ISTREAM *sd) {
    S_destroyStream(sd);
}