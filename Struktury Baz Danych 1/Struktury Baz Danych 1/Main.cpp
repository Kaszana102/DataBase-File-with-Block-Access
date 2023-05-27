#include <stdio.h>

#include <iostream>
#include <fstream>
#include "FileSorter.h"

using namespace std;

bool showEachPhase;
bool printWholeFiles;


void TestDebug() {
	File file("src");
	for (int i = 12; i >= 0; i--) {
		file.WriteRecord(Record(2 * i));
		file.WriteRecord(Record(i));		
	}

	FileSorter fileSorter(showEachPhase);
	fileSorter.PrintWholeFiles(printWholeFiles);

	File* sortedFile = fileSorter.SortFile(&file);
	cout <<endl<< "zawartosc pliku:\n" << *sortedFile;
}

void TextFromCommandLine() {
	cout << "write records number\n";
	int recordsNumber;
	cin >> recordsNumber;
	int key;

	File file("src");
	FileSorter fileSorter(showEachPhase);
	fileSorter.PrintWholeFiles(printWholeFiles);

	for (int i = 0; i < recordsNumber; i++) {
		cin >> key;
		file.WriteRecord(Record(key));
	}

	File* sortedFile = fileSorter.SortFile(&file);
	cout << endl << "zawartosc pliku:\n" << *sortedFile;
}

void TextFromFile() {
	File file("filesrc");
	ifstream src("TestFile1.txt", ios::in); //choose file
	ofstream otp("otp.txt", ios::out);		
	//read whole file	
	src>>file;
	//print whole file
	//cout<<file;


	FileSorter fileSorter(showEachPhase);
	fileSorter.PrintWholeFiles(printWholeFiles);
	File* sortedFile = fileSorter.SortFile(&file);
	
	//otp << *sortedFile;
	otp << *sortedFile;	
	otp.close();	

	cout << endl << "zawartosc pliku:\n" << *sortedFile;
}


void GenerateTestFiles() {

	char filename[] = "TestFile?.txt";

	for (int i = 0; i < 5; i++) { //stworz 5 plików
		File file("");
		for (int j = 0; j < pow(10,i+1); j++) { //stworz 5 plików
			Record record(rand(),rand(),rand(), rand(), rand());
			file.WriteRecord(record);

		}
		filename[8] = '0' + i + 1;				
		//file.SaveFile(filename);
	}
}


int main() {
	//GenerateTestFiles();
	
	cout << "print whole files? (1-true,0-false)\n";
	cin >> printWholeFiles;
	cout << "show each phase? (1-true,0-false)\n";
	cin >> showEachPhase;

	int opcja = -1;
	while (opcja < 1 || opcja > 3) {
		cout << "Wybierz opcje\n";
		cout << "1) TestDebug\n";
		cout << "2) TextFromCommandLine\n";
		cout << "3) TextFromFile\n";
		
		cin >> opcja;
	}

	switch (opcja) {
	case 1:
		TestDebug();
		break;
	case 2:
		TextFromCommandLine();
		break;
	case 3:	
		TextFromFile();
		break;
	}
	
	


	return 0;
}