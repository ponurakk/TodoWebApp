#ifndef LOGGER_H_
#define LOGGER_H_

#include <crow/logging.h>
#include <string>

using crow::LogLevel, crow::ILogHandler;
using std::string;

class CustomLogger : public ILogHandler {
public:
  CustomLogger() {}
  void log(string message, LogLevel level) override;
};

string logLevelToString(LogLevel level);
int logLevelToColor(LogLevel level);
void setColor(int color);
void resetColor();
string getCurrentTime(string format);

#endif
