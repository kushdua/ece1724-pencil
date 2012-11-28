/**
 * Logging functionality
 */

#include <cstdlib>
#include "logger.h"

Logger *Logger::instance = NULL;

Logger *Logger::getInstance()
{
	if (instance == NULL)
		instance = new Logger;

      return instance;
}
