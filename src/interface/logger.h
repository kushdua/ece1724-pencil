/**
 * Logging functionality
 * /
 
#ifndef LOGGER_H
#define LOGGER_H

#define LOG_NAME "global_log.xml"

class Logger
{
   public:
      getInstance();

   private:
      static Logger instance = NULL;
      
#endif
