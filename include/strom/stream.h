#ifndef XPSTREAM_STREAM_H
#define XPSTREAM_STREAM_H 1

#include <stddef.h>

#define S_READ      1
#define S_WRITE     2
#define S_EOF       4

#ifndef S_BUFFERSIZE
#define S_BUFFERSIZE 64
#endif

/*
 * The native handle allows us to reference a native data structure which
 * represents where our data in the stream is coming from. On some systems, this
 * handle may be an integer (for example UNIX file descriptors), and on others
 * it may be a pointer (for example a libc FILE* structure).
 *
 * This native handle should only be used by the native functions implementing
 * the stream. A generic stream function should NEVER touch the native handle.
 */
typedef union nativehandle_t {
    void *pointer;
    int integer;
} nativehandle_t;


/* Optional typedefs to identify whether a stream is input or output. */

/*
 * An input stream. While it is not mandated (the ISTREAM type is an alias to
 * STREAM which may be input or output), it is assumed that this stream is only
 * for reading. The stream is assumed to be constructed as READ ONLY.
 *
 * See also: STREAM
 */
typedef struct STREAM ISTREAM;

/*
 * An output stream.
 * See also: STREAM
 */
typedef struct STREAM OSTREAM;

typedef union bufferFunction_t {
    void ((*fill)(ISTREAM *sd));
    void ((*flush)(OSTREAM *sd));
} bufferFunction_t;

/*
 * A stream of data, this may come from a file, string, socket, or anything.
 *
 * Please note that you should not modify this struct directly. Please use the
 * associated functions.
 */
typedef struct STREAM {

    /* Buffer, for either input or output */
    char *buffer;
    size_t ptr;
    size_t size;

    /*
     * A union of semi-private functions for refilling or flushing the buffer
     * depending on whether the stream is an input or output stream. Both the
     * fill and flush functions have the same design recipe, so the union is
     * purely for readability.
     *
     * This function is assumed to be set by the native stream creation
     * function.
     */
    bufferFunction_t bufferFunction;

    /*
     * Holds the native information used to interact with this stream. This may
     * be a file descriptor, socket, or otherwise.
     *
     * Note: The only time the native handle should be accessed is by specific
     * native functions, for example the buffer fill native function. The handle
     * should NEVER be used in a generic stream function.
     */
    nativehandle_t handle;

    unsigned char flags;
} STREAM;

/*
 * Create a new blank stream. This function is semi-private. It is desigend to
 * be used by libraries which implement streams. If you are trying to create a
 * stream to use, for instance, a file stream, please use their specific "open"
 * functions. This function will only create a blank, and ultimately useless
 * stream.
 *
 * Returns: A newly allocated stream.
 */
STREAM *S_createStream(void);

/*
 * Semi-private function to destroy a stream. it ONLY touches the stream itself,
 * and NOT the underlying native handle. If you want to completely destroy a
 * stream, please use the function from whatever implements the stream, for
 * example if you opened a file with S_openFileInput, close it with
 * S_closeFileInput.
 *
 * Make sure to use the exact function, if you open a stream that has both read
 * and write stream components, and close just the read with the wrong function,
 * it may destroy the write native handle as well.
 *
 * Arguments:
 *  sd - A stream which is to be destroyed.
 */
void S_destroyStream(STREAM *sd);

/*
 * Works exactly like the standard library fgetc() or the posix getch(). Gets a
 * single character from the stream. Returns -1 on error.
 *
 * This function can only be called on Input Streams.
 *
 * Arguments:
 *  sd - The input stream to read the character from.
 *
 * Returns: A character read from the stream, or -1 on error.
 */
extern int S_getch(ISTREAM *sd);

/*
 * Pushes a character back into the buffer. This can be used to "unread" a
 * character, or to push new data onto the buffer.
 *
 * This function can only be called on Input Streams.
 *
 * Arguments:
 *  sd - The input stream to push the character onto the buffer of.
 *  ch - The character to push onto the buffer, doesn't have to have been read
 *       from the stream.
 *
 * Returns: The character pushed onto the buffer, or -1 on error.
 */
extern int S_ungetch(ISTREAM *sd, int ch);

/*
 * Returns true (1) if the stream is at the end of its file, or whatever it is
 * reading. EOF is simply used here for the familiarity of the term, it does not
 * need to be read from a file to produce an EOF. For an EOF to happen, the
 * read buffer needs to be completely empty, and when asked for more data, the
 * native stream needs to not have anymore.
 *
 * Arguments:
 *  sd - The input stream to check if EOF is reached.
 *
 * Returns: 1 if at end of file, 0 if not.
 */
extern int S_eof(ISTREAM *sd);

/*
 * Functions identically to the POSIX read function, or the fread function from
 * the C standard library. Reads 'count' of data of size 'size' into the buffer.
 * This can be used to read characters, as well as other sized data such as
 * shorts or integers.
 *
 * Arguments:
 *  buffer - the buffer (of unspecified type) to read the data to.
 *  size   - the size of the data to read. Please use sizeof(datatype).
 *  count  - the amount of data to copy. Think "five" in "five bytes".
 *  sd     - the input stream to read from.
 *
 * Returns: The size of the data read into the buffer. A size of < count may or
 *          may not indicate EOF.
 */
extern size_t S_read(void *buffer, size_t size, size_t count, STREAM *sd);

/*
 * Reads a word from the stream. This function finds a word as any text
 * delineated by a whitespace character, that is a character ascii 32 and below.
 *
 * Note: the text returned is allocated by the function and therefore MUST be
 * freed manually using the free() function.
 *
 * Arguments:
 *  sd - The stream to read from
 *
 * Returns: The word read from the stream
 */
extern char *S_readWord(ISTREAM *sd);

/*
 * Reads from the stream until a specific character is found. This character is
 * not included in the output of the function, and is also no longer present in
 * the buffer.
 *
 * Note: The text returned is allocated by this function and therefore MUST be
 * freed manually using the free() function.
 *
 * Arguments:
 *  sd          - The stream to read from
 *  delimiter   - The delimiter to stop reading at
 *
 * Returns: The text until the delimiter
 */
extern char *S_readUntil(STREAM *sd, char delimiter);

/*
 * Reads a line from the stream. Calls S_readUntil with the delimiter being \n.
 * This function also truncates \r from the end of the line, that way \r\n line
 * endings are properly handled.
 *
 * Note: The text returned is allocated by this function and therefore MUST be
 * freed manually using the free() function.
 *
 * Arguments:
 *  sd - the stream to read from
 *
 * Returns: A single line of input
 */
extern char *S_readLine(STREAM *sd);

#endif