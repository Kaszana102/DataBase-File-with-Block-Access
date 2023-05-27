#include "Block.h"

Block::Block(int size)
{
	this->size = size;
	numberOfReads = 0;
	numberOfWrites = 0;
	//recordIndexInFile = 0;
}

Block::Block(int size, File* file)
{
	this->size = size;
	this->file = file;
	lastBlockIndex = size;
	numberOfReads = 0;
	numberOfWrites = 0;
	//recordIndexInFile = 0;
}

void Block::SetFile(File* file)
{
	this->file = file;	
	lastBlockIndex = size;
}

void Block::ReadBlock()
{
	records.clear();
	Record record;
	/*
	for (int i = 0; i < size; i++) {
		
		//sprawdzenie czy mozna wczytac!!
		record = file->ReadRecord(i);
		if (!record.GetNull()) {
			records.push_back(record);
		}
		

	}
	*/
	int recordsNumber;
	int* binaryStr;
	binaryStr =(int*) file->ReadBlock(this->size,&recordsNumber);

	for (int i = 0; i < recordsNumber; i++) {
		record.ConvertFromBinary((char*) & (binaryStr[5 * i]));
		records.push_back(record);
	}

	numberOfReads++;
}

void Block::WriteBlock()
{
	if (records.size() != 0) {
		for (int i = 0; i < records.size(); i++) {
			file->WriteRecord(records.at(i));
		}
		records.clear();
		numberOfWrites++;
	}
	
}

Record* Block::ReadRecord()
{
	if(!readLastReadRecord) {
		if (lastBlockIndex == size) {
			lastBlockIndex = 0;
			ReadBlock();
		}

		if (lastBlockIndex < records.size()) {
			lastReadRecord = records.at(lastBlockIndex);
			return &records.at(lastBlockIndex++);
		}
		else {
			return nullptr;
		}
	}
	else {
		readLastReadRecord = false;
		return &lastReadRecord;
	}

}

void Block::ReadTheSameRecordNextTime()
{
	this->readLastReadRecord = true;

}

void Block::WriteRecord(Record record)
{	
	if (records.size() == size) {
		WriteBlock();			
	}
	records.push_back(record);
	
}

void Block::Flush()
{
	records.clear();
}

void Block::ClearFile()
{
	file->ClearFile();
}

void Block::DeleteFirstRecordInFile()
{
	file->DeleteFirstRecord();
}

void Block::StartReadingFromBeggining()
{
	this->lastBlockIndex = size;
	//this->recordIndexInFile = 0;

}

int Block::GetNumberOfRead()
{
	return numberOfReads;
}

int Block::GetNumberOfWrites()
{
	return numberOfWrites;
}
