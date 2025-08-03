/**************************************************************\

 ██╗  ██╗ █████╗ ██████╗ ████████╗ █████╗ ███╗   ██╗██╗ █████╗ 
 ╚██╗██╔╝██╔══██╗██╔══██╗╚══██╔══╝██╔══██╗████╗  ██║██║██╔══██╗
  ╚███╔╝ ███████║██████╔╝   ██║   ███████║██╔██╗ ██║██║███████║
  ██╔██╗ ██╔══██║██╔══██╗   ██║   ██╔══██║██║╚██╗██║██║██╔══██║
 ██╔╝ ██╗██║  ██║██║  ██║   ██║   ██║  ██║██║ ╚████║██║██║  ██║
 ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝╚═╝  ╚═╝

Edition:
##  03/08/2025 by Tsukini

File Name:
##  map_file.c

File Description:
##  Transforme the file content to sperated line & size
\**************************************************************/

#include "array.h"      // array functions
#include "memory.h"     // my_malloc_c function
#include "editor.h"     // editor_t type, display defines
#include "error.h"      // error handling
#include <stdlib.h>     // malloc function
#include <stddef.h>     // size_t type, NULL define

/* Map file content function
----------------------------------------------------------------
 * Change the content of the file to line separate and line size
----------------------------------------------------------------
##  content -> content of the file
----------------------------------------------------------------
##  return -> the line with theire size and content separate
*/
array_t *map_file(char *content)
{
    array_t *file_map = NULL;
    line_t *line = NULL;
    char *ptr = NULL;

    // Check for potential null pointer
    if (!content)
        return err_prog_n(PTR_ERR, ERR_INFO);

    // malloc the basic array
    file_map = new_array();
    if (!file_map)
        return err_prog_n(UNDEF_ERR, ERR_INFO);

    // separate the content in lines
    ptr = content;
    for (size_t i = 0; content[i]; i++) {
        if (content[i] == '\n' || !content[i + 1]) {
            // create the line
            line = malloc(sizeof(line_t));
            if (!line)
                return err_prog_n(MALLOC_ERR, ERR_INFO);
            line->len = (&(content[i]) - ptr);
            if (my_malloc_c(&(line->content), line->len + 1) == KO)
                return err_prog_n(UNDEF_ERR, ERR_INFO);
            
            // setup the content of the ligne
            content[i] *= !(content[i] == '\n');
            for (size_t j = 0; ptr[j]; j++)
                line->content[j] = ptr[j];
            if (!content[i])
                content[i] = '\n';

            // add the line to the array
            if (add_array(file_map, line) == KO)
                return err_prog_n(UNDEF_ERR, ERR_INFO);

            // setup var for the next ligne
            ptr = &(content[i + 1]);
        }
    }
    return file_map;
}
