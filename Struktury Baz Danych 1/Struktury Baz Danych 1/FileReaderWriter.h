#pragma once
#include "File.h"
class FileReaderWriter
{

public:
	Record ReadRecord();
	void WriteRecord(Record record); 
};

