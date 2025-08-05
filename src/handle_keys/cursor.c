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
#include <wchar.h>      // wchar_t type, wcslen function

// cursor movement on x with ctrl for <- & ->
static int ctrl_movement(editor_t *data, const int ch)
{
    wchar_t *line = NULL;
    size_t len = 0;

    // Check for potential null pointer
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    
    // movement
    line = data->file_lines->data[data->cursor_row];
    if (ch == 554) { // ctrl + <-
        if (data->cursor_col == 0 && data->cursor_row > 0) {
            line = data->file_lines->data[data->cursor_row - 1];
            len = wcslen(line);
            data->cursor_col = len + 1;
            data->cursor_row--;
        } else {
            for (;line[data->cursor_col] != L' ' && data->cursor_col > 0; data->cursor_col--);
            for (;line[data->cursor_col] == L' ' && data->cursor_col > 0; data->cursor_col--);
        }
    } else if (ch == 569) { // ctrl + ->
        len = wcslen(line);
        if (data->cursor_col >= len && data->cursor_row < data->file_lines->len - 1) {
            data->cursor_col = 0;
            data->cursor_row++;
        } else {
            for (;line[data->cursor_col] != L' ' && data->cursor_col < len; data->cursor_col++);
            for (;line[data->cursor_col] == L' ' && data->cursor_col < len; data->cursor_col++);
        }
    }
    return OK;
}

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

    // cursor movement x + ctrl
    if (ctrl_movement(data, ch) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    
    // update the remanent col
    if (ch == KEY_LEFT || ch == KEY_RIGHT || ch == 554 || ch == 569)
        data->cursor_actual_col = data->cursor_col;
    return OK;
}
