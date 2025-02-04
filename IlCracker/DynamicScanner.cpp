#include "DynamicScanner.h"
#include <Windows.h>
#include <iostream>
#include <iomanip>

Scanner::Scanner(ScanTarget* target){
	this->target = target;
}

Scanner::~Scanner(){
	delete target;
	delete[] buffer1;
	delete[] buffer2;
}

void Scanner::updateBuffer(BYTE* buffer){
	SIZE_T bytesRead;
	if(!ReadProcessMemory(target->hProcess, currentRead, buffer, 1024, &bytesRead)){
		std::cout << "ERROR when read " << *currentRead << std::endl;
	}
	freeBytes = 1024;
}

void Scanner::swapBuffer(){
	if(currentBuffer){
		updateBuffer(buffer1);
		currentBuffer = 1;
	} else{
		updateBuffer(buffer2);
		currentBuffer = 0;
	}
}

inline BYTE* Scanner::getCurrentBuffer(){
	return currentBuffer == 0 ? buffer1 : buffer2;
}

void Scanner::getBytes(BYTE* byte, unsigned long long size){
	for(size_t i = 0; i < size; i++){
		if(!freeBytes){
			swapBuffer();
		}
		byte[i] = getCurrentBuffer()[1024-freeBytes];
	}
}

void Scanner::scanMethodField(ScanTarget& target, ScanMethodFieldSetting& setting){
	
}