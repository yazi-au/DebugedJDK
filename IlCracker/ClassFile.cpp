#include "Attribute.h"
#include "BasicTool.h"
#include "ConstPool.h"
#include "ClassReader.h"
#include "ClassFile.h"

#include <string>

ClassFile::ClassFile(char* data){
	this->classReader = new ClassReader(data);
}
void ClassFile::readFile(const char* path){
	std::fstream stream;
	stream.open(path, std::ios::in | std::ios::binary);
	stream.seekg(0, std::ios::end);
	std::streamsize size = stream.tellg();
	stream.seekg(0, std::ios::beg);
	classReader->data = new char[size];
	stream.read(classReader->data, size);
	stream.close();
	classReader->current = 8;

	mainVersion = getMainVersion();
	magicNumber = getMagicNumber();
	minorVersion = getMinorVersion();
	readConstPool();
	readClass();
}
unsigned int ClassFile::getMainVersion(){
	int prev_cur = classReader->current;
	classReader->current = 6;
	int r = classReader->get2Byte();
	classReader->current = prev_cur;
	return r;
}
unsigned int ClassFile::getMagicNumber(){
	int prev_cur = classReader->current;
	classReader->current = 0;
	unsigned int magic_number = classReader->get4Byte();
	classReader->current = prev_cur;
	return magic_number;
}
unsigned short ClassFile::getMinorVersion(){
	int prev_cur = classReader->current;
	classReader->current = 4;
	unsigned short minor_version = classReader->get2Byte();
	classReader->current = prev_cur;
	return minor_version;
}

void ClassFile::readConstPool(){
	classReader->current = 8;
	constPool = new ConstPool();
	constPool->count = classReader->get2Byte();
	constPool->tag = new unsigned char[constPool->count];
	for(int i = 1; i < constPool->count; i++){
		unsigned char tag = classReader->getByte();
		*(constPool->tag + i) = tag;
		switch(tag){
		case UTF8: { //UTF8
			unsigned short len = classReader->get2Byte();
			char* target = new char[len + 1];
			classReader->getBytes(len, target);
			target[len] = '\0';
			constPool->data[i] = target;
			break;
		}
		case INT:
		case FLOAT: { //INT FLOAT
			char* target = new char[4];
			*target = classReader->get4Byte();
			constPool->data[i] = target;
			break;
		}
		case LONG:
		case DOUBLE: { //LONG DOUBLE
			//pass
			classReader->get4Byte();
			classReader->get4Byte();
			i++;
			break;
		}
		case STRING: //STRING
		case CLASS: { //CLASS
			char* target = new char[2];
			*target = classReader->get2Byte();
			constPool->data[i] = target;
			break;
		}
		case METHODREF:
		case INTERFACE_METHOD_REF:
		case FIELDREF:
		case NAME_AND_TYPE:{
			int* temp = new int;
			short classIndex = classReader->get2Byte();
			short nameAndTypeIndex = classReader->get2Byte();
			*temp = (classIndex << 16) | (nameAndTypeIndex & 0xFFFF);
			(constPool->data[i]) = (char*)temp;
			break;
		}
		case METHOD_HANDLE:{
			char* target = new char[3];
			classReader->getBytes(3, target);
			constPool->data[i] = target;
			break;
		}
		case METHOD_TYPE: {
			char* target = new char[2];
			classReader->getBytes(2, target);
			constPool->data[i] = target;
			break;
		}
		case INVOKEDYNAMIC: {
			char* target = new char[4];
			classReader->getBytes(4, target);
			constPool->data[i] = target;
			break;
		}
		default:
			SEND("[ConstPool] Unknown const pool index: ", std::to_string(i).c_str(), COLOR_RED);
			break;
		}
	}
	attributeReader = new AttributeReader(*constPool);
}

void ClassFile::readClassInfo(){
	accessFlags = classReader->get2Byte();
	std::cout << getClassAccessFlagsMapping(accessFlags) << std::endl;
	short c = classReader->get2Byte();
	char* chars = constPool->data[c];
	name_class = charsToShort(chars);
	std::cout << "MainClass:" << constPool->data[name_class] << std::endl;
	chars = constPool->data[classReader->get2Byte()];
	name_super = charsToShort(chars);
	std::cout << "SuperClass:" << constPool->data[name_super] << std::endl;
}

void ClassFile::readInterface(){
	unsigned short interfaceCount = classReader->get2Byte();
	if(interfaceCount > 0){
		interfaces.resize(interfaceCount);
		for(size_t i = 0; i < interfaceCount; i++){
			Interface* interface = new Interface();
			interface->constPoolIndex = charsToShort(constPool->data[classReader->get2Byte()]);
			interface->name = constPool->data[interface->constPoolIndex];
			std::cout << "Interface[" << i << "] name:" << interface->name << std::endl;
			interfaces[i] = interface;
		}
	}
	std::cout << "Interface Count:" << interfaces.size() << std::endl;
}

void ClassFile::readField(){
	unsigned short fieldCount = classReader->get2Byte();
	if(fieldCount > 0){
		fields.resize(fieldCount);
		for(size_t i = 0; i < fieldCount; i++){
			Field* field = new Field();
			field->accessFlag = classReader->get2Byte();
			field->nameIndex = classReader->get2Byte();
			field->descriptorIndex = classReader->get2Byte();
			field->attributes = readAttribute();
			fields[i] = field;
			std::cout << constPool->data[field->descriptorIndex] << " Field[" << i << "] = " << constPool->data[field->nameIndex] << std::endl;
		}
	}
}

void ClassFile::readMethod(){
	unsigned short methodCount = classReader->get2Byte();
	if(methodCount > 0){
		methods.resize(methodCount);
		for(size_t i = 0; i < methodCount; i++){
			Method* method = new Method();
			method->classfile = this;
			method->accessFlag = classReader->get2Byte();
			method->nameIndex = classReader->get2Byte();
			method->methodName = constPool->data[method->nameIndex];
			method->descriptorIndex = classReader->get2Byte();
			methods[i] = method;
			method->attributes = readAttribute();
		}
	}
}
void ClassFile::readClass(){
	readClassInfo();
	readInterface();
	readField();
	readMethod();
	attributes = readAttribute();
}

std::vector<Attribute*> ClassFile::readAttribute(){
	return attributeReader->readAttribute(*classReader);
}

void ClassFile::cleanUp(){
	constPool->cleanUp();
	for(auto t : interfaces){
		delete t;
	}
	interfaces.clear();
	interfaces.resize(0);
	for(auto t : fields){
		delete t;
	}
	fields.clear();
	fields.resize(0);
	for(size_t i = 0; i < methods.size(); i++){
		methods[i]->cleanUp();
		delete methods[i];
	}
	methods.clear();
	methods.resize(0);
}

void Method::cleanUp(){}