#include "logging.h"

int main()
{
    addlog("HELLO WORLD !!!","/root/ehsantest.log");
    LOGGING(NULL,NULL,0,"HELLO WORLD !!!!");
    _LogInfo LogInfo;
    LogInfo.logenable=1;
    LogInfo.loglevel=2;
    LogInfo.socklogfd=1;
    LOGGING(NULL,&LogInfo,0,"HELLO WORLD %d !!!!",1);
    LOGGING(NULL,&LogInfo,2,"HELLO WORLD %d !!!!",2);
    LOGGING(NULL,&LogInfo,1,"HELLO WORLD %d !!!!",3);
    LogInfo.socklogfd=2;
    LOGGING("/root/ehsantest.log",&LogInfo,1,"HELLO WORLD %d !!!!",3);
    LOGGING(NULL,&LogInfo,1,"HELLO WORLD %d !!!!",3);
    return 0;
}

