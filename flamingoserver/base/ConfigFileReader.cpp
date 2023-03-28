#include "ConfigFileReader.h"

#include<cstdio>
#include<cstring>

CConfigFileReader::CConfigFileReader(const char* filename)
{
	loadFile(filename);
}

CConfigFileReader::~CConfigFileReader(){}

char* CConfigFileReader::getConfigName(const char* name) 
{
	if (!m_load_ok)
		return NULL;
	char* value = nullptr;

	//如果m_config_map是const 的  那么返回的是const_iterator
	//iterator find( const Key& key );
	//const_iterator find( const Key& key ) const;
	std::map<std::string,std::string>::iterator it = m_config_map.find(name);

	//返回的是const char*,返回的是不可修改的部分
	if (it != m_config_map.end())
		value =const_cast<char*>(it->second.c_str());

	return value;

}

int CConfigFileReader::setConfigValue(const char* name, const char* value)
{
	if (!m_load_ok)
		return -1;

	std::map<std::string, std::string>::iterator it = m_config_map.find(name);
	if (it != m_config_map.end())
		it->second = value;
	else
		m_config_map.insert(std::make_pair(name, value));

	return writeFile();
}

void CConfigFileReader::loadFile(const char* filename)
{
	m_config_file.clear();
	m_config_file.append(filename);
	FILE* fp = fopen(filename, "r");
	if (!fp) return;

	char buf[256];
	for (;;)
	{
		char* p = fgets(buf, 256, fp);
		if (!p) break;
		size_t len = strlen(buf);

		//remove string start with \n
		if (buf[len - 1] == '\n')
			buf[len - 1] = 0;

		//查找第一次出现 #的位置
		//应该是注释
		char* ch = strchr(buf, '#');
		if (ch) *ch = 0;
		if (strlen(buf) == 0)
			continue;

		//解析这行配置
		parseLine(buf);
	}

	fclose(fp);
	m_load_ok = true;
}

void CConfigFileReader::parseLine(char* line)
{
	char* p = strchr(line, '=');
	if (p == nullptr)
		return;

	*p = 0;
	char* key = trimSpace(line);
	char* value = trimSpace(p + 1);

	if (key && value)
		m_config_map.insert(std::make_pair(key, value));
}

char* CConfigFileReader::trimSpace(char* name)
{
	//remove starting space or tab or 回车

	char* start_pos = name;
	while ((*start_pos == ' ') || (*start_pos == '\t') ||(*start_pos == '\r')) {
		start_pos++;
	}

	//如果字符串长度为0
	if (strlen(start_pos) == 0)
		return nullptr;

	//remove end space or tab
	char* end_pos = name + strlen(name) - 1;
	while ((*end_pos == ' ') || (*end_pos == '\t') || (*end_pos == '\r')) {
		*end_pos = 0;
		end_pos--;
	}

	int len = end_pos - start_pos + 1;
	if (len <= 0) return nullptr;

	return start_pos;
}

int CConfigFileReader::writeFile(const char* filename)
{
	FILE* fp = nullptr;
	if (filename == nullptr)
		fp = fopen(m_config_file.c_str(), "w");
	else
		fp = fopen(filename, "w+");

	if (nullptr == fp)
		return -1;

	char szPaire[128];
	std::map<std::string, std::string>::iterator it = m_config_map.begin();
	for (; it != m_config_map.end(); it++) {
		//初始化为0
		memset(szPaire, 0, sizeof(szPaire));
		snprintf(szPaire, sizeof(szPaire), "%s=%s\n", it->first.c_str(), it->second.c_str());
		size_t ret = fwrite(szPaire, strlen(szPaire), 1, fp);
		if (ret != 1) {
			fclose(fp);
			return -1;
		}
	}
	fclose(fp);
	return 0;
}