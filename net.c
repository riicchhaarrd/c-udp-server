#include "net.h"

#include <sys/ioctl.h>

/* packet read functions */

int8_t pk_readint8(packet_t *p) {
	if(p->readsize + sizeof(int8_t) > p->bufsize)
		return 0;
	p->readsize += sizeof(int8_t);
	return *(int8_t*)(p->buf + p->readsize - sizeof(int8_t));
}

uint8_t pk_readuint8(packet_t *p) {
	if(p->readsize + sizeof(uint8_t) > p->bufsize)
		return 0;
	p->readsize += sizeof(uint8_t);
	return *(uint8_t*)(p->buf + p->readsize - sizeof(uint8_t));
}

uint32_t pk_readuint32(packet_t *p) {
	if(p->readsize + sizeof(uint32_t) > p->bufsize)
		return 0;
	p->readsize += sizeof(uint32_t);
	return ntohl(*(uint32_t*)(p->buf + p->readsize - sizeof(uint32_t)));
}

int32_t pk_readint32(packet_t *p) {
	if(p->readsize + sizeof(int32_t) > p->bufsize)
		return 0;
	p->readsize += sizeof(int32_t);
	return ntohl(*(int32_t*)(p->buf + p->readsize - sizeof(int32_t)));
}

int16_t pk_readint16(packet_t *p) {
	if(p->readsize + sizeof(int16_t) > p->bufsize)
		return 0;
	p->readsize += sizeof(int16_t);
	return ntohs(*(int16_t*)(p->buf + p->readsize - sizeof(int16_t)));
}

uint16_t pk_readuint16(packet_t *p) {
	if(p->readsize + sizeof(uint16_t) > p->bufsize)
		return 0;
	p->readsize += sizeof(uint16_t);
	return ntohs(*(uint16_t*)(p->buf + p->readsize - sizeof(uint16_t)));
}

void pk_copybuffer(packet_t *p, char *buf, size_t buflen) {
	if(p->readsize + buflen > p->bufsize)
		return;
	memcpy(buf, (void*)(p->buf + p->readsize), buflen);
	p->readsize += buflen;
}

char *pk_readbuffer(packet_t *p) {
	char *buf = (p->buf + p->readsize);
	p->readsize = p->bufsize;
	return buf;
}

/* packet write functions */

void pk_writebuffer(packet_t *p, char *buf, size_t buflen) {
	if(p->bufsize + buflen >= MAX_PACKET_BUF) {
		printf("pk_writebuffer: oversize\n");
		return;
	}
	memcpy(p->buf + p->bufsize, buf, buflen);
	p->bufsize += buflen;
}

void pk_writeint8(packet_t *pack, int8_t i) {
	if(pack->bufsize + sizeof(int8_t) >= MAX_PACKET_BUF)
		return;
	*(int8_t*)(pack->buf + pack->bufsize) = i;
	pack->bufsize += sizeof(int8_t);
}

void pk_writeuint8(packet_t *pack, uint8_t i) {
	if(pack->bufsize + sizeof(uint8_t) >= MAX_PACKET_BUF)
		return;
	*(uint8_t*)(pack->buf + pack->bufsize) = i;
	pack->bufsize += sizeof(uint8_t);
}

void pk_writeint16(packet_t *pack, int16_t i) {
	if(pack->bufsize + sizeof(int16_t) >= MAX_PACKET_BUF)
		return;
	*(int16_t*)(pack->buf + pack->bufsize) = htons(i);
	pack->bufsize += sizeof(int16_t);
}

void pk_writeuint16(packet_t *pack, uint16_t i) {
	if(pack->bufsize + sizeof(uint16_t) >= MAX_PACKET_BUF)
		return;
	*(uint16_t*)(pack->buf + pack->bufsize) = htons(i);
	pack->bufsize += sizeof(uint16_t);
}

void pk_writeint32(packet_t *pack, int32_t i) {
	if(pack->bufsize + sizeof(int32_t) >= MAX_PACKET_BUF)
		return;
	*(int32_t*)(pack->buf + pack->bufsize) = htonl(i);
	pack->bufsize += sizeof(int32_t);
}

void pk_writeuint32(packet_t *pack, uint32_t i) {
	if(pack->bufsize + sizeof(uint32_t) >= MAX_PACKET_BUF)
		return;
	*(uint32_t*)(pack->buf + pack->bufsize) = htonl(i);
	pack->bufsize += sizeof(uint32_t);
}

/* net functions */

int net_create_socket(unsigned short port) {
	int sock;
	
	sock = socket(AF_INET, SOCK_DGRAM,IPPROTO_UDP);
	
	if(socket == -1) {
		fprintf(stderr, "failed to create socket\n");
		exit(-1);
	}
	
	int opt = 1;
	if(-1 == ioctl(sock, FIONBIO, &opt)) {
		fprintf(stderr, "failed to set non-blocking socket\n");
		exit(-1);
	}
	
	
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htons(INADDR_ANY);
	addr.sin_port = htons(port);
	if(bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		fprintf(stderr, "failed to bind, is the port in use?");
		close(sock);
		return -1;
	}
	
	return sock;
}

int net_send(int sock, struct sockaddr_in *to, char *buf, size_t buflen) {
	int ret;
	
	ret = sendto(sock, buf, buflen, 0, (struct sockaddr*)to, sizeof(*to));
	return ret;
}

int net_handleoobpacket(int sock, packet_t *packet) {
	printf("got oob packet = %s\n", packet->buf);
	
	
	return 0;
}

packet_t *create_packet() {
	packet_t *p = NULL;
	p = (packet_t*)calloc(1, sizeof(packet_t));
	if(p == NULL) {
		fprintf(stderr, "failed to allocate memory for packet_t!");
		exit(-1);
	}
	return p;
}

int net_handlepacket(int sock, packet_t *packet) {
	if(packet->len < 4)
		return 0; //discard
	
	int op = pk_readint(packet);
	if(op == -1)
		return net_handleoobpacket(sock, packet);
	
	int cmd = pk_readint(packet);
	
	if(cmd == ecmd_invalid)
		return 1;
	
	switch(cmd) {
		case ecmd_heartbeat: {
			
			/* handle heartbeat or other commands here */
		} break;
	}
	
	/* send back a test response */
	autofree packet_t *r = create_packet();
	pk_writeint(r, -1);
	
	char *str = "hello world";
	pk_writebuffer(r, str, strlen(str));
	pk_writebyte(r, 0);
	
	printf("got packet %s\n", packet->buf);
	net_send(sock, &packet->addr, r->buf, r->bufsize);
	return 0;
}

int net_getpacket(int sock, packet_t *packet) {
	int ret;
	
	struct sockaddr_in from;
	int fromlen = sizeof(from);
	memset(packet, 0, sizeof(*packet));
	ret = recvfrom(sock, packet->buf, sizeof(packet->buf), 0, (struct sockaddr*)&from, &fromlen);
	
	if(ret == -1) {
		if(errno == EWOULDBLOCK)
			return 1;
	}
	packet->buf[ret] = 0;
	
	packet->len = ret;
	packet->addr = from;
	return 0;
}

int net_close(int sock) {
	/* maybe free here more later */
	close(sock);
}