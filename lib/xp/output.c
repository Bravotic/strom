#include <strom/stream.h>

void S_flush(OSTREAM *sd) {
    if (sd->flags & S_WRITE) {
        sd->bufferFunction.flush(sd);
    }
}

int S_putch(OSTREAM *sd, char ch) {

    /* Only allow putch if the stream is writable */
    /* TODO: Should we use EOF on write streams? Seems weird but could be
     * understandable */
    if (sd->flags & S_WRITE) {

        /* If our pointer is larger than our buffer size limit, we need to flush
         * the buffer */
        /* We use S_BUFFERSIZE here since sd->size is used to track how much
         * is loaded into the buffer. In this case, we want the actual upper
         * limit of the buffer */
        if (sd->ptr >= S_BUFFERSIZE) {
            sd->bufferFunction.flush(sd);
        }

        /* Add the character to the pointer */
        sd->buffer[sd->ptr++] = ch;

        return ch;
    }
    else {
        return -1;
    }
}