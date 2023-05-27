#include "Record.h"

Record::Record()
{
	for (int i = 0; i < 5; i++) {
		vals[i] = 0;
	}
	key = 0.0f;
}

Record::Record(int val)
{	
	for (int i = 0; i < 5; i++) {
		this->vals[i] = val;
	}
	key = val;
}

Record::Record(int vals[5])
{	
	for (int i = 0; i < 5; i++) {
		this->vals[i] = vals[i];		
	}	

	UpdateKey();
}

Record::Record(int a, int b, int c, int d, int e)
{
	vals[0] = a;
	vals[1] = b;
	vals[2] = c;
	vals[3] = d;
	vals[4] = e;

	key = (float)(a + b + c + d + e) / 5;
	
}

void Record::UpdateKey()
{
	int sum = 0;
	for (int i = 0; i < 5; i++) {		
		sum += vals[i];
	}
	key = (float)(sum) / 5;
}

void Record::SetHollow()
{
	this->isHollow = true;
}

bool Record::GetHollow()
{
	return isHollow;
}

void Record::SetNull()
{
	isNull = true;
}

bool Record::GetNull()
{
	return isNull;
}

void Record::Write()
{
}

float Record::GetKey()
{
	return key;
}

string Record::ToText()
{
	string otp="";
	for (int i = 0; i < 5; i++) {
		otp += std::to_string(vals[i]);
		otp += " ";
	}
	return otp;
}

string Record::OnlyValsText()
{
	string otp = "";
	for (int i = 0; i < 5; i++) {
		otp += to_string(vals[i]);
		otp += " ";
	}
	return otp;
}

void Record::ConvertFromBinary(char str[5 * sizeof(int)])
{
	int* ints = (int*)str;
	for (int i = 0; i < 5; i++) {
		vals[i] = ints[i];
	}

	this->UpdateKey();
}

char* Record::ConvertToBinary()
{
	int* otp = new int[sizeof(int) * 5];

	for (int i = 0; i < 5; i++) {
		 otp[i] = vals[i];
	}

	return (char*)otp;
}
