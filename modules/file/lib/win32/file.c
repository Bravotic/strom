#include <strom/file.h>
#include <strom/stream.h>
#include <windows.h>

void S_fileRefillBuffer(ISTREAM *sd) {
    DWORD len;
    HANDLE file;

    file = sd->handle.pointer;

    /* Initialize our length to -1 just in case it isn't set*/
    /* TODO: Is this needed? */
    len = -1;

    /* If our read returns FALSE, or if our length is zero, we are probably at 
     * end of file */
    if (!ReadFile(file, sd->buffer, S_BUFFERSIZE, &len, NULL) || len == 0) {
        sd->flags |= S_EOF;
    }
    else {
        sd->ptr = 0;
        sd->size = len;
    }

}

ISTREAM *S_openFileInput(const char *path, const char *mode) {
    ISTREAM *sd;
    HANDLE file;

    /* In POSIX/UNIX systems, readBinary and read are the same operation */
    if (mode[0] == 'r' && (mode[1] == 0 || (mode[1] == 'b' && mode[2] == 0))) {

        /* Open file as generic read */
        /* TODO: Should overlap be used? */
        file = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, 
          OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

        /* There was an error opening the file */
        if (file == INVALID_HANDLE_VALUE) {
            return NULL;
        }

        sd = S_createStream();
        sd->handle.pointer = file;
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
    CloseHandle(sd->handle.pointer);
    S_destroyStream(sd);
}
