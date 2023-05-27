#include "FileSorter.h"

#define BLOCK_SIZE 100

using namespace std;


// T1 T2
// 0  0
// 1  0
// 1  1
// 2  1
// 2  3
// 5  3
//...

void FileSorter::FillFibonacilly(Block* src, Block* block1, Block* block2)
{
	int file1Size = 0, file2Size = 0;
	int newFib;
	int runsToWrite = 1;
	float lastBlock1Key = -INFINITY;
	float lastBlock2Key = -INFINITY;
	bool filling_block1=true;
	Record* record;	
	
	record = src->ReadRecord();
	src->DeleteFirstRecordInFile();
	while (record != nullptr) {  
		if (filling_block1) {
			if (record->GetKey() < lastBlock1Key) {
				runsToWrite--;
				file1Size++;
				if (runsToWrite == 0) {
					filling_block1 = !filling_block1;					

					runsToWrite = file1Size;

					//check sklejenie
					if (record->GetKey() > lastBlock2Key && lastBlock2Key!= -INFINITY) {
						runsToWrite += 1;
						file2Size--;
					}
				}
			}
		}
		else {
			if (record->GetKey() < lastBlock2Key) {
				runsToWrite--;
				file2Size++;

				if (runsToWrite == 0) {
					filling_block1 = !filling_block1;
					runsToWrite = file2Size;

					//check sklejenie
					if (record->GetKey() > lastBlock1Key && lastBlock1Key != -INFINITY) {
						runsToWrite += 1;
						file1Size--;
					}
				}
			}
		}

		if (filling_block1) {
			block1->WriteRecord(*record);			
			lastBlock1Key = record->GetKey();
		}
		else {
			block2->WriteRecord(*record);		
			lastBlock2Key = record->GetKey();
		}
		
		record = src->ReadRecord();	
		src->DeleteFirstRecordInFile();
	}

	
	runsToWrite--;
	hollowRecords = runsToWrite;
	if(filling_block1) {
		hollowRecordsOnTape1 = true;
	}
	else {
		hollowRecordsOnTape1 = false;
	}

	block1->WriteBlock();
	block2->WriteBlock();		
}


//returns false when one tape was empty
bool FileSorter::MergeSingleRun(Block* otp, Block* tape1, Block* tape2) {
	bool merging = true;
	bool tapeEmptied=false;
	bool taping1 = true, taping2 = true;
	Record *record1, *record2;
	record1 = tape1->ReadRecord();
	record2 = tape2->ReadRecord();

	float lastrecord1Key;
	float lastrecord2Key;
	
	if (record1 == nullptr ) {
		taping1 = false;
		tapeEmptied = true;
	}
	else {
		tape1->DeleteFirstRecordInFile();
		lastrecord1Key = record1->GetKey();
	}
	if (record2 == nullptr) {
		taping2 = false;
		tapeEmptied = true;
	}
	else {
		lastrecord2Key = record2->GetKey();
		tape2->DeleteFirstRecordInFile();
	}

	while (merging) {
		if (taping1 && taping2) {
			//compare both keys
			if (record1->GetKey() <= record2->GetKey()) {
				otp->WriteRecord(*record1);
				record1 = tape1->ReadRecord();
				if (record1 != nullptr) {
					if (lastrecord1Key > record1->GetKey()) {
						taping1 = false;
						tape1->ReadTheSameRecordNextTime();
					}
					else {
						tape1->DeleteFirstRecordInFile();
					}
					lastrecord1Key = record1->GetKey();
					
				}
				else {					
					taping1 = false;
					tapeEmptied = true;
				}				
			}
			else {
				otp->WriteRecord(*record2);
				record2 = tape2->ReadRecord();
				if (record2 != nullptr) {
					if (lastrecord2Key > record2->GetKey()) {
						taping2 = false;	
						tape2->ReadTheSameRecordNextTime();
					}
					else {
						tape2->DeleteFirstRecordInFile();
					}
					lastrecord2Key = record2->GetKey();					
				}
				else {
					taping2 = false;
					tapeEmptied = true;
				}
			}
		}
		else {
			if (taping1) {
				//put only record1 key
				otp->WriteRecord(*record1);
				record1 = tape1->ReadRecord();
				if (record1 != nullptr) {
					if (lastrecord1Key > record1->GetKey()) {
						taping1 = false;	
						tape1->ReadTheSameRecordNextTime();
					}
					else {
						tape1->DeleteFirstRecordInFile();
					}
					lastrecord1Key = record1->GetKey();
					
				}
				else {
					taping1 = false;
					tapeEmptied = true;
				}
			}
			else if(taping2) {
				//put only record2 key
				otp->WriteRecord(*record2);
				record2 = tape2->ReadRecord();
				if (record2 != nullptr) {
					if (lastrecord2Key > record2->GetKey()) {
						taping2 = false;							
						tape2->ReadTheSameRecordNextTime();
					}
					else {
						tape2->DeleteFirstRecordInFile();
					}
					lastrecord2Key = record2->GetKey();					
				}
				else {
					taping2 = false;
					tapeEmptied = true;
				}
			}
		}
		if (!taping1 && !taping2) {
			merging = false;
		}
		
	}

	//otp->WriteBlock();
	

	return tapeEmptied;
}



bool FileSorter::MergeToTape(Block* tape1, Block* tape2, Block* tape3)
{
	bool stillNeedSorting = true;
	switch (tapeOtp) {
		case 1:
			while (!MergeSingleRun(tape1, tape2, tape3)) {				
			};
			tape1->WriteBlock();
			
			//check which tape is empty
			if (tape2->ReadRecord() == nullptr) {				
				//tape2.->ClearFile();
				tape2->Flush();
				if (tape3->ReadRecord() == nullptr) {
					stillNeedSorting = false;
					//tape3->ClearFile();
					tape3->Flush();
				}
				else {
					//tape3->ReadTheSameRecordNextTime();
					this->tapeOtp = 2;
				}
			}		
			else {
				//tape2->ReadTheSameRecordNextTime();
				this->tapeOtp = 3;
				//tape3->ClearFile();
				tape3->Flush();
			}

			break;
		case 2:
			while (!MergeSingleRun(tape2, tape1, tape3)) {				
			};
			tape2->WriteBlock();
			//check which tape is empty
			if (tape1->ReadRecord() == nullptr) {
				//tape1->ClearFile();
				tape1->Flush();
				if (tape3->ReadRecord() == nullptr) {
					stillNeedSorting = false;
					//tape3->ClearFile();
					tape3->Flush();
				}
				else {
					//tape3->ReadTheSameRecordNextTime();
					this->tapeOtp = 1;
				}
			}
			else {
				//tape1->ReadTheSameRecordNextTime();
				this->tapeOtp = 3;
				//tape3->ClearFile();
				tape3->Flush();
			}
			break;
		case 3:			
			//only at the beginnig!
			while (hollowRecords > 0) {
				if (hollowRecordsOnTape1) {
					MergeSingleHollow(tape3,tape2);					
					tape2->ReadTheSameRecordNextTime();
				}
				else {
					MergeSingleHollow(tape3, tape1);					
					tape1->ReadTheSameRecordNextTime();
				}
				hollowRecords--;
				if (hollowRecords == 0) {												
				}
			}			


			while(!MergeSingleRun(tape3, tape1, tape2)) {				
			};
			tape3->WriteBlock();

			//check which tape is empty
			if (tape1->ReadRecord() == nullptr) {
				//tape1->ClearFile();
				tape1->Flush();
				if (tape2->ReadRecord() == nullptr) {
					stillNeedSorting = false;
					//tape2->ClearFile();
					tape2->Flush();
				}
				else {
					//tape2->ReadTheSameRecordNextTime();
					this->tapeOtp = 1;
				}
			}
			else {
				//tape1->ReadTheSameRecordNextTime();
				this->tapeOtp = 2;
				//tape2->ClearFile();
				tape2->Flush();
			}

			break;
	}

	return stillNeedSorting;
}

void FileSorter::PrintFiles(File* file1, File* file2, File* file3)
{
	if (showWholeFile) {

		if (hollowRecords > 0) {
			if (hollowRecordsOnTape1) {
				cout << "I   " << *file1;
				for (int i = 0; i < hollowRecords; i++) {
					cout << " " << "([x] | x x x x x)";
				}
				cout << endl;

				cout << "II  " << *file2 << endl;
			}
			else {
				cout << "I   " << *file1<<endl;				
				cout << "II  " << *file2;
				for (int i = 0; i < hollowRecords; i++) {
					cout << " " << "([x] | x x x x x)";
				}				
				cout << endl;
			}
			cout << "III " << *file3 << endl << endl;
		}
		else{
			cout << "------------------------" << endl;
			cout << "I   " << *file1 << endl;
			cout << "II  " << *file2 << endl;
			cout << "III " << *file3 << endl;
			cout << "------------------------" << endl << endl;
		}
	}
	else {
		//show Only number of records
		if (hollowRecords > 0) {
			cout << "------------------------" << endl;
			if (hollowRecordsOnTape1) {
				cout << "I   " << file1->GetRunsNumber() + hollowRecords << '(' << hollowRecords << ')' << endl;
				cout << "II  " << file2->GetRunsNumber() << endl;
			}
			else {
				cout << "I   " << file1->GetRunsNumber() << endl;
				cout << "II  " << file2->GetRunsNumber() + hollowRecords << '(' << hollowRecords << ')' << endl;
			}
			cout << "III " << file3->GetRunsNumber() << endl;
			cout << "------------------------" << endl << endl;
		}
		else {
			cout << "------------------------" << endl;
			cout << "I   " << file1->GetRunsNumber() << endl;
			cout << "II  " << file2->GetRunsNumber() << endl;
			cout << "III " << file3->GetRunsNumber() << endl;
			cout << "------------------------" << endl << endl;
		}
	}
}

void FileSorter::PrintWholeFiles(bool state) {
	showWholeFile = state;
}

FileSorter::FileSorter()
{
	showEachPhase = false;
	tapeOtp = 3;
}

FileSorter::FileSorter(bool showEachPhase)
{
	this->showEachPhase = showEachPhase;
	tapeOtp = 3;
}


/// <summary>
/// 
/// </summary>
/// <param name="otp">destination block</param>
/// <param name="src">source block</param>
void FileSorter::MergeSingleHollow(Block* otp, Block* src)
{
	/*Record* record;
	record = src->ReadRecord();
	src->DeleteFirstRecordInFile();
	otp->WriteRecord(*record);
	*/

	bool merging = true;	
	Record* record1;	

	float lastrecord1Key;
	float lastrecord2Key;
	
	record1 = src->ReadRecord();
	src->DeleteFirstRecordInFile();
	lastrecord1Key = record1->GetKey();

	while (merging) {
			//put only record1 key
			otp->WriteRecord(*record1);
			record1 = src->ReadRecord();
			if (record1 != nullptr) {
				if (lastrecord1Key > record1->GetKey()) {
					merging = false;
				}
				else {
					src->DeleteFirstRecordInFile();
				}
				lastrecord1Key = record1->GetKey();

			}
			else {
				merging = false;				
			}				

	}

	//otp->WriteBlock();



}

File* FileSorter::SortFile(File* src)
{
	File  *file1, *file2, *file3;

	file1 = new File("1");
	file2 = new File("2");
	file3 = new File("3");
	Block blockSrc(BLOCK_SIZE,src);
	Block block1(BLOCK_SIZE,file1), block2(BLOCK_SIZE, file2), block3(BLOCK_SIZE, file3);//buffers	
	
	int numberOfRunsAtStart = src->GetRunsNumber();
	int numberOfRecordsAtStart = src->GetRecordsNumber();


	//fill first time (may need empty runs)
	FillFibonacilly(&blockSrc,&block1,&block2);
	
	PrintFiles( file1,  file2, file3);
	


	//sort while unsorted
	bool sorting = true;	
	tapeOtp = 3;
	int phasesNumber = 0;
// ----------------
// 	1  |  2  |  3
// ----------------
//     |     |  
	while (sorting) {
		sorting = MergeToTape(&block1, &block2, &block3);		
		block1.StartReadingFromBeggining();		
		block2.StartReadingFromBeggining();
		block3.StartReadingFromBeggining();

		if (showEachPhase) {
			PrintFiles(file1, file2, file3);
		}
		phasesNumber++;
	}


	//print sorting numbers	
	//liczba faz
	cout << "oczekiwana liczba faz: ceil(" << 1.45 * std::log2(numberOfRunsAtStart) <<") = " <<ceil(1.45 * std::log2(numberOfRunsAtStart)) << endl;
	cout << "faktyczna liczba faz: " << phasesNumber<<endl;
	//liczba odczytów
	int reads = block1.GetNumberOfRead() + block2.GetNumberOfRead() + block3.GetNumberOfRead();
	cout << "liczba odczytow: " << reads << endl;
	//liczba zapisów
	int writes = block1.GetNumberOfWrites() + block2.GetNumberOfWrites() + block3.GetNumberOfWrites();
	cout << "liczba zapisow: " << writes << endl;

	//liczba operacji dyskowych
	cout << "szaconawa liczba operacji dyskowych: " << 2 * numberOfRecordsAtStart * (1.04 * std::log2(numberOfRunsAtStart)+1)/BLOCK_SIZE << endl;
	cout << "liczba operacji dyskowych: " << writes + reads << endl;



	if (file1->GetRecordsNumber() > 0) {
		return file1;
	}

	if (file2->GetRecordsNumber() > 0) {
		return file2;
	}

	if (file3->GetRecordsNumber() > 0) {
		return file3;
	}
	return nullptr;
}
