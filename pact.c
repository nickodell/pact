#define _POSIX_SOURCE
#define _POSIX_C_SOURCE 199309L

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>


#ifdef PRINT_DEBUG
#define DEBUG(...) fprintf(stdout, __VA_ARGS__)
#else
#define DEBUG(...)
#endif
#define ERROR(...) fprintf(stderr, __VA_ARGS__)

struct child_proc {
	pid_t pid;
	bool monitorProc;
	bool killProc;
	bool procChanged;
};

bool killAll = false;

void on_SIGTERM(int signal) {
	killAll = true;
}


int main(int argc, char *argv[]) {
	// Trap SIGTERM
	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = on_SIGTERM;
	sigaction(SIGTERM, &action, NULL);

	int numProcs = argc - 1;
	// Check the arguments are sane and that every argument corresponds
	// to a process
	if(argc < 2) {
		ERROR("You must give pact some PID's!\n");
		ERROR("Usage: ");
		ERROR(argv[0]);
		ERROR(" [K/M]PID ...\n");
		return 42;
	}
	struct child_proc procs[numProcs];
	memset(&procs, 0, sizeof(procs));
	for(int i = 0; i < numProcs; i++) {
		char* currentArg = argv[i + 1];
		int pid;
		char modifier;
		int ret;
		bool hasModifier = !isdigit(currentArg[0]);
		if(hasModifier) {
			DEBUG("Arg has modifier\n");
			modifier = currentArg[0];
			currentArg++;
			ret = sscanf(currentArg, "%d", &pid);
			switch (modifier) {
			case 'K':
				procs[i].monitorProc = false;
				procs[i].killProc = true;
				break;
			case 'M':
				procs[i].monitorProc = true;
				procs[i].killProc = false;
				break;
			default:
				ERROR("Unrecognized modifier %c\n", modifier);
				break;
			}
		} else {
			DEBUG("Arg has no modifier\n");
			ret = sscanf(currentArg, "%d", &pid);
			procs[i].monitorProc = true;
			procs[i].killProc = true;
		}
		if(ret != 1) {
			ERROR("Error - \"%s\" is not a valid argument.\n", currentArg);
			killAll = true;
		} else {
			procs[i].pid = pid;

		}
	}
	// Fork and let the child handle the work
	int ret = fork();
	if(ret > 0) {
		exit(0);
	} else if(ret < 0) {
		ERROR("Cannot fork()\n");
	} else if(ret == 0) {
		// We're the child, continue with the program
	}
	// Wait for one of those processes to die
	while(!killAll) {
		for(int i = 0; i < numProcs; i++) {
			pid_t pid = procs[i].pid;
			if(kill(pid, 0) == -1) {
				// Process no longer exists, don't kill it
				// later
				procs[i].procChanged = true;
				// Don't start killing processes if we got
				// the K modifier for this process
				if(procs[i].monitorProc) {
					killAll = true;
					break;
				}
			}
		}
		if(!killAll) {
			DEBUG("Sleeping\n");
			struct timespec ts;
			ts.tv_sec = 1;
			ts.tv_nsec = 0;
			nanosleep(&ts, NULL);
		}
	}
	// Kill all other processes
	for(int i = 0; i < numProcs; i++) {
		pid_t pid = procs[i].pid;
		if(!procs[i].killProc || procs[i].procChanged || pid == 0) {
			continue;
		}
		DEBUG("Killing %d\n", pid);
		kill(pid, SIGTERM);
	}
	return 0;
}
