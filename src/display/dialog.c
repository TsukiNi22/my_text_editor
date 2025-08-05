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
##  dialog.c

File Description:
##  Get an anwser from the user
\**************************************************************/

#include "memory.h"     // my_malloc_c function
#include "editor.h"     // editor_t type, get_file function
#include "error.h"      // error handling
#include <ncurses.h>    // ncurses function
#include <stddef.h>     // size_t type, NULL define

// handle the key pressed (simplified version)
static int handle_keys_simplified(editor_t *data, char *anwser, const size_t size, const int ch)
{
    size_t len = 0;
    
    // Check for potential null pointer
    if (!data || !anwser)
        return err_prog(PTR_ERR, KO, ERR_INFO);

    // setup var
    for (len = 0; anwser[len]; len++);
    
    // movement cursor x
    if (ch == KEY_LEFT && data->cursor_dialog_col > 0)
        data->cursor_dialog_col--;
    else if (ch == KEY_RIGHT && data->cursor_dialog_col < len && data->cursor_dialog_col < size)
        data->cursor_dialog_col++;

    // del char
    if (ch == KEY_DC || ch == 127 || (ch == KEY_BACKSPACE && data->cursor_dialog_col > 0)) {
        if (ch == KEY_BACKSPACE)
            data->cursor_dialog_col--;
        for (size_t i = data->cursor_dialog_col; i < len; i++)
            anwser[i] = anwser[i + 1];
    }

    // add char
    if (ch >= 32 && ch <= 126 && len < size) {
        for (size_t i = len; i > data->cursor_dialog_col; i--)
            anwser[i] = anwser[i - 1];
        anwser[data->cursor_dialog_col++] = ch;
    }
    return OK;
}



// Udpate the display of the dialog
static int display(editor_t *data, char *anwser, const char *question)
{
    // Check for potential null pointer
    if (!data || !anwser || !question)
        return err_prog(PTR_ERR, KO, ERR_INFO);

    // reset the line
    bkgdset(COLOR_PAIR(3) | A_BOLD);
    move(LINES - 2, 0);
    clrtoeol();

    // print the content
    mvprintw(LINES - 2, 0, "%s: %s", question, anwser);
    return OK;
}

/* Dialog Function
----------------------------------------------------------------
 * Wait for an anwser of the user
----------------------------------------------------------------
##  data -> main data structure
----------------------------------------------------------------
##  return -> user anwser
----------------------------------------------------------------
*/
char *dialog(editor_t *data, const char *question, size_t size)
{
    char *anwser = NULL;
    size_t len = 0;

    // Check for potential null pointer
    if (!data || !question)
        return err_prog_n(PTR_ERR, ERR_INFO);
    
    // initialisation
    for (len = 0; question[len]; len++);
    data->cursor_dialog_col = 0;
    curs_set(1);

    // init the color
    init_pair(3, COLOR_BLACK, COLOR_WHITE);
    
    // setup the anwser
    if (size == (size_t) KO)
        size = MAX_ANWSER_SIZE;
    if (my_malloc_c(&anwser, size) == KO)
        return err_prog_n(MALLOC_ERR, ERR_INFO);

    // loop until validation
    for (int ch = KO; ch != KEY_ENTER && ch != 10; ch = getch()) {
        // handle key pressed
        if (ch != KO && handle_keys_simplified(data, anwser, size, ch) == KO)
            return err_prog_n(UNDEF_ERR, ERR_INFO);

        // udpate the display
        if (display(data, anwser, question) == KO)
            return err_prog_n(UNDEF_ERR, ERR_INFO);

        // update cursor position
        move(LINES - 2, len + 2 + data->cursor_dialog_col);
    }

    // reset the style
    bkgdset(A_NORMAL);
    return anwser;
}
