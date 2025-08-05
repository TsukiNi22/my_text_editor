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
##  help.c

File Description:
##  Handle the setup to display the help
\**************************************************************/

#include "array.h"      // insert_array function
#include "editor.h"     // editor_t type
#include "error.h"      // error handling
#include <ncurses.h>    // ncurses function
#include <stdlib.h>     // free function
#include <stddef.h>     // size_t type, NULL define

// Free simple pointer
static int free_ptr(void *ptr)
{
    // Check for potential null pointer
    if (!ptr)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    free(ptr);
    return OK;
}

/* Help setup function
----------------------------------------------------------------
 * Setup the help display for the content to display
----------------------------------------------------------------
##  data -> main data structure
----------------------------------------------------------------
*/
int content_help(editor_t *data)
{
    array_t *tmp_lines = NULL;

    // Check for potential null pointer
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);

    if (data->display_help) {
        // setup the mode
        data->mode = data->mode_old;

        // reset the ancient file content
        if (delete_array(&data->file_lines, &free_ptr) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        data->file_lines = data->help_lines;
        data->help_lines = NULL;
    } else {
        // setup the mode
        data->mode_old = data->mode;
        data->mode = NONE;
        
        // replace the file content
        data->help_lines = get_help_lines();
        if (!data->help_lines)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        tmp_lines = data->file_lines;
        data->file_lines = data->help_lines;
        data->help_lines = tmp_lines;
    }

    // udpate the status
    data->display_help = !data->display_help;
    return OK;
}
