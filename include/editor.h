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
##  editor.h

File Description:
## The main header of the banana writer editor
\**************************************************************/

#ifndef EDITOR_H
    #define EDITOR_H

    //----------------------------------------------------------------//
    /* INCLUDE */

    /* type */
    #include "array.h"      // array_t type
    #include <wchar.h>      // wchar_t type
    #include <stddef.h>     // size_t type
    #include <stdbool.h>    // boolean type

    //----------------------------------------------------------------//
    /* DEFINE */

    /* display */
    #define CURSOR_ROW_FROM_BORDER 2
    #define CURSOR_COL_FROM_BORDER 5

//----------------------------------------------------------------//
/* TYPEDEF */

/* mode */
typedef enum file_mode_e {
    NONE = 0,
    READ,       
    WRITE,
    SELECT,
    EXE,
} file_mode_t;

/* data */
typedef struct editor_s {
    /* global_info */
    int argc;
    char const **argv;
    char const *exe_name;

    /* argument information */
    array_t *files;
 
    /* global file data */
    file_mode_t mode;
    file_mode_t mode_old;
    const char *file;
    char *content;
    array_t *file_lines;
    
    /* help data */
    bool display_help;
    array_t *help_lines;

    /* screen */
    size_t screen_row;
    size_t screen_col;

    /* cursor */
    size_t cursor_actual_col;
    size_t cursor_row;
    size_t cursor_col;

    /* ouput */
    bool help;
    bool err_sys;
    size_t nb_error;
    size_t nb_warning;
} editor_t;

//----------------------------------------------------------------//
/* PROTOTYPE */

/* kamion */
int editor(int const argc, char const *argv[], editor_t *data); // Error: KO

/* init_data */
int init_data(editor_t *data); // Error: KO
int init_global(editor_t *data); // Error: KO
int init_ncurses(void); // Error: None

/* init_flag */
int init_flag(editor_t *data, int const argc, char const *argv[]); // Error: KO
int flag_help(void); // Error: KO
int flag_null(editor_t *data, int const argc, char const *argv[]); // Error: KO
int flag_directory(editor_t *data, int const argc, char const *argv[]); // Error: KO
int flag_Directory(editor_t *data, int const argc, char const *argv[]); // Error: KO

/* useful */
bool is_valid_dir(editor_t *data, char const *path, bool err); // Error: false
bool is_valid_file(editor_t *data, char const *path, bool err); // Error: false
char *get_file(const char *file); // Error: NULL
wchar_t *char_to_wchar(const char *line); // Error: NULL

/* display */
int display_file(editor_t *data, const char *file); // Error: KO
int display_top_header(editor_t *data); // Error: KO
int display_bottom_header(editor_t *data); // Error: KO

/* update/setup display */
array_t *get_help_lines(); // Error: NULL
array_t *get_file_lines(char *content); // Error: NULL
int update_pos(editor_t *data, int max_cols, int max_rows); // Error: KO
wchar_t **format_lines(editor_t *data, int max_cols, int max_rows); // Error: NULL

/*handle keys */
int handle_keys(editor_t *data, const int ch); // Error: KO
int keys_cursor(editor_t *data, const int ch); // Error: KO
int keys_edit(editor_t *data, const int ch); // Error: KO
int save_content(editor_t *data); // Error: KO
int changement_mode(editor_t *data, const int ch); // Error: KO
int content_help(editor_t *data); // Error: KO

/* exit */
int free_data(editor_t *data); // Error: KO

//----------------------------------------------------------------//
/* GLOBAL_CONST */

/* flag */
extern char const flags[];
extern char const *full_flags[];
extern int const flags_argc[];
extern int (* const flag_functions[])(editor_t *, int const, char const *[]);

/* help */
extern char help_content[];

/* modes */
extern char const *modes[];

#endif /* EDITOR_H */
