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
##  cursor.c

File Description:
##  Hanle the cursor movement
\**************************************************************/

#include "editor.h"     // editor_t type
#include "error.h"      // error handling
#include <ncurses.h>    // ncurses function

/* Handle cursor function
----------------------------------------------------------------
 * Handle the arrow direction to change the cursor position x/y
----------------------------------------------------------------
##  data -> main data structure
##  ch -> the key code
----------------------------------------------------------------
*/
int keys_cursor(editor_t *data, const int ch)
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
