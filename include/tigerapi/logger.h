#ifndef LOGGER_H
#define LOGGER_H
#include "easylogging++.h"
#include "win32.h"

namespace TIGER_API
{
	class OPENAPI_EXPORT LoggerConfig
	{
	public:
		LoggerConfig() {};
		~LoggerConfig() {};

		static void set_log_level(el::Level level);
	};
}
#endif
