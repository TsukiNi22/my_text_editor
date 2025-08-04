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
## Handle the key pressed during the display
\**************************************************************/

#include "memory.h"     // my_malloc_c function
#include "array.h"      // insert_array function
#include "editor.h"     // editor_t type
#include "error.h"      // error handling
#include <ncurses.h>    // ncurses function
#include <wchar.h>      // wchar_t type, wclen function
#include <stdlib.h>     // free function
#include <stdio.h>      // file handle functions
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

// edit the content from the key pressed
static int keys_edit(editor_t *data, const int ch)
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

// setup the help display
static int content_help(editor_t *data)
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
        curs_set(1);
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
        curs_set(0);
    }

    // udpate the status
    data->display_help = !data->display_help;
    return OK;
}

// set the mode from the key pressed
static int changement_mode(editor_t *data, const int ch)
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

// save the actual file modified
static int save_content(editor_t *data)
{
    FILE *file = NULL;
    char *file_content = NULL;
    char *line = NULL;
    size_t size, index = 0;

    // Check for potential null pointer
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    
    // get the full size
    for (size_t i = 0; i < data->file_lines->len; i++) {
        line = data->file_lines->data[i];
        for (size_t j = 0; line[j]; j++, size++);
    }
    if (my_malloc_c(&file_content, size + data->file_lines->len + 1) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    
    // setup the content
    for (size_t i = 0; i < data->file_lines->len; i++) {
        line = data->file_lines->data[i];
        for (size_t j = 0; line[j]; j++, index++)
            file_content[index] = line[j];
        if (i + 1 < data->file_lines->len)
            file_content[index++] = '\n';
    }

    // write the file
    file = fopen(data->file, "w");
    if (!file)
        return err_prog(OP_FILE_ERR, KO, ERR_INFO);
    fputs(file_content, file);
    fclose(file);
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
    if (data->mode != NONE && keys_cursor(data, ch) == KO)
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
    
    if (data->mode != NONE && ch == KEY_F(2) && save_content(data) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    return OK;
}
