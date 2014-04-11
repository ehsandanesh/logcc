#include <pthread.h>
#include <time.h>
#include "logging.h"
#include <sys/socket.h>
int sendall(int sockfd, char *buf, int *len)
{
    int total = 0;        // how many bytes we've sent
    int bytesleft = *len; // how many we have left to send
    int n=0;
    while(total < *len)
    {
        if ( sockfd ==1 || sockfd ==2)
        {
            n = write(sockfd, buf+total, bytesleft);
        }
        else
        {
            n = send(sockfd, buf+total, bytesleft, MSG_NOSIGNAL|MSG_DONTWAIT);
        }
        if (n == -1) { break; }
        total += n;
        bytesleft -= n;
    }

    *len = total; // return number actually sent here

    return n==-1?-1:0; // return -1 on failure, 0 on success
}
void Get_Time_From_tm(char *curr_time,struct tm *timeinfo)
{
    sprintf(curr_time,"%d-%d-%d %d:%d:%d",timeinfo->tm_year+1900
                                        ,timeinfo->tm_mon+1
                                        ,timeinfo->tm_mday
                                        ,timeinfo->tm_hour
                                        ,timeinfo->tm_min
                                        ,timeinfo->tm_sec);
}

void Get_Time_From_time_t(char *curr_time,time_t *rawtime)
{
    struct tm *timeinfo;
    timeinfo = localtime (rawtime);
    Get_Time_From_tm(curr_time,timeinfo);

}

void addlog(const char * logtext,const char *logpath)
{
    struct tm *timeinfo;
    time_t rawtime;
    if (logpath != NULL )
    {

        if (strlen(logpath) > 4)
        {
            FILE *Lfile;
            Lfile = fopen(logpath,"a");
            if (Lfile)
            {
                char cur_dtime[50];
                char strin2[2048];
                time ( &rawtime );
                timeinfo = localtime (&rawtime);
                strcpy(cur_dtime,asctime(timeinfo));
                cur_dtime[strlen(cur_dtime)-1] = '\0';
                sprintf(strin2,"%s:: %d ::  %s;\r\n",cur_dtime,(unsigned int)pthread_self(),logtext);
                fputs(strin2,Lfile);
                fclose(Lfile);
            }
            else
            {
                fprintf(stderr,"can not open log file %s\n",logpath);
            }
        }
    }
}

void Log_On_Socket(_LogInfo *LogInfo,int loglevel,char *fmt,...)
{

    if (LogInfo != NULL)
    {
#ifdef TESTING
        LogInfo->logenable=1;
#endif

        if (LogInfo->logenable==1)
        {
            char strin[2048];
            char strin2[2048];
            va_list arg;
            time_t rawtime;
            char cur_dtime[50];
#ifndef TESTING
            int length=0;
#endif
            memset(strin,0,sizeof(strin));
            memset(strin2,0,sizeof(strin2));

            va_start(arg, fmt);
            vsprintf(strin, fmt, arg);
            va_end(arg);
#ifdef TESTING
            loglevel=-1;
#endif
            if (LogInfo->loglevel>=loglevel)
            {
                memset(cur_dtime,0,sizeof(cur_dtime));
                time ( &rawtime );
                Get_Time_From_time_t(cur_dtime,&rawtime);
                sprintf(strin2,"%s:: %d ::  %s;\r\n",cur_dtime,(unsigned int)pthread_self(),strin);
#ifndef TESTING
                length=strlen(strin2);
#endif

#ifdef TESTING
                if(loglevel==500)
                {
                    fprintf(stdout,"%s",strin2);
                }
                else
                {
                    fprintf(stderr,"%s",strin2);
                }
#else
                if (sendall(LogInfo->socklogfd,strin2,&length)<0)
                {
                    close(LogInfo->socklogfd);
                    LogInfo->logenable=0;
                }
#endif
            }
        }
    }
#ifdef TESTING
    else
    {
        char strin[2048];
        char strin2[2048];
        va_list arg;
        time_t rawtime;
        char cur_dtime[50];
        memset(strin,0,sizeof(strin));
        memset(strin2,0,sizeof(strin2));

        va_start(arg, fmt);
        vsprintf(strin, fmt, arg);
        va_end(arg);
        memset(cur_dtime,0,sizeof(cur_dtime));
        time ( &rawtime );
        Get_Time_From_time_t(cur_dtime,&rawtime);

        sprintf(strin2,"%s:: %d ::  %s;\r\n",cur_dtime,(unsigned int)pthread_self(),strin);
        if(loglevel==500)
        {
            fprintf(stdout,"%s",strin2);
        }
        else
        {
            fprintf(stderr,"%s",strin2);
        }
    }
#endif
}

int LOGGING(const char *logpath,_LogInfo *LogInfo,int loglevel,char *fmt,...)
{
    char logtext[1024];
    va_list arg;
    memset(logtext,0,sizeof(logtext));

    va_start(arg, fmt);
    vsprintf(logtext, fmt, arg);
    va_end(arg);

#ifndef TESTING
    if (LogInfo == NULL)
    {
        if ( logpath != NULL )
        {
            addlog(logtext,logpath);
        }
        else
        {
            addlog(logtext,"/dev/stderr");
        }
    }
    else
    {
        Log_On_Socket(LogInfo,loglevel,logtext);
    }
#else
        Log_On_Socket(LogInfo,loglevel,logtext);
#endif

    return 0;
}
