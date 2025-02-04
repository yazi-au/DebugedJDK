typedef unsigned char BYTE;
typedef void* HANDLE;

class ScanMethodFieldSetting{
public:
	bool magicNumber;
	//通过扫描MagicNumber: CAFE_BABE 查找潜在的classFile
	//对于经过特殊ClassLoader混淆后的程序应当关闭此选项
	bool metaDataRange;
	//通过metaData各项范围查找
	bool constPoolKVRange;
	//通过检测constPool中的varType范围和对应的value格式查找
	bool fieldRange;
	//通过检测字段的结构查找
	bool methodInfoRange;
	//通过检测方法声明的结构查找
};

class ScanTarget{
public:
	int pid;
	HANDLE hProcess;
};

class Scanner{
	ScanTarget* target;
	int currentBuffer = 0;
	unsigned long long* currentRead = 0;
	unsigned long long* currentPointer = 0;
	int freeBytes = 0;
	BYTE* buffer1 = new BYTE[1024];
	BYTE* buffer2 = new BYTE[1024];
public:
	Scanner(ScanTarget* target);
	~Scanner();
	void updateBuffer(BYTE* buffer);
	void swapBuffer();
	inline BYTE* getCurrentBuffer();
	void getBytes(BYTE* byte,unsigned long long size);
	void scanMethodField(ScanTarget& target,ScanMethodFieldSetting& setting);
};