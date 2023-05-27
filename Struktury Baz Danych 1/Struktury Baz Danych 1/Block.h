#pragma once
#include "File.h"
class Block
{
	int size;
	vector<Record> records;
	Record lastReadRecord;
	bool readLastReadRecord;
	File* file;	
	int lastBlockIndex;

	int numberOfReads, numberOfWrites;
	//int recordIndexInFile;

public:

	Block(int size);	
	Block(int size, File* file);	

	void SetFile(File* file);

	void ReadBlock();
	void WriteBlock();
	Record* ReadRecord();
	void ReadTheSameRecordNextTime();
	void WriteRecord(Record record);	
	void Flush();
	void ClearFile();
	void DeleteFirstRecordInFile();
	void StartReadingFromBeggining();


	int GetNumberOfRead();
	int GetNumberOfWrites();

};

