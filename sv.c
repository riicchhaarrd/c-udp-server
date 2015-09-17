#include "net.h"
#include <signal.h>
#include <time.h>
#include <sys/time.h>

int fps = 20;
unsigned long startMsec;

unsigned long get_milliseconds() {
	struct timeval tv;
	if(gettimeofday(&tv, NULL) != 0)
		return 0;
	return (unsigned long)((tv.tv_sec * 1000ul) + (tv.tv_usec / 1000ul));
}

int sockfd;

volatile sig_atomic_t signaled = 0;
volatile sig_atomic_t isRunning = 1;
 
void signal_int(int parm) {
	signaled = 1;
	isRunning = 0;
	printf("Shutting down server...\n");
}

void setup_signal_handlers() {
	signal(SIGINT, signal_int);
}

static unsigned long tMsec;

void update() {
	unsigned long nowMsec = get_milliseconds();
	
}

int main(int argc, char **argv) {
	unsigned short port = NET_PORT;
	
	for(int i = 0; i < argc; i++) {
		if(!strcmp(argv[i], "-port")) {
			port = (unsigned short)atoi(argv[i + 1]);
		} else if(!strcmp(argv[i], "-fps")) {
			fps = atoi(argv[i + 1]);
		}
	}
	
	
	setup_signal_handlers();
	
	
	sockfd = net_create_socket(port);
	
	if(-1 == sockfd) {
		printf("failed to bind the socket! is the port already in use?\n");
		return 0;
	}
	packet_t packet;
	startMsec = tMsec = get_milliseconds();
	
	for(;isRunning;) {
		if(!net_getpacket(sockfd, &packet)) {
			net_handlepacket(sockfd, &packet);
		}
		
		update();
		#define MILLI_TO_MICRO(x) (x * 1000)
		usleep(MILLI_TO_MICRO( 1000 / fps ));
	}
	net_close(sockfd);
	return EXIT_SUCCESS;
}