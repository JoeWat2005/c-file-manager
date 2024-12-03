#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//include c standard libs

char big_buf[4096];
char sml_buf[256];
char file_n[50];
char n_file_n[50];
//char buffers and char limits

void get_f(const char *prompt){
    printf("%s", prompt);
    getchar();
    fgets(file_n, sizeof(file_n), stdin);
    file_n[strcspn(file_n, "\n")] = 0;
}
//change prompt
//utility function for handling user inputs, minimising repetitive code

int count_lines(const char *file_n){
    FILE *selected_file = fopen(file_n, "r");
    if (!selected_file){
        perror("Error opening file");
        return -1;
    }
    int l_count = 0;
    while (fgets(big_buf, sizeof(big_buf), selected_file)){
        l_count = l_count + 1;
    }
    fclose(selected_file);
    return l_count;
}
//utility function which handles counting the number of lines a file has

int check_exist(const char *file_n){
    FILE *selected_file = fopen(file_n, "r");
    if (selected_file){
        fclose(selected_file);
        return 1;
    }
    return 0;
}
//utility function which handles checking if files exist

void record(const char *op, const char *file_n){
    FILE *selected_file = fopen("log.txt", "a");
    if (selected_file){
        int l_count = count_lines(file_n);
        if (l_count >= 0){
            fprintf(selected_file, "%s: %s (%d lines)\n", op, file_n, l_count);
        } 
        else{
            fprintf(selected_file, "%s: %s (File does not exist)\n", op, file_n);
        }
        fclose(selected_file);
    }
}
//utility function which handles recording to the change log file

void log_f() {
    FILE *selected_file = fopen("log.txt", "r");
    if (selected_file){
        printf("\nChange Log:\n");
        printf("----------------------------------------\n");
        while (fgets(big_buf, sizeof(big_buf), selected_file)){
            printf("%s", big_buf);
        }
        printf("----------------------------------------\n");
        fclose(selected_file);
    } 
    else{
        perror("No change log found");
    }
}
//function which handles displaying the changelog to user

void create_f(){
    get_f("Enter the name of the file to create: ");
    if (check_exist(file_n)){
        printf("The file '%s' already exists.\n", file_n);
    } 
    else{
        FILE *selected_file = fopen(file_n, "w");
        if (selected_file){
            fclose(selected_file);
            printf("File '%s' created successfully.\n", file_n);
            record("Created file", file_n);
        } 
        else{
            perror("Error creating file");
        }
    }
}
//function that handles the creation of new files by user

void delete_f(){
    get_f("Enter the name of the file to delete: ");
    if (check_exist(file_n)){
        if (remove(file_n) == 0){
            printf("File '%s' deleted successfully.\n", file_n);
            record("Deleted file", file_n);
        } 
        else{
            perror("Error deleting file");
        }
    } 
    else{
        printf("File '%s' does not exist.\n", file_n);
    }
}
//function that handles the deletion of files by user

void file_c(){
    get_f("Enter the name of the file to read: ");
    if (!check_exist(file_n)){
        printf("File '%s' does not exist.\n", file_n);
        return;
    }
    FILE *selected_file = fopen(file_n, "r");
    if (selected_file){
        printf("Contents of '%s':\n", file_n);
        while (fgets(big_buf, sizeof(big_buf), selected_file)){
            printf("%s", big_buf);
        }
        fclose(selected_file);
        printf("\n");
    }
}
//function that handles displaying the contents of a file to user

void copy_f(){
    get_f("Enter the name of the file to copy: ");
    if (!check_exist(file_n)){
        printf("File '%s' does not exist.\n", file_n);
        return;
    }
    printf("Enter the name of the new file: ");
    fgets(n_file_n, sizeof(n_file_n), stdin);
    n_file_n[strcspn(n_file_n, "\n")] = 0;
    if (check_exist(n_file_n)){
        printf("File '%s' already exists.\n", n_file_n);
        return;
    }
    FILE *first_file = fopen(file_n, "r");
    FILE *second_file = fopen(n_file_n, "w");
    while (fgets(big_buf, sizeof(big_buf), first_file)){
        fputs(big_buf, second_file);
    }
    fclose(first_file);
    fclose(second_file);
    printf("File copied successfully.\n");
    record("Copied file", n_file_n);
}
//function that handles copying a user selected file to a new file

void show_line(){
    get_f("Enter the name of the file: ");
    if (!check_exist(file_n)){
        printf("File '%s' does not exist.\n", file_n);
        return;
    }
    int total_l = count_lines(file_n);
    printf("Enter line number (1-%d): ", total_l);
    int ln;
    if (scanf("%d", &ln) != 1 || ln < 1 || ln > total_l){
        printf("Invalid line number.\n");
        return;
    }
    FILE *selected_file = fopen(file_n, "r");
    int current_l = 1;
    while (fgets(big_buf, sizeof(big_buf), selected_file)){
        if (current_l == ln){
            printf("Line %d: %s", ln, big_buf);
            break;
        }
        current_l = current_l + 1;
    }
    fclose(selected_file);
}
//function which handles showing specific lines in a file to the user once selected

void num_lines(){
    get_f("Enter the name of the file: ");
    if (!check_exist(file_n)){
        printf("File '%s' does not exist.\n", file_n);
        return;
    }
    int total_lines = count_lines(file_n);
    if (total_lines >= 0){
        printf("File '%s' has %d lines.\n", file_n, total_lines);
    }
}
//function which hasndles showing the number of lines in a file to the user

void rmline_f(){
    get_f("Enter the name of the file: ");
    if (!check_exist(file_n)){
        printf("File '%s' does not exist.\n", file_n);
        return;
    }
    int total_l = count_lines(file_n);
    printf("Enter line number to delete (1-%d): ", total_l);
    int ln;
    if (scanf("%d", &ln) != 1 || ln < 1 || ln > total_l){
        printf("Invalid line number.\n");
        return;
    }
    FILE *selected_file = fopen(file_n, "r");
    FILE *tempor_file = fopen("tempor.txt", "w");
    int current_l = 1;
    while (fgets(big_buf, sizeof(big_buf), selected_file)){
        if (current_l != ln) {
            fputs(big_buf, tempor_file);
        }
        current_l = current_l + 1;
    }
    fclose(selected_file);
    fclose(tempor_file);
    remove(file_n);
    rename("tempor.txt", file_n);
    printf("Line deleted successfully.\n");
    record("Deleted line from file", file_n);
}
//function which handles deleting a line from a file specified by user

void apline_f(){
    get_f("Enter the name of the file to append to: ");
    if (!check_exist(file_n)){
        printf("File '%s' does not exist.\n", file_n);
        return;
    }
    FILE *selected_file = fopen(file_n, "a");
    if (selected_file){
        printf("Enter line to append: ");
        fgets(big_buf, sizeof(big_buf), stdin);
        big_buf[strcspn(big_buf, "\n")] = 0;
        fprintf(selected_file, "%s\n", big_buf);
        fclose(selected_file);
        printf("Line appended successfully.\n");
        record("Appended to file", file_n);
    }
}
//function which handles appending lines to a file

void insline_f(){
    get_f("Enter the name of the file to insert into: ");
    if (!check_exist(file_n)){
        printf("File '%s' does not exist.\n", file_n);
        return;
    }
    int total_l = count_lines(file_n);
    printf("Enter line number (1-%d): ", total_l + 1);
    int ln;
    scanf("%d", &ln);

    if (ln < 1 || ln > total_l + 1){
        printf("Invalid line number. Please enter a number between 1 and %d.\n", total_l + 1);
        return;
    }
    
    printf("Enter line to insert: ");
    getchar();
    fgets(big_buf, sizeof(big_buf), stdin);
    big_buf[strcspn(big_buf, "\n")] = 0;
    FILE *selected_file = fopen(file_n, "r");
    FILE *tempor_file = fopen("tempor.txt", "w");
    int current_l = 1;
    while (fgets(sml_buf, sizeof(sml_buf), selected_file)){
        if (current_l == ln){
            fprintf(tempor_file, "%s\n", big_buf);
        }
        fputs(sml_buf, tempor_file);
        current_l = current_l + 1;
    }
    if (ln == current_l){
        fprintf(tempor_file, "%s\n", big_buf);
    }
    fclose(selected_file);
    fclose(tempor_file);
    remove(file_n);
    rename("tempor.txt", file_n);
    printf("Line inserted successfully.\n");
    record("Inserted line into file", file_n);
}
//function which handles inserting lines into lines in a file, designated by user

void dir(){
    printf("Files in current directory:\n");
    system("dir");
    //change to ls if on macos (I think - I use windows 11, and this works!)
}
//utility function for listing all files in working directory

void rename_f(){
    get_f("Enter the name of the file to rename: ");
    if (!check_exist(file_n)){
        printf("File '%s' does not exist.\n", file_n);
        return;
    }
    printf("Enter new name: ");
    fgets(n_file_n, sizeof(n_file_n), stdin);
    n_file_n[strcspn(n_file_n, "\n")] = 0;
    if (check_exist(n_file_n)){
        printf("File '%s' already exists.\n", n_file_n);
        return;
    }
    rename(file_n, n_file_n);
    printf("File renamed successfully.\n");
    record("Renamed file", n_file_n);
}
//utility function handling the renaming of created files

int main(){
    FILE *log_file = fopen("log.txt", "w");
    if (log_file){
        fclose(log_file);
    }

    int choice;
    while (1) {
        printf("\nMain Menu - Select an Option\n");
        printf("1. Create a file\n");
        printf("2. Copy a file\n");
        printf("3. Delete a file\n");
        printf("4. Show file contents\n");
        printf("5. Append line to file\n");
        printf("6. Delete line from file\n");
        printf("7. Insert line into file\n");
        printf("8. Show specific line\n");
        printf("9. Show number of lines\n");
        printf("10. List files in directory\n");
        printf("11. Rename file\n");
        printf("12. Display change log\n");
        printf("13. Exit\n");
        printf("Enter your choice (1-13) >");

        if (scanf("%d", &choice) != 1){
            printf("Invalid input! Please enter a number between 1 and 14.\n");
            while (getchar() != '\n');
            continue;
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
            //error handling
        }
    }
}
//main c program loop