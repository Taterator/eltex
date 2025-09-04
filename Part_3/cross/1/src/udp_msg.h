#ifndef UDP_MSG_H_
#define UDP_MSG_H_

int send_udp(int sock_fd, char *msg, struct sockaddr_in src, struct sockaddr_in dst);
int recieve_udp(int sock_fd, char *msg, struct sockaddr_in *src, const struct sockaddr_in dst);

#endif