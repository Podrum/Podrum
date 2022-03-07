/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#ifndef PODRUM_QUEUE_H
#define PODRUM_QUEUE_H

#include <podrum/worker.h>
#include <stdlib.h>

typedef struct {
	worker_mutex_t lock;
	void **items;
	size_t items_count;
} queue_t;

queue_t new_queue();

void *get_queue(queue_t *queue);

void put_queue(void *data, queue_t *queue);

#endif