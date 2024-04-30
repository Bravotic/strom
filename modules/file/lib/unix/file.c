#include <strom/file.h>
#include <strom/stream.h>
#include <unistd.h>
#include <fcntl.h>

void S_fileRefillBuffer(ISTREAM *sd) {
    size_t len;
    int fd;

    fd = sd->handle.integer;

    len = read(fd, sd->buffer, S_BUFFERSIZE);

    /* If we have read zero bytes, we can assume we hit end of file */
    if (len == 0) {
        sd->flags |= S_EOF;
    }
    else {
        sd->ptr = 0;
        sd->size = len;
    }

}

ISTREAM *S_openFileInput(const char *path, const char *mode) {
    ISTREAM *sd;
    int fd;

    /* In POSIX/UNIX systems, readBinary and read are the same operation */
    if (mode[0] == 'r' && (mode[1] == 0 || (mode[1] == 'b' && mode[2] == 0))) {
        fd = open(path, O_RDONLY);

        /* There was an error opening the file */
        if (fd < 0) {
            return NULL;
        }

        sd = S_createStream();
        sd->handle.integer = fd;
        sd->bufferFunction.fill = S_fileRefillBuffer;

        sd->flags |= S_READ;

        return sd;
    }

    /* This mode cannot be used to open this file */
    else {
        return NULL;
    }

}

void S_closeFileInput(ISTREAM *sd) {
    close(sd->handle.integer);
    S_destroyStream(sd);
}
