#ifndef __LOG_H__
#define __LOG_H__

#include <sstream>
#include <string>
#include <stdio.h>
#include <iomanip>
#include <stdarg.h>
#include <ThreadModel.h>
#define MAX_LOG_BUFFER_DEFAULT size_t(256)

#define __OUTPUT2STREAM__ //push output stream define macro before log.h

#if defined(__OUTPUT2FILE__)
#include "Output2File.h"
#elif defined(__OUTPUT2STREAM__)
#include "Output2Stream.h"
#elif defined(__OUTPUT2SOCKET__)
#include "Output2Socket.h"
#endif


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#   if defined (BUILDING_FILELOG_DLL)
#       define FILELOG_DECLSPEC   __declspec (dllexport)
#   elif defined (USING_FILELOG_DLL)
#       define FILELOG_DECLSPEC   __declspec (dllimport)
#   else
#       define FILELOG_DECLSPEC
#   endif // BUILDING_DBSIMPLE_DLL
#else
#   define FILELOG_DECLSPEC
#endif // _WIN32


#define FILE_NAME	(strrchr(__FILE__, 92) ? strrchr(__FILE__, 92) + 1: __FILE__)
#define TIME	(CurrentTime().c_str())

std::string CurrentTime();

enum TLogLevel { logNONE, logERROR, logWARNING, logINFO, logDEBUG, logDEBUG1, logDEBUG2, logDEBUG3, logDEBUG4};

template <typename OutputDevice>
class Log
{
private:
	std::ostringstream os;
	static TLogLevel	m_logLevel;
	static  std::string m_module_name;

	Log(const Log&);
	Log& operator =(const Log&) = delete;

public:
    Log()
	{
		os.imbue(std::locale(""));
	}

	virtual ~Log()
	{
		OutputDevice::Output(os.str());
	}

	void Print(TLogLevel level, const char* format, ...)
	{
		char in_buffer[MAX_LOG_BUFFER_DEFAULT] = { 0 };
		va_list arglist;
		va_start(arglist, format);
		vsprintf_s(in_buffer, sizeof(in_buffer), format, arglist);
		va_end(arglist);

		os << TIME << " "
			<< "Thread[" << std::this_thread::get_id() << "] "
			<< "Module[" << m_module_name.c_str() << "] "
			<< ToString(level).c_str() << " "
			<< FILE_NAME << " "
			<< __LINE__ << " "
			<< __FUNCTION__ << " "
			<< in_buffer
			<< '\n';
	}

public:
	static inline void SetLogLevel(TLogLevel level)
	{
		m_logLevel = level;
	}

	static inline TLogLevel GetLogLevel()
	{
		return m_logLevel;
	}

	static inline void SetModuleName(std::string module_name)
	{
		m_module_name = module_name;
	}

	static inline std::string GetModuleName()
	{
		return m_module_name;
	}

    static std::string ToString(TLogLevel level)
	{
		static const char* const buffer[] = { "NONE", "ERROR", "WARNING", "INFO", "DEBUG", "DEBUG1", "DEBUG2", "DEBUG3", "DEBUG4" };
		return buffer[level];
	}

    static TLogLevel FromString(const std::string& level)
	{
		if (level == "DEBUG4")
			return logDEBUG4;
		if (level == "DEBUG3")
			return logDEBUG3;
		if (level == "DEBUG2")
			return logDEBUG2;
		if (level == "DEBUG1")
			return logDEBUG1;
		if (level == "DEBUG")
			return logDEBUG;
		if (level == "INFO")
			return logINFO;
		if (level == "WARNING")
			return logWARNING;
		if (level == "ERROR")
			return logERROR;
		Log<T>().Get(logWARNING) << "Unknown logging level '" << level << "'. Using INFO level as default.";
		return logINFO;
	}
};

template <typename T>
TLogLevel Log<T>::m_logLevel = logNONE;

template <typename T>
std::string Log<T>::m_module_name = "unknow";
#if defined(__OUTPUT2FILE__)
typedef Log<Output2File> LogObject;
#elif defined(__OUTPUT2STREAM__)
typedef Log<Output2Stream> LogObject;
#elif defined(__OUTPUT2SOCKET__)
typedef Log<Output2Socket> LogObject;
#endif

//#define LOG_LEVEL(level) (LogObject::ToString(level).c_str())
#define LOG_PRINT(level, format, ...)	 \
    if (level > LogObject::GetLogLevel()) ;\
    else LogObject().Print(level, format, __VA_ARGS__)

#define LOG_ERROR(format, ...)	LOG_PRINT(logERROR ,format, __VA_ARGS__)
#define LOG_WARN(format, ...)	LOG_PRINT(logWARNING format, __VA_ARGS__)
#define LOG_INFO(format, ...)	LOG_PRINT(logINFO , format, __VA_ARGS__)
#define LOG_DEBUG(format, ...)	LOG_PRINT(logDEBUG ,format, __VA_ARGS__)
#define LOG_DEBUG1(format, ...) LOG_PRINT(logDEBUG1 format, __VA_ARGS__)
#define LOG_DEBUG2(format, ...) LOG_PRINT(logDEBUG2,format, __VA_ARGS__)
#define LOG_DEBUG3(format, ...) LOG_PRINT(logDEBUG3 format, __VA_ARGS__)
#define LOG_DEBUG4(format, ...) LOG_PRINT(logDEBUG4,format, __VA_ARGS__)

#define FUNC_IN(format, ...)	LOG_DEBUG4("[IN] " format, __VA_ARGS__)
#define FUNC_OUT(format, ...)	LOG_DEBUG4("[OUT] " format, __VA_ARGS__)


#endif //__LOG_H__
