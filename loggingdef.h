#ifndef LOGGINGDEF_H
#define LOGGINGDEF_H

typedef struct _LogInfo
{
    int socklogfd;
    unsigned short int logenable:1;
    unsigned short int loglevel:4;
    long double	lastdeadtime;
}_LogInfo;


#endif
