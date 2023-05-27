#pragma once

#include "Record.h"
#include <vector>
#include <fstream>

#define IN 

using namespace std;
class File
{
	//vector<Record> records;	//TODO 
	fstream *recordFile; //PLIK SEKWENCYJY (ten najgorszy XD)
	string name;
	int recordsToOmit;

	bool onlyKeys;


public:	
	File(string name);

	void SetOnlyKeys(bool state);

	void WriteRecord(Record record);

	Record ReadRecord();
	Record ReadRecord(int index);
	char* ReadBlock(int size, IN int* numberOfRecords );

	void DeleteFirstRecord();
	void ClearFile();

	int GetRunsNumber();
	int GetRecordsNumber();	



	friend ostream& operator<<(ostream& os, File& file)
	{
		/*
		// write obj to stream
		for (int i = 0; i < file.records.size(); i++) {
			if (file.onlyKeys) {
				os << file.records[i].GetKey();

				if (i != file.records.size() - 1) {
					os << " | ";
				}
			}
			else {
				os << file.records[i];

				if (i != file.records.size() - 1) {
					os << " | ";
				}
			}
		}		
		*/

		//file.recordFile->open(file.name, fstream::in);

		Record record;
		for (int i = 0; record.GetNull() != true; i++) {
			record = file.ReadRecord(i);
			if (record.GetNull() != true) {
				os << record << " ";
			}
		}		

		//file.recordFile->close();
		return os;
	}



	friend istream& operator>>(istream& is, File& file)
	{
		Record record;
		while (is >> record) {
			file.WriteRecord(record);
		}				

		return is;
	}

};

