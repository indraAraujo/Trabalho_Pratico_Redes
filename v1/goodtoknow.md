Funções e coisas novas que fui aprendendo enquanto pesquisava para fazer esse projeto funcionar: 


- sem_init() - initialise an unnamed semaphore (REALTIME)

```
#include <semaphore.h>

int sem_init(sem_t *sem, int pshared, unsigned int value);
```

- socket - create an endpoint for communication

```
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
```

- struct sockaddr_in, struct in_addr

Structures for handling internet addresses

```
#include <netinet/in.h>

struct sockaddr_in {
    short            sin_family;   // e.g. AF_INET
    unsigned short   sin_port;     // e.g. htons(3490)
    struct in_addr   sin_addr;     // see struct in_addr, below
    char             sin_zero[8];  // zero this if you want to
};

struct in_addr {
    unsigned long s_addr;  // load with inet_aton()
};
```

- bzero, explicit_bzero - zero a byte string

```
#include <strings.h>

void bzero(void *s, size_t n);

#include <string.h>

void explicit_bzero(void *s, size_t n);
```

- bind - bind a name to a socket

```
#include <sys/socket.h>

int bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
```

- listen - listen for connections on a socket

```
#include <sys/socket.h>

int listen(int sockfd, int backlog);
```