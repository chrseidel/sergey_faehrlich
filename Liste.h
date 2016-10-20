#pragma once

class Liste
{
private:
	static Liste *lstAnfang;
	static Liste *lstEnde;
	Liste *prev;
	Liste *next;
public:
	Liste();
	~Liste();
};
