/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include <podrum/worker.h>

worker_t create_worker(worker_executor_t executor, void *args)
{
	#ifdef _WIN32
	DWORD thread_id;

	return CreateThread(NULL, 0, executor, args, 0, &thread_id);

	#else

	worker_t worker;
	pthread_create(&worker, NULL, executor, args);
	return worker;

	#endif
}

void join_worker(worker_t worker)
{
	#ifdef _WIN32

	WaitForSingleObject(worker, INFINITE);

	#else

	pthread_join(worker, NULL);

	#endif
}

void worker_create_mutex(worker_mutex_t *lock)
{
	#ifdef _WIN32

	*lock = CreateMutex(NULL, FALSE, NULL);

	#else

	pthread_mutex_init(lock, NULL);

	#endif
}

void worker_destroy_mutex(worker_mutex_t *lock)
{
	#ifdef _WIN32

	CloseHandle(*lock);

	#else

	pthread_mutex_destroy(lock);

	#endif
}

void worker_mutex_lock(worker_mutex_t *lock)
{
	#ifdef _WIN32

	WaitForSingleObject(*lock, INFINITE);

	#else

	pthread_mutex_lock(lock);

	#endif
}

void worker_mutex_unlock(worker_mutex_t *lock)
{
	#ifdef _WIN32

	ReleaseMutex(*lock);

	#else

	pthread_mutex_unlock(lock);

	#endif
}
