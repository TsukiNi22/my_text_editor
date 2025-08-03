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
##  handle_keys.c

File Description:
## Handle the key pressed during the display
\**************************************************************/

#include "editor.h"     // editor_t type
#include "error.h"      // error handling
#include <ncurses.h>    // ncurses function

// change the cursor position x/y
static int keys_cursor(editor_t *data, const int ch)
{
    // Check for potential null pointer
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
  
    // cursor movement y
    if (ch == KEY_UP && data->cursor_row > 0)
        data->cursor_row--;
    else if (ch == KEY_DOWN)
        data->cursor_row++;
    if (ch == KEY_UP || ch == KEY_DOWN)
        data->cursor_col = data->cursor_actual_col;
    
    // cursor movement x
    if (ch == KEY_LEFT && data->cursor_col > 0)
        data->cursor_col--;
    else if (ch == KEY_RIGHT)
        data->cursor_col++;
    if (ch == KEY_LEFT || ch == KEY_RIGHT)
        data->cursor_actual_col = data->cursor_col;
    return OK;
}

/* Handle key pressed function
----------------------------------------------------------------
 * Handle the key pressed during the display of a File
 * and do/call the corresponding action
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
        return err_prog(PTR_ERR, KO, ERR_INFO);
    return OK;
}
