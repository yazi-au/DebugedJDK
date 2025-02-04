#include <iostream>
#include <vector>
#include <fstream>
class ClassFile;
class ClassReader;
class ConstPool;
class AttributeReader;
class Attribute;
struct Method {
public:
	ClassFile* classfile;
	short accessFlag;
	short nameIndex;
	const char* methodName;
	short descriptorIndex;
	std::vector<Attribute*> attributes;
	void cleanUp();
};

struct Field {
public:
	/*
	PUBLIC 0x0001
	FINAL 0x0010
	PRIVATE 0x002
	PROTECTED 0x0004
	STATIC 0x0008
	VOLATILE 0x0040
	TRANSIENT 0x0080
	SYNTHETIC 0x1000
	ENUM 0x4000
	*/
	short accessFlag;
	short nameIndex;
	/*
	B byte
	C char
	F float
	D double
	I int
	J long
	S short
	Z boolean
	V void
	L Object
	*/
	short descriptorIndex;
	std::vector<Attribute*> attributes;
};

struct Interface {
public:
	short constPoolIndex;
	const char* name;
};

class ClassFile{
public:
	ClassReader* classReader;
	AttributeReader* attributeReader;
	ConstPool* constPool;
	/*
	PUBLIC 0x0001
	FINAL 0x0010
	SUPER 0x0020
	INTERFACE 0x0200
	ABSTRACT 0x0300
	SYNTHETIC 0x1000
	ANNOTATION 0x2000
	ENUM 0x4000
	*/
	unsigned int mainVersion;
	unsigned int magicNumber;
	unsigned short minorVersion;

	short accessFlags;
	short name_class;
	short name_super;
	std::vector<Interface*> interfaces;
	std::vector<Field*> fields;
	std::vector<Method*> methods;
	std::vector<Attribute*> attributes;
	ClassFile(char* data);
	void readFile(const char* path);
	void cleanUp();
private:
	std::vector<Attribute*> readAttribute();
	unsigned int getMainVersion();
	unsigned int getMagicNumber();
	unsigned short getMinorVersion();
	void readConstPool();
	void readClass();
	void readMethod();
	void readField();
	void readInterface();
	void readClassInfo();
};