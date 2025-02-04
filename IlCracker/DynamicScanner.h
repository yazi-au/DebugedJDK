typedef unsigned char BYTE;
typedef void* HANDLE;

class ScanMethodFieldSetting{
public:
	bool magicNumber;
	//ͨ��ɨ��MagicNumber: CAFE_BABE ����Ǳ�ڵ�classFile
	//���ھ�������ClassLoader������ĳ���Ӧ���رմ�ѡ��
	bool metaDataRange;
	//ͨ��metaData���Χ����
	bool constPoolKVRange;
	//ͨ�����constPool�е�varType��Χ�Ͷ�Ӧ��value��ʽ����
	bool fieldRange;
	//ͨ������ֶεĽṹ����
	bool methodInfoRange;
	//ͨ����ⷽ�������Ľṹ����
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