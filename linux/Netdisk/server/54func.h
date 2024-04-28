#ifndef __54FUNC__
#define __54FUNC__
#include <stdio.h>
#include <mysql/mysql.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <syslog.h>
#include <signal.h>
#include <sys/time.h>
#include <pthread.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/sendfile.h>
#include <shadow.h>
#include <crypt.h>
#include <mysql/mysql.h>
#include <openssl/evp.h>
#include <l8w8jwt/decode.h>
#include <l8w8jwt/encode.h>
#define ARGS_CHECK(argc,num) {if((argc) != num) {fprintf(stderr,"argc error!\n");exit(-1);}}
#define THREAD_ERROR_CHECK(ret,msg) {if(ret != 0) {fprintf(stderr,"%s:%s\n",msg,strerror(ret));exit(-1);}}

#endif
