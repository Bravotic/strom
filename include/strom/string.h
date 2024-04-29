#ifndef CSTREAMS_STRING_H
#define CSTREAMS_STRING_H 1

#include <strom/stream.h>

/*
 * Opens an input stream using a string as its source of data. Note that the
 * string is read only, meaning it will not be able to be written to (note the
 * word input in openStringInput). This function is suitable to be used with
 * primitive constant strings.
 *
 * Arguments:
 *  string - the string to use for data
 *
 * Returns: An input (read only) stream using the string as its source of data.
 */
extern ISTREAM *S_openStringInput(const char *string);

/*
 * Closes a stream from a string. Please note that this function does NOT free
 * the string at all. If the string was allocated, it must be freed normally.
 *
 * Arguments:
 *  sd - The input stream to free
 */
extern void S_closeStringInput(ISTREAM *sd);

#endif