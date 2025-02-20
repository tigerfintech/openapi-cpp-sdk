#include "../include/tigerapi/logger.h"

INITIALIZE_EASYLOGGINGPP

void TIGER_API::LoggerConfig::set_log_level(el::Level level)
{
	el::Loggers::setLoggingLevel(level);
}
