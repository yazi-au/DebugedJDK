#include <variant>
#include <vector>

class Attribute;
class ClassReader;
class ConstPool;
class AttributeReader {
	ConstPool* constPool;
public:
	AttributeReader(ConstPool& constPool);
	std::vector<Attribute*> readAttribute(ClassReader& reader);
};
class Attribute {
public:
	unsigned short nameIndex;
	unsigned int attributeLength;
	virtual void read(ClassReader& reader,AttributeReader& attribute_reader);
	virtual void cleanUp();
};

class Attri_ConstantValue : public Attribute {
public:
	unsigned short constantValueIndex;
	void read(ClassReader& reader, AttributeReader& attribute_reader) override;
	void cleanUp() override;
};
class Attri_Code : public Attribute {
public:
	short max_stack;
	short max_locals;
	int code_length;
	char* code;
	short exception_table_length;
	struct ExceptionItem
	{
		short start_pc;
		short end_pc;
		short handler_pc;
		short catch_type;
	} *exception_table;
	std::vector<Attribute*> attributes;
	void read(ClassReader& reader, AttributeReader& attribute_reader) override;
	void cleanUp() override;
};
class Attri_Exception : public Attribute{
public:
	short number_of_index;
	short* exception_index_table; //class constPool
	void read(ClassReader& reader,AttributeReader& attribute_reader) override;
	void cleanUp() override;
};
class Attri_InnerClasses : public Attribute {
public:
	short number_of_classes;
	struct InnerClass {
		short inner_class_info_index;
		short outer_class_info_index;
		short inner_name_index;
		short inner_class_access_flag;
	}* classes;
	void read(ClassReader& reader, AttributeReader& attribute_reader) override;
	void cleanUp() override;
};
class Attri_EnclosingMethod : public Attribute {
public:
	short classIndex;
	short methodIndex;
	void read(ClassReader& reader, AttributeReader& attribute_reader) override;
	void cleanUp() override;
};
class Attri_Synthetic : public Attribute {
public:
	void read(ClassReader& reader, AttributeReader& attribute_reader) override;
	void cleanUp() override;
};
class Attri_Signature : public Attribute {
public:
	short signature_index;
	void read(ClassReader& reader, AttributeReader& attribute_reader) override;
	void cleanUp() override;
};
class Attri_SourceFile : public Attribute {
public:
	short sourceFileIndex;
	void read(ClassReader& reader, AttributeReader& attribute_reader) override;
	void cleanUp() override;
};
class Attri_SourceDebugExtension : public Attribute {
public:
	char* debugExtension; //length = attribute length
	void read(ClassReader& reader, AttributeReader& attribute_reader) override;
	void cleanUp() override;
};
class Attri_LineNumberTable : public Attribute{
public:
	short lineNumberTableLength;
	struct TableItem {
		short startPc;
		short lineNumber;
	}* lineNumberTable;
	void read(ClassReader& reader, AttributeReader& attribute_reader) override;
	void cleanUp() override;
};
class Attri_LocalVariableTable : public Attribute {
public:
	short variableTableLength;
	struct TableItem {
		short startPc;
		short length;
		short nameIndex;
		short signatureIndex;
		short index;
	}* variableTable;
	void read(ClassReader& reader, AttributeReader& attribute_reader) override;
	void cleanUp() override;
};

class Attri_StackMapTable : public Attribute {
public:
	struct Top_variable_info{
		char tag = 0;
	};
	struct Integer_variable_info{
		char tag = 1;
	};
	struct Float_variable_info{
		char tag = 2;
	};
	struct Null_variable_info{
		char tag = 5;
	};
	struct UninitializedThis_variable_info{
		char tag = 6;
	};
	struct Object_variable_info{
		char tag = 7;
		short cpool_index;
	};
	struct Uninitialized_variable_info{
		char tag = 8;
		short offset;
	};
	struct Long_variable_info{
		char tag = 4;
	};
	struct Double_variable_info{
		char tag = 3;
	};
	union verification_type_info{
		Top_variable_info top_var;
		Integer_variable_info integer_var;
		Float_variable_info float_var;
		Long_variable_info long_var;
		Double_variable_info double_var;
		Null_variable_info null_var;
		UninitializedThis_variable_info uninitializedThis_var;
		Object_variable_info object_var;
		Uninitialized_variable_info uninitialized_var;
		verification_type_info(){}
	};
	struct stack_map_frame{
		unsigned char frame_type;
		short offset_delta;
		short number_of_locals;
		verification_type_info* locals;
		short number_of_stack_items;
		verification_type_info* stacks;
	};

	short number_of_entries;
	std::vector<stack_map_frame> frames;
	void read(ClassReader& reader, AttributeReader& attribute_reader) override;
	void cleanUp() override;
};

class Attri_RuntimeVisibleAnnotations : public Attribute{
public:
	struct array_value;
	struct annotation;
	struct element_value;
	struct enum_const_value{
		short type_name_index;
		short const_name_index;
	};
	enum tag{
		BYTE,
		CHAR,
		DOUBLE,
		FLOAT,
		INT,
		LONG,
		SHORT,
		BOOLEAN,
		STRING,
		ENUM,
		CLASS,
		ANNOTATION,
		ARRAY
	};

	struct array_value{
		short num_values;
		element_value* values;
	};

	struct element_value_pairs{
		short element_name_index;
		element_value* value;
	};

	struct annotation{
		short type_index;
		short num_element_value_pairs;
		element_value_pairs* pairs;
	};

	union element{
		short const_value_index;
		enum_const_value enumValues;
		short class_info_index;
		annotation annotation_value;
		array_value arrays;
		element(){}
	};

	struct element_value{
		tag tag;
		element* value;
		void read(ClassReader& reader);
		void cleanUp();
	};

	short num_annotations;
	annotation* annotations;
	void read(ClassReader& reader, AttributeReader& attribute_reader) override;
	void cleanUp() override; //update: clean Up
};

class Attri_RuntimeInvisibleAnnotations : public Attri_RuntimeVisibleAnnotations{};

class Attri_LocalVariableTypeTable : public Attribute{
public:
	short local_variable_type_table_length;
	struct local_variable_type{
		short start_pc;
		short length;
		short name_index;
		short signature_index;
		short index;
	};
	local_variable_type* local_variable_type_table;
	void read(ClassReader& reader, AttributeReader& attribute_reader) override;
	void cleanUp() override;
};

class Attri_BootstrapMethod : public Attribute{
public:
	short num_bootstrap_methods;
	struct bootstrap_method{
		short bootstrap_method_ref;
		short num_bootstrap_arguments;
		short* bootstrap_arguments;
	};
	bootstrap_method* bootstrapMethods;
	void read(ClassReader& reader, AttributeReader& attribute_reader) override;
	void cleanUp() override;
};