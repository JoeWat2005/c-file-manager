#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//include relevant standard c libraries

char big_buf[4096];
char sml_buf[256];
char file_n[50];
char n_file_n[50];
//define global buffers / char arrays to be used throughout the entire program


void get_f(const char *text){
    printf("%s", text); 
    getchar();
    fgets(file_n, sizeof(file_n), stdin);
    file_n[strcspn(file_n, "\n")] = 0;
}


int count_lines(const char *file_n){
    FILE *selected_file = fopen(file_n, "r");
    //open passed param file in read mode
    if (!selected_file){
        perror("Error opening file");
        return -1;
        //print error message and return -1 if file cannot be opened
    }
    int l_count = 0;
    while (fgets(big_buf, sizeof(big_buf), selected_file)){
        l_count = l_count + 1;
    }
    fclose(selected_file);
    return l_count;
    //setup line counter variable, read line by line and increment counter line by line, close file, return l_count
}
//utility function handling counting lines in a file

int check_exist(const char *file_n){
    FILE *selected_file = fopen(file_n, "r");
    //open passed param file in read mode
    if (selected_file){
        fclose(selected_file);
        return 1;
        //if exists, return 1 (true)
    }
    return 0;
    //if does not exist, return 0 (fasle)
}
//utility function handling file existence

void record(const char *op, const char *file_n){
    FILE *selected_file = fopen("log.txt", "a");
    //open log.txt in append mode
    if (selected_file){
        //if log file opens
        int l_count = count_lines(file_n);
        //count lines in passed param file
        if (l_count >= 0){
            fprintf(selected_file, "%s: %s (%d lines)\n", op, file_n, l_count);
            //append file name, op, and line count to logfile
        } 
        else{
            fprintf(selected_file, "%s: %s (File does not exist)\n", op, file_n);
            //append file name, op, and file does not exist to logfile
        }
        fclose(selected_file);
        //close file
    }
}
//utility function handling recordings to log

void log_f() {
    FILE *selected_file = fopen("log.txt", "r");
    //open log file in read mode
    if (selected_file){
        //if opens successfully
        printf("\nChange Log:\n");
        printf("----------------------------------------\n");
        while (fgets(big_buf, sizeof(big_buf), selected_file)){
            printf("%s", big_buf);
        }
        printf("----------------------------------------\n");
        fclose(selected_file);
        //print logfile line by line with large char buffer, with a bit of formatting
    } 
    else{
        perror("No change log found");
        //catch no log file error on program start
    }
}
//function which handles showing log file

void create_f(){
    get_f("Enter the name of the file to create: ");
    //call get_f utility function, passing prompt, storing file name in file_n buffer
    if (check_exist(file_n)){
        printf("The file '%s' already exists.\n", file_n);
    }
    //check if file exists passing the name into the check_exist() function as a parameter
    else{
        FILE *selected_file = fopen(file_n, "w");
        //if not, create a new file with name
        if (selected_file){
            fclose(selected_file);
            printf("File '%s' created successfully.\n", file_n);
            record("Created file", file_n);
            //if successful, close file, notify user and record to logfile
        } 
        else{
            perror("Error creating file");
            //catch error
        }
    }
}
//function that handles new file creation

void delete_f(){
    get_f("Enter the name of the file to delete: ");
    //call get_f utility function, passing prompt, storing file name in file_n buffer
    if (check_exist(file_n)){
        //check exists
        if (remove(file_n) == 0){
            printf("File '%s' deleted successfully.\n", file_n);
            record("Deleted file", file_n);
            //attempt to delete, notifying user and logging deletion to logfile
        } 
        else{
            perror("Error deleting file");
            //error catch
        }
    } 
    else{
        printf("File '%s' does not exist.\n", file_n);
        //catch case where file does not exist - user typo?
    }
}
//function that handles file deletion

void file_c(){
    get_f("Enter the name of the file to read: ");
    //call user input util function
    if (!check_exist(file_n)){
        printf("File '%s' does not exist.\n", file_n);
        return;
        //catch case where file does not exist
    }
    FILE *selected_file = fopen(file_n, "r");
    //open passed file in read mode
    if (selected_file){
        printf("Contents of '%s':\n", file_n);
        while (fgets(big_buf, sizeof(big_buf), selected_file)){
            printf("%s", big_buf);
        }
        fclose(selected_file);
        printf("\n");
        //iterate through and print file contents to cli line by line
    }
}
//function that handles displaying file contents

void copy_f(){
    get_f("Enter the name of the file to copy: ");
    //get user input with util function
    if (!check_exist(file_n)){
        printf("File '%s' does not exist.\n", file_n);
        return;
        //catch invalid input errors, preventing program crash
    }
    printf("Enter the name of the new file: ");
    fgets(n_file_n, sizeof(n_file_n), stdin);
    n_file_n[strcspn(n_file_n, "\n")] = 0;
    //get user input for new file name, cannot use file_n buffer, have to use n_file_n to store input
    if (check_exist(n_file_n)){
        printf("File '%s' already exists.\n", n_file_n);
        return;
    }
    //check if file already has name stored in n_file_n char array
    FILE *first_file = fopen(file_n, "r");
    FILE *second_file = fopen(n_file_n, "w");
    //open first file based on file name in file_n buffer in read mode
    //open second file based on file name in n_file_n in write mode
    while (fgets(big_buf, sizeof(big_buf), first_file)){
        fputs(big_buf, second_file);
    }
    //read text from first file and write to second file
    fclose(first_file);
    fclose(second_file);
    //close files
    printf("File copied successfully.\n");
    record("Copied file", n_file_n);
    //notify user and log
}
//function that handles copying files

void show_line(){
    get_f("Enter the name of the file: ");
    //get user filename from user input util function
    if (!check_exist(file_n)){
        printf("File '%s' does not exist.\n", file_n);
        return;
    }
    //catch an error if file does not exist
    int total_l = count_lines(file_n);
    //store in total_l total number of lines in specified file by using util function count_lines()
    printf("Enter line number (1-%d): ", total_l);
    //prompt user to enter line number, showing range of lines
    int ln;
    if (scanf("%d", &ln) != 1 || ln < 1 || ln > total_l){
        printf("Invalid line number.\n");
        return;
    }
    //get user input and validate it against line range
    FILE *selected_file = fopen(file_n, "r");
    int current_l = 1;
    //open file in read mode and set current line to 1
    while (fgets(big_buf, sizeof(big_buf), selected_file)){
        if (current_l == ln){
            printf("Line %d: %s", ln, big_buf);
            break;
        }
        current_l = current_l + 1;
        //iterate through file lines until int ln (user choice) is equal to current_l, printing the line to cli
    }
    fclose(selected_file);
}
//function which handles displaying a line in a file

void num_lines(){
    get_f("Enter the name of the file: ");
    //get input
    if (!check_exist(file_n)){
        printf("File '%s' does not exist.\n", file_n);
        return;
    }
    //check exists
    int total_lines = count_lines(file_n);
    printf("File '%s' has %d lines.\n", file_n, total_lines);
    //print file name and no. of lines
}
//function which hasndles counting number of lines (and displaying in cli)

void rmline_f(){
    get_f("Enter the name of the file: ");
    if (!check_exist(file_n)){
        printf("File '%s' does not exist.\n", file_n);
        return;
    }
    //get user file input and check if file exists based on string name of file stored in file_n
    int total_l = count_lines(file_n);
    printf("Enter line number to delete (1-%d): ", total_l);
    //count lines and display range to user, asking which line they want to remove
    int ln;
    if (scanf("%d", &ln) != 1 || ln < 1 || ln > total_l){
        printf("Invalid line number.\n");
        return;
    }
    //get user input and check against line bounds in file
    FILE *selected_file = fopen(file_n, "r");
    FILE *tempor_file = fopen("tempor.txt", "w");
    //open file in read mode for main file
    //open temp file in write mode
    int current_l = 1;
    while (fgets(big_buf, sizeof(big_buf), selected_file)){
        if (current_l != ln) {
            fputs(big_buf, tempor_file);
        }
        current_l = current_l + 1;
    }
    //iterate through selected_file (main file) copying all lines to tempor_file except from line chosen to be removed stored in ln (as an int)
    fclose(selected_file);
    fclose(tempor_file);
    remove(file_n);
    //close files and remove old file
    rename("tempor.txt", file_n);
    printf("Line deleted successfully.\n");
    record("Deleted line from file", file_n);
    //rename temp file with string stored in char file_n buffer so it has original name
}
//function which handles deleting line from a file

void apline_f(){
    get_f("Enter the name of the file to append to: ");
    if (!check_exist(file_n)){
        printf("File '%s' does not exist.\n", file_n);
        return;
    }
    //get user input, store in file_n char buffer, check if exists to catch error
    FILE *selected_file = fopen(file_n, "a");
    //open file based on user input in append mode
    if (selected_file){
        printf("Enter line to append: ");
        fgets(big_buf, sizeof(big_buf), stdin);
        big_buf[strcspn(big_buf, "\n")] = 0;
        //get user input usng big buffer and cleaning input
        fprintf(selected_file, "%s\n", big_buf);
        fclose(selected_file);
        //write line to file and close it
        printf("Line appended successfully.\n");
        record("Appended to file", file_n);
        //notify user and record to log
    }
}
//function which handles appending line to file

void insline_f(){
    get_f("Enter the name of the file to insert into: ");
    if (!check_exist(file_n)){
        printf("File '%s' does not exist.\n", file_n);
        return;
    }
    //get file name from user and check if file exists
    int total_l = count_lines(file_n);
    //get total number of lines in file
    printf("Enter line number (1-%d): ", total_l + 1);
    //total plus 1 for appending a line
    int ln;
    scanf("%d", &ln);
    //get line number from user input and store in ln

    if (ln < 1 || ln > total_l + 1){
        printf("Invalid line number. Please enter a number between 1 and %d.\n", total_l + 1);
        return;
    }
    //check user input bounds and catch errors

    printf("Enter line to insert: ");
    getchar();
    fgets(big_buf, sizeof(big_buf), stdin);
    big_buf[strcspn(big_buf, "\n")] = 0;
    //prompt user to enter a line and clean their input. Input stored in big buffer
    FILE *selected_file = fopen(file_n, "r");
    FILE *tempor_file = fopen("tempor.txt", "w");
    //open user inputted file in read mode
    //open temporary file in write mode
    int current_l = 1;
    while (fgets(sml_buf, sizeof(sml_buf), selected_file)){
        if (current_l == ln){
            fprintf(tempor_file, "%s\n", big_buf);
        }
        //add new line to temp file
        fputs(sml_buf, tempor_file);
        current_l = current_l + 1;
        //add other lines to temp file normally
    }
    if (ln == current_l){
        fprintf(tempor_file, "%s\n", big_buf);
    }
    //edge case for adding to the end of the file
    fclose(selected_file);
    fclose(tempor_file);
    remove(file_n);
    rename("tempor.txt", file_n);
    printf("Line inserted successfully.\n");
    record("Inserted line into file", file_n);
    //close files, remove orignal file, rename temp file with og file name, notify user and record to log
}
//function which handles inserting lines to files

void dir(){
    printf("Files in current directory:\n");
    system("dir");
    //change to ls if on macos (I think - I use windows 11, and this works!)
}
//utility function for displaying working directory in cli

void rename_f(){
    get_f("Enter the name of the file to rename: ");
    if (!check_exist(file_n)){
        printf("File '%s' does not exist.\n", file_n);
        return;
    }
    //get input and check if file exists
    printf("Enter new name: ");
    fgets(n_file_n, sizeof(n_file_n), stdin);
    n_file_n[strcspn(n_file_n, "\n")] = 0;
    //get user input for new file and clean, storing new string in char buffer n_file_n
    if (check_exist(n_file_n)){
        printf("File '%s' already exists.\n", n_file_n);
        return;
    }
    //catch errors checking if a file already has that new name stored in n_file_n
    rename(file_n, n_file_n);
    printf("File renamed successfully.\n");
    record("Renamed file", n_file_n);
    //rename og file with n_file_n buffers stored string and record to log
}
//utility function handling renaming files

int main(){
    FILE *log_file = fopen("log.txt", "w");
    if (log_file){
        fclose(log_file);
    }
    //create or clear new log file on start

    int choice;
    //initialise choice variable for user input
    while (1) {
        printf("\nSelect Operation:\n");
        printf("1. Create file\n");
        printf("2. Copy file\n");
        printf("3. Delete file\n");
        printf("4. Show file contents\n");
        printf("5. Append line to file\n");
        printf("6. Delete line from file\n");
        printf("7. Insert line into file\n");
        printf("8. Show specific line in file\n");
        printf("9. Show number of lines in file\n");
        printf("10. List files in working directory\n");
        printf("11. Rename file\n");
        printf("12. Display changelog\n");
        printf("13. Exit program\n");
        printf("Enter choice (1-13) >");
        //file manager operation menu

        if (scanf("%d", &choice) != 1){
            printf("Invalid input! Please enter a number between 1 and 13.\n");
            //get user input and check if it is a valid integer
            while (getchar() != '\n');
            continue;
            //clear input buffer of invalid inputs preventing an infinte program loop
        }
        switch (choice){
            case 1:
                create_f();
                break;
            //create file func call
            case 2:
                copy_f();
                break;
            //copy file func call
            case 3:
                delete_f();
                break;
            //delete file func call
            case 4:
                file_c();
                break;
            //file contents func call
            case 5:
                apline_f();
                break;
            //append line to file func call
            case 6:
                rmline_f();
                break;
            //remove line in file func call
            case 7:
                insline_f();
                break;
            //insert line in file func call
            case 8:
                show_line();
                break;
            //show line func call
            case 9:
                num_lines();
                break;
            //number of lines func call
            case 10:
                dir();
                break;
            //directory file list func call
            case 11:
                rename_f();
                break;
            //rename func call
            case 12:
                log_f();
                break;
            //log func call
            case 13:
                printf("Exiting program...\n");
                return 0;
            //exit condition
            default:
                printf("Please enter a number between 1 and 14.\n");
            //catch for user inputs where integer input is out of bounds and therefore not in switch case
        }
    }
}
//main program loop