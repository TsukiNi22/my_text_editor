/**************************************************************\

 ██╗  ██╗ █████╗ ██████╗ ████████╗ █████╗ ███╗   ██╗██╗ █████╗ 
 ╚██╗██╔╝██╔══██╗██╔══██╗╚══██╔══╝██╔══██╗████╗  ██║██║██╔══██╗
  ╚███╔╝ ███████║██████╔╝   ██║   ███████║██╔██╗ ██║██║███████║
  ██╔██╗ ██╔══██║██╔══██╗   ██║   ██╔══██║██║╚██╗██║██║██╔══██║
 ██╔╝ ██╗██║  ██║██║  ██║   ██║   ██║  ██║██║ ╚████║██║██║  ██║
 ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝╚═╝  ╚═╝

Edition:
##  04/08/2025 by Tsukini

File Name:
##  map_file.c

File Description:
##  Transforme the ehlp content to sperated line
\**************************************************************/

#include "array.h"      // array functions
#include "memory.h"     // my_malloc_c function
#include "editor.h"     // editor_t type, display defines
#include "error.h"      // error handling
#include <stdlib.h>     // malloc function
#include <stddef.h>     // size_t type, NULL define

/* Get help lines function
----------------------------------------------------------------
 * Change the content of the help to line separate
----------------------------------------------------------------
##  return -> the line with theire content separate
*/
array_t *get_help_lines(void)
{
    array_t *file_lines = NULL;
    char *line = NULL;
    char *ptr = NULL;

    // malloc the basic array
    file_lines = new_array();
    if (!file_lines)
        return err_prog_n(UNDEF_ERR, ERR_INFO);

    // separate the content in lines
    ptr = help_content;
    for (size_t i = 0; help_content[i]; i++) {
        if (help_content[i] == '\n' || !help_content[i + 1]) {
            // create the line
            if (my_malloc_c(&(line), (&(help_content[i]) - ptr) + 1) == KO)
                return err_prog_n(UNDEF_ERR, ERR_INFO);
            
            // setup the content of the ligne
            help_content[i] *= !(help_content[i] == '\n');
            for (size_t j = 0; ptr[j]; j++)
                line[j] = ptr[j];
            if (!help_content[i])
                help_content[i] = '\n';

            // add the line to the array
            if (add_array(file_lines, line) == KO)
                return err_prog_n(UNDEF_ERR, ERR_INFO);

            // setup var for the next ligne
            ptr = &(help_content[i + 1]);
        }
    }
    return file_lines;
}
