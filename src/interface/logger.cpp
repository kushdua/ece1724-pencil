/**
 * Logging functionality
 */
 
#include <logger.h>

Logger Logger::instance = NULL;

void Logger::getInstance()
{
	if (instance == NULL)
		instance = new Logger();

      return instance;
}
