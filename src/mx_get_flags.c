#include "uls.h"

static void mx_flags_def(t_flags *flags, char *str) {
    for (int i = 1; str[i] != '\0'; i++) {
        printf("Flag: %c\n", str[i]);
        switch (str[i]) {
            case 'r':
                flags->r = true;
                break;
            case 'S':
                flags->S = true;
                break;
            case 't':
                flags->t = true;
                break;
            case 'u':
                flags->u = true;
                break;
            case 'c':
                flags->c = true;
                break;
            case 'U':
                flags->u = true;
                break;
            case 'l':
                flags->l = true;
                break;
            case 'a':
                flags->a = true;
                break;
            case 'A':
                flags->A = true;
                break;
            case 'G':
                flags->G = true;
                break;
            case 'h':
                flags->h = true;
                break;
            case 'i':
                flags->i = true;
                break;
            case 'n':
                flags->n = true;
                break;
            case 'p':
                flags->p = true;
                break;
            case '1':
                flags->one = true;
                break;
            default:
                mx_invalid_flag(str[i]);
                break;
        }
    }
    
}

void mx_get_flags(int argc, char **argv, t_uls *data, t_flags *flags) {
    int k = 0;
    int count = 0;
    data->argcf = 0;

    // data->flags = (char *)malloc(sizeof(char)); 

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] != '\0') {
            mx_flags_def(flags, argv[i]);
            data->argcf++;
        }
    }
    data->flags_num = count;
    // printf("%d\n", flags->r);
}
