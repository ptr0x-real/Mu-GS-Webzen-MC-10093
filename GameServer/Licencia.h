#pragma once

struct LICENSE
{
	unsigned long  TempSerial;
	unsigned long  Key;
};

extern LICENSE pLicense;

void ReadyLicense();
bool MACProtection();

