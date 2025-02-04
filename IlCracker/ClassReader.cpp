#include "ClassReader.h"

ClassReader::ClassReader(char* data) {
	this->current = 0;
	this->data = data;
}
unsigned char ClassReader::getByte() {
	unsigned char* res = (unsigned char*)(data + current);
	current++;
	return (*res);
}
unsigned short ClassReader::get2Byte() {
	unsigned int res = 0;
	for (int i = 0; i < 2; i++) {
		res = (res << 8) | static_cast<unsigned char>(data[current + i]);
	}
	current += 2;
	return res;
}
unsigned int ClassReader::get4Byte() {
	unsigned int res = 0;
	for (int i = 0; i < 4; i++) {
		res = (res << 8) | static_cast<unsigned char>(data[current + i]);
	}
	current += 4;
	return res;
}
void ClassReader::getBytes(short len, char* target) {
	for (int i = 0; i < len; i++) {
		target[i] = getByte();
	}
}