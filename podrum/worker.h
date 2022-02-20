/*
                   Podrum R3 Copyright MFDGaming & PodrumTeam
                 This file is licensed under the GPLv2 license.
              To use this file you must own a copy of the license.
                       If you do not you can get it from:
            http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
 */

#ifndef PODRUM_WORKER_H
#define PODRUM_WORKER_H

#ifdef _WIN32

#include <windows.h>

#else

#include <pthread.h>

#endif

#ifdef _WIN32

typedef HANDLE worker_t;
#define RETURN_WORKER_EXECUTOR DWORD WINAPI
#define ARGS_WORKER_EXECUTOR LPVOID
typedef DWORD (WINAPI *worker_executor_t)(LPVOID argvp);

#else

typedef pthread_t worker_t;
#define RETURN_WORKER_EXECUTOR void *
#define ARGS_WORKER_EXECUTOR void *
typedef void *(*worker_executor_t)(void *argv);

#endif

worker_t create_worker(worker_executor_t executor, void *args);

void join_worker(worker_t worker);

#endif