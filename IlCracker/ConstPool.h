#include <map>
#include <vector>

enum ConstPoolType{
	UTF8 = 1,
	INT = 3,
	FLOAT = 4,
	LONG = 5,
	DOUBLE = 6,
	CLASS = 7,
	STRING = 8,
	FIELDREF = 9,
	METHODREF = 10,
	INTERFACE_METHOD_REF = 11,
	NAME_AND_TYPE = 12,
	METHOD_HANDLE = 15,
	METHOD_TYPE = 16,
	INVOKEDYNAMIC = 18
};

class ConstPool {
public:
	unsigned short count;
	unsigned char* tag;
	std::map<int, char*> data; //update: int,object
	void cleanUp();
};