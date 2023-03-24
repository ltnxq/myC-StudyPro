#include <ctime>
#include<time.h>
#include<sys/timeb.h>
#include<stdio.h>
#include<cstring>
#include<sstream>
#include<iostream>
#include<stdarg.h>

#include"AsyncLog.h"

#define MAX_LINE_LENGTH   256
#define DEFAULT_ROLL_SIZE 10*1024*1024

bool CAsyncLog::m_bTruncateLongLog = false;
FILE* CAsyncLog::m_hLogFile = NULL;
std::string CAsyncLog::m_strFileName = "default";
std::string CAsyncLog::m_strFileNamePID = "";
LOG_LEVEL CAsyncLog::m_nCurrentLevel = LOG_LEVEL::LOG_L