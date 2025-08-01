/**************************************************************\

 ██╗  ██╗ █████╗ ██████╗ ████████╗ █████╗ ███╗   ██╗██╗ █████╗ 
 ╚██╗██╔╝██╔══██╗██╔══██╗╚══██╔══╝██╔══██╗████╗  ██║██║██╔══██╗
  ╚███╔╝ ███████║██████╔╝   ██║   ███████║██╔██╗ ██║██║███████║
  ██╔██╗ ██╔══██║██╔══██╗   ██║   ██╔══██║██║╚██╗██║██║██╔══██║
 ██╔╝ ██╗██║  ██║██║  ██║   ██║   ██║  ██║██║ ╚████║██║██║  ██║
 ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝╚═╝  ╚═╝

Edition:
##  01/08/2025 by Tsukini

File Name:
##  init_global.c

File Description:
## Initialisation of global var
\**************************************************************/

#include "editor.h"     // editor_t type
#include "error.h"      // error handling
#include <stdlib.h>     // malloc function
#include <stdbool.h>    // bool type

/* Global initialisation function
----------------------------------------------------------------
 * Initialisation of var that will be used in every file
----------------------------------------------------------------
##  data -> main data structure
----------------------------------------------------------------
*/
int init_global(editor_t *data)
{
    // Check for potential null pointer
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);

    // Init array for File information
    data->files = new_array();
    if (!data->files)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return OK;
}
