#include <strom/stream.h>
#include <stdlib.h>
#include <string.h>

int S_getch(ISTREAM *sd) {
    /* If we are not able to read, or we have nothing left to read, we should
     * return an error, in this case a -1 */
    if (!(sd->flags & S_READ) || sd->flags & S_EOF) {
        return -1;
    }

    /* If we are able to read, return the next character (if we can) */
    else {
        /* If our read pointer is larger than our buffer size, we need to 
         * refill the buffer with new data */
        if (sd->ptr >= sd->size || sd->size == 0) {
            sd->bufferFunction.fill(sd);

            /* After trying to refill, if we find we are really at the end of
             * the file, quit out */
            if (sd->flags & S_EOF) {
                return -1;
            }
        }

        /* Return the next character in the buffer */
        return sd->buffer[sd->ptr++];
    }
}

int S_ungetch(ISTREAM *sd, int ch) {
    if (sd->flags & S_READ) {

        /* If our pointer is at our size, and we still have space left in the
         * buffer, just append ch to the end of the buffer. */
        if (sd->ptr == sd->size && sd->size < S_BUFFERSIZE) {
            sd->buffer[sd->size] = (char)ch;
            sd->size++;
        }

        /* If our pointer is at 0, and our buffer is full, we need to expand */
        else if (sd->ptr == 0 && sd->size == S_BUFFERSIZE) {
            char *expandedBuffer;
            expandedBuffer = malloc(sizeof(char) * S_BUFFERSIZE + 1);

            memcpy(&expandedBuffer[1], sd->buffer, S_BUFFERSIZE);
            free(sd->buffer);

            sd->buffer = expandedBuffer;

            sd->buffer[sd->ptr] = (char)ch;
        }

        /* If our buffer isn't full, we can just shift right */
        else if (sd->ptr == 0) {
            memmove(&sd->buffer[1], sd->buffer, S_BUFFERSIZE);

            sd->buffer[sd->ptr] = (char)ch;
        }

        /* If we are just in the middle of the buffer, and out pointer is after
         * place 0. */
        else {
            sd->buffer[--sd->ptr] = (char)ch;
        }

        return ch;
    }
    else {
        return -1;
    }
}

int S_eof(ISTREAM *sd) {
    /* Fill our buffer if it is empty, if it sets the EOF flag, we have hit the
     * end of our stream */
    if (sd->ptr >= sd->size || sd->size == 0) {
        sd->bufferFunction.fill(sd);
    }
    return sd->flags & S_EOF;
}

size_t S_read(void *buffer, size_t size, size_t count, STREAM *sd) {
    if (sd->flags & S_READ) {
        size_t sizeRead, sizeInBytes;
        char *byteBuffer = (char*)buffer;

        /* Our real size (in bytes) is our count * size */
        sizeInBytes = count * size;

        /* Keep track of how many bytes we have read */
        sizeRead = 0;

        do {
            size_t copySize;

            /* Refill our buffer if it is empty */
            if (sd->ptr >= sd->size || sd->size == 0) {
                sd->bufferFunction.fill(sd);
            }

            /* If we hit EOF after refill, break out of the loop */
            if (S_eof(sd)) {
                break;
            }

                /* If the size between our pointer and the size of our buffer is
                 * less than the amount we still need to read, we only read the
                 * lesser size as not to overflow the buffer. */
            else if ((sd->size - sd->ptr) < (sizeInBytes - sizeRead)) {
                copySize = sd->size - sd->ptr;
            }

                /* If our buffer is sufficiently large, we can just read our
                 * remaining size */
            else {
                copySize = (sizeInBytes - sizeRead);
            }

            memcpy(&byteBuffer[sizeRead], &sd->buffer[sd->ptr], copySize);

            sizeRead += copySize;
            sd->ptr += copySize;

        } while (sizeRead < sizeInBytes);

        return sizeRead / size;
    }
    else {
        return 0;
    }
}

char *S_readWord(STREAM *sd) {
    size_t wordPtr, returnBufferPtr, returnBufferSize;
    char *returnBuffer;

    returnBuffer = (char*)malloc(sizeof(char) * S_BUFFERSIZE);
    returnBufferPtr = 0;
    returnBufferSize = S_BUFFERSIZE;

    /* FIXME: This loop could use a rework */
    for (wordPtr = 0;;wordPtr++) {

        /* If our pointer plus wordPtr equals the size of the buffer, we need
         * to copy the current word we have been reading to our return buffer
         * are refill the stream buffer. Or if we hit a space, we need to copy
         * the word into the buffer and quit. */
        if (sd->ptr + wordPtr >= sd->size || sd->buffer[sd->ptr + wordPtr] <= ' ') {
            /* Increase the size of our return buffer if needed */
            if (returnBufferPtr + wordPtr >= returnBufferSize) {
                returnBufferSize += wordPtr + 1;
                returnBuffer = realloc(returnBuffer, returnBufferSize * sizeof(char));

            }

            memcpy(&returnBuffer[returnBufferPtr], &sd->buffer[sd->ptr], wordPtr * sizeof(char));
            returnBufferPtr += wordPtr;

            if (sd->ptr + wordPtr >= sd->size) {
                sd->bufferFunction.fill(sd);
                wordPtr = 0;
            }

            if (S_eof(sd) || sd->buffer[sd->ptr + wordPtr] <= ' '){
                returnBuffer[returnBufferPtr] = 0;
                sd->ptr += wordPtr + 1;

                return returnBuffer;
            }
        }
    }
}

char *S_readUntil(STREAM *sd, char delimiter) {
    size_t wordPtr, returnBufferPtr, returnBufferSize;
    char *returnBuffer;

    returnBuffer = (char*)malloc(sizeof(char) * S_BUFFERSIZE);
    returnBufferPtr = 0;
    returnBufferSize = S_BUFFERSIZE;

    /* FIXME: This loop could _also_ use a rework */
    for (wordPtr = 0;;wordPtr++) {

        /* If our pointer plus wordPtr equals the size of the buffer, we need
         * to copy the current word we have been reading to our return buffer
         * are refill the stream buffer. Or if we hit a space, we need to copy
         * the word into the buffer and quit. */
        if (sd->ptr + wordPtr >= sd->size || sd->buffer[sd->ptr + wordPtr] == delimiter) {
            /* Increase the size of our return buffer if needed */
            if (returnBufferPtr + wordPtr >= returnBufferSize) {
                returnBufferSize += wordPtr + 1;
                returnBuffer = realloc(returnBuffer, returnBufferSize * sizeof(char));

            }

            memcpy(&returnBuffer[returnBufferPtr], &sd->buffer[sd->ptr], wordPtr * sizeof(char));
            returnBufferPtr += wordPtr;

            if (sd->ptr + wordPtr >= sd->size) {
                sd->bufferFunction.fill(sd);
                wordPtr = 0;
            }

            if (S_eof(sd) || sd->buffer[sd->ptr + wordPtr] == delimiter){
                returnBuffer[returnBufferPtr] = 0;
                sd->ptr += wordPtr + 1;

                return returnBuffer;
            }
        }
    }
}

char *S_readLine(ISTREAM *sd) {
    char *word;
    size_t len;

    word = S_readUntil(sd, '\n');
    len = strlen(word);

    /* Truncate \r from the end of the string, this will happen on Windows
     * systems and various network protocols, which will use the line ending
     * \r\n instead of just \n which most programmers expect. */
    if (word[len - 1] == '\r') {
        word[len - 1] = 0;
    }

    return word;
}