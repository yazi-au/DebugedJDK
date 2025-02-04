#include "BasicTool.h"
#include <windows.h>
#include <iostream>

int mainVersionMapping(int mainVersion) {
	return mainVersion - 44;
}

std::string getClassAccessFlagsMapping(short flag) {
	std::string r;
	if ((flag & 0xF) == 0x1) r.append("PUBLIC ");
	if (((flag >> 4) & 0xF) == 0x1) r.append("FINAL ");
	if (((flag >> 4) & 0xF) == 0x2) r.append("SUPER ");
	if (((flag >> 8) & 0xF) == 0x2) r.append("INTERFACE ");
	if (((flag >> 8) & 0xF) == 0x3) r.append("ABSTRACT ");
	if (((flag >> 12) & 0xF) == 0x1) r.append("SYNTHETIC ");
	if (((flag >> 12) & 0xF) == 0x2) r.append("ANNOTATION ");
	if (((flag >> 12) & 0xF) == 0x4) r.append("ENUM ");
	return r;
}
short charsToShort(char* chars) {
	return (chars[1] << 16) | (chars[0] & 0xFFFF);
}
char* shortToChars(short s) {
	char* bytes = new char[2];
	memcpy(bytes, &s, sizeof(short));
	return bytes;
}
void SEND(const char* prefix, const char* msg, int color) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | color);
	std::cout << prefix << msg << std::endl;
	SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | 7);
}