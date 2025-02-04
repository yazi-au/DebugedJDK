#include "ConstPool.h"
#include "BasicTool.h"
#include "ClassFile.h"
#include "ClassReader.h"

#include <iostream>

std::string waitForString(std::string message){
	if(!message.empty()) std::cout << message << std::endl;
	std::string r;
	std::cin >> r;
	return r;
}

int waitForInt(std::string message){
	if(!message.empty()) std::cout << message << std::endl;
	std::string r;
	while(true){
		try{
			std::cin >> r;
			return std::stoi(r);
		} catch(std::exception& e){
			std::cout << "You must input a vaild number" << std::endl;
		}
	}
}

int main(){
	ClassFile* classFile = new ClassFile(nullptr);
	int mode = waitForInt("Choose a mode to start debug: [1.static | 2.dynamic]");
	if(mode == 1){
		//static
		std::string path = waitForString("Please input the path of jar file:");
		classFile->readFile(path.c_str());
		std::cout << classFile->constPool->count << std::endl;
	} else{
		//dynamic
		system("tasklist");
		int pid = waitForInt("Please Input target PID:");
	}
	SEND("[Test] ","done! input help get help",COLOR_WHITE);
	while(true){
		std::string cmd = waitForString("");
		if(strcmp("quit", cmd.c_str()) == 0){
			break;
		} else if(strcmp("cp", cmd.c_str()) == 0){
			int index = waitForInt("");
			std::cout << "[ConstPool] " << index << " | " << classFile->constPool->data[index] << std::endl;
		} else{
			SEND("[Test] ","Unknown command!",COLOR_YELLOW);
		}
	}
	system("pause");
	return 0;
}