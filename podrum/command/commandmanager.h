/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#ifndef PODRUM_COMMAND_COMMANDMANAGER_H
#define PODRUM_COMMAND_COMMANDMANAGER_H

typedef void (*command_executor_t)(int argc, char **argv);

typedef struct {
    char *name;
    char *description;
    char *usage;
    char *prefix;
    unsigned char flags; /* 1 = Error flag */
    command_executor_t executor;
} command_t;

typedef struct {
	command_t *commands;
	int commands_count;
} command_manager_t;

typedef struct{
    int lenght;
} command_map_t;

void register_command(command_t command, command_manager_t *manager);

command_t get_command(char *name, command_manager_t *manager);

void delete_command(char *name, command_manager_t *manager);

void execute(char *name, int argc, char **argv, command_manager_t *manager);

#endif