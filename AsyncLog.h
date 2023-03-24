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
	LOG_LEVEL_ERROR,   //����ҵ�����
	LOG_LEVEL_SYSERROR,//���ڼ�����ܱ������
	LOG_LEVEL_FATAL,  //FATAL �������־�����ڳ��������־���˳�
	LOG_LEVEL_CRITICAL //CRITICAL  ��־���ܼ�����ƣ��������
};


//�겻��Ҫ�ֺŽ�β
#define LOGT(...)    CAsyncLog::output(LOG_LEVEL::LOG_LEVEL_TRACE,__FILE__,__LINE__,__VA_ARGS__)
#define LOGD(...)    CAsyncLog::output(LOG_LEVEL::LOG_LEVEL_DEBUG,__FILE__,__LINE__,__VA_ARGS__)
#define LOGI(...)    CAsyncLog::output(LOG_LEVEL::LOG_LEVEL_INFO,__FILE__,__LINE__,__VA_ARGS__)
#define LOGW(...)    CAsyncLog::output(LOG_LEVEL::LOG_LEVEL_WARNING,__FILE__,__LINE__,__VA_ARGS__)
#define LOGE(...)    CAsyncLog::output(LOG_LEVEL::LOG_LEVEL_ERROR,__FILE__,__LINE__,__VA_ARGS__)
#define LOGSYSE(...)    CAsyncLog::output(LOG_LEVEL::LOG_LEVEL_SYSERROR,__FILE__,__LINE__,__VA_ARGS__)
#define LOGF(...)    CAsyncLog::output(LOG_LEVEL::LOG_LEVEL_FATAL,__FILE__,__LINE__,__VA_ARGS__);
#define LOGC(...)    CAsyncLog::output(LOG_LEVEL::LOG_LEVEL_CRITICAL,__FILE__,__LINE__,__VA_ARGS__)

//����������ݰ��Ķ����Ƹ�ʽ
#define LOG_DEBUG_BIN(buf,buflength) CAsyncLog::outputBinary(buf,buflength)


class LOG_API CAsyncLog {
public:
	static bool init(const char* pszLogFileName = nullptr, bool m_bTruncateLongLine = false, int64_t nRollSize = 10 * 1024 * 1024);
	static void setLevel(LOG_LEVEL nlevel);
	static bool isRunning();

	//������߳�ID�ź����ں���ǩ�����к�
	static bool output(long nlevel, const char* pszFmt, ...);
	//����߳�ID�ź����ں���ǩ�����к�
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
		static std::string   m_strFileName;//��־�ļ�����
		static std::string   m_strFileNamePID;//��Ӧ�Ľ���ID
		static bool          m_bTruncateLongLog;//����־�Ƿ��ܽض�
		static LOG_LEVEL     m_nCurrentLevel;//��־����
		static int64_t       m_nFileRollSize;//������־�ļ�������ֽ���
		static int64_t       m_nCurentWrittenSize;//�Ѿ�д����ֽ���
		static std::list<std::string> m_listLinesToWrite;//��д�����־
		static std::unique_ptr<std::thread> m_spWriteThread;//������־���߳�
		static std::mutex    m_mutexWrite;//��ռ��
		static std::condition_variable   m_cvWrite;
		static bool          m_bExit;   //�Ƴ�����
		static bool          m_bRunning;//���б�־

};







#endif // 
