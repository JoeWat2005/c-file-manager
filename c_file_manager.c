#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char file_name[100];

int check_if_file_exists(const char *file_name){
    FILE *file = fopen(file_name, "r");
    if (file){
        fclose(file);
        return 1;
    }
    return 0;
}

void create_new_file(){
    printf("Enter the name of the file to create: ");
    getchar();
    fgets(file_name, sizeof(file_name), stdin);
    file_name[strcspn(file_name, "\n")] = 0;

    if (check_if_file_exists(file_name)){
        printf("File '%s' already exists.\n", file_name);
    } 
    else{
        FILE *file = fopen(file_name, "w");
        if (file){
            fclose(file);
            printf("File '%s' created successfully.\n", file_name);
        } 
        else{
            perror("Error creating file");
        }
    }
}

void delete_file(){
    printf("Enter the name of the file to delete: ");
    getchar();
    fgets(file_name, sizeof(file_name), stdin);
    file_name[strcspn(file_name, "\n")] = 0;

    if (check_if_file_exists(file_name)){
        if (remove(file_name) == 0){
            printf("File '%s' deleted successfully.\n", file_name);
        } 
        else{
            perror("Error deleting file");
        }
    } 
    else{
        printf("File '%s' does not exist.\n", file_name);
    }
}

int main(){
    int choice;

    while (1){
        printf("\nFile Manager Menu\n");
        printf("1. Create a file\n");
        printf("2. Copy a file (not implemented)\n");
        printf("3. Delete a file\n");
        printf("4. Show all file contents (not implemented)\n");
        printf("5. Append a line to a file (not implemented)\n");
        printf("6. Delete a line from a file (not implemented)\n");
        printf("7. Insert a line into a file (not implemented)\n");
        printf("8. Show a specific line in a file (not implemented)\n");
        printf("9. Show change log (not implemented)\n");
        printf("10. Show number of lines in a file (not implemented)\n");
        printf("11. Exit\n");

        printf("Enter your choice (1-11) > ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number between 1 and 11.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice){
            case 1:
                create_new_file();
                break;
            case 3:
                delete_file();
                break;
            case 11:
                printf("Exiting Program...\n");
                return 0;
            default:
                printf("Feature not implemented or invalid choice. Please enter a number between 1 and 11.\n");
                break;
        }
    }
}
