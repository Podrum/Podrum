/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#include "./worker.h"

worker_t create_worker(worker_executor_t executor)
{
	#ifdef _WIN32
	DWORD thread_id;

	return CreateThread(NULL, 0, executor, NULL, 0, &thread_id);

	#else

	worker_t worker;
	pthread_create(&worker, NULL, executor, NULL);
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