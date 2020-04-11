#pragma once
#include <cstdint>
enum ErrorType: uint16_t
{
	SUCCESS = 0,
	FAILED,
	TIMEOUT,

	RESOURCE_ACQUIRE_ERROR_START = 0x0100,
	RESOURCE_ACQUIRE_FAILED,
	NO_THREAD_ID_FOUND,


	UNKNOWN_ERROR,

	ERROR_TYPE_MAX,
};

struct ErrorInfo {
	ErrorType	errorType;
	const char*	errorString;

	ErrorInfo(ErrorType type);
	~ErrorInfo();

	inline bool IsSuccess();
};