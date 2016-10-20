#include "Liste.h"

Liste *Liste::lstAnfang = 0;
Liste *Liste::lstEnde = 0;

Liste::Liste()
{
	prev=lstEnde;
	next=0;

	if(lstEnde)
		lstEnde->next=this;
	else
		lstAnfang=this;
	lstEnde=this;
}

Liste::~Liste()
{
	if( prev)
		prev->next = next;
	else
		lstAnfang = next;

	if( next)
		next->prev = prev;
	else
		lstEnde = prev;
}
