#pragma once
#include <stdio.h>
#include <Windows.h>
#include "Setting.h"

void getCurrentDirectory(char *currentDirectory) {
	GetCurrentDirectory(CHARBUFF, currentDirectory);
}
int readInt(const char *secton, const char *keyword, int defaultValue, const char *filePath, const char *name) {
	char section[CHARBUFF];
	sprintf_s(section, secton);
	char keyWord[CHARBUFF];
	sprintf_s(keyWord, keyword);
	char settingFile[CHARBUFF];
	sprintf_s(settingFile, "%s\\%s input.ini", filePath, name);
	fprintf(stdout, "%s,%d\n", settingFile, GetPrivateProfileInt(section, keyWord, defaultValue, settingFile));
	return GetPrivateProfileInt(section, keyWord, defaultValue, settingFile);
}
double readDouble(const char *secton, const char *keyword, double defaultValue, const char *filePath, const char *name) {
	char section[CHARBUFF];
	sprintf_s(section, secton);
	char keyWord[CHARBUFF];
	sprintf_s(keyWord, keyword);
	char settingFile[CHARBUFF];
	sprintf_s(settingFile, "%s\\%s input.ini", filePath, name);
	fprintf(stdout, "%s,%g\n", settingFile, (double)GetPrivateProfileInt(section, keyWord, defaultValue, settingFile));
	return (double)GetPrivateProfileInt(section, keyWord, defaultValue, settingFile);
}


bool readChar(const char *secton, const char *keyword, const char *defaultValue, char *returnValue, const char *filePath, const char *name) {
	char section[CHARBUFF];
	sprintf_s(section, secton);
	char keyWord[CHARBUFF];
	sprintf_s(keyWord, keyword);
	char settingFile[CHARBUFF];
	sprintf_s(settingFile, "%s\\%s input.ini", filePath, name);
	if (GetPrivateProfileString(section, keyWord, defaultValue, returnValue, CHARBUFF, settingFile) != 0) {
		fprintf(stdout, "%s , %s\n", settingFile, returnValue);
		return 1;
	}
	else {
		fprintf(stdout, "%s doesn't contain [$s] %s\n", settingFile, section, keyWord);
		return 0;
	}
}
bool writeIntInput(const char *secton, const char *keyword, int returnValue, const char *filePath, const char *name) {
	char section[CHARBUFF];
	sprintf_s(section, secton);
	char keyWord[CHARBUFF];
	sprintf_s(keyWord, keyword);
	char settingFile[CHARBUFF];
	char number[CHARBUFF];
	sprintf_s(number, "%d", returnValue);
	sprintf_s(settingFile, "%s\\%s input.ini", filePath, name);
	if (WritePrivateProfileString(section, keyWord, number, settingFile)) {
		return 1;
	}
	else {
		fprintf(stderr, "failed to write [$s] %s to %s \n", section, keyWord, settingFile);
		return 0;
	}
}
bool writeDoubleInput(const char *secton, const char *keyword, double returnValue, const char *filePath, const char *name) {
	char section[CHARBUFF];
	sprintf_s(section, secton);
	char keyWord[CHARBUFF];
	sprintf_s(keyWord, keyword);
	char settingFile[CHARBUFF];
	char number[CHARBUFF];
	sprintf_s(number, "%lf", returnValue);
	sprintf_s(settingFile, "%s\\%s input.ini", filePath, name);
	if (WritePrivateProfileString(section, keyWord, number, settingFile)) {
		return 1;
	}
	else {
		fprintf(stderr, "failed to write [$s] %s to %s \n", section, keyWord, settingFile);
		return 0;
	}
}
bool writeCharInput(const char *secton, const char *keyword, const char *returnValue, const char *filePath, const char *name) {
	char section[CHARBUFF];
	sprintf_s(section, secton);
	char keyWord[CHARBUFF];
	sprintf_s(keyWord, keyword);
	char settingFile[CHARBUFF];
	sprintf_s(settingFile, "%s\\%s input.ini", filePath, name);
	if (WritePrivateProfileString(section, keyWord, returnValue, settingFile)) {
		return 1;
	}
	else {
		fprintf(stderr, "failed to write [$s] %s to %s \n", section, keyWord, settingFile);
		return 0;
	}
}