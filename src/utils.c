#include "uls.h"

//check if directory is root
bool mx_is_root(const char *dir) {
    return (!mx_strcmp("..", dir) || !mx_strcmp(".", dir));
}

//check if directory is hidden directory
bool mx_is_hidden(const char *dir) {
    if (*dir == '.' && !mx_is_root(dir))
        return true;
    else
        return false;
}

//form full path string
char *mx_get_fullpath(const char *name, const char *path) {
    char *fullpath;

        //if path is empty (at first entry), do not add '/'
    if (path == NULL)
        fullpath = mx_strdup(name);
    else {
        char *temp = mx_strjoin(path, "/");
        fullpath = mx_strjoin(temp, name);
        mx_strdel(&temp);
    }
    return fullpath;
}

