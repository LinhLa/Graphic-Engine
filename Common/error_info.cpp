#include "stdafx.h"
#include "inc\error_info.h"
#include <unordered_map>

typedef std::unordered_map<ErrorType, std::string> ErrorTableLookupType;

static ErrorTableLookupType ErrorTableLookup =
{
	{SUCCESS, "success"},
	{FAILED, "failed"},
	{TIMEOUT,"timeout"},
	{RESOURCE_ACQUIRE_FAILED, "failed to acquire resource"},
	{NO_THREAD_ID_FOUND , "no thread id found in resource pending list"},
	{UNKNOWN_ERROR, "unknown error"}
};

ErrorInfo::ErrorInfo(ErrorType type) :errorType(type), errorString(ErrorTableLookup[type].c_str()) {}

ErrorInfo::~ErrorInfo() {}

inline bool ErrorInfo::IsSuccess()
{
	return (SUCCESS == errorType);
}