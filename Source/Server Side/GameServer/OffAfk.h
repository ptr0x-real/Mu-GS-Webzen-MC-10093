#ifndef __OFFSYSTEM_H__
#define __OFFSYSTEM_H__

class COFFSystem
{
public:
	void CreateOfflineSystem(int aIndex);
	void Closeoffafk(char * AccountID);
	bool Updateoffafk(int UserIndex, BYTE Type);
}; 

extern COFFSystem OFFSystem;

#endif