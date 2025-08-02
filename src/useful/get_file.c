/**************************************************************\

 ██╗  ██╗ █████╗ ██████╗ ████████╗ █████╗ ███╗   ██╗██╗ █████╗ 
 ╚██╗██╔╝██╔══██╗██╔══██╗╚══██╔══╝██╔══██╗████╗  ██║██║██╔══██╗
  ╚███╔╝ ███████║██████╔╝   ██║   ███████║██╔██╗ ██║██║███████║
  ██╔██╗ ██╔══██║██╔══██╗   ██║   ██╔══██║██║╚██╗██║██║██╔══██║
 ██╔╝ ██╗██║  ██║██║  ██║   ██║   ██║  ██║██║ ╚████║██║██║  ██║
 ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝╚═╝  ╚═╝

Edition:
##  02/08/2025 by Tsukini

File Name:
##  get_file.c

File Description:
## Return the content of the given file
\**************************************************************/

#include "memory.h" // my_malloc_c function
#include "editor.h" // editor_t type
#include "error.h"  // error handling
#include <stdio.h>  // file edit function
#include <stddef.h> // NULL define

/* Get file content function
----------------------------------------------------------------
 * Get the full file in a char array
----------------------------------------------------------------
##  file -> file path
----------------------------------------------------------------
*/
char *get_file(const char *file)
{
    FILE *fs = NULL;
    char *buff = NULL;
    size_t size = 0;
    size_t res = 0;

    // Check for potential null pointer
    if (!file)
        return err_prog_n(PTR_ERR, ERR_INFO);

    // Set the stream for the file to tokenize
    fs = fopen(file, "r");
    if (!fs)
        return err_prog_n(UNDEF_ERR, ERR_INFO);

    // Get the size
    fseek(fs, 0, SEEK_END);
    size = ftell(fs);
    rewind(fs);

    // Malloc the good size for the whole file
    if (my_malloc_c(&buff, (size * 2) + 2) == KO)
        return err_prog_n(UNDEF_ERR, ERR_INFO);

    // Set the file content
    res = fread(buff, 1, size, fs);
    if (res != size)
        return err_prog_n(UNDEF_ERR, ERR_INFO);

    // close the stream
    fclose(fs);
    return buff;
}
