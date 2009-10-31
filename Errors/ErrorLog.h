#ifndef ERROR_LOG
#define ERROR_LOG

#include "../Headers.h"

bool initErrorLog();
bool closeErrorLog(void);
bool logError(const char *format, ...);

#endif
