#include "uls.h"

//prints needed amount of whitespaces for pretty output
void print_tab(int max_size, char *str) {
    int spaces = max_size - mx_strlen(str) + TAB_SIZE;

    while (spaces-- > 0)
        mx_printchar(' ');
}

//calculates max column width for every column in the table
void get_max_width(t_table *table) {
    int file_so_far = 1;

    for (int col = 0; col < table->cols; col++) {
        table->max_col_size[col] = 0;

        for (int row = 0; row < table->rows; row++) {
            // printf("For row %d, col %d, str = %s\n", row,col, table->table[row][col]);
            if (1) {
                int len = mx_strlen(table->table[row][col]);
                // printf("For row %d, col %d, len = %d\n", row, col, len);
                if (len > table->max_col_size[col])
                    table->max_col_size[col] = len;
            }
            file_so_far++;
        }
    }
}

//allocates string table of given size
static t_table *allocate_table(int rows, int cols) {
    t_table *table = malloc(sizeof(t_table));

    table->rows = rows;
    table->cols = cols;
        //allocate max_col_size arrat
    table->max_col_size = malloc((cols) * sizeof(int));
        //allocate 2-D string array
    table->table = malloc(table->rows * sizeof(char**));
    for (int i = 0; i < table->rows; i++) {
        table->table[i] = malloc(table->cols * sizeof(char*));
    }
        //initialize table with NULLs
    for (int row = 0; row < table->rows; row++) {
        for (int col = 0; col < table->cols; col++) {
            table->table[row][col] = NULL;
    }
}
    return table;
}

//prints table
static void print_table(t_table *table) {
    int file_so_far = 1;
    get_max_width(table);
    for (int row = 0; row < table->rows; row++) {
        for (int col = 0; col < table->cols; col++) {
            if (table->table[row][col] != NULL) {
                mx_printstr(table->table[row][col]);
                if (col + 1 < table->cols)
                    if (table->table[row][col+1] != NULL)
                        print_tab(table->max_col_size[col], table->table[row][col]);
                    // mx_printstr(table->table[row][col]);
            }
        file_so_far++;
        }
    mx_printchar('\n');
    }
}

//fill table with given shototam
static void fill_table(t_table *table, t_file *files, t_flags *flags) {
    t_file *file = files;
        //iterate over list
    int row_so_far = 0, col_so_far = 0;
    while (file != NULL) {
        //get file name
        char *filename = mx_get_name(file, flags);
        table->table[row_so_far][col_so_far] = filename;
        row_so_far++;
        //go to next column
        if(row_so_far >= table->rows) {
            col_so_far++;
            row_so_far = 0;
        }
        file = file->next;
    }
}

static void fill_table_long(t_table *table, t_file *files, t_flags *flags) {
    t_file *file = files;
    int row = 0;
        //iterate over list
    while (file != NULL) {
        int col = 0;
            //fill ID (if -i flag is active)
        if (flags->i)
            table->table[row][col++] = mx_get_index_number(file);
            //fill permissions
        table->table[row][col++] = mx_get_permissions(file);
            //fill links
        table->table[row][col++] = mx_get_links_num(file);
            //fill owner (if -g flag is not active)
        if (!flags->g)
            table->table[row][col++] = mx_get_user_id(file, flags);
            //fill group (if -G flag is not active)
        if (!flags->G)
            table->table[row][col++] = mx_get_group_id(file, flags);
            //fill size
        table->table[row][col++] = mx_get_size(file, flags);
            //fill date
        table->table[row][col++] = mx_get_time(file, flags);
            //fill name
        table->table[row][col] = mx_get_name(file, flags);
            //go to next file
        row++;
        file = file->next;
    }
}

//prints directory contents
void mx_print_dir(t_file *dir, t_flags *flags) {
        //get rows and columns
    int rows = 0, cols = 0;
    mx_get_rows_cols(&rows, &cols, dir->level, flags);
        //allocate table
    t_table *table = allocate_table(rows, cols);
        //fill table
    if (flags->l || flags->n)
        fill_table_long(table, dir->level, flags);
    else
        fill_table(table, dir->level, flags);
        //print table
    if ((flags ->l || flags->n) && (dir->level != NULL))
        mx_print_tot_size(dir->level, flags);
    print_table(table);

    mx_free_table(table);
} 

//copy nodes from file tree to new list
static t_file *copy_node(t_file *node) {
    t_file *copy = (t_file*)malloc(sizeof(t_file));

    if (node == NULL) {
        copy->name = NULL;
        copy->path = NULL;
    } else {
        copy->name = mx_strdup(node->name);
        copy->path = mx_strdup(node->path);
        copy->filestat = node->filestat;
    }
    copy->level = NULL;
    copy->next = NULL;
    
    return copy;
}

//push back copy to the new list
static void copy_push_back(t_file **dst, t_file *src) {
    if (*dst == NULL) {
        *dst = copy_node(src);
        return;
    }

    t_file *current = *dst;
    while(current->next != NULL)
        current = current->next;
    current->next = copy_node(src);
}

//prints only files (or directory itself if -d flag)
static bool print_files(t_file *tree, t_flags *flags) {
    //indicates if there was any files to print
    bool res = false;
        //allocate new instance
    t_file *files = copy_node(NULL);
        //push back all that shit
    while (tree != NULL) {
        if(!S_ISDIR(tree->filestat.st_mode)) {
            copy_push_back(&(files->level), tree);
            res = true;
        }
        tree = tree->next;
    }
    if(res)
        mx_print_dir(files, flags);
    mx_free_dir(files);
    return res;
}

//prints file tree recursively
void print_tree_rec(t_file *tree, t_flags *flags) {
    while (tree != NULL) {
        if(S_ISDIR(tree->filestat.st_mode)) {
            if (tree->next != NULL) {
                mx_printchar('\n');
                mx_printstr(tree->path);
                mx_printchar('/');
                mx_printstr(tree->name);
                mx_printstr(":\n");
            }
            mx_print_dir(tree, flags);
            if(flags->R && S_ISDIR(tree->filestat.st_mode))
                    print_tree_rec(tree->level, flags);
            }
        tree = tree->next;
    }
}

void mx_print_tree(t_file *tree, t_flags *flags) {
        //try to print files first
    print_files(tree, flags);
        //if only one file/dir to print, do not print it's name
    if(tree->next != NULL) {
        mx_printchar('\n');
        mx_printstr(tree->path);
        mx_printchar('/');
        mx_printstr(tree->name);
        mx_printstr(":\n");
    }
       print_tree_rec(tree, flags);
}
