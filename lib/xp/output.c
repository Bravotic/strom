#include <strom/stream.h>
#include <string.h>

void S_flush(OSTREAM *sd) {
    if (sd->flags & S_WRITE) {
        sd->bufferFunction.flush(sd);
    }
}

int S_putch(char ch, OSTREAM *sd) {

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

size_t S_write(const void *buffer, size_t size, size_t count, OSTREAM *sd) {
    if (sd->flags & S_WRITE) {
        char *bytebuffer;
        size_t realSize, i;

        /* From here, we will treat what we are writing as bytes (implying they
         * aren't already) */
        bytebuffer = (char*)buffer;

        /* Calculate our size in bytes, which is the size of the type we are
         * writing times the count of them */
        realSize = count * size;

        for (i = 0; i < realSize; i++) {
            if (sd->ptr >= S_BUFFERSIZE) {
                sd->bufferFunction.flush(sd);
            }

            sd->buffer[sd->ptr++] = bytebuffer[i];
        }

        return count;
    }
    else {
        return -1;
    }
}

int S_puts(const char *str, OSTREAM *sd) {
    if (sd->flags & S_WRITE) {
        size_t len, writelen;

        len = strlen(str);
        writelen = S_write(str, sizeof(char), len, sd);

        if (len != writelen) {
            return -1;
        }
        else {
            return 0;
        }
    }
    else {
        return -1;
    }
}
