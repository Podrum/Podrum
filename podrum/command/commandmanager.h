
typedef void (*exec_t)(char *args[]);

typedef struct{
    char *name;
    char *description;
    char *usage;
    char *prefix;
    unsigned char flags; // 1 = Error flag
    exec_t executor;
} command_t;

typedef struct{
    int lenght;
} command_map_t;

void register_command(command_t command);
void init_commands();
command_t get_command(char *name);
void delete_command(char *name);