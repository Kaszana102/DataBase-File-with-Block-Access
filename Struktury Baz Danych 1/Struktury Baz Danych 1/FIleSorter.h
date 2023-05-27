#pragma once
#include "File.h"
#include "Block.h"

class FileSorter
{
	bool showEachPhase = false;
	bool showWholeFile = false;

	int tapeOtp;
	int hollowRecords;
	bool hollowRecordsOnTape1;
	void FillFibonacilly(Block* src, Block* block1, Block* block2);
	bool MergeToTape(Block* tape1, Block* tape2, Block* tape3);
	bool MergeSingleRun(Block* otp, Block* tape1, Block* tape2);
	void MergeSingleHollow(Block* otp, Block* src);


	void PrintFiles(File* file1, File* file2,File* file3);
public:
	FileSorter();
	FileSorter(bool showEachPhase);
	File* SortFile(File* src);

	void PrintWholeFiles(bool state);

};

