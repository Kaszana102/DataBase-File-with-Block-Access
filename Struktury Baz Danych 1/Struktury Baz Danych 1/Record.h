#pragma once

#include <iostream>
#include <string>

using namespace std;

class Record
{
	int vals[5];
	float key;
	bool isHollow=false;
	bool isNull=false;
public:
	Record();
	Record(int val);
	Record(int vals[5]);
	Record(int a, int b, int c, int d, int  );

	void UpdateKey();


	void SetHollow();
	bool GetHollow();

	void SetNull();
	bool GetNull();

	void Write();
	float GetKey();

	string ToText();
	string OnlyValsText();
	char* ConvertToBinary();
	void ConvertFromBinary(char str[5 * sizeof(int)]);

	friend ostream& operator<<(ostream& os, const Record& record)
	{
		if (!record.isHollow) {
			// write obj to stream
			os << "([" << record.key << "]";
			for (int i = 0; i < 5; i++) {
				os << " " << record.vals[i];
			}
			os << ")";
		}
		else {
			os << "([X]";
			for (int i = 0; i < 5; i++) {
				os << " X";
			}
			os << ")";
		}
		return os;
	}



	/// <summary>
	/// format rekordu a b c d e 
	/// 5 liczb. potem jest wyliczany klucz glowny
	/// </summary>	
	friend istream& operator>>(istream& is, Record& record)
	{
		for (int i = 0; i < 5; i++) {
			is >> record.vals[i];
		}
		record.UpdateKey();
		
		return is;
	}
};

