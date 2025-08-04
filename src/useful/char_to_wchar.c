/**************************************************************\

 ██╗  ██╗ █████╗ ██████╗ ████████╗ █████╗ ███╗   ██╗██╗ █████╗ 
 ╚██╗██╔╝██╔══██╗██╔══██╗╚══██╔══╝██╔══██╗████╗  ██║██║██╔══██╗
  ╚███╔╝ ███████║██████╔╝   ██║   ███████║██╔██╗ ██║██║███████║
  ██╔██╗ ██╔══██║██╔══██╗   ██║   ██╔══██║██║╚██╗██║██║██╔══██║
 ██╔╝ ██╗██║  ██║██║  ██║   ██║   ██║  ██║██║ ╚████║██║██║  ██║
 ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝╚═╝  ╚═╝

Edition:
##  05/08/2025 by Tsukini

File Name:
##  char_to_wchar.c

File Description:
##  Convert a line of char to a line of wchar
\**************************************************************/

#include "error.h"  // error handling
#include <string.h> // memset functions
#include <wchar.h>  // wchar functions
#include <stdlib.h> // malloc function
#include <stddef.h> // NULL define

/* Convert char to wchar function
----------------------------------------------------------------
 * Take a line and return the version of the line in wchar
----------------------------------------------------------------
##  line -> line to convert
----------------------------------------------------------------
*/
wchar_t *char_to_wchar(const char *line)
{
    const char *src = line;
    wchar_t *wline = NULL;
    mbstate_t state = {0};
    size_t len = 0;

    // Check for potential null pointer
    if (!line)
        return err_prog_n(PTR_ERR, ERR_INFO);

    // get the size of the line converted in wchar
    len = mbsrtowcs(NULL, &src, 0, &state);
    if (len == (size_t) KO)
        return err_prog_n(UNDEF_ERR, ERR_INFO);

    // malloc the wline
    wline = malloc(sizeof(wchar_t) * (len + 1));
    if (!wline)
        return err_prog_n(MALLOC_ERR, ERR_INFO);

    // set the wline
    memset(&state, 0, sizeof(state));
    if (mbsrtowcs(wline, &line, len + 1, &state) == (size_t) KO)
        return err_prog_n(UNDEF_ERR, ERR_INFO);
    return wline;
}
