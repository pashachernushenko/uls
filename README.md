# uls
Recode the system's utility ls.

To-do:
 - add pipe handling
 - add sorting (at least aplhabetical)
 - add -i flag for print (print file ID)
 - add -d flag (list directory itself)
 - add -F flag (classificator)
 - add -g flag (in long list, do not print owner)
 - add flags mentioned in 'print.c' file
 - add -m flag (print all files as comma-separated)
 - add -o flag (like l+G)
 - add -x flag (list by lines instead of columns) (may be hard)

 - remove final tabulation in column output (DONE)
 - while calculating number of rows and columns, take into account number of tab spaces (DONE)
 - pack print parts into one module
IDEA FOR SPEED IMPROVEMENT:
read files' stat() in print section, only when needed (with -l flag);
