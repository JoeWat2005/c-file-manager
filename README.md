# File manager in C

A menu-driven command-line program for creating, copying, editing and inspecting text files, written in C using only the standard library.

## Context

My solution to a C programming assignment for a university module. The assignment focused on low-level file handling and procedural programming in C.

## Features

- Create, copy, rename and delete files
- Show a file's contents, a single line, or its line count
- Append a line, insert a line at a given position, or delete a line
- List the files in the working directory
- Record each change in a change log (`log.txt`) and show the log from the menu
- Check that files exist and that line numbers are in range before acting on them

## Tech stack

- C, using only `stdio.h`, `stdlib.h` and `string.h`
- One source file, `c_file_manager.c`, with no build system

## Building and running

You need a C compiler. The repository has no Makefile or build script. With GCC, for example:

```sh
gcc c_file_manager.c -o c_file_manager
./c_file_manager
```

On Windows, run `c_file_manager.exe` instead.

The program works on files in the directory you start it from. It shows a numbered menu and reads your choice:

```text
Select Operation:
1. Create file
2. Copy file
3. Delete file
4. Show file contents
5. Append line to file
6. Delete line from file
7. Insert line into file
8. Show specific line in file
9. Show number of lines in file
10. List files in working directory
11. Rename file
12. Display changelog
13. Exit program
Enter choice (1-13) >
```

Each option then asks for a file name and, where needed, a line number, a new name or the text of a line.

## How it works

`main()` creates or empties `log.txt`, then loops: it prints the menu, reads a number with `scanf`, and uses a `switch` to call one function per operation. If the input isn't a number, it clears the input buffer and shows the menu again.

| Option | Function | What it does |
| --- | --- | --- |
| 1 | `create_f()` | Creates an empty file, unless the name is already taken |
| 2 | `copy_f()` | Copies a file line by line to a new name |
| 3 | `delete_f()` | Deletes a file with `remove()` |
| 4 | `file_c()` | Prints a file's contents |
| 5 | `apline_f()` | Appends a line to the end of a file |
| 6 | `rmline_f()` | Deletes the line with a given number |
| 7 | `insline_f()` | Inserts a line before a given line number, or at the end |
| 8 | `show_line()` | Prints the line with a given number |
| 9 | `num_lines()` | Prints the number of lines |
| 10 | `dir()` | Lists the working directory by running `system("dir")` |
| 11 | `rename_f()` | Renames a file with `rename()` |
| 12 | `log_f()` | Prints the change log |
| 13 | none | Exits the program |

Shared helpers:

- `get_f()` prints a prompt, reads a file name into the global buffer `file_n` and strips the newline.
- `check_exist()` checks whether a file can be opened for reading.
- `count_lines()` counts the lines in a file with `fgets`.
- `record()` appends the operation, the file name and the file's line count (or a note that it no longer exists) to `log.txt`.

Deleting or inserting a line rewrites the file. The program copies it line by line into a temporary file, `tempor.txt`, leaving out or adding the target line, then deletes the original and renames the temporary file to the original name.

## Notes

- `log.txt` is emptied each time the program starts, so the change log only covers the current session.
- Option 10 runs `dir` through `system()`. A comment in the source says it was tested on Windows 11 and suggests `ls` for macOS.
- File names are read into 50-byte buffers, so names longer than 49 characters are cut short.

## Project structure

```text
c-file-manager/
├── c_file_manager.c   # the whole program: menu loop and file operations
├── .gitignore         # standard C ignore rules
└── README.md
```

The program creates `log.txt` and, briefly, `tempor.txt` in the working directory when it runs.
