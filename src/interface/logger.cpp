/**
 * Logging functionality
 * /
 
#include <logger.h>

class Logger
{

  void getInstance()
  {
      if(instance==NULL) instance = new Logger();
      return instance;
  }
}
