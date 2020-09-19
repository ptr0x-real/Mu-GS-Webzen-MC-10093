#ifndef __OFFTRADE_H__
#define __OFFTRADE_H__

class COFFtrade
{
public:
	void CreateOfflinetrade(int aIndex);
	void Closeofftrade(char * AccountID);
	bool Updateofftrade(int UserIndex, BYTE Type);
}; 

extern COFFtrade OFFtrade;

#endif