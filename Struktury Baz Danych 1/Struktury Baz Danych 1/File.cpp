#include "File.h"
#include <fstream>
#include <stdlib.h>


File::File(string name)
{
	onlyKeys = false;
	recordFile = new fstream(name, fstream::out);
	recordFile->close();
	this->name = name;		
	recordsToOmit = 0;
}

void File::SetOnlyKeys(bool state)
{
	onlyKeys = state;
}

void File::WriteRecord(Record record)
{
	//records.push_back(record);
	recordFile->open(this->name, fstream::app | std::ios::binary | fstream::out);
		
	recordFile->write(record.ConvertToBinary(), sizeof(int) * 5); // or with recent C++: reinterpret_cast<std::byte*>

	recordFile->close();
}

Record File::ReadRecord()
{
	/*
	if (records.size()>jê0) {
		Record record = records.at(0);				
		return record;
	}
	else {
		Record nullRecord;
		nullRecord.SetNull();
		return nullRecord;
	}
	*/
	return ReadRecord(0);
}

Record File::ReadRecord(int index)
{
	/*
	if (records.size() > index) {
		Record record = records.at(index);
		return record;
	}
	else {
		Record nullRecord;
		nullRecord.SetNull();
		return nullRecord;
	}
	*/


	Record record;

	recordFile->open(this->name, fstream::in | fstream::binary);

	//MOVE TO PROPER PLACE AND THEN READ
	recordFile->seekp(sizeof(int)*5 * (recordsToOmit + index));

	if (recordFile->peek()!=EOF) {//if record exists
		char str[5 * sizeof(int)];
		recordFile->read(str, 5 * sizeof(int));

		record.ConvertFromBinary(str);
	}
	else {
		record.SetNull();		
	}
	recordFile->close();
	return record;
}

/// <summary>
/// 
/// </summary>
/// <param name="recordIndex"></param>
/// <param name="size">- block size</param>
/// <param name="numberOfRecords"></param>
/// <returns></returns>
char* File::ReadBlock(int size, IN int* numberOfRecords)
{	
	char* binaryStr = new char[size * sizeof(int) * 5];

	recordFile->open(this->name, fstream::in | fstream::binary);

	//MOVE TO PROPER PLACE AND THEN READ
	recordFile->seekp(sizeof(int) * 5 * recordsToOmit);

	if (recordFile->peek() != EOF) {//if record exists				
		recordFile->read(binaryStr, 5 * sizeof(int) * size);
		
		*numberOfRecords = recordFile->gcount() / (5 * sizeof(int)); //not working :<
	}
	else {
		*numberOfRecords = 0;
	}
	recordFile->close();
	return binaryStr;
}


void File::DeleteFirstRecord()
{	

	/*
	recordFile->open(this->name, fstream::in);

	Record record;	
	fstream* newFile;
	newFile = new fstream("temp", fstream::out);

	*recordFile >> record; //ignore the first one

	while (*recordFile >> record) {		
		*newFile << record.OnlyValsText();
	}	

	//close files
	recordFile->close();
	newFile->close();

	std::remove(this->name.c_str());	//delete old
	std::rename("temp", this->name.c_str()); //rename new
	*/
	recordsToOmit++;

	/*
	if (records.size() > 0) {
		records.erase(records.begin());
	}
	*/	
}

void File::ClearFile()
{
	//records.clear();
	
	recordFile->close();
	recordFile->open(this->name, std::ifstream::trunc);
	recordFile->close();

}

int File::GetRunsNumber()
{
	float lastKey = INFINITY;
	int runsNumber = 0;
	/*
	for(int i=0; i< records.size(); i++){
		if (records[i].GetKey() < lastKey) {
			runsNumber++;
		}		
		lastKey = records[i].GetKey();

	}*/
	
		

	Record record= ReadRecord(0);
	for (int i = 1; record.GetNull() != true; i++) {
		if (record.GetKey() < lastKey) {
			runsNumber++;
		}
		lastKey = record.GetKey();
		record = ReadRecord(i);		
	}
	
	return runsNumber;
}

int File::GetRecordsNumber()
{
	int recordNumber = 0;
	Record record = ReadRecord(0);
	for (int i = 1; record.GetNull() != true; i++) {
		recordNumber++;
		record = ReadRecord(i);
	}
	
	
	//return records.size();
	return recordNumber;
}

