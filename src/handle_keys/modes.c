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
##  modes.c

File Description:
##  Handle the changement of mode
\**************************************************************/

#include "editor.h"     // editor_t type
#include "error.h"      // error handling
#include <ncurses.h>    // ncurses function

/* Mode changement function
----------------------------------------------------------------
 * Change the mode from the key pressed
----------------------------------------------------------------
##  data -> main data structure
##  ch -> the key code
----------------------------------------------------------------
*/
int changement_mode(editor_t *data, const int ch)
{
    file_mode_t tmp_mode = NONE;

    // Check for potential null pointer
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    
    if (ch == KEY_F(3)) {
        data->mode_old = data->mode;
        if (data->mode == WRITE)
            data->mode = READ;
        else
            data->mode = WRITE;
    } else if (ch == KEY_F(4)) {
        data->mode_old = data->mode;
        data->mode = EXE;
    } else if (ch == 27) { // ESC
        data->mode_old = data->mode;
        data->mode = SELECT;
    } else if (ch == KEY_F(6)) {
        tmp_mode = data->mode;
        data->mode = data->mode_old;
        data->mode_old = tmp_mode;
    }

    return OK;
}
