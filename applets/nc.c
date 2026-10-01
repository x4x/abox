/*
** nc.c
**
** A custom find by x4x
** 20260917 x4x
**
** nc [Options] [ADDRESS] [PORT]
** -u  use udp
** -l  server mode, ADDRESS can be blank for default ip
** -4  use IPv4
** -6  use IPv6
*/
//#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <poll.h>

#include "applets.h"

#define BUFERSIZE 4096

struct Flags {
    const char* ip_address;
    const char* port;
    int mode;
    int sockmode;
    int famely;
    bool debug;
};

static struct Flags flags= {
    .ip_address = NULL,
    .port       = NULL,
    .mode       = 0,
    .sockmode   = SOCK_STREAM,
    .famely     = AF_UNSPEC,
    .debug      = false,
};

void nc_print_help(char* app_name) {
    printf("%s [Options] [ADDRESS] [PORT]\n", app_name);
    printf("-l  server mode, ADDRESS can be blank for default ip\n");
    printf("-4  use IPv4\n");
    printf("-6  use IPv6\n");
}

int relay_data(int sock_fd, unsigned char *buffer) {
    struct pollfd fds[2];
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;
    
    fds[1].fd = sock_fd;
    fds[1].events = POLLIN;

    ssize_t buf_n = 0;  
        
    while(true) {
        if (poll(fds, 2,-1) == -1) {
            fprintf(stderr, "poll error\n");
            return 1;
        }

        if(fds[0].revents & (POLLIN | POLLHUP)) {
            // keyboard action
            if((buf_n = read(STDIN_FILENO, buffer, BUFERSIZE)) > 0 ) {
                for(ssize_t sentacum = 0; buf_n > sentacum;) {
                    ssize_t sent = send(sock_fd, buffer + sentacum, (size_t)buf_n - sentacum, 0);
                    if( sent == -1 ) {
                        fprintf(stderr, "send error\n");
                        return 1;
                    }
                    sentacum += sent;
                }
            } else if(buf_n == 0) {
                // stdin is EOF. file or pipe is finisched
                if(shutdown(sock_fd, SHUT_WR) == -1) {
                    fprintf(stderr, "shutdown error\n");
                    return 1;
                }
                fds[0].fd = -1; // stop polling
            } else {
                fprintf(stderr, "read error\n");
                return 1;
            }
        }

        if(fds[1].revents & POLLIN) {
            // net. clinet action
            buf_n = recv(sock_fd, buffer, BUFERSIZE, 0);
            
            if(buf_n == -1) {
                fprintf(stderr, "recv error\n");
                return 1;
            }
            if ( buf_n == 0) {
                fprintf(stderr, "peer disconnected\n");
                return 1;
            }

            fwrite(buffer, 1, (size_t)buf_n, stdout);
            fflush(stdout);
        }
    }
    return 0;
}

int nc_main(int argc, char** argv) {
    int relay_result = 0;
    int active_arg = 1;

    // options
    if(argc > 1) {
        while(argc > active_arg &&
              argv[active_arg][0] == '-') {
            for (int i=1; argv[active_arg][i] != '\0'; i++ ) {
                if (argv[active_arg][i] == 'u') {
                    flags.sockmode = SOCK_DGRAM;
                }else if(argv[active_arg][i] == 'l') {
                    flags.mode = AI_PASSIVE;
                }else if (argv[active_arg][i] == '4') {
                    if(flags.famely == AF_UNSPEC) {
                        flags.famely = AF_INET;
                    } else {
                        fprintf(stderr, "error force both ipv6 and 4\n");
                        return 1;
                    }
                }else if (argv[active_arg][i] == '6') {
                    if(flags.famely == AF_UNSPEC) {
                        flags.famely = AF_INET6;
                    } else {
                        fprintf(stderr, "error force both ipv4 and 6\n");
                        return 1;
                    }
                }else if(argv[active_arg][i] == 'v') {
                    flags.debug = true;
                }else if(argv[active_arg][i] == 'h') {
                    nc_print_help(argv[0]);
                    return 0;
                } else {
                    fprintf(stderr, "Unknown option: -%c\n", argv[active_arg][i]);
                    nc_print_help(argv[0]);
                    return 1;
                }
            }
            active_arg++;
        }

        if(argc > active_arg +1 ) {  // ip and port param
            flags.ip_address = argv[active_arg];
            flags.port = argv[++active_arg];
        } else if (argc > active_arg) { // just port given
            if (flags.mode == AI_PASSIVE) {  // server mode
                flags.port = argv[active_arg];
            } else {
                fprintf(stderr, "Not enuth arguments!\n");
                return 1;
            }
        } else {
            fprintf(stderr, "Not enouth arguments!\n");
            nc_print_help(argv[0]);
            return 1;
        }
    } else {
        nc_print_help(argv[0]);
        return 0;
    }


    // start connection
    static unsigned char buffer[BUFERSIZE];

    // getaddrinfo
    struct addrinfo hints;
    struct addrinfo *result = NULL;

    memset(&hints, 0, sizeof(hints));
    
    hints.ai_family = flags.famely;
    hints.ai_socktype = flags.sockmode;
    hints.ai_flags = flags.mode;

    int rc = getaddrinfo(flags.ip_address, flags.port, &hints, &result);

    if(rc != 0) {
        fprintf(stderr, "address error: %s\n", gai_strerror(rc));
        return 1;
    }

    if(flags.debug) {
        printf("resolved: address=%s port=%s family=%d socket_type=%d flags=%d\n",
               flags.ip_address ? flags.ip_address : "(wildcard)",
               flags.port,
               hints.ai_family,
               hints.ai_socktype,
               hints.ai_flags);

        printf("result: family=%d socktype=%d protocol=%d\n",
               result->ai_family,
               result->ai_socktype,
               result->ai_protocol);
    }

    // socket
    int listen_fd = socket(result->ai_family,
                           result->ai_socktype,
                           result->ai_protocol);
    if(listen_fd == -1) {
        fprintf(stderr, "sockat error\n");
        freeaddrinfo(result);
        return 1;
    }

    if(flags.sockmode == SOCK_DGRAM ) { // udp mode
        printf("udp not implemented\n");
        // TDDO: implement udp server
        // TODO: implement udp client
    } else { // normal/tcp mode
        if(flags.mode == AI_PASSIVE ) {  // server mode
            // setsockopt
            int enable = 1;
            if(setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) == -1) {
                perror("setsockopt");
                close(listen_fd);
                freeaddrinfo(result);
                return 1;
            }

            // bind
            if(bind(listen_fd, result->ai_addr, result->ai_addrlen) == -1) {
                perror("bind");
                close(listen_fd);
                freeaddrinfo(result);
                return 1;
            }

            // listener
            if(listen(listen_fd, 16) == -1) {
                perror("listen");
                close(listen_fd);
                freeaddrinfo(result);
                return 1;
            }

            //accept client
            struct sockaddr_storage client_address;
            socklen_t client_address_lenth = sizeof(client_address);

            int client_fd = accept(
                listen_fd,
                (struct sockaddr*)&client_address,
                &client_address_lenth);

            if(client_fd == -1) {
                perror("accept");
                close(listen_fd);
                freeaddrinfo(result);
                return 1;            
            }

            // start tcp server mode
            relay_result = relay_data(client_fd, buffer);
        } else {
            // tcp cleint mode
            if(connect(listen_fd, result->ai_addr, result->ai_addrlen) == -1) {
                perror("connect");
                close(listen_fd);
                freeaddrinfo(result);
                return 1;
            }
        
            relay_result = relay_data(listen_fd, buffer);
        }
    }
    
    close(listen_fd);
    freeaddrinfo(result);
    return relay_result;
}
