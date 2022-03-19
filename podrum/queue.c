/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include <podrum/queue.h>

queue_t new_queue()
{
	queue_t queue;
	queue.items_count = 0;
	queue.items = (void **) malloc(0);
	worker_create_mutex(&queue.lock);
	return queue;
}

void *get_queue(queue_t *queue)
{
	worker_mutex_lock((&(queue->lock)));
	void *item = NULL;
	if (queue->items_count > 0) {
		--queue->items_count;
		item = queue->items[0];
		void **items = (void **) malloc(queue->items_count * sizeof(void *));
		size_t i;
		for (i = 0; i < queue->items_count; ++i) {
			items[i] = queue->items[i + 1];
		}
		free(queue->items);
		queue->items = items;
	}
	worker_mutex_unlock((&(queue->lock)));
	return item;
}

void put_queue(void *data, queue_t *queue)
{
	worker_mutex_lock((&(queue->lock)));
	++queue->items_count;
	queue->items = (void **) realloc(queue->items, queue->items_count * sizeof(void *));
	queue->items[queue->items_count - 1] = data;
	worker_mutex_unlock((&(queue->lock)));
}