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
#include "array.h"      // insert_array function
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

// supr the char on the cursor from the content
static int content_del(editor_t *data, const int ch)
{
    char *line = NULL;
    size_t i, len = 0;

    // Check for potential null pointer
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);

    if (ch == KEY_BACKSPACE)
        data->cursor_col--;
    line = data->file_lines->data[data->cursor_row];
    for (len = 0; line[len]; len++);
    for (i = data->cursor_col; i < len; i++)
        line[i] = line[i + 1];
    return OK;
}

// insert a new line on the cursor
static int content_new_line(editor_t *data)
{
    char *line, *new_line = NULL;
    size_t len = 0;
    
    // Check for potential null pointer
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    
    // init of the new_line
    line = data->file_lines->data[data->cursor_row];
    for (size_t i = data->cursor_col; line[i]; i++, len++);
    if (my_malloc_c(&new_line, len + 1) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    
    // setup the new_line
    for (size_t i = 0; i < len; i++)
        new_line[i] = line[i + data->cursor_col];
    
    // setup of the var
    line[data->cursor_col] = '\0';
    if (insert_array(data->file_lines, new_line, data->cursor_row + 1) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    data->cursor_row++;
    data->cursor_actual_col = 0;
    data->cursor_col = 0;
    return OK;
}

// add the char pressed to the content
static int content_add(editor_t *data, const int ch)
{
    char *line, *new_line = NULL;
    size_t i, len = 0;

    // Check for potential null pointer
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);

    // init of the new_line
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
    data->cursor_col++;
    return OK;
}

// edit the content from the key pressed
static int keys_edit(editor_t *data, const int ch)
{
    // Check for potential null pointer
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
  
    // supr of a char
    if (ch == KEY_DC || ch == 127 || (ch == KEY_BACKSPACE && data->cursor_col > 0)) {
        if (content_del(data, ch) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }

    // new line
    if (ch == KEY_ENTER || ch == 10) {
        if (content_new_line(data) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }

    // add of a char
    if (ch >= 32 && ch <= 126) {
        if (content_add(data, ch) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
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
