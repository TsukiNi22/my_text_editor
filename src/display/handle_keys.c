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

#include "memory.h"     // my_malloc_c function
#include "editor.h"     // editor_t type
#include "error.h"      // error handling
#include <ncurses.h>    // ncurses function
#include <stdlib.h>     // free function
#include <stddef.h>     // size_t type, NULL define

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

// add the char pressed to the content
static int keys_edit(editor_t *data, const int ch)
{
    char *line, *new_line = NULL;
    size_t i, len = 0;

    // Check for potential null pointer
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
  
    if (ch == KEY_DC || ch == KEY_BACKSPACE || ch == 127) {
        line = data->file_lines->data[data->cursor_row];
        for (len = 0; line[len]; len++);
        for (i = data->cursor_col; i < len; i++)
            line[i] = line[i + 1];
    } else if (ch >= 32 && ch <= 126) {
        line = data->file_lines->data[data->cursor_row];
        for (len = 0; line[len]; len++);
        if (my_malloc_c(&new_line, len + 2) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        
        // setup of the new line
        for (i = 0; i < data->cursor_col && i < len; i++)
            new_line[i] = line[i];
        new_line[i++] = ch;
        for (; i < len + 1; i++)
            new_line[i] = line[i - 1];

        // set of the ptr
        free(line);
        data->file_lines->data[data->cursor_row] = new_line;
    }
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

    // edit content
    if (keys_edit(data, ch) == KO)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    return OK;
}
