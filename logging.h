#ifndef LOGGING_H
#define LOGGING_H

#include "loggingdef.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>

void addlog(const char * logtext,const char *logpath);
void Log_On_Socket(_LogInfo *LogInfo,int loglevel,char *fmt,...);
int LOGGING(const char *logpath,_LogInfo *LogInfo,int loglevel,char *fmt,...);
#endif
