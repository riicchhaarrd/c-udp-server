#include "stdheader.h"
#include <sys/socket.h>
#include <netinet/in.h>

#define NET_PORT 29645

#define MAX_PACKET_BUF 32768

typedef enum {
	/* add here commands */
	ecmd_invalid,
	ecmd_heartbeat
} e_cmd_type;

typedef struct {
	struct sockaddr_in addr;
	char buf[MAX_PACKET_BUF];
	size_t len;
	int readsize, bufsize;
} packet_t;

int net_create_socket(unsigned short);
int net_get_packet(int, packet_t*);
int net_send(int sock, struct sockaddr_in *to, char *buf, size_t buflen);
int net_handlepacket(int sock, packet_t *packet);


#define pk_readbyte pk_readuint8
#define pk_readint pk_readint32
#define pk_readshort pk_readint16
#define pk_readuint pk_readuint32
#define pk_readushort pk_readuint16

#define pk_writebyte pk_writeuint8
#define pk_writeint pk_writeint32
#define pk_writeshort pk_writeint16
#define pk_writeushort pk_writeuint16
#define pk_writeuint pk_writeuint32