#pragma once
#include "SpaCE/applikation.h"
#include "Attribute.h"

class mein_partikelsystem : public partikelsystem
	{
	private:
		partikeldaten *pd;
		D3DXVECTOR3		Posi;
	public:
		mein_partikelsystem(){pd = 0;}
		~mein_partikelsystem(){if( pd) delete[] pd;}
		void loeschen() { if( pd) 
							{
							 partikeldaten *hpd=pd;
							 pd=0;
							 delete[] hpd;
							}
						}
		void set_partikel( int context, int i, D3DXVECTOR3 &pos, D3DCOLOR &ambient, D3DCOLOR &emissive);
		void create( int anz,int Art);
		void set_posi(D3DXVECTOR3 &Offset){Posi=Offset;}
	};