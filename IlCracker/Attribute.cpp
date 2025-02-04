#include "Attribute.h"
#include "ClassReader.h"
#include "ConstPool.h"
#include "BasicTool.h"
#include <iostream>

AttributeReader::AttributeReader(ConstPool& pool) {
	constPool = &pool;
}
std::vector<Attribute*> AttributeReader::readAttribute(ClassReader& reader) {
	short length = reader.get2Byte();
	std::vector<Attribute*> attris;
	attris.reserve(length);
	for (size_t i = 0; i < length; i++)
	{
		Attribute attribute{};
		attribute.read(reader,*this);
		const char* name = constPool->data[attribute.nameIndex];
		if (strcmp("Code",name) == 0) {
			attris.push_back(new Attri_Code());
		}
		else if(strcmp("ConstantValue",name) == 0) {
			attris.push_back(new Attri_ConstantValue());
		}
		else if (strcmp("StackMapTable",name) == 0) {
			attris.push_back(new Attri_StackMapTable());
		}
		else if (strcmp("Exceptions", name) == 0) {
			attris.push_back(new Attri_Exception());
		}
		else if (strcmp("InnerClasses", name) == 0) {
			attris.push_back(new Attri_InnerClasses());
		}
		else if(strcmp("EnclosingMethod", name) == 0){
			attris.push_back(new Attri_EnclosingMethod());
		}
		else if (strcmp("Synthetic", name) == 0) {
			attris.push_back(new Attri_Synthetic());
		} else if(strcmp("LineNumberTable",name) == 0){
			attris.push_back(new Attri_LineNumberTable());
		} else if(strcmp("LocalVariableTable",name) == 0){
			attris.push_back(new Attri_LocalVariableTable());
		} else if(strcmp("RuntimeVisibleAnnotations",name) == 0){
			attris.push_back(new Attri_RuntimeVisibleAnnotations());
		} else if(strcmp("LocalVariableTypeTable",name) == 0){
			attris.push_back(new Attri_LocalVariableTypeTable());
		} else if(strcmp("SourceFile", name) == 0){
			attris.push_back(new Attri_SourceFile());
		} else if(strcmp("BootstrapMethods", name) == 0){
			attris.push_back(new Attri_BootstrapMethod());
		} else if(strcmp("Signature", name) == 0){
			attris.push_back(new Attri_Signature());
		} else if(strcmp("RuntimeInvisibleAnnotations", name) == 0){
			attris.push_back(new Attri_RuntimeInvisibleAnnotations());
		}
		else {
			SEND("[AttributeReader] Unknown attribute! : ",name,COLOR_RED);
		}
		attris[i]->nameIndex = attribute.nameIndex;
		SEND("", constPool->data.at(attribute.nameIndex),COLOR_YELLOW);
		attris[i]->attributeLength = attribute.attributeLength;
		Attribute* att = attris[i];
		attris[i]->read(reader, *this);
	}
	return attris;
}

void Attribute::read(ClassReader& reader, AttributeReader& attribute_reader) {
	nameIndex = reader.get2Byte();
	attributeLength = reader.get4Byte();
}
void Attribute::cleanUp() {}
void Attri_ConstantValue::read(ClassReader& reader, AttributeReader& attribute_reader) {
	constantValueIndex = reader.get2Byte();
}
void Attri_ConstantValue::cleanUp() {}
void Attri_Code::read(ClassReader& reader, AttributeReader& attribute_reader) {
	max_stack = reader.get2Byte();
	max_locals = reader.get2Byte();
	code_length = reader.get4Byte();
	code = new char[code_length];
	reader.getBytes(code_length,code);
	exception_table_length = reader.get2Byte();
	exception_table = new ExceptionItem[exception_table_length];
	for (size_t i = 0; i < exception_table_length; i++)
	{
		ExceptionItem item{};
		item.start_pc = reader.get2Byte();
		item.end_pc = reader.get2Byte();
		item.handler_pc = reader.get2Byte();
		item.catch_type = reader.get2Byte();
		exception_table[i] = item;
	}
	attributes = attribute_reader.readAttribute(reader);
}
void Attri_Code::cleanUp() {
	delete[] exception_table;
	for (Attribute* attribute : attributes) {
		delete attribute;
	}
	attributes.clear();
	attributes.resize(0);
	delete[] code;
}
void Attri_Exception::read(ClassReader& reader, AttributeReader& attribute_reader) {
	number_of_index = reader.get2Byte();
	exception_index_table = new short[number_of_index];
	for (size_t i = 0; i < number_of_index; i++)
	{
		exception_index_table[i] = reader.get2Byte();
	}
}
void Attri_Exception::cleanUp() {
	delete[] exception_index_table;
}
void Attri_InnerClasses::read(ClassReader& reader,AttributeReader& attributeReader) {
	number_of_classes = reader.get2Byte();
	classes = new InnerClass[number_of_classes];
	for (size_t i = 0; i < number_of_classes; i++)
	{
		classes[i].inner_class_info_index = reader.get2Byte();
		classes[i].outer_class_info_index = reader.get2Byte();
		classes[i].inner_name_index = reader.get2Byte();
		classes[i].inner_class_access_flag = reader.get2Byte();
	}
}
void Attri_InnerClasses::cleanUp() {
	delete[] classes;
}
void Attri_EnclosingMethod::read(ClassReader& reader,AttributeReader& attributeReader) {
	classIndex = reader.get2Byte();
	methodIndex = reader.get2Byte();
}
void Attri_EnclosingMethod::cleanUp() {}
void Attri_Synthetic::read(ClassReader& reader, AttributeReader& attributeReader) {}
void Attri_Synthetic::cleanUp() {}
void Attri_Signature::read(ClassReader& reader, AttributeReader& attributeReader) {
	signature_index = reader.get2Byte();
}
void Attri_Signature::cleanUp() {}
void Attri_SourceFile::read(ClassReader& reader, AttributeReader& attributeReader) {
	sourceFileIndex = reader.get2Byte();
}
void Attri_SourceFile::cleanUp() {}
void Attri_SourceDebugExtension::read(ClassReader& reader, AttributeReader& attributeReader) {
	debugExtension = new char[attributeLength];
	reader.getBytes(attributeLength,debugExtension);
}
void Attri_SourceDebugExtension::cleanUp() {
	delete[] debugExtension;
}
void Attri_LineNumberTable::read(ClassReader& reader, AttributeReader& attributeReader) {
	lineNumberTableLength = reader.get2Byte();
	lineNumberTable = new TableItem[lineNumberTableLength];
	for (size_t i = 0; i < lineNumberTableLength; i++)
	{
		lineNumberTable[i].startPc = reader.get2Byte();
		lineNumberTable[i].lineNumber = reader.get2Byte();
	}
}
void Attri_LineNumberTable::cleanUp() {
	delete[] lineNumberTable;
}
void Attri_LocalVariableTable::read(ClassReader& reader, AttributeReader& attributeReader) {
	variableTableLength = reader.get2Byte();
	variableTable = new TableItem[variableTableLength];
	for (size_t i = 0; i < variableTableLength; i++)
	{
		variableTable[i].startPc = reader.get2Byte();
		variableTable[i].length = reader.get2Byte();
		variableTable[i].nameIndex = reader.get2Byte();
		variableTable[i].signatureIndex = reader.get2Byte();
		variableTable[i].index = reader.get2Byte();
	}
}
void Attri_LocalVariableTable::cleanUp() {
	delete[] variableTable;
}
void Attri_RuntimeVisibleAnnotations::element_value::read(ClassReader& reader){
	value = new Attri_RuntimeVisibleAnnotations::element();
	char a = reader.getByte();
	switch(a){
	case 'B':{
		tag = BYTE;
		value->const_value_index = reader.get2Byte();
		break;
	}
	case 'C':{
		tag = CHAR;
		value->const_value_index = reader.get2Byte();
		break;
	}
	case 'D':{
		tag = DOUBLE;
		value->const_value_index = reader.get2Byte();
		break;
	}
	case 'F':{
		tag = FLOAT;
		value->const_value_index = reader.get2Byte();
		break;
	}
	case 'I':{
		tag = INT;
		value->const_value_index = reader.get2Byte();
		break;
	}
	case 'J':{
		tag = LONG;
		value->const_value_index = reader.get2Byte();
		break;
	}
	case 'S':{
		tag = SHORT;
		value->const_value_index = reader.get2Byte();
		break;
	}
	case 'Z':{
		tag = BOOLEAN;
		value->const_value_index = reader.get2Byte();
		break;
	}
	case 's':{
		tag = STRING;
		value->const_value_index = reader.get2Byte();
		break;
	}
	case 'e':{
		tag = ENUM;
		value->enumValues.type_name_index = reader.get2Byte();
		value->enumValues.const_name_index = reader.get2Byte();
		break;
	}
	case 'c':{
		tag = CLASS;
		value->class_info_index = reader.get2Byte();
		break;
	}
	case '@':{
		tag = ANNOTATION;
		value->annotation_value.type_index = reader.get2Byte();
		value->annotation_value.num_element_value_pairs = reader.get2Byte();
		value->annotation_value.pairs = new Attri_RuntimeVisibleAnnotations::element_value_pairs[value->annotation_value.num_element_value_pairs];
		for(size_t i = 0; i < value->annotation_value.num_element_value_pairs; i++){
			value->annotation_value.pairs[i].element_name_index = reader.get2Byte();
			value->annotation_value.pairs[i].value = new Attri_RuntimeVisibleAnnotations::element_value();
			value->annotation_value.pairs[i].value->read(reader);
		}
		break;
	}
	case '[':{
		tag = ARRAY;
		value->arrays.num_values = reader.get2Byte();
		value->arrays.values->read(reader);
		break;
	}
	default:{
		SEND("[Attribute tag]Unknown tag:",a+"", COLOR_RED);
	}
	}
}
void Attri_RuntimeVisibleAnnotations::read(ClassReader& reader, AttributeReader& attribute_reader){
	num_annotations = reader.get2Byte();
	for(size_t i = 0; i < num_annotations; i++){
		annotation* annotation = new Attri_RuntimeVisibleAnnotations::annotation();
		annotation->type_index = reader.get2Byte();
		annotation->num_element_value_pairs = reader.get2Byte();
		for(size_t i = 0; i < annotation->num_element_value_pairs; i++){
			element_value_pairs* pair = new Attri_RuntimeVisibleAnnotations::element_value_pairs();
			pair->element_name_index = reader.get2Byte();
			pair->value = new Attri_RuntimeVisibleAnnotations::element_value();
			pair->value->read(reader);
		}
	}
}
void Attri_RuntimeVisibleAnnotations::element_value::cleanUp(){
	if(tag == ANNOTATION){
		for(size_t i = 0; i < value->annotation_value.num_element_value_pairs; i++){
			value->annotation_value.pairs[i].value->cleanUp();
		}
	} else if(tag == ARRAY){
		for(size_t i = 0; i < value->arrays.num_values; i++){
			value->arrays.values[i].cleanUp();
		}
	}
}
void Attri_RuntimeVisibleAnnotations::cleanUp(){
	for(size_t i = 0; i < num_annotations; i++){
		for(size_t j = 0; j < annotations[i].num_element_value_pairs; j++){
			annotations[i].pairs[j].value->cleanUp();
		}
		delete[] annotations[i].pairs;
	}
	delete[] annotations;
}

void Attri_StackMapTable::read(ClassReader& reader, AttributeReader& attribute_reader){
	number_of_entries = reader.get2Byte();
	frames.resize(number_of_entries);
	for(size_t i = 0; i < number_of_entries; i++){
		frames[i].frame_type = reader.getByte();
		if(inrange(0,frames[i].frame_type,63)){
		} else if(inrange(64, frames[i].frame_type, 127)){

			frames[i].stacks = new Attri_StackMapTable::verification_type_info[1];
			frames[i].stacks[0].top_var.tag = reader.getByte();
			if(frames[i].stacks[0].top_var.tag == 7 || frames[i].stacks[0].top_var.tag == 8){
				frames[i].stacks[0].object_var.cpool_index = reader.get2Byte();
			}

		} else if(inrange(247, frames[i].frame_type, 247)){

			frames[i].offset_delta = reader.get2Byte();
			frames[i].stacks = new Attri_StackMapTable::verification_type_info[1];
			frames[i].stacks[0].top_var.tag = reader.getByte();
			if(frames[i].stacks[0].top_var.tag == 7 || frames[i].stacks[0].top_var.tag == 8){
				frames[i].stacks[0].object_var.cpool_index = reader.get2Byte();
			}

		} else if(inrange(248, frames[i].frame_type, 250)){

			frames[i].offset_delta = reader.get2Byte();

		} else if(inrange(251, frames[i].frame_type, 251)){

			frames[i].offset_delta = reader.get2Byte();

		} else if(inrange(252, frames[i].frame_type, 254)){

			frames[i].offset_delta = reader.get2Byte();
			frames[i].locals = new Attri_StackMapTable::verification_type_info[frames[i].number_of_locals];
			for(size_t j = 0; j < frames[i].frame_type-251; j++){
				frames[i].locals[j].top_var.tag = reader.getByte();
				if(frames[i].locals[j].top_var.tag == 7 || frames[i].locals[j].top_var.tag == 8){
					frames[i].locals[j].object_var.cpool_index = reader.get2Byte();
				}
			}
		} else{
			frames[i].offset_delta = reader.get2Byte();
			frames[i].number_of_locals = reader.get2Byte();
			frames[i].locals = new Attri_StackMapTable::verification_type_info[frames[i].number_of_locals];

			for(size_t j = 0; j < frames[i].number_of_locals; j++){
				frames[i].locals[j].top_var.tag = reader.getByte();
				if(frames[i].locals[j].top_var.tag == 7 || frames[i].locals[j].top_var.tag == 8){
					frames[i].locals[j].object_var.cpool_index = reader.get2Byte();
				}
			}

			frames[i].number_of_stack_items = reader.get2Byte();
			frames[i].stacks = new Attri_StackMapTable::verification_type_info[frames[i].number_of_stack_items];

			for(size_t j = 0; j < frames[i].number_of_stack_items; j++){
				frames[i].stacks[j].top_var.tag = reader.getByte();
				if(frames[i].stacks[j].top_var.tag == 7 || frames[i].stacks[j].top_var.tag == 8){
					frames[i].stacks[j].object_var.cpool_index = reader.get2Byte();
				}
			}
		}
	}
}
void Attri_StackMapTable::cleanUp() {
	for(size_t i = 0; i < number_of_entries; i++){
		delete[] frames[i].locals;
		delete[] frames[i].stacks;
	}
}

void Attri_LocalVariableTypeTable::read(ClassReader& reader, AttributeReader& attribute_reader){
	local_variable_type_table_length = reader.get2Byte();
	local_variable_type_table = new local_variable_type[local_variable_type_table_length];
	for(size_t i = 0; i < local_variable_type_table_length; i++){
		local_variable_type_table[i].start_pc = reader.get2Byte();
		local_variable_type_table[i].length = reader.get2Byte();
		local_variable_type_table[i].name_index = reader.get2Byte();
		local_variable_type_table[i].signature_index = reader.get2Byte();
		local_variable_type_table[i].index = reader.get2Byte();
	}
}

void Attri_LocalVariableTypeTable::cleanUp(){
	delete[] local_variable_type_table;
}

void Attri_BootstrapMethod::read(ClassReader& reader, AttributeReader& attribute_reader){
	num_bootstrap_methods = reader.get2Byte();
	bootstrapMethods = new Attri_BootstrapMethod::bootstrap_method[num_bootstrap_methods];
	for(size_t i = 0; i < num_bootstrap_methods; i++){
		bootstrapMethods[i].bootstrap_method_ref = reader.get2Byte();
		bootstrapMethods[i].num_bootstrap_arguments= reader.get2Byte();
		bootstrapMethods[i].bootstrap_arguments = new short[bootstrapMethods[i].num_bootstrap_arguments];
		for(size_t j = 0; j < bootstrapMethods[i].num_bootstrap_arguments; j++){
			bootstrapMethods[i].bootstrap_arguments[j] = reader.get2Byte();
		}
	}
}

void Attri_BootstrapMethod::cleanUp(){
	for(size_t i = 0; i < num_bootstrap_methods; i++){
		delete[] bootstrapMethods[i].bootstrap_arguments;
	}
	delete[] bootstrapMethods;
}