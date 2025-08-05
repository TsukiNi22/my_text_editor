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
##  edit_content.c

File Description:
##  Handle the edit of the content
\**************************************************************/

#include "array.h"      // insert_array function
#include "editor.h"     // editor_t type
#include "error.h"      // error handling
#include <ncurses.h>    // ncurses function
#include <wchar.h>      // wchar_t type, wclen function
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

// insert a new line on the cursor
static int content_del_line(editor_t *data, const int ch)
{
    wchar_t *line[2] = {NULL};
    wchar_t *new_line = NULL;
    size_t len[2] = {0};
    
    // Check for potential null pointer
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);

    // setup of the line 0 & 1
    if (ch == KEY_BACKSPACE) {
        line[0] = data->file_lines->data[data->cursor_row - 1];
        line[1] = data->file_lines->data[data->cursor_row];
    } else {
        line[0] = data->file_lines->data[data->cursor_row];
        line[1] = data->file_lines->data[data->cursor_row + 1];
    }

    // init of the new_line
    len[0] = wcslen(line[0]);
    len[1] = wcslen(line[1]);
    new_line = malloc(sizeof(wchar_t) * (len[0] + len[1] + 1));
    if (!new_line)
        return err_prog(MALLOC_ERR, KO, ERR_INFO);
        
    // setup the new_line
    wcscpy(new_line, line[0]);
    wcscat(new_line, line[1]);
    
    if (ch == KEY_BACKSPACE) {
        // setup of the var
        if (pop_array(data->file_lines, &free_ptr, data->cursor_row) == KO
            || pop_array(data->file_lines, &free_ptr, data->cursor_row - 1) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        if (insert_array(data->file_lines, new_line, data->cursor_row - 1) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        data->cursor_row--;
        data->cursor_actual_col = len[0];
        data->cursor_col = len[0];
    } else {
        // setup of the var
        if (pop_array(data->file_lines, &free_ptr, data->cursor_row + 1) == KO
            || pop_array(data->file_lines, &free_ptr, data->cursor_row) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        if (insert_array(data->file_lines, new_line, data->cursor_row) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        data->cursor_actual_col = len[0];
        data->cursor_col = len[0];
    }
    return OK;
}

// supr the char on the cursor from the content
static int content_del(editor_t *data, const int ch)
{
    wchar_t *line = NULL;
    size_t len = 0;

    // Check for potential null pointer
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);

    if (ch == KEY_BACKSPACE)
        data->cursor_col--;
    line = data->file_lines->data[data->cursor_row];
    len = wcslen(line);
    for (size_t i = data->cursor_col; i < len; i++)
        line[i] = line[i + 1];
    return OK;
}

// insert a new line on the cursor
static int content_new_line(editor_t *data)
{
    wchar_t *line, *new_line = NULL;
    size_t len = 0;
    
    // Check for potential null pointer
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    
    // init of the new_line
    line = data->file_lines->data[data->cursor_row];
    len = wcslen(&(line[data->cursor_col]));
    new_line = malloc(sizeof(wchar_t) * (len + 1));
    if (!new_line)
        return err_prog(MALLOC_ERR, KO, ERR_INFO);
    
    // setup the new_line
    for (size_t i = 0; i < len; i++)
        new_line[i] = line[i + data->cursor_col];
    new_line[len] = L'\0';
    
    // setup of the var
    line[data->cursor_col] = L'\0';
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
    wchar_t *line, *new_line = NULL;
    size_t i, len = 0;

    // Check for potential null pointer
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);

    // init of the new_line
    line = data->file_lines->data[data->cursor_row];
    len = wcslen(line);
    new_line = malloc(sizeof(wchar_t) * (len + 2));
    if (!new_line)
        return err_prog(MALLOC_ERR, KO, ERR_INFO);
    
    // setup of the new line
    for (i = 0; i < data->cursor_col && i < len; i++)
        new_line[i] = line[i];
    new_line[i++] = (wchar_t) ch;
    for (; i < len + 1; i++)
        new_line[i] = line[i - 1];
    new_line[len + 1] = L'\0';

    // set of the ptr
    free(line);
    data->file_lines->data[data->cursor_row] = new_line;
    data->cursor_col++;
    return OK;
}

/* Handle key pressed function
----------------------------------------------------------------
 * Dispatch the key to edit the content from the key pressed
----------------------------------------------------------------
##  data -> main data structure
##  ch -> the key code
----------------------------------------------------------------
*/
int keys_edit(editor_t *data, const int ch)
{
    wchar_t *line = NULL;
    size_t len = 0;

    // Check for potential null pointer
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
  
    // supr of a char
    if (ch == KEY_DC || ch == 127 || ch == KEY_BACKSPACE) {
        line = data->file_lines->data[data->cursor_row];
        len = wcslen(line);
        if ((data->cursor_row > 0 && data->cursor_col == 0 && ch == KEY_BACKSPACE)
            || (data->cursor_row < data->file_lines->len - 1 && data->cursor_col == len && (ch == KEY_DC || ch == 127))) {
            if (content_del_line(data, ch) == KO)
                return err_prog(UNDEF_ERR, KO, ERR_INFO);
        } else if (!(data->cursor_col == 0 && ch == KEY_BACKSPACE) && content_del(data, ch) == KO)
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
