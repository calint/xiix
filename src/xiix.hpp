#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/epoll.h>

namespace xiix{

// example from http://asistobe851.web.fc2.com/C_and_Cpp/epoll-client.html

void
setNonblocking ( int sock ) {
    int flag = fcntl ( sock, F_GETFL, 0 );
    fcntl ( sock, F_SETFL, flag | O_NONBLOCK );
}

int
getSocket ( char *hostname, int portNumber ) {

    struct sockaddr_in addr;
    struct hostent *he;
    int sock;

    if ( hostname == NULL ) {
        perror ( "host is undefined!" );
        exit ( 1 );
    }

    if ( ( sock = socket ( AF_INET, SOCK_STREAM, 0 ) ) < 0 ) {
        perror ( "socket" );
        exit ( 2 );
    }

    bzero ( ( char * ) &addr, sizeof ( addr ) );

    if ( ( he = gethostbyname ( hostname ) ) == NULL ) {
        perror ( "No such host" );
        exit ( 3 );
    }
    bcopy ( he->h_addr, &addr.sin_addr, he->h_length );
    addr.sin_family = AF_INET;
    addr.sin_port = htons ( portNumber );

    if ( connect ( sock, ( struct sockaddr * ) &addr, sizeof ( addr ) ) < 0 ) {
        perror ( "connect" );
        exit ( 4 );
    }

    return sock;
}

int
main (int argc,char **argv ) {
    char *host;
    int port;
    char *requestURI;
    int i = 0;
//    int maxClients = 0;

    if ( argc != 5 ) {
        fprintf ( stderr, "Usage: %s [host] [port] [requestURI] [maxClients]\n", argv[0] );
        return -1;
    }
    host = argv[1];
    port = atoi ( argv[2] );
    requestURI = argv[3];
//    maxClients = atoi(argv[4]);
    const int maxClients = 64;

    //--------------------------------------------------
    // create epoll's descriptors
    //--------------------------------------------------
    int epfd;
    if ( ( epfd = epoll_create ( maxClients ) ) < 0 ) {
        perror ( "epoll_create" );
        exit ( 1 );
    }

    //--------------------------------------------------
    // create as many socket handles as needed
    //--------------------------------------------------
    int sock[maxClients];
    for ( i = 0; i < maxClients; i++ ) {
        sock[i] = getSocket ( host, port );
        setNonblocking ( sock[i] );

        fprintf ( stderr, "... socket handle created ... \n" );
    }

    //--------------------------------------------------
    // add all the sockets into epoll
    //--------------------------------------------------
    struct epoll_event ev;
    struct epoll_event events[maxClients];

    for ( i = 0; i < maxClients; i++ ) {
        memset ( &ev, 0, sizeof ev );
        ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
        ev.data.fd = sock[i];
        if ( epoll_ctl ( epfd, EPOLL_CTL_ADD, sock[i], &ev ) < 0 ) {
            fprintf ( stderr, "epoll_ctl()\n" );
            exit ( 2 );
        }
    }

    //--------------------------------------------------------------
    // wait for the available status of each epoll descriptor
    //--------------------------------------------------------------
    int nfd;
    while ( 1 ) {
        nfd = epoll_wait ( epfd, events, maxClients, -1 );

        for ( i = 0; i < nfd; ++i ) {
            fprintf ( stderr, "*** now polling *** i=%d\n", i );

            int client = events[i].data.fd;
            char message[1024];

            if ( events[i].events & EPOLLOUT ) {
                sprintf ( message, "GET %s HTTP/1.1\r\nHost: %s:%d\r\n\r\n",
                          requestURI, host, port );
                int write_len = write ( client, message, strlen ( message ) );
                printf ( "write_len=%d\n", write_len );
            }

            if ( events[i].events & EPOLLIN ) {
                memset ( message, 0, sizeof message );
                int read_len = read ( client, message, sizeof message );
                printf ( "read_len=%d\n", read_len );

                // detached
                epoll_ctl ( epfd, EPOLL_CTL_DEL, client, &ev );
                // close socket
                close ( client );

                // add new descriptor
                int newSock = getSocket ( host, port );
                setNonblocking ( newSock );

                memset ( &ev, 0, sizeof ev );
                ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
                ev.data.fd = newSock;
                if ( epoll_ctl ( epfd, EPOLL_CTL_ADD, newSock, &ev ) < 0 ) {
                    fprintf ( stderr, "epoll_ctl()\n" );
                    exit ( 3 );
                }
            }

        }
    }
    return 0;
}


class client{
public:
	client(int argc,char**argv,char**env){
		auto p=argv;
		for(int i=0;i<argc;i++)
			puts(*p++);
		main(argc,argv);
	}
};
}
