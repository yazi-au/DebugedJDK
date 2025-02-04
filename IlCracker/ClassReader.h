class ClassReader {
public:
	char* data;
	int current;
	ClassReader(char* data);
	unsigned char getByte();
	unsigned short get2Byte();
	unsigned int get4Byte();
	void getBytes(short len, char* target);
};