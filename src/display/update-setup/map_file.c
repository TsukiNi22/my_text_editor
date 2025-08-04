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
##  map_file.c

File Description:
##  Transforme the file content to sperated line
\**************************************************************/

#include "array.h"      // array functions
#include "memory.h"     // my_malloc_c function
#include "editor.h"     // editor_t type, display defines
#include "error.h"      // error handling
#include <stdlib.h>     // free function
#include <stddef.h>     // size_t type, NULL define

/* Get file lines content function
----------------------------------------------------------------
 * Change the content of the file to line separate
----------------------------------------------------------------
##  content -> content of the file
----------------------------------------------------------------
##  return -> the line with theire content separate
*/
array_t *get_file_lines(char *content)
{
    array_t *file_lines = NULL;
    char *line = NULL;
    char *ptr = NULL;

    // Check for potential null pointer
    if (!content)
        return err_prog_n(PTR_ERR, ERR_INFO);

    // malloc the basic array
    file_lines = new_array();
    if (!file_lines)
        return err_prog_n(UNDEF_ERR, ERR_INFO);

    // separate the content in lines
    ptr = content;
    for (size_t i = 0; content[i]; i++) {
        if (content[i] == '\n' || !content[i + 1]) {
            // create the line
            if (my_malloc_c(&(line), (&(content[i]) - ptr) + 1) == KO)
                return err_prog_n(UNDEF_ERR, ERR_INFO);
            
            // setup the content of the ligne
            content[i] *= !(content[i] == '\n');
            for (size_t j = 0; ptr[j]; j++)
                line[j] = ptr[j];
            if (!content[i])
                content[i] = '\n';

            // add the line to the array
            if (add_array(file_lines, char_to_wchar(line)) == KO)
                return err_prog_n(UNDEF_ERR, ERR_INFO);
            free(line);

            // setup var for the next ligne
            ptr = &(content[i + 1]);
        }
    }

    // empty file initialisation
    if (!content[0]) {
        // add the line to the array
        if (add_array(file_lines, char_to_wchar("")) == KO)
            return err_prog_n(UNDEF_ERR, ERR_INFO);
    }
    return file_lines;
}
