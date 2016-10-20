#include "explosionen.h"
#include <time.h>

class zufall
	{
	public:
		zufall(){srand(timeGetTime());}
		int zahl( int min, int max){ return rand()%(max-min+1) + min;}
	};

zufall mein_zufall1;

void mein_partikelsystem::set_partikel( int context, int i, D3DXVECTOR3 &position, D3DCOLOR &ambient, D3DCOLOR &emissive)
{
	if( context == 0)
	{
		position = D3DXVECTOR3( 0, 0, 0);
		pd[i].life = 1;//-mein_zufall1.zahl( 190, 200);
	}
	if( context == 1)
	{
		D3DXMATRIX m;
		D3DXVECTOR3 sp;
		float z;
		pd[i].steps=0;
		pd[i].steps+=mein_zufall1.zahl(0,10);
		pd[i].life = mein_zufall1.zahl(10,50);
		position =  Posi;
		position.x+= (mein_zufall1.zahl(-4000,0)/1000.0f)-5.0f;
		position.y+= (mein_zufall1.zahl(-500,500)/1000.0f);
		position.z+= (mein_zufall1.zahl(-500,500)/1000.0f)+0.2f;
		z = mein_zufall1.zahl( -90, 90)/1000.0f;
		sp = D3DXVECTOR3( 0, z, z);
		D3DXMatrixRotationYawPitchRoll( &m, mein_zufall1.zahl( 0, 1000)/2500.0f, mein_zufall1.zahl( 0, 1000)/2500.0f, mein_zufall1.zahl( 0, 1000)/2500.0f);
		D3DXVec3TransformNormal( &sp, &sp, &m);
		sp.x-=1.4f;
		pd[i].speed = sp;
		ambient = D3DCOLOR_ARGB(255,140,140,220);//(mein_zufall1.zahl( 125,200)), (mein_zufall1.zahl( 0,0)) ,(mein_zufall1.zahl( 0, 0)));
		emissive  = ambient;
	}
	if(context == 2)
	{
		if( pd[i].life < 0)
			pd[i].life++;
		else
			pd[i].life--;

		if( pd[i].life)
		{
			pd[i].steps++;
			pd[i].speed.x += (mein_zufall1.zahl( -80, 80)/1000.0f);
			pd[i].speed.y += (mein_zufall1.zahl( -20, 20)/1000.0f);
			pd[i].speed.z += (mein_zufall1.zahl( -20, 20)/1000.0f);
			position += pd[i].speed;
			int farb= (mein_zufall1.zahl( 125,150));
			if(pd[i].steps>=20 && pd[i].steps<=47)
			{
				ambient = D3DCOLOR_ARGB( 255,farb,farb-100,farb-100);
			
			}
			if(pd[i].steps<10)
			{
				ambient = D3DCOLOR_ARGB( 255,farb-50,farb-5,farb );//(mein_zufall1.zahl( 125,200)), (mein_zufall1.zahl( 0,0)) ,(mein_zufall1.zahl( 0, 0)));
			}
			emissive  = ambient;
			pd[i].steps+=mein_zufall1.zahl(0,2);
			//ambient = D3DCOLOR_ARGB( 255 ,0 , 0, 0);//ambient = D3DCOLOR_ARGB( 0, 255, 255, 255);
			//emissive  = D3DCOLOR_ARGB( 0, (mein_zufall1.zahl( 0,255)), (mein_zufall1.zahl( 0, 255)) ,(mein_zufall1.zahl( 0, 255)));
		}
		else
			set_partikel( 1, i, position, ambient, emissive);
	}
	if(context==3)
	{
		if( pd[i].life < 0)
			pd[i].life++;
		else
			pd[i].life--;

		if( pd[i].life)
		{
			pd[i].steps++;
			pd[i].speed.x += (mein_zufall1.zahl( -1000, 1000)/1000.0f);
			pd[i].speed.y += (mein_zufall1.zahl( -1000, 900)/1000.0f);
			pd[i].speed.z += (mein_zufall1.zahl( -1000, 1000)/1000.0f);
			position += pd[i].speed;
			ambient = D3DCOLOR_ARGB( 255 ,0 , 0, 0);//ambient = D3DCOLOR_ARGB( 0, 255, 255, 255);
			int fHelp;
			fHelp=mein_zufall1.zahl(0,255);
			ambient = D3DCOLOR_ARGB( 255, 255, 255-(int)(pd[i].life*4.6f) ,0);//256-pd[i].life*4);
			if(pd[i].steps>5)
			{
			//ambient = D3DCOLOR_ARGB( 255, pd[i].life*4,0 ,0);//256-pd[i].life*4);
			}
			else
			{
			//ambient = D3DCOLOR_ARGB( 255, 0,pd[i].life*4-40,pd[i].life*4-40);//256-pd[i].life*4);
			}
			emissive  = ambient;
			//emissive  = D3DCOLOR_ARGB( 0, fHelp, 50, 0);//(mein_zufall1.zahl( 0, 255)));
		}
		else
			set_partikel( 4, i, position, ambient, emissive);
	}
	if( context == 4)
	{
		D3DXMATRIX m;
		D3DXVECTOR3 sp;
		//float z;
		pd[i].steps=0;
		pd[i].life = 60;
		pd[i].speed.x = (mein_zufall1.zahl( -1,1))/3.0f;
		pd[i].speed.y = (mein_zufall1.zahl( -1,1))/3.0f;
		pd[i].speed.z = (mein_zufall1.zahl( -1,1))/3.0f;
		position = Posi;
		//z = mein_zufall1.zahl( -20, 20)/1000.0f;
		//sp = D3DXVECTOR3( 0, z, z);
		//D3DXMatrixRotationYawPitchRoll( &m, mein_zufall1.zahl( 0, 1000)/2500.0f, mein_zufall1.zahl( 0, 1000)/2500.0f, mein_zufall1.zahl( 0, 1000)/2500.0f);
		//D3DXVec3TransformNormal( &sp, &sp, &m);
		//pd[i].speed = sp;
	}
}

void mein_partikelsystem::create( int anz,int Art)
	{
	if(Art==0)
		{
		partikelsystem::create( anz, 0.2f);
		pd = new partikeldaten[ anz];
		set(0);
		}
	if(Art==1)
		{
		partikelsystem::create( anz, 0.6f);
		pd = new partikeldaten[ anz];
		set(0);
		}
	}
