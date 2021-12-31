/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include <stdlib.h>
#include "commandmanager.h"

command_t *commands;
int command_count = 0;

void init_commands()
{
    commands = malloc(0);
}

void register_command(command_t command)
{
    command_count++;
    commands = realloc(commands, command_count * sizeof(command_t));
    commands[command_count - 1] = command;
}

command_t get_command(char *name)
{
    int i;
    for (i = 0; i < command_count; ++i)
    {
        if (commands[i].name == name)
        {
            return commands[i];
        }
    }
    command_t noret;
    noret.flags |= 1 << 0; // Set error flag
    return noret;
}

void delete_command(char *name)
{
    if (command_count != 0){
        int i;
        command_t *temp_commands;
        temp_commands = malloc(command_count - 1);
        int offset = 0;
        for (i = 0; i < command_count; ++i)
        {
            if (commands[i].name != name)
            {
                temp_commands[offset] = commands[i];
                ++offset;
            }
        }
        free(commands);
        commands = temp_commands;
    }
}

void execute(){
	return;
}