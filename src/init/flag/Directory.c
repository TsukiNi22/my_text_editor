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
##  Directory.c

File Description:
## Handle the Directory flag option
\**************************************************************/

#include "my_string.h"  // get_fullpath, my_strcmp function
#include "array.h"      // add_array function
#include "editor.h"     // editor_t type
#include "error.h"      // error handling
#include <string.h>     // strerror function
#include <stdio.h>      // dir function
#include <stdlib.h>     // free function
#include <dirent.h>     // dir struct
#include <errno.h>      // errno global var

// Read file recursively in the given dir_name
static int readdir_rec(editor_t *data, char const *dir_name)
{
    struct dirent *entry = NULL;
    DIR *dir = NULL;
    char *path = NULL;

    // Check for potential null pointer
    if (!data || !dir_name)
        return err_prog(PTR_ERR, KO, ERR_INFO);

    // Open the directory to read it's content
    dir = opendir(dir_name);
    if (!dir)
        return err_system(data, KO, dir_name, strerror(errno));

    while ((entry = readdir(dir))) {
        // Set the file in the array
        path = get_full_path(dir_name, entry->d_name);
        if (my_strcmp(entry->d_name, ".") != 0 && my_strcmp(entry->d_name, "..") != 0
            && is_valid_dir(data, path, false)) {
            if (readdir_rec(data, path) == KO)
                return err_prog(UNDEF_ERR, KO, ERR_INFO);
            free(path);
        } else if (is_valid_file(data, path, false)) {
            if (add_array(data->files, path) == KO)
                return err_prog(UNDEF_ERR, KO, ERR_INFO);
        } else
            free(path);
    }
    closedir(dir);
    return OK;
}

/* Directory function
----------------------------------------------------------------
 * That will extract the file in the given directory
 * and use them in the compilation (recursively)
----------------------------------------------------------------
##  data -> main data structure
##  argc -> number of argument given to the binary
##  argv -> arguments given to the binary
----------------------------------------------------------------
*/
int flag_Directory(editor_t *data, int const argc, char const *argv[])
{
    size_t before = 0;

    // Check for potential null pointer
    if (!data || !argv)
        return err_prog(PTR_ERR, KO, ERR_INFO);

    // Check argument
    if (argc < 2 || argv[1][0] == '-')
        return err_bw_arg(data, KO, "Argument", "Insufficient argument, need a 'directory_path'", *argv, "directory_path", false);
    if (!is_valid_dir(data, argv[1], false))
        return err_bw_arg(data, KO, "Argument", "Invalid directory given", argv[1], NULL, false);

    // Start the recursive for obtain file who end with the right extension
    before = data->files->len;
    if (readdir_rec(data, argv[1]) == KO)
        return err_prog(UNDEF_ERR, KO, ERR_INFO);
    if (before == data->files->len)
        err_bw_arg(data, KO, "Argument", "No file has been found in the given path", argv[1], NULL, true);
    return OK;
}
