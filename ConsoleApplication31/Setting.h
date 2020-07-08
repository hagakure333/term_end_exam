#pragma once
#define CHARBUFF 124
int readInt(const char *secton, const char *keyword, int defaultValue, const char *filePath, const char *name);
bool readChar(const char *secton, const char *keyword, const char *defaultValue, char *returnValue, const char *filePath, const char *name);
double readDouble(const char *secton, const char *keyword, double defaultValue, const char *filePath, const char *name);
void getCurrentDirectory(char *currentDirectory);
bool writeCharInput(const char *secton, const char *keyword, const char *returnValue, const char *filePath, const char *name);
bool writeIntInput(const char *secton, const char *keyword, int returnValue, const char *filePath, const char *name);
bool writeDoubleInput(const char *secton, const char *keyword, double returnValue, const char *fliePath, const char *name);
bool writeCharOutput(const char *secton, const char *keyword, const char *returnValue, const char *filePath, const char *name);
bool writeIntOutput(const char *secton, const char *keyword, int returnValue, const char *filePath, const char *name);
bool writeDoubleOutput(const char *secton, const char *keyword, double returnValue, const char *fliePath, const char *name);
