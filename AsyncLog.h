#ifndef __ASYNC_LOG_H
#define __ASYNC_LOG_H
#include<cstdio>
#include<string>
#include<list>
#include<thread>
#include<memory>
#include<mutex>
#include<condition_variable>



#define LOG_API

enum class LOG_LEVEL
{
	LOG_LEVEL_TRACE,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_INFO,
	LOG_LEVEL_WARNING,
	LOG_LEVEL_ERROR,   //用于业务错误
	LOG_LEVEL_SYSERROR,//用于技术框架本身错误
	LOG_LEVEL_FATAL,  //FATAL 级别的日志会让在程序输出日志后退出
	LOG_LEVEL_CRITICAL //CRITICAL  日志不受级别控制，总是输出
};


//宏不需要分号结尾
#define LOGT(...)    CAsyncLog::output(LOG_LEVEL::LOG_LEVEL_TRACE,__FILE__,__LINE__,__VA_ARGS__)
#define LOGD(...)    CAsyncLog::output(LOG_LEVEL::LOG_LEVEL_DEBUG,__FILE__,__LINE__,__VA_ARGS__)
#define LOGI(...)    CAsyncLog::output(LOG_LEVEL::LOG_LEVEL_INFO,__FILE__,__LINE__,__VA_ARGS__)
#define LOGW(...)    CAsyncLog::output(LOG_LEVEL::LOG_LEVEL_WARNING,__FILE__,__LINE__,__VA_ARGS__)
#define LOGE(...)    CAsyncLog::output(LOG_LEVEL::LOG_LEVEL_ERROR,__FILE__,__LINE__,__VA_ARGS__)
#define LOGSYSE(...)    CAsyncLog::output(LOG_LEVEL::LOG_LEVEL_SYSERROR,__FILE__,__LINE__,__VA_ARGS__)
#define LOGF(...)    CAsyncLog::output(LOG_LEVEL::LOG_LEVEL_FATAL,__FILE__,__LINE__,__VA_ARGS__);
#define LOGC(...)    CAsyncLog::output(LOG_LEVEL::LOG_LEVEL_CRITICAL,__FILE__,__LINE__,__VA_ARGS__)

//用于输出数据包的二进制格式
#define LOG_DEBUG_BIN(buf,buflength) CAsyncLog::outputBinary(buf,buflength)


class LOG_API CAsyncLog {
public:
	static bool init(const char* pszLogFileName = nullptr, bool m_bTruncateLongLine = false, int64_t nRollSize = 10 * 1024 * 1024);
	static void setLevel(LOG_LEVEL nlevel);
	static bool isRunning();

	//不输出线程ID号和所在函数签名，行号
	static bool output(long nlevel, const char* pszFmt, ...);
	//输出线程ID号和所在函数签名，行号
	static bool output(long nlevel, const char* pszFileName,int nLineNo,const char* pszFmt, ...);

	static bool outputBinary(unsigned char* buffer, size_t size);


private:
	CAsyncLog() = delete;
	~CAsyncLog() = delete;

	CAsyncLog(const CAsyncLog& rhs) = delete;
	CAsyncLog& operator=(const CAsyncLog& rhs) = delete;

	static void makeLinePrefix(long nLevel, std::string& strPrefix);
	static void getTime(char* pszTime, int nTimeStrLength);
	static bool createNewFile(const char* pszLogFileName);
	static bool writeToFile(const std::string& data);

	//
	static void crash();

	static const char* ullto4Str(int n);
	static char* formLog(int& index, char* szbuf, size_t size_buf, unsigned char* buffer, size_t size);
private:
		static bool          m_bToFile;
	    static FILE*         m_hLogFile;
		static std::string   m_strFileName;//日志文件名称
		static std::string   m_strFileNamePID;//对应的进程ID
		static bool          m_bTruncateLongLog;//长日志是否能截断
		static LOG_LEVEL     m_nCurrentLevel;//日志级别
		static int64_t       m_nFileRollSize;//单个日志文件的最大字节数
		static int64_t       m_nCurentWrittenSize;//已经写入的字节数
		static std::list<std::string> m_listLinesToWrite;//待写入的日志
		static std::unique_ptr<std::thread> m_spWriteThread;//处理日志的线程
		static std::mutex    m_mutexWrite;//独占锁
		static std::condition_variable   m_cvWrite;
		static bool          m_bExit;   //推出标致
		static bool          m_bRunning;//运行标志

};







#endif // 
