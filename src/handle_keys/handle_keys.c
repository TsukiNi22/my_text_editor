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
##  handle_keys.c

File Description:
##  Dispatch the key action
\**************************************************************/

#include "editor.h"     // editor_t type
#include "error.h"      // error handling
#include <ncurses.h>    // ncurses function

/* Handle key pressed function
----------------------------------------------------------------
 * Handle the key pressed during the display of a File
 * and call the corresponding action
----------------------------------------------------------------
##  data -> main data structure
##  ch -> the key code
----------------------------------------------------------------
*/
int handle_keys(editor_t *data, const int ch)
{
    // Check for potential null pointer
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);

    // cursor movement
    if (keys_cursor(data, ch) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);

    // edit content
    if (data->mode == WRITE && keys_edit(data, ch) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);

    // display help
    if (ch == KEY_F(5) && content_help(data) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);

    // mode changement
    if (!data->display_help && changement_mode(data, ch) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    
    if (data->mode != NONE && data->changed && ch == KEY_F(2) && save_content(data) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return OK;
}
