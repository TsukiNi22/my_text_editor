/**************************************************************\

 ██╗  ██╗ █████╗ ██████╗ ████████╗ █████╗ ███╗   ██╗██╗ █████╗ 
 ╚██╗██╔╝██╔══██╗██╔══██╗╚══██╔══╝██╔══██╗████╗  ██║██║██╔══██╗
  ╚███╔╝ ███████║██████╔╝   ██║   ███████║██╔██╗ ██║██║███████║
  ██╔██╗ ██╔══██║██╔══██╗   ██║   ██╔══██║██║╚██╗██║██║██╔══██║
 ██╔╝ ██╗██║  ██║██║  ██║   ██║   ██║  ██║██║ ╚████║██║██║  ██║
 ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝╚═╝  ╚═╝

Edition:
##  01/08/2025 by Tsukini

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
    #include <stddef.h>     // size_t type
    #include <stdbool.h>    // boolean type

//----------------------------------------------------------------//
/* TYPEDEF */

/* data */
typedef struct editor_s {
    /* global_info */
    int argc;
    char const **argv;
    char const *exe_name;

    /* argument information */
    array_t *files;
    
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

/* init_flag */
int init_flag(editor_t *data, int const argc, char const *argv[]); // Error: KO
int flag_help(void); // Error: KO
int flag_null(editor_t *data, int const argc, char const *argv[]); // Error: KO
int flag_directory(editor_t *data, int const argc, char const *argv[]); // Error: KO
int flag_Directory(editor_t *data, int const argc, char const *argv[]); // Error: KO

/* useful */
bool is_valid_dir(editor_t *data, char const *path, bool err); // Error: false
bool is_valid_file(editor_t *data, char const *path, bool err); // Error: false

/* exit */
int free_data(editor_t *data); // Error: KO

//----------------------------------------------------------------//
/* GLOBAL_CONST */

/* flag */
extern char const flags[];
extern char const *full_flags[];
extern int const flags_argc[];
extern int (* const flag_functions[])(editor_t *, int const, char const *[]);

#endif /* EDITOR_H */
