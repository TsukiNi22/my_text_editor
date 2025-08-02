/**************************************************************\

 ██╗  ██╗ █████╗ ██████╗ ████████╗ █████╗ ███╗   ██╗██╗ █████╗ 
 ╚██╗██╔╝██╔══██╗██╔══██╗╚══██╔══╝██╔══██╗████╗  ██║██║██╔══██╗
  ╚███╔╝ ███████║██████╔╝   ██║   ███████║██╔██╗ ██║██║███████║
  ██╔██╗ ██╔══██║██╔══██╗   ██║   ██╔══██║██║╚██╗██║██║██╔══██║
 ██╔╝ ██╗██║  ██║██║  ██║   ██║   ██║  ██║██║ ╚████║██║██║  ██║
 ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝╚═╝  ╚═╝

Edition:
##  02/08/2025 by Tsukini

File Name:
##  kamion.c

File Description:
## Main file of the editor,
## init all the uninitialized data and check the falg '-' | '--'
## and call each part of the editor
\**************************************************************/

#include "memory.h"     // my_strdup function
#include "array.h"      // array_t type
#include "editor.h"     // editor_t type
#include "error.h"      // error handling
#include <ncurses.h>    // ncurses function

// Add the file given directly to the list
static int direct_file(editor_t *data, int const argc, char const *argv[])
{
    // Check for potential null pointer
    if (!data || !argv)
        return err_prog(PTR_ERR, KO, ERR_INFO);

    for (int i = 1; i < argc; i++) {
        // End of the normal file definition
        if (argv[i][0] == '-')
            break;

        // Check if it's a file or a directory
        if (!is_valid_file(data, argv[i], false))
            return err_bw_arg(data, KO, "Argument", "Invalid file given", argv[i], NULL, false);

        // Set the file in the array
        if (add_array(data->files, my_strdup((char *) argv[i])) == KO)
            return err_prog(UNDEF_ERR, KO, ERR_INFO);
    }
    return OK;
}

/* Main editor function
----------------------------------------------------------------
 * Init uninitialized data in the main structure,
 * setup the option and other thing related to the flags
 * Call each of the part of the editor and check the return
----------------------------------------------------------------
##  argc -> number of argument given to the binary
##  argv -> arguments given to the binary
##  data -> main data structure
----------------------------------------------------------------
*/
int editor(int const argc, char const *argv[], editor_t *data)
{
    // Check for potential null pointer
    if (!data || !argv)
        return err_prog(PTR_ERR, KO, ERR_INFO);
    
    // Init the main structure & given arguments
    if (init_data(data) == KO)
        return err_custom("Data initialisation error", FATAL_ERR, ERR_INFO);
    if (init_flag(data, argc, argv) == KO)
        return err_custom("Flag initialisation error", KO, ERR_INFO);
    if (direct_file(data, argc, argv) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);

    // Exit in case of a '-h' or '--help'
    if (data->help)
        return OK;

    // Execption
    if (argc > 1 && data->files->len == 0)
        return err_bw_arg(data, KO, "File", "No valid file found for the edition", NULL, NULL, false);

    // Temporary test display
    int ch;
    int row = 1;
    mvprintw(0, 0, "F1 to quit...");
    while ((ch = getch()) != KEY_F(1)) {
        mvprintw(row, 0, "Key pressed: ");
        if (ch >= 32 && ch <= 126)
            printw("'%c' (code %d)", ch, ch);
        else
            printw("code %d", ch);
        row++;
        if (row >= LINES) {
            row = 1;
            clear();
            mvprintw(0, 0, "F1 to quit...");
        }
        refresh();
    }

    return OK;
}
