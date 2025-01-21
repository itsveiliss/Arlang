#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>

#define MAX_VARIABLES 100
#define MAX_SET_SIZE 100
#define MAX_PROGRAM_SIZE 1024

typedef struct {
    char name[50];
    char value[50];
} Variable;

typedef struct {
    char values[MAX_SET_SIZE][50];
    int count;
} Set;

Variable variables[MAX_VARIABLES];
int variable_count = 0;
Set mySet;
char current_directory[256];

void print_command(const char *argument);
void make_directory(const char *directory);
void change_directory(const char *directory);
void delete_file(const char *file);
void create_file(const char *filename);
void list_files();
void show_help();
void set_variable(char *var_name, char *var_value);
void get_variable(const char *variable);
void add_to_set(const char *argument);
void check_in_set(const char *argument);
void remove_from_set(const char *argument);
void count_set();
void clear_screen();
void show_time();
void random_command(const char *argument);
void play_sound(const char *filename);
void stop_sound();
double evaluate_expression(const char *expression);
double parse_expression(const char **expr);
double parse_term(const char **expr);
double parse_factor(const char **expr);
void skip_whitespace(const char **expr);
void execute_operator(const char *argument);
void save_program(const char *filename, const char *code);
void run_program(const char *filename);
void install_package(const char *package_name);
void remove_package(const char *package_name);
void update_language();
void input_variable(char *var_name);

int main() {
    printf("Welcome to Arlang! Type 'help' to see available commands.\n");
    getcwd(current_directory, sizeof(current_directory));

    while (1) {
        printf("> ");
        char input[256];
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0; // Удаляем символ новой строки

        if (strlen(input) == 0) continue;

        char *command = strtok(input, ";");
        while (command != NULL) {
            char *arg = strtok(command, "(");
            char *argument = strtok(NULL, ")");

            if (argument != NULL) {
                argument = strtok(argument, " "); // Удаляем лишние пробелы
            }

            if (strcmp(arg, "print") == 0) {
                print_command(argument);
            } else if (strcmp(arg, "makedir") == 0) {
                make_directory(argument);
            } else if (strcmp(arg, "cd") == 0) {
                change_directory(argument);
            } else if (strcmp(arg, "del") == 0) {
                delete_file(argument);
            } else if (strcmp(arg, "create") == 0) {
                create_file(argument);
            } else if (strcmp(arg, "lf") == 0) {
                list_files();
            } else if (strcmp(arg, "exit") == 0) {
                printf("Exiting Arlang.\n");
                return 0;
            } else if (strcmp(arg, "help") == 0) {
                show_help();
            } else if (strcmp(arg, "set") == 0) {
                char var_name[50], var_value[50];
                sscanf(argument, "%s %s", var_name, var_value);
                set_variable(var_name, var_value);
            } else if (strcmp(arg, "get") == 0) {
                get_variable(argument);
            } else if (strcmp(arg, "addset") == 0) {
                add_to_set(argument);
            } else if (strcmp(arg, "checkset") == 0) {
                check_in_set(argument);
            } else if (strcmp(arg, "remove") == 0) {
                remove_from_set(argument);
            } else if (strcmp(arg, "countset") == 0) {
                count_set();
            } else if (strcmp(arg, "time") == 0) {
                show_time();
            } else if (strcmp(arg, "random") == 0) {
                random_command(argument);
            } else if (strcmp(arg, "play") == 0) {
                play_sound(argument);
            } else if (strcmp(arg, "stop") == 0) {
                stop_sound();
            } else if (strcmp(arg, "calc") == 0) {
                if (argument != NULL) {
                    double result = evaluate_expression(argument);
                    printf ("Result: %.2lf\n", result);
                } else {
                    printf("Usage: calc(<expression>)\n");
                }
            } else if (strcmp(arg, "install") == 0) {
                install_package(argument);
            } else if (strcmp(arg, "remove") == 0) {
                remove_package(argument);
            } else if (strcmp(arg, "update") == 0) {
                update_language();
            } else if (strstr(arg, "=") != NULL) {
                execute_operator(command);
            } else if (strcmp(arg, "input") == 0) {
                char var_name[50];
                sscanf(argument, "%s", var_name);
                input_variable(var_name);
            } else {
                printf("Unknown command: %s\n", arg);
            }
            command = strtok(NULL, ";");
        }
    }
}

void input_variable(char *var_name) {
    char input_value[50];
    printf("Enter value for %s: ", var_name);
    fgets(input_value, sizeof(input_value), stdin);
    input_value[strcspn(input_value, "\n")] = 0; // Удаляем символ новой строки
    set_variable(var_name, input_value);
}

void create_file(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file) {
        printf("File %s created successfully.\n", filename);
        fclose(file);
    } else {
        perror("Error creating file");
    }
}

void install_package(const char *package_name) {
    printf("Installing package: %s\n", package_name);
    // Здесь можно добавить логику для установки пакета
}

void remove_package(const char *package_name) {
    printf("Removing package: %s\n", package_name);
    // Здесь можно добавить логику для удаления пакета
}

void update_language() {
    printf("Updating language...\n");
    // Здесь можно добавить логику для обновления языка
}

void save_program(const char *filename, const char *code) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error saving program to %s\n", filename);
        return;
    }
    fprintf(file, "%s", code);
    fclose(file);
    printf("Program saved as %s\n", filename);
}

void run_program(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening program %s\n", filename);
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0; // Удаляем символ новой строки
        if (strstr(line, "=") != NULL) {
            execute_operator(line);
        } else if (strncmp(line, "print", 5) == 0) {
            print_command(line + 6); // Пропускаем "print "
        } else if (strncmp(line, "set", 3) == 0) {
            char var_name[50], var_value[50];
            sscanf(line + 4, "%s %s", var_name, var_value);
            set_variable(var_name, var_value); // Исправлено на два аргумента
        } else {
            printf("Unknown command in program: %s\n", line);
        }
    }
    fclose(file);
    printf("Program %s executed.\n", filename);
}

void set_variable(char *var_name, char *var_value) {
    for (int i = 0; i < variable_count; i++) {
        if (strcmp(variables[i].name, var_name) == 0) {
            snprintf(variables[i].value, sizeof(variables[i].value), "%s", var_value);
            printf("Updated variable %s to %s\n", var_name, var_value);
            return;
        }
    }

    if (variable_count < MAX_VARIABLES ) {
        snprintf(variables[variable_count].name, sizeof(variables[variable_count].name), "%s", var_name);
        snprintf(variables[variable_count].value, sizeof(variables[variable_count].value), "%s", var_value);
        variable_count++;
        printf("Variable %s set to %s\n", var_name, var_value);
    } else {
        printf("Error: Maximum number of variables reached.\n");
    }
}

void get_variable(const char *variable) {
    for (int i = 0; i < variable_count; i++) {
        if (strcmp(variables[i].name, variable) == 0) {
            printf("%s = %s\n", variable, variables[i].value);
            return;
        }
    }
    printf("Variable %s not found.\n", variable);
}

void print_command(const char *argument) {
    if (argument != NULL) {
        printf("%s\n", argument);
    } else {
        printf("Usage: print(<message>)\n");
    }
}

void make_directory(const char *directory) {
    if (mkdir(directory) == 0) {
        printf("Directory %s created.\n", directory);
    } else {
        perror("Error creating directory");
    }
}

void change_directory(const char *directory) {
    if (chdir(directory) == 0) {
        getcwd(current_directory, sizeof(current_directory));
        printf("Changed directory to %s\n", current_directory);
    } else {
        perror("Error changing directory");
    }
}

void delete_file(const char *file) {
    if (remove(file) == 0) {
        printf("File %s deleted.\n", file);
    } else {
        perror("Error deleting file");
    }
}

void list_files() {
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(current_directory)) != NULL) {
        printf("Files in %s:\n", current_directory);
        while ((ent = readdir(dir)) != NULL) {
            printf("%s\n", ent->d_name);
        }
        closedir(dir);
    } else {
        perror("Error opening directory");
    }
}

void show_help() {
    printf("Available commands:\n");
    printf("  print(<message>) - Print a message\n");
    printf("  makedir(<directory>) - Create a new directory\n");
    printf("  cd(<directory>) - Change current directory\n");
    printf("  del(<file>) - Delete a file\n");
    printf("  create(<filename>) - Create a new file\n");
    printf("  lf - List files in the current directory\n");
    printf("  exit - Exit the program\n");
    printf("  help - Show this help message\n");
    printf("  set(<variable> <value>) - Set a variable\n");
    printf("  get(<variable>) - Get the value of a variable\n");
    printf("  addset(<value>) - Add a value to the set\n");
    printf("  checkset(<value>) - Check if a value is in the set\n");
    printf("  remove(<value>) - Remove a value from the set\n");
    printf("  countset - Count the number of values in the set\n");
    printf("  time - Show current time\n");
    printf("  random(<max>) - Generate a random number up to max\n");
    printf("  play(<filename>) - Play a sound file\n");
    printf("  stop - Stop playing sound\n");
    printf("  calc(<expression>) - Calculate a mathematical expression\n");
    printf("  install(<package_name>) - Install a package\n");
    printf("  remove(<package_name>) - Remove a package\n");
    printf("  update - Update the language\n");
    printf("  save(<program_name>) - Save a program\n");
    printf("  run(<program_name>) - Run a saved program\n");
}

void add_to_set(const char *argument) {
    if (mySet.count < MAX_SET_SIZE) {
        snprintf(mySet.values[mySet.count], sizeof(mySet.values[mySet.count]), "%s", argument);
        mySet.count++;
        printf("Added %s to the set.\n", argument);
    } else {
        printf("Error: Maximum set size reached.\n");
    }
}

void check_in_set(const char *argument) {
    for (int i = 0; i < mySet.count; i++) {
        if (strcmp(mySet.values[i], argument) == 0) {
            printf("%s is in the set.\n", argument);
            return;
        }
    }
    printf("%s is not in the set.\n", argument);
}

void remove_from_set(const char *argument ) {
    for (int i = 0; i < mySet.count; i++) {
        if (strcmp(mySet.values[i], argument) == 0) {
            for (int j = i; j < mySet.count - 1; j++) {
                strcpy(mySet.values[j], mySet.values[j + 1]);
            }
            mySet.count--;
            printf("Removed %s from the set.\n", argument);
            return;
        }
    }
    printf("%s not found in the set.\n", argument);
}

void count_set() {
    printf("Set contains %d values.\n", mySet.count);
}

void clear_screen() {
    printf("\033[H\033[J");
}

void show_time() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("Current time: %02d:%02d:%02d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
}

void random_command(const char *argument) {
    int max = atoi(argument);
    if (max > 0) {
        int random_value = rand() % max;
        printf("Random number: %d\n", random_value);
    } else {
        printf("Usage: random(<max>)\n");
    }
}

void play_sound(const char *filename) {
    // Placeholder for sound playing functionality
    printf("Playing sound: %s\n", filename);
}

void stop_sound() {
    // Placeholder for stopping sound functionality
    printf("Sound stopped.\n");
}

double evaluate_expression(const char *expression) {
    const char *expr = expression;
    return parse_expression(&expr);
}

double parse_expression(const char **expr) {
    double result = parse_term(expr);
    while (1) {
        skip_whitespace(expr);
        char op = **expr;
        if (op == '+' || op == '-') {
            (*expr)++;
            double term = parse_term(expr);
            result += (op == '+') ? term : -term;
        } else break;
    }
    return result;
}

double parse_term(const char **expr) {
    double result = parse_factor(expr);
    while (1) {
        skip_whitespace(expr);
        char op = **expr;
        if (op == '*' || op == '/') {
            (*expr)++;
            double factor = parse_factor(expr);
            result = (op == '*') ? result * factor : result / factor;
        } else {
            break;
        }
    }
    return result;
}

double parse_factor(const char **expr) {
    skip_whitespace(expr);
    double result = 0.0;
    if (**expr == '(') {
        (*expr)++;
        result = parse_expression(expr);
        if (**expr == ')') {
            (*expr)++;
        }
    } else {
        char *end;
        result = strtod(*expr, &end);
        *expr = end;
    }
    return result;
}

void skip_whitespace(const char **expr) {
    while (isspace(**expr)) {
        (*expr)++;
    }
}

void execute_operator(const char *argument) {
    char var_name[50];
    char expression[100];
    sscanf(argument, "%s = %s", var_name, expression);
    double result = evaluate_expression(expression);
    char result_str[50];
    snprintf(result_str, sizeof(result_str), "%.2lf", result);
    set_variable(var_name, result_str);
    printf("Executed: %s = %.2lf\n", var_name, result);
}