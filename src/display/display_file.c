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
##  display_file.c

File Description:
## Return the content of the given file
\**************************************************************/

#include "my_string.h"              // my_strcmp function
#include "memory.h"                 // my_malloc_c function
#include "editor.h"                 // editor_t type, get_file function
#include "error.h"                  // error handling
#define _XOPEN_SOURCE_EXTENDED 1    // for the extented version
#include <ncursesw/curses.h>        // ncursesw functions
#include <wchar.h>                  // mbstate_t, wchar_t type, mbsrtowcs function
#include <stdlib.h>                 // free, malloc function
#include <stdbool.h>                // bool type
#include <stddef.h>                 // size_t type, NULL define

// Free simple pointer
static int free_ptr(void *ptr)
{
    // Check for potential null pointer
    if (!ptr)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    free(ptr);
    return OK;
}

// display the line in the terminal
static int display_content(editor_t *data, int max_cols, int max_rows)
{
    wchar_t **formated_lines = NULL;
    
    // Check for potential null pointer
    if (!data)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    
    // format the lines for the row & col number
    resize_term(0, 0);
    formated_lines = format_lines(data, max_cols, max_rows);
    if (!formated_lines)
        return err_prog(PTR_ERR, KO, ERR_INFO);

    // clear the screen
    for (int i = 0; i < LINES; i++) {
        move(i, 0);
        clrtoeol();
    }
    
    // display the content
    if (display_top_header(data) == KO || display_bottom_header(data) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    for (int i = 1, row = 0; formated_lines[row] && i < LINES  - 1; i++, row++)
        mvaddwstr(i, 0, formated_lines[row]);
    refresh();

    // free memory alloced
    for (size_t i = 0; formated_lines[i]; i++)
        free(formated_lines[i]);
    free(formated_lines);
    return OK;
}

// init the var for the display
static int init_var(editor_t *data, const char *file)
{    
    // Check for potential null pointer
    if (!data || !file)
        return err_prog(PTR_ERR, KO, ERR_INFO);

    // init global data
    curs_set(1);
    data->mode = WRITE;
    data->mode_old = WRITE;
    data->changed = false;
    data->display_help = false;

    // init file information
    data->file = file;
    if (is_valid_file(data, file, false))
        data->content = get_file(file);
    else if (my_malloc_c(&(data->content), 1) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    data->file_lines = get_file_lines(data->content);
    if (!data->content || !data->file_lines)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    
    // init pos of screen & cursor
    data->screen_row = 0;
    data->screen_col = 0;
    data->cursor_actual_col = 0;
    data->cursor_row = 0;
    data->cursor_col = 0;
    return OK;
}

/* Handle open/close file function
----------------------------------------------------------------
 * Handle the opening & closing of the file in the display
----------------------------------------------------------------
##  data -> main data structure
----------------------------------------------------------------
*/
int display_file(editor_t *data, const char *file)
{
    char *anwser = NULL;
    int max_cols, max_rows = 0;
    bool out = false;
    
    // Check for potential null pointer
    if (!data || !file)
        return err_prog(PTR_ERR, KO, ERR_INFO);

    // init the file value
    if (init_var(data, file) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);

    // wait for F1 to quit
    for (int ch = KO; !out; ch = getch()) {
        max_cols = COLS, max_rows = LINES - 2;
     
        // check if the user forgot to save
        out = (ch == KEY_F(1));
        if (out && data->changed) {
            anwser = dialog(data, "Do you want to leave with unsaved change? (y/n)", KO);
            if (!anwser)
                return err_prog(UNDEF_ERR, KO, ERR_INFO);
            if (my_strcmp(anwser, "N") == 0 || my_strcmp(anwser, "n") == 0
                || my_strcmp(anwser, "No") == 0 || my_strcmp(anwser, "no") == 0
                || my_strcmp(anwser, "Non") == 0 || my_strcmp(anwser, "non") == 0)
                out = false;
            free(anwser);
        }
        if (out)
            break;

        // handle the key pressed
        if (ch != KO && handle_keys(data, ch) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);

        // update the col & row of screen & cursor
        if (update_pos(data, max_cols, max_rows) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        
        // update the display
        if (display_content(data, max_cols, max_rows) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
        move(data->cursor_row - data->screen_row + 1, data->cursor_col - data->screen_col);
    }

    // free memory alloced
    if (delete_array(&data->file_lines, &free_ptr) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    free(data->content);
    return OK;
}
