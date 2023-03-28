#include <cstdio>
#include<stdarg.h>
#include<unistd.h>
#include "AsyncLog.h"

int sum(int, ...);

int main()
{
    printf("hello from %s!\n", "myChat");
    std::string logFileFullPath = "D:\\zyz\\test";
    CAsyncLog::init(logFileFullPath.c_str(),true,16);

    LOGI("fileserver initialization completed, now you can use client to connect it.");

    LOGI("fileserver initialization again, now you can use client to connect it.");


    while (true)
    {
        sleep(2);
    }
    

    return 0;
}


int sum(int num_args, ...)
{
    int val = 0;
    va_list ap;
    int i;

    //va_start初始化ap，后面参数是num_args
    va_start(ap, num_args);

    //va_arg获取对应的参数
    for ( i = 0; i < num_args; i++)
    {
        val += va_arg(ap, int);
    }
    
    va_end(ap);

    return val;
}