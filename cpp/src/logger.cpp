#include "../include/logger.h"
#include <crow/logging.h>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

using std::cout, std::cerr, std::endl, std::string, std::put_time,
    std::ostringstream;

string logLevelToString(LogLevel level) {
  switch (level) {
  case LogLevel::DEBUG:
    return "DEBUG";
  case LogLevel::INFO:
    return "INFO";
  case LogLevel::WARNING:
    return "WARNING";
  case LogLevel::ERROR:
    return "ERROR";
  case LogLevel::CRITICAL:
    return "CRITICAL";
  default:
    return "UNKNOWN";
  }
}

int logLevelToColor(LogLevel level) {
  switch (level) {
  case LogLevel::DEBUG:
    return 34;
  case LogLevel::INFO:
    return 32;
  case LogLevel::WARNING:
    return 93;
  case LogLevel::ERROR:
    return 31;
  case LogLevel::CRITICAL:
    return 91;
  default:
    return 0;
  }
}

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef _WIN32
void setColor(int color) {
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
void resetColor() { setColor(7); }
#else
void setColor(int color) { cout << "\033[1;" << color << "m"; }
void resetColor() { setColor(0); }
#endif

string getCurrentTime(string format) {
  time_t currentTime;
  struct tm localTime;

  time(&currentTime);
#ifdef _MSC_VER
  localtime_s(&currentTime, &localTime);
#else
  localtime_r(&currentTime, &localTime);
#endif // DEBUG
  ostringstream oss;
  oss << put_time(&localTime, format.c_str());
  return oss.str();
}

void CustomLogger::log(string message, crow::LogLevel level) {
  cerr << "(" << getCurrentTime("%H:%M:%S") << ") [";
  setColor(logLevelToColor(level));
  cerr << logLevelToString(level);
  resetColor();
  cerr << "] " << message << endl;
}
