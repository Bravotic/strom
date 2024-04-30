#include <strom/stream.h>

/*
 * Opens a file using the standard way outlined in libc. Please note that modes
 * may change their meaning between systems. Since this backend is system
 * specific, please know what each mode specifically means. For example, UNIX
 * systems completely ignore any of the binary modes.
 *
 * Arguments:
 *  path - the path of the file to open.
 *  mode - the mode of the file to open, must be read only.
 *
 * Returns: an input stream using the opened file as its source of data.
 */
extern ISTREAM *S_openFileInput(const char *path, const char *mode);

/*
 * Closes an open file input stream.
 *
 * Arguments:
 *  sd - The input stream to close
 */
extern void S_closeFileInput(ISTREAM *sd);