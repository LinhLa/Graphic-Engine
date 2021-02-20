#ifndef __LOG_H__
#define __LOG_H__
/**
* @file log.h
*
* @brief
*
* @ingroup LogFramework.lib
* (Note: this needs exactly one @defgroup somewhere)
*
* @author Linh la
* Contact: lamanhlinh2000@yahoo.com
*
*/
#include <sstream>
#include <string>
#include <stdio.h>
#include <iomanip>
#include <stdarg.h>
#include <thread>
#include "ThreadModel.h"
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

enum TLogLevel:uint8_t { FATAL = 0, ERROR, WARN, INFO, DEBUG, TRACE, ALL, OFF };

template <typename OutputDevice>
class Log
{
private:
	std::ostringstream os;
	static TLogLevel	m_logLevel;
	static  std::string m_module_name;
	static const char* const m_log_dictionary[8];
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


	/**
	 * @brief      Write message log to output string stream, the message will be truncate if longer then MAX_LOG_BUFFER_DEFAULT
	 *
	 * @param[in]  level      The level
	 * @param[in]  format     The format
	 * @param[in]  <unnamed>  { parameter_description }
	 */
	void Print(TLogLevel level, const char* format, ...)
	{
		if (level > m_logLevel)
		{
			return;
		}

		char in_buffer[MAX_LOG_BUFFER_DEFAULT] = { 0 };
		va_list arglist;
		va_start(arglist, format);
		vsprintf_s(in_buffer, sizeof(in_buffer), format, arglist);
		va_end(arglist);

		os << TIME << " "
			<< "Thread[0x" << std::hex << std::this_thread::get_id() << "] "
			<< "[" << m_module_name.c_str() << "] "
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
		return m_log_dictionary[level];
	}

    static TLogLevel FromString(const std::string& level)
	{
		for(uint8_t index = 0; index <= TLogLevel::OFF; ++ index)
		{
			if (level == m_log_dictionary[index])
			{
				return TLogLevel(index);
			}
		};
		Log<T>().Get(WARN) << "Unknown logging level[" << level << "]. Using INFO level as default.";
		return INFO;
	}
};

template <typename T>
TLogLevel Log<T>::m_logLevel = DEBUG;

template <typename T>
std::string Log<T>::m_module_name = "UNKNOWN";

template <typename T>
const char* const Log<T>::m_log_dictionary[8] = { "FATAL", "ERROR", "WARN", "INFO", "DEBUG", "TRACE", "ALL", "OFF" };


#if defined(__OUTPUT2FILE__)
typedef Log<Output2File> LogObject;
#elif defined(__OUTPUT2STREAM__)
typedef Log<Output2Stream> LogObject;
#elif defined(__OUTPUT2SOCKET__)
typedef Log<Output2Socket> LogObject;
#endif

#define LOG_FATAL(format, ...)	LogObject().Print(FATAL,"%s :" format, __FUNCTION__, __VA_ARGS__)
#define LOG_ERROR(format, ...)	LogObject().Print(ERROR, "%s :" format, __FUNCTION__, __VA_ARGS__)
#define LOG_WARN(format, ...)	LogObject().Print(WARN , "%s :" format, __FUNCTION__, __VA_ARGS__)
#define LOG_INFO(format, ...)	LogObject().Print(INFO , "%s :" format, __FUNCTION__, __VA_ARGS__)
#define LOG_DEBUG(format, ...) 	LogObject().Print(DEBUG, "%s :" format, __FUNCTION__, __VA_ARGS__)
#define LOG_TRACE(format, ...) 	LogObject().Print(TRACE, "%s :" format, __FUNCTION__, __VA_ARGS__)
#define LOG_ALL(format, ...) 	LogObject().Print(ALL, "%s :" format, __FUNCTION__, __VA_ARGS__)
#define LOG_OFF(format, ...)

#define FUNC_IN(format, ...)	LOG_TRACE("[IN] " format, __VA_ARGS__)
#define FUNC_OUT(format, ...)	LOG_TRACE("[OUT] " format, __VA_ARGS__)


#endif //__LOG_H__
