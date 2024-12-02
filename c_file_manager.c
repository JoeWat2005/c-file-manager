#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Come back and remove duplicate code
Use just a buffer for reading lines, counting lines, etc - consolidate into one
*/

char file_name[64];
char new_file_name[64];
char buffer[5000];
char line[256];

int check_if_file_exists(const char *file_name){
  FILE *file = fopen(file_name, "r");
  if (file){
    fclose(file);
    return 1;
  }
  return 0;
}
//utility function to check if a file exists

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
//function which handles file creation, avoiding duplicates

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
//function for handling file deletion

void show_file_content(){
  printf("Enter the name of the file to read: ");
  getchar();
  fgets(file_name, sizeof(file_name), stdin);
  file_name[strcspn(file_name, "\n")] = 0;
  if (check_if_file_exists(file_name)){
    FILE *file = fopen(file_name, "r");
    if (file){
      char line[256];
      printf("Contents of '%s': \n", file_name);
      while (fgets(line, sizeof(line), file)){
        printf("%s", line);
      }
      fclose(file);
      printf("\n");
    } 
    else{
      perror("Error opening file. \n");
    }
  } 
  else{
    printf("File '%s' does not exist.\n", file_name);
  }
}
//function to display file contents

void copy_file(){
  printf("Enter the name of the file to copy: ");
  getchar();
  fgets(file_name, sizeof(file_name), stdin);
  file_name[strcspn(file_name, "\n")] = 0;
  if (!check_if_file_exists(file_name)){
    printf("File '%s' does not exist.\n", file_name);
    return;
  }
  printf("Enter the name of the new file: ");
  fgets(new_file_name, sizeof(new_file_name), stdin);
  new_file_name[strcspn(new_file_name, "\n")] = 0;
  if (check_if_file_exists(new_file_name)){
    printf("The file '%s' already exists.\n", new_file_name);
    return;
  }
  FILE *file = fopen(file_name, "r");
  FILE *new_file = fopen(new_file_name, "w");
  while (fgets(line, sizeof(line), file)){
    fputs(line, new_file);
  }
  fclose(file);
  fclose(new_file);
  printf("File '%s' copied to '%s' successfully.\n", file_name, new_file_name);
}
//function for copying a file

int count_lines_in_file(const char *file_name){
  FILE *file = fopen(file_name, "r");
  if (!file){
    perror("Error opening file");
    return -1;
  }
  int lines = 0;
  while (fgets(line, sizeof(line), file)){
    lines = lines + 1;
  }
  fclose(file);
  return lines;
}
//utility function for counting number of lines in a file

void show_line(){
  printf("Enter the name of the file: ");
  getchar();
  fgets(file_name, sizeof(file_name), stdin);
  file_name[strcspn(file_name, "\n")] = 0;
  if (!check_if_file_exists(file_name)){
    printf("File '%s' does not exist.\n", file_name);
    return;
  }
  int total_lines = count_lines_in_file(file_name);
  if (total_lines == -1){
    return;
  }
  printf("Enter the line number to show (1-%d): ", total_lines);
  int line_number;
  if (scanf("%d", &line_number) != 1 || line_number < 1 || line_number > total_lines) {
    printf("Invalid line number.\n");
    return;
  }
  FILE *file = fopen(file_name, "r");
  int current_line = 1;
  while (fgets(buffer, sizeof(buffer), file)){
    if (current_line == line_number){
      printf("Line %d: %s", line_number, buffer);
      break;
    }
    current_line = current_line + 1;
  }
  fclose(file);
}
//function to show a specific line in a file

void show_number_of_lines(){
  printf("Enter the name of the file: ");
  getchar();
  fgets(file_name, sizeof(file_name), stdin);
  file_name[strcspn(file_name, "\n")] = 0;

  if (!check_if_file_exists(file_name)){
    printf("File '%s' does not exist.\n", file_name);
    return;
  }
  int total_lines = count_lines_in_file(file_name);
  if (total_lines == -1){
    return;
  }
  printf("The file '%s' has %d lines.\n", file_name, total_lines);
}
//function to show the total number of lines in a file

void delete_line_from_file(){
  printf("Enter the name of the file: ");
  getchar();
  fgets(file_name, sizeof(file_name), stdin);
  file_name[strcspn(file_name, "\n")] = 0;
  if (!check_if_file_exists(file_name)){
    printf("File '%s' does not exist.\n", file_name);
  }
  int total_lines = count_lines_in_file(file_name);
  if (total_lines == -1){
    return;
  }
  printf("Enter the line number to delete (1-%d): ", total_lines);
  int line_number;
  if (scanf("%d", &line_number) != 1 || line_number < 1 || line_number > total_lines){
    printf("Invalid line number.\n");
    return;
  }
  FILE *file = fopen(file_name, "r");
  FILE *temp_file = fopen("temp.txt", "w");
  int current_line = 1;
  while (fgets(buffer, sizeof(buffer), file)){
    if (current_line != line_number){
      fputs(buffer, temp_file);
    }
    current_line = current_line + 1;
  }
  fclose(file);
  fclose(temp_file);
  remove(file_name);
  rename("temp.txt", file_name);
  printf("Line %d deleted from file '%s'.\n", line_number, file_name);
}
//function for deleting specific line in a file

void append_line_to_file(){
  printf("Enter the name of the file to append to: ");
  getchar();
  fgets(file_name, sizeof(file_name), stdin);
  file_name[strcspn(file_name, "\n")] = 0;
  FILE *file = fopen(file_name, "a");
  if (file){
    printf("Enter the line to append: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    fprintf(file, "%s\n", buffer);
    fclose(file);
    printf("Line appended to file '%s'.\n", file_name);
  } 
  else{
    perror("Error opening file");
  }
}
//function to handle appending a line to the end of a file

void insert_line_into_file(){
  printf("Enter the name of the file to insert into: ");
  getchar();
  fgets(file_name, sizeof(file_name), stdin);
  file_name[strcspn(file_name, "\n")] = 0;
  int total_lines = count_lines_in_file(file_name);
  printf("Enter the line number to insert into (1-%d): ", total_lines + 1);
  int line_number;
  scanf("%d", &line_number);
  printf("Enter the line to insert: ");
  getchar();
  fgets(buffer, sizeof(buffer), stdin);
  buffer[strcspn(buffer, "\n")] = 0;
  FILE *file = fopen(file_name, "r");
  FILE *temp_file = fopen("temp.txt", "w");
  int current_line = 1;
  while (fgets(line, sizeof(line), file)){
    if (current_line == line_number){
      fprintf(temp_file, "%s\n", buffer);
    }
    fputs(line, temp_file);
    current_line = current_line + 1;
  }
  if (line_number == current_line) {
    fprintf(temp_file, "%s\n", buffer);
  }
  fclose(file);
  fclose(temp_file);
  remove(file_name);
  rename("temp.txt", file_name);
  printf("Line successsfully inserted into file '%s'.\n", file_name);
}
//function handling specific line inserts into a file

void list_files_in_directory(){
  printf("Files in current directory: \n");
  system("dir");
}
//utility function to list all files in the working directory (windows)

void rename_file(){
  printf("Enter the name of the file to rename: ");
  getchar();
  fgets(file_name, sizeof(file_name), stdin);
  file_name[strcspn(file_name, "\n")] = 0;
  if (!check_if_file_exists(file_name)){
    printf("File '%s' does not exist.\n", file_name);  
    return;
  }
  printf("Enter the new name for the file: ");
  fgets(new_file_name, sizeof(new_file_name), stdin);
  new_file_name[strcspn(new_file_name, "\n")] = 0;
  if (check_if_file_exists(new_file_name)){
    printf("The file '%s' already exists.\n", new_file_name);
    return;
  }
  rename(file_name, new_file_name);
  printf("File '%s' renamed to '%s'.\n", file_name, new_file_name);
}
//function for renaming files

int main(){
  int choice;
  while (1) {
    printf("\nMain Menu\n");
    printf("1. Create a file.\n");
    printf("2. Copy a file.\n");
    printf("3. Delete a file.\n");
    printf("4. Show all file contents.\n");
    printf("5. Append a line to a file.\n");
    printf("6. Delete a line from a file.\n");
    printf("7. Insert a line into a file.\n");
    printf("8. Show a specific line in a file.\n");
    printf("9. Show change log (not implemented)\n");
    printf("10. Show number of lines in a file.\n");
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
    case 2:
      copy_file();
      break;
    case 3:
      delete_file();
      break;
    case 4:
      show_file_content();
      break;
    case 5:
      append_line_to_file();
      break;
    case 6:
      delete_line_from_file();
      break;
    case 7:
      insert_line_into_file();
      break;
    case 8:
      show_line();
      break;
    case 10:
      show_number_of_lines();
      break;
    case 11:
      list_files_in_directory();
      break;
    case 12:
      rename_file();
    case 13:
      printf("Exiting Program...\n");
      return 0;
    default:
      printf("Please enter a number between 1 and 13.\n");
      break;
    }
  }
}