#include "SpaCE/applikation.h"
#include "Landschaft.h"
#include "Spieler.h"
#include "Schuss.h"
#include "Gegner.h"
#include "Spiel.h"
#include "Level.h"
#include "explosionen.h"
#include <windows.h>
#include <cstdlib>

#pragma warning( disable : 4996 ) 


#define SCHRITTE_MENU 8
#define KUGELANZAHL 4
#define ROT_MENU (SCHRITTE_MENU*2.0f)
#define KONSTANTER_CPS_WERT 30
#define DEFAULTLEVEL 0
#define GEGNER_SPAWNTIME 0.5f
#define CPS_DELAY_ZWISCHEN_SCHUESSEN 5
#define CPS_DELAY_PICKUPZEIT 100
#define OFFSETZ_PICKUP 60
#define LÄNGE_EINES_LANDSCHAFTSOBJEKTES 100.0f

// Menü und Profile laden + Menü initialisieren
void spiel::setup()
{	
	splashscreen Ladeanzeige;
	Ladeanzeige.create("Sprites\\Ladebildschirm.bmp",1,1);
	set_title("Captain Sergej Faehrlich");
	// Menü instanzieren
	objMenu		= new objekt();
	objMenu->load("Menu.x","Menu");
	Ladeanzeige.progress(10);

	objGameOver	= new objekt();
	objGameOver->load("GameOver.x", "GameOver");
	Ladeanzeige.progress(20);

	objGewonnen	= new objekt();
	objGewonnen->load("pokal.x", "Pokal");
	Ladeanzeige.progress(25);

	load_sounds(17,	"Sounds/fire100.wav","Sounds/bull.wav","Sounds/song.wav","Sounds/hit1.wav",
		"Sounds/1.wav","Sounds/2.wav","Sounds/3.wav","Sounds/fire1.wav","Sounds/wah.wav",
		"Sounds/shield.wav","Sounds/hallo.wav","Sounds/punkt.wav","Sounds/tada.wav",
		"Sounds/gas.wav","Sounds/Fahrstuhl1.wav","Sounds/Stuhl.wav","Sounds/settings.wav");
	Ladeanzeige.progress(45);
	// alles was zum ini files auslesen zuständig ist in dieser Unterfunktion
	//____________________INI-Files_________________
	profmodloadup();
	//______________________________________________
	Ladeanzeige.progress(60);


	// Sprites laden
	initialisiereSprites();
	Ladeanzeige.progress(70);

	MausGe = 6.0f;
	ALandElem=10;
	iFlagDoppelL = 0;
	iFlagDoppelR = iFlagVideo = 0;
	iMenu = -1;

	set_window(0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN)-30);
	get_windowsize(&x,&y,&breite,&hoehe);

	strcpy(cName,"NoName");

	sky.create( 1000, "skybox/space.jpg", "skybox/space.jpg", 
		"skybox/space.jpg", "skybox/space.jpg", 
		"skybox/space.jpg", "skybox/space.jpg");
	Ladeanzeige.progress(80);

	endlosvideo.create("Video/endlosschleife.wmv",1);
	video1.create("Video/treffer1.wmv");
	video2.create("Video/treffer2.wmv");
	video3.create("Video/treffer3.wmv");

	video1.setup((breite-(breite*0.275f)),(hoehe-(hoehe*0.29f)),breite*0.275f,hoehe*0.29f);
	video2.setup((breite-(breite*0.275f)),(hoehe-(hoehe*0.29f)),breite*0.275f,hoehe*0.29f);
	video3.setup((breite-(breite*0.275f)),(hoehe-(hoehe*0.29f)),breite*0.275f,hoehe*0.29f);
	endlosvideo.setup((breite-(breite*0.275f)),(hoehe-(hoehe*0.30f)),breite*0.275f,hoehe*0.27f);
	Ladeanzeige.progress(85);

	Highscoreladen();
	Ladeanzeige.progress(90);
	menueinst();
	Ladeanzeige.progress(100);
}

void spiel::initialisiereSprites()
{
	HUD.load("Sprites\\HUD.bmp",0x00ff00ff);
	Energiebalken[0].load("Sprites\\Energiebalken00.bmp",0x00ff00ff);
	Energiebalken[1].load("Sprites\\Energiebalken10.bmp",0x00ff00ff);
	Energiebalken[2].load("Sprites\\Energiebalken20.bmp",0x00ff00ff);
	Energiebalken[3].load("Sprites\\Energiebalken30.bmp",0x00ff00ff);
	Energiebalken[4].load("Sprites\\Energiebalken40.bmp",0x00ff00ff);
	Energiebalken[5].load("Sprites\\Energiebalken50.bmp",0x00ff00ff);
	Energiebalken[6].load("Sprites\\Energiebalken60.bmp",0x00ff00ff);
	Energiebalken[7].load("Sprites\\Energiebalken70.bmp",0x00ff00ff);
	Energiebalken[8].load("Sprites\\Energiebalken80.bmp",0x00ff00ff);
	Energiebalken[9].load("Sprites\\Energiebalken90.bmp",0x00ff00ff);
	Energiebalken[10].load("Sprites\\Energiebalken100.bmp",0x00ff00ff);
	Pause.load("Sprites\\Pause.bmp",0x00ff00ff);
	Finger.load("Menu\\finger.bmp",0xffff00ff);
	schild.load("schild.x","Raumschiff",D3DCULL_CCW);
	D3DXMatrixScaling(&matrix,1.3f,0.7f,0.7f);
	schild.add_transform(&matrix);	
}

// Spiel initialisieren
void spiel::spieleinst()
{

	stop_sound(14);   //Menu Sound anhalten

	iSteps=0;
	iRot=0;
	iSchild=-1;
	iDead=4;
	iAlarm=0;
	iAufladen=0;
	iBlink=0;
	iHallo=10;
	iPunkte=0;
	iPunkteanzeige=0;
	iPoweronoff=1;
	iTime=0;
	iExplo=-20;
	iScheinwerfer=0;
	iScheinwerfer=0;
	iTada=0;
	iGas=0;
	iNebel=0;
	iPickupzeit=CPS_DELAY_PICKUPZEIT;
	iKeyPause=0;
	iPause=1;
	set_ambilight(0); // Umgebungslicht ausschalten
	iVidflag =0;
	iVideo = 0;

	// Level mit allen Spawnpunkten laden
	level = new Level(DEFAULTLEVEL);

	//Laden und erstellen der Lösch-Box
	//____________________LOESCH BOX_______________
	objLoeschbox = new objekt();
	objLoeschbox->load("LoeschBox.x", "LoeschBox");

	D3DXMATRIX loeschbox;
	D3DXMatrixScaling(&loeschbox,3.0f,1.0f,1.0f);
	objLoeschbox->add_transform(&loeschbox);
	D3DXMatrixTranslation(&loeschbox,-30,0,0);
	objLoeschbox->add_transform(&loeschbox);
	//______________________________________________

	//Globale Bewegung einstellen
	D3DXMatrixTranslation(&matrGlobalBewegung,GLOBALER_BEWEGUNGSVEKTOR);

	//Perspektive,Farbe usw
	set_bkcolor( 0, 0, 0);
	set_perspektive( D3DX_PI/4, 1.0f, 100000.0f);
	standort = D3DXVECTOR3( -50,50,0 );
	blickrichtung = D3DXVECTOR3(1.0f,-0.6f,0);


	//Landschaft laden
	//_______________LANDSCHAFT-Laden______________________________________________
	list<objekt *>::iterator iterLandschaft = lstLandschaftobj.begin();
	D3DXMATRIX matrOffset;

	int q = lstLandschaftobj.size();
	for(int i=0; i<=ALandElem;i++, iterLandschaft++)
	{
		if (iterLandschaft == lstLandschaftobj.end()) iterLandschaft = lstLandschaftobj.begin();
		D3DXMatrixTranslation(&matrOffset,i*LÄNGE_EINES_LANDSCHAFTSOBJEKTES,0.0f, 0.0f);
		D3DXMatrixScaling(&matrix,6.0f,6.0f,4.0f);
		Landschaft *landschaft_hilf = new Landschaft(*iterLandschaft, matrOffset);
		landschaft_hilf->add_transform(&matrix);
		lstLandschaft.push_back(landschaft_hilf);
	}
	//______________________________________________________________________________

	iSteps=0;
	iRot=0;

	//Spieler laden + Schild plazieren
	spieler=new Spieler();
	spieler->get_pos(vSpieleraltepos);
	D3DXVECTOR3 Lichteinfall;
	Lichteinfall.x=0;
	Lichteinfall.y=1;
	Lichteinfall.z=-1;
	set_sunlight( 0, &Lichteinfall, 0.2f, 0.2f, 0.2f);

	// Partikelinitialisierungen
	partAntrieb1.create(1200,0); // 1200 Partikel und Partikelanfangsgröße = 0
	partAntrieb2.create(1200,0);
	partExplosion.create(350,1);

	set_cps(KONSTANTER_CPS_WERT);
	set_textcolor(255,255,255);

	fSpawntimeInSec = GEGNER_SPAWNTIME; // Alle 2 Sekunden soll ein Gegner kommen
	spawn_timer = KONSTANTER_CPS_WERT * fSpawntimeInSec;


	// Sprites initialisieren
	set_window(0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN)-30);
	get_windowsize(&x,&y,&breite,&hoehe);
	HUD.move(0,0,breite,hoehe);
	for ( int i = 0; i<=10; i++)
		Energiebalken[i].move(0,0,breite,hoehe);

	endlosvideo.resume();
}

//Game - Over laden
void spiel::gameovereinst()
{
	standort = D3DXVECTOR3 (-300,140,0);
	blickrichtung = D3DXVECTOR3(1.0f,-0.4f,0);

	play_sound(14,1); //Menu Sound abspielen

	fog_off();

	set_ambilight(0);

	D3DXVECTOR3 hilfe1 = D3DXVECTOR3(-650,100,-30);
	D3DXVECTOR3 hilfe2 = D3DXVECTOR3(1,0,0);

	set_spotlight(10, &hilfe1, &hilfe2, 400 , 1, 1, 0);

	//set_sunlight( 0, &vector, 0.9f, 0.5f, 0.2f);
}

// Menüelemente auf Ursprungswerte zurücksetzen
void spiel::menueinst()
{
	vector.x = 1.0f;
	vector.y = -0.2f;
	vector.x = 0;

	//fog_on(300,2500,RGB (0,0,0));

	set_ambilight(0);

	/*D3DXVECTOR3 hilfe1 = D3DXVECTOR3(-450,200,-30);
	D3DXVECTOR3 hilfe2 = D3DXVECTOR3(1,-0.5f,0);


	set_spotlight(0, &hilfe1, &hilfe2, 200.0f, 0.2f, 0.5f, 0.2f);*/

	set_sunlight( 0, &vector, 0.9f, 0.2f, 0.2f);

	//Laden und erstellen des Menü
	//____________________MENÜ_______________________

	standort = D3DXVECTOR3 (-600,140,0);
	blickrichtung = D3DXVECTOR3(1.0f,-0.4f,0);

	SettingsLvL=0;
	hszaehler=0;
	iFlagL=iFlagR=iFlagD=iFlagU=iFlagHS=iFlagSET=iFlagSTART=0;
	iStandort=0;
	iStandorth=0;
	hszaehler=0;
	set_bkcolor( 0, 0, 0);
	set_perspektive( D3DX_PI/4, 1.0f, 100000.0f);

	if(!iFlagVideo)
	{
		videostart.create("Video/GIP.wmv");
		videostart.getsize(&br,&ho);
		videostart.setup(breite-(breite/2)-br/2,hoehe-(hoehe/2)-ho/2,br,ho);
		videostart.play();	
	}


}


// Was soll gerendert werden?
int spiel::render()
{

	poll_keyboard(key);

	if((videostart.getlength() == videostart.getpos()) && !iFlagVideo || (key[DIK_SPACE] && iMenu==-1))
	{
		videostart.destroy();
		iMenu =	7;
		play_sound(14,1);
		iFlagVideo = 1;
	}
	switch(iMenu)
	{
	case 0:
		rendergame();
		break;
	case 7:
		rendermenu();
		break;
	case 1:
		renderhighs();
		break;
	case 2:
		rendersettings();
		break;
	case 3:
		return 0;
	case 8:
		rendergameover();
		break;
	case 9:
		renderlevelcompleted();
		break;
	}
	return 1;
}

// Spiel rendern
void spiel::rendergame()
{
	if(!iPause)
	{
		get_windowsize(&x,&y,&breite,&hoehe);
		Pause.move(0,0,breite,hoehe);
		Pause.render();
		spieler->render();
	}

	if(iPause!=0)
	{

		if(iNebel<=90)
			iNebelplus=1;
		if(iNebel>=200)
			iNebelplus=0;

		if(iNebelplus)
			iNebel+=4;
		else
			iNebel-=4;

		fog_on(350,700,RGB(0,0,iNebel));

	}


	get_windowsize(&x,&y,&breite,&hoehe);
	hoehe -= 20;

	HUD.move(0,0,breite,hoehe);
	HUD.render();

	Energiebalken[(int) (spieler->get_hp() / 10) ].move(0,0,breite,hoehe);
	Energiebalken[(int) (spieler->get_hp() / 10) ].render();

	set_textcolor(255,255,255);
	if (iPunkteanzeige<iPunkte) iPunkteanzeige += 5;
	display_text((int)breite*0.005,(int)hoehe*0.7,"Punkte: %d",iPunkteanzeige);
	display_text((int)breite*0.005,(int)hoehe*0.8,"Waffenlevel: %d",spieler->get_waffe());
	if ( spieler->get_waffentimer() > 0) display_text((int)breite*0.005,(int)hoehe*0.85,"Waffentimer: %d",spieler->get_waffentimer());


	spieler->get_pos(vector);
	

	display_text((int)breite*0.005,(int)hoehe*0.9,"Position: %d",(int) vector.x);

	if(iPause!=0)
		iTime++;

	// nach 10 Frames einmal den Begrüßungssound abspielen
	if(iHallo==0)
		play_sound(10);
	if(iHallo>=0)
		iHallo--;

	if(iPause!=0)
	{

		// Energie langsam regenerieren
		iAufladen++;
		if(iAufladen>=120)
		{
			spieler->hp_aufladen(5); // Bei ca. 60fps alle 2 Sekunden HP um 2 erhöhen
			iAufladen=20;			// Wenn man länger nicht getroffen wurde, lädt sich die Energie schneller auf ( Delay nur 100Frames)
		}

		// Bei über 7 HP soll der Alarm aus sein und nach regeneration einen sound abspielen
		if(spieler->get_hp()>=7) 
		{
			iAlarm=0;
			if(iPoweronoff==0)
			{
				iPoweronoff=1;
				iGas=0;
				play_sound(9);
			}
		}
		else // Wenn unter 7 HP
		{
			if(iGas==300) // Wenn man länger unter 7 HP ist Sound abspielen
				play_sound(13);
			iGas++;	
		}

		verarbeite_eingaben();

		// Wenn spieler unter 7HP nur blinked rendern
		if(iAlarm)
		{
			if(iBlink<=0)
			{
				iBlink++;
			}
			else
			{
				if(iBlink<=3)
				{
					spieler->render();
					iBlink++;
				}
				else
					iBlink=0;
			}
		}
		else
			spieler->render();

		// Jedesmal nachdem man 10 Punkte bekommen hat kommt ein Sound
		if(!((iPunkte+1)%10))
		{
			if(!iTada)
				if(!((iPunkte+1)%40))
					play_sound(12);
				else
					if(!((iPunkte+1)%30))
						play_sound(255);
					else
						if(!((iPunkte+1)%20))		// Sound noch machen
							play_sound(254);
						else
							if(!((iPunkte+1)%10))
								play_sound(253);
			iTada=1;
		}
		else
		{
			iTada=0;
		}

		iSteps--;



		spawn_timer--;

		if ( spawn_timer == 0)
		{
			gegner_spawnen(); // Wenn Timer abgelaufen, spawne den nächsten Gegner
			spawn_timer = KONSTANTER_CPS_WERT * fSpawntimeInSec; // Timer nachladen
		}
	}
	int flag=0;

	list<Landschaft *>::iterator iterLandschaft;
	list<Schuss *>::iterator iterSchuss;
	list<Gegner *>::iterator iterGegner;
	list<Schuss *>::iterator iterGegSchuss;


	for ( iterLandschaft = lstLandschaft.begin(); iterLandschaft != lstLandschaft.end(); iterLandschaft++)
	{
		// iter ist selber ein Zeiger und in der Liste sind nochmal Zeiger, die dereferenziert werden müssen
		(*iterLandschaft)->render();
	}

	Schuss *schHelp=0;
	int iWaffentyp=0;
	int iSchussprof=0;


	if(iPause!=0)
	{
		for (iterGegner = lstGegner.begin(); iterGegner != lstGegner.end();)
		{

			if((*iterGegner)->step())
			{
				(*iterGegner)->get_pos(vector);
				iWaffentyp=(*iterGegner)->get_waffe();
				iSchussprof=(*iterGegner)->get_Schusstyp();
				int i=0;
				for(iterSchussfst=lstSchussfst.begin(),i=0;(iterSchussfst != lstSchussfst.end()) && i<iSchussprof; iterSchussfst++, i++); 
				for(int i=-iWaffentyp; i<=iWaffentyp; i++)
				{
					if((*iterSchussfst)->iStaerke==1)
					{
						int a=0;
					}

					schHelp=new Schuss(lstSchussobj.back(),*iterSchussfst,1,i);
					D3DXMatrixScaling(&matrix,10.0f,1.0f,1.0f);
					schHelp->add_transform(&matrix);
					schHelp->set_pos(vector);
					lstGegSchuss.push_back(schHelp);
				}
				play_sound(7);
			}
			if((*iterGegner)->out(objLoeschbox))
			{
				Gegner *gegnerhilf = *iterGegner;
				list<Gegner *>::iterator iterhilf = iterGegner;
				iterGegner++;
				lstGegner.erase(iterhilf);
				delete(gegnerhilf);
			}
			else
				iterGegner++;
		}

		kollisionSpielerschuesseGegner();
		kollisionGegnerschuesseSpieler();
		kollisionSpielerGegner();



		for (iterGegSchuss = lstGegSchuss.begin(); iterGegSchuss != lstGegSchuss.end();)
		{
			(*iterGegSchuss)->step();
			if((*iterGegSchuss)->out(objLoeschbox))
			{
				Schuss *schusshilf = *iterGegSchuss;
				list<Schuss *>::iterator iterhilf = iterGegSchuss;
				iterGegSchuss++;
				lstGegSchuss.erase(iterhilf);
				delete(schusshilf);
			}
			else
				iterGegSchuss++;
		}

		behandle_partikel();

		// Explosionen
		if(iExplo>0)
		{
			partExplosion.pointsize=float(iExplo)/15.0f;
			partExplosion.render();
			partExplosion.set(3);
			set_pointlight(3,&abc,50,0.2f,0.2f,0.2f);
			iExplo--;
		}
		else
		{
			D3DXVECTOR3 help123;
			help123.x=0;
			help123.y=0;
			help123.z=0;
			set_pointlight(3,&help123,0,0,0,0);
		}

		// Schild
		if(iSchild>=0)
		{
			D3DXVECTOR3 spielerpos;
			spieler->get_pos(spielerpos);
			//spielerpos.x+=0;
			spielerpos.z+=0.5f;
			schild.set_pos(spielerpos);
			schild.render(1, RENDER_TRANSPARENT);
			iSchild-=1;
		}

		// Spot nach vorne
		D3DXVECTOR3 abc,lichtdir;
		spieler->get_pos(abc);
		lichtdir.x=1.0f;
		lichtdir.y=-0.23f;
		lichtdir.z=0;
		set_spotlight( 1, &abc,&lichtdir, 500, 0.5f, 0.4f, 0.4f);



		D3DXVec3TransformCoord(&standort,&standort,&matrGlobalBewegung); // Kamera bewegen
		spieler->add_transform(	&matrGlobalBewegung);	// Raumschiff bewegen

		/*set_timer(1);
		display_text(100,50,"Gegnertimer %d",spawn_timer);*/

		createPickup();
		if(pPick)
		{
			kollisionSpielerPickup();
		}

		if(pPick)
		{
			pPick->get_pos(vector);
			pPick->set_pos(D3DXVECTOR3(0,0,0));
			D3DXMatrixRotationX(&matrix,D3DX_PI/45);
			pPick->add_transform(&matrix);
			D3DXMatrixRotationZ(&matrix,D3DX_PI/180);
			pPick->add_transform(&matrix);
			D3DXMatrixRotationY(&matrix,D3DX_PI/45);
			pPick->add_transform(&matrix);
			pPick->set_pos(vector);
			pPick->render();
		}
	}

	// Wenn "e" gedrückt wird, soll das spiel beendet werden und man kehrt zurück ins Menü
	poll_keyboard(key);
	if(spieler->get_hp() < 0)
	{
		iMenu=8;
		return_to_menu();
	}

	// Wenn das Ende des Levels erreicht wurde

	if(key[DIK_P])
	{
		if(!iKeyPause)
			if(iPause)
			{
				D3DXVECTOR3 Lichteinfall;
				Lichteinfall.x=0;
				Lichteinfall.y=1;
				Lichteinfall.z=-1;
				set_sunlight( 0, &Lichteinfall, 0.1f, 0.1f, 0.1f);
				iPause=0;
			}
			else
			{
				D3DXVECTOR3 Lichteinfall;
				Lichteinfall.x=0;
				Lichteinfall.y=1;
				Lichteinfall.z=-1;
				set_sunlight( 0, &Lichteinfall, 0.4f, 0.2f, 0.2f);
				iPause=1;
			}

			iKeyPause=1;
	}
	else
	{
		iKeyPause=0;
	}

	if (standort.x >= (ALandElem*LÄNGE_EINES_LANDSCHAFTSOBJEKTES))
	{
		iMenu=9;
		return_to_menu();
	}

	if (endlosvideo.getpos() >= endlosvideo.getend())
		endlosvideo.setpos(0,1);

	if ((video1.getpos() == 0 || video1.getpos()>=video1.getend()) && (video2.getpos() ==0 || video2.getpos()>=video2.getend())&& (video3.getpos() ==0 || video3.getpos()>=video3.getend()) && iVidflag==0)
	{
		//video1.stop();
		video1.pause(1);
		video2.pause(1);
		video3.pause(1);
		endlosvideo.resume();
		iVidflag=1;
	}
}

// Aus dem Spiel zurück ins Menü gehen
void spiel::return_to_menu()
{
	endlosvideo.pause(1);
	video1.pause(1);
	video2.pause(1);
	video3.pause(1);
	gameovereinst();
	set_sunlight( 0, &vector, 0.9f, 0.2f, 0.2f);
	delete objLoeschbox;
	delete level;
	delete spieler;

	if(pPick)
	{
		Pickup *pHelp=pPick;
		delete (pHelp);
		pPick=0;
	}

	for (iterLandschaft = lstLandschaft.begin(); iterLandschaft != lstLandschaft.end();)
	{
		Landschaft *landschafthilf = *iterLandschaft;
		list<Landschaft *>::iterator iterhilf = iterLandschaft;
		iterLandschaft++;
		lstLandschaft.erase(iterhilf);
		delete(landschafthilf);
	}

	for (iterGegner = lstGegner.begin(); iterGegner != lstGegner.end();)
	{
		Gegner *gegnerhilf = *iterGegner;
		list<Gegner *>::iterator iterhilf = iterGegner;
		iterGegner++;
		lstGegner.erase(iterhilf);
		delete(gegnerhilf);
	}

	for (iterSchuss = lstSchuss.begin(); iterSchuss != lstSchuss.end();)
	{
		Schuss *schusshilf = *iterSchuss;
		list<Schuss *>::iterator iterhilf = iterSchuss;
		iterSchuss++;
		lstSchuss.erase(iterhilf);
		delete(schusshilf);
	}

	for (iterGegSchuss = lstGegSchuss.begin(); iterGegSchuss !=lstGegSchuss.end();)
	{
		Schuss *Schusshilf = *iterGegSchuss;
		list<Schuss *>::iterator itergeghilf = iterGegSchuss;
		iterGegSchuss++;
		lstGegSchuss.erase(itergeghilf);
		delete(Schusshilf);
	}

	// Partikelinitialisierungen
	partAntrieb1.loeschen();// 1200 Partikel und Partikelanfangsgröße = 0
	partAntrieb2.loeschen();
	partExplosion.loeschen();

	// Berechnung der HS und speichern  in Highscore\HS.txt
	int i=0,j=0,m=0,k=0;
	char hilf[255];
	i_punkte[10] = iPunkte*10; 


	itoa((iPunkte*10),hilf,10);

	strcpy(highsc[10],cName); 
	strcat(highsc[10]," ");
	strcat(highsc[10],hilf);					

	for(i = 0; i < 11; i++)
	{
		for(j=0;j<11; j++)
		{
			if(i_punkte[i] >= i_punkte[j])
			{
				m = i_punkte[j];
				i_punkte[j] = i_punkte[i];
				i_punkte[i] = m;

				strcpy(hilf,highsc[j]);
				strcpy(highsc[j],highsc[i]);
				strcpy(highsc[i],hilf);

				for(k=0;hilf[k];k++)
					hilf[k] = 0;
			}
		}
	}

	ofstream f;

	f.open("Highscore\\HS.txt");

	for(i=0;i<10;i++)
	{
		f << highsc[i];
		f << endl;
	}

	f.close();

}

// Menü rendern
void spiel::rendermenu()
{
	poll_keyboard(key);

	if(!key[DIK_RETURN] )
	{
		iFlagHS = 0;
		iFlagSET = 0;
	}

	sky.render(&D3DXVECTOR3(0,0,0));

	if(menu_beweg == (SCHRITTE_MENU*iStandort))
	{
		if(key[DIK_RIGHT] && !iFlagR && !iFlagMenuSE && !iFlagL && !iFlagDoppelL && !iFlagRotR)
		{
			play_sound(15,0);
			rechts();
		}

		if(key[DIK_LEFT] && !iFlagL && !iFlagMenuSE &&  !iFlagR && !iFlagDoppelR && !iFlagRotL)
		{
			play_sound(15,0);
			links();
		}
	}

	if(iStandort ==0  && key[DIK_RETURN] && !iFlagRotL && !iFlagRotR && !iFlagSTART)
	{
		play_sound(16,0);
		AuswahlSpielStarten();
	}

	if(iStandort ==1  && key[DIK_RETURN] && !iFlagRotL && !iFlagRotR && !iFlagHS)
	{
		iFlagHS = 1;
		play_sound(16,0);
		AuswahlHighscoreStarten();		
	}

	if(iStandort ==2  && key[DIK_RETURN] && !iFlagRotL && !iFlagRotR && !iFlagSET)
	{	
		iFlagSET = 1;
		play_sound(16,0);
		AuswahlSettingsStarten();
	}

	if(iStandort ==3  && key[DIK_RETURN] && !iFlagRotL && !iFlagRotR)
	{
		stop_sound(14);
		iMenu = 3;
	}

	if(iStandort ==4  && key[DIK_RETURN] && !iFlagRotL && !iFlagRotR)
	{

	}

	if(iStandort ==5  && key[DIK_RETURN] && !iFlagRotL && !iFlagRotR)
	{

	}

	if(iStandort ==6  && key[DIK_RETURN] && !iFlagRotL && !iFlagRotR)
	{

	}

	/*set_textcolor(255,255,255);
	display_text(100,50,"Standort %d",iStandort);
	display_text(100,100,"Menu Beweg %d",menu_beweg);*/

	Ueberpruefe();

	objMenu->render();

}
// Game - Over Bildschirm anzeigen 
void spiel::rendergameover()
{
	//Bildschirm Game Over anzeigen bis Enter druck

	poll_keyboard(key);

	sky.render(&D3DXVECTOR3(0,0,0));

	D3DXMatrixRotationY(&matrix, D3DX_PI/64);
	objGameOver->add_transform(&matrix);

	objGameOver->render();

	if(key[DIK_RETURN] && !iFlagHS)
	{
		menueinst();
		iMenu = 1;
	}

}

// Level beendet
void spiel::renderlevelcompleted()
{

	poll_keyboard(key);

	sky.render(&D3DXVECTOR3(0,0,0));

	D3DXMatrixRotationY(&matrix, D3DX_PI/8);
	objGewonnen->add_transform(&matrix);

	objGewonnen->render();

	if(key[DIK_RETURN] && !iFlagHS)
	{
		menueinst();
		iMenu = 1;
	}
}

void spiel::Ueberpruefe()
{
	if(!key[DIK_RIGHT] && !key[DIK_LEFT])
	{
		iFlagMenuSE = 0;
	}

	if(!key[DIK_RIGHT] )
	{
		iFlagR = 0;
	}

	if(iFlagRotR)
	{
		MenuRotR();
	}

	if(!key[DIK_LEFT] )
	{
		iFlagL = 0;

	}

	if(!key[DIK_RETURN] )
	{
		iFlagSTART = 0;
	}

	if(iFlagRotL)
	{
		MenuRotL();
	}
}
// Rotation des Menüs
void spiel::MenuRotR()
{
	menu_beweg++;

	D3DXMatrixRotationY (&matrix, (D3DX_PI/ROT_MENU)*menu_beweg);
	objMenu->set_transform(&matrix);

	if(iStandort == KUGELANZAHL && menu_beweg == (SCHRITTE_MENU*KUGELANZAHL))
	{
		menu_beweg = 0;
		iStandort = 0;
		iFlagDoppelR = 0;
		iFlagRotR = 0;
	}
	else
	{
		if(menu_beweg == (SCHRITTE_MENU*iStandort))
		{
			iFlagRotR = 0;
			iFlagDoppelR = 0;
		}
	}

}
void spiel::MenuRotL()
{
	menu_beweg--;

	D3DXMatrixRotationY (&matrix, (D3DX_PI/ROT_MENU)*menu_beweg);
	objMenu->set_transform(&matrix);

	if(iStandort == (KUGELANZAHL-1) && menu_beweg == -1)
	{
		menu_beweg = (SCHRITTE_MENU*KUGELANZAHL);

		iFlagDoppelL = 0;

		if(iStandort == (KUGELANZAHL-1) && menu_beweg == (SCHRITTE_MENU*(KUGELANZAHL-1)))
			iFlagRotL = 0;
	}
	else
	{
		if(menu_beweg == (SCHRITTE_MENU*iStandort))
		{
			iFlagRotL = 0;
			iFlagDoppelL = 0;
		}
	}
}

void spiel::rechts()
{
	iFlagR = 1;
	iFlagRotR = 1;
	iFlagDoppelR = 1;
	iStandort++;

	if(iStandort==(KUGELANZAHL+1))
		iStandort=0;

	D3DXMatrixRotationY (&matrix, -(D3DX_PI/3.5f)*iStandort);
	objMenu->add_transform(&matrix);

	iStandorth++;

	if(iStandorth==(KUGELANZAHL+1))
		iStandorth=0;

}
void spiel::links()
{
	iFlagL = 1;
	iFlagRotL = 1;
	iFlagDoppelL = 1;
	iStandort--;

	if(iStandort==-1)
		iStandort=(KUGELANZAHL-1);

	D3DXMatrixRotationY (&matrix, -(D3DX_PI/3.5f)*iStandort);
	objMenu->add_transform(&matrix);

	iStandorth++;

	if(iStandorth==-1)
		iStandorth=(KUGELANZAHL-1);
}
// Menüunterpunkt "Settings" anzeigen
void spiel::rendersettings()
{
	poll_keyboard(key);

	sky.render(&D3DXVECTOR3(0,0,0));

	if(key[DIK_DOWN] && !iFlagD)
	{
		play_sound(16,0);
		SettingsLvL++;
		iFlagD=1;
		if(SettingsLvL>3)
			SettingsLvL=0;
	}

	if(!key[DIK_RETURN] )
	{
		iFlagSET = 0;
	}


	if(key[DIK_UP] && !iFlagU)
	{
		play_sound(16,0);
		SettingsLvL--;
		iFlagU=1;
		if(SettingsLvL<0)
			SettingsLvL=3;
	}

	if(!key[DIK_DOWN] )
	{
		iFlagD = 0;
	}

	if(!key[DIK_UP] )
	{
		iFlagU = 0;
	}

	set_textcolor(255,130,8);

	display_text(200,20,"SETTINGS");
	display_text(200,60*11,"EXIT");

	set_textcolor(255,255,255);

	display_text(300,20,"Anzahl der Landschaftselemente");
	display_text(640,20,"%d",ALandElem);
	display_text(300,60,"Mausempfindlichkeit");
	display_text(640,60,"%.2f",MausGe);
	display_text(300,100,"Name");
	display_text(640,100,"%s",cName);

	if(SettingsLvL == 0)
	{
		Finger.move(750,10,885,81);

		if(key[DIK_RIGHT] && !iFlagR)
		{
			ALandElem++;
			iFlagR=1;
			if(ALandElem > 30)
				ALandElem =0;
		}

		if(key[DIK_LEFT] && !iFlagL)
		{
			ALandElem--;
			iFlagL=1;
			if(ALandElem<0)
				ALandElem=30;

		}
	}

	if(SettingsLvL == 1)
	{
		Finger.move(750,50,885,121);

		if(key[DIK_RIGHT] && !iFlagR)
		{
			MausGe+=0.2f;
			iFlagR=1;
			if(MausGe > 30)
				MausGe = 0;

		}

		if(key[DIK_LEFT] && !iFlagL)
		{
			MausGe-=0.2f;
			iFlagL=1;
			if(MausGe<0)
				MausGe=30;

		}
	}

	if(SettingsLvL == 2)
	{
		Finger.move(750,90,885,161);


		if(key[DIK_BACK])
		{
			strcpy(cName,"");		
		}

		EingabeName();

	}



	if(!key[DIK_RIGHT] )
	{
		iFlagR=0;
	}


	if(!key[DIK_LEFT] )
	{
		iFlagL=0;
	}

	if(SettingsLvL == 3 )
	{
		Finger.move(300,650,435,721);

		if(key[DIK_RETURN] && !iFlagSET)
		{
			play_sound(16,0);
			iMenu = 7;
			iFlagSET = 1;
			SettingsLvL = 0;
		}

	}

	Finger.render();
}
void spiel::EingabeName()
{if(key[DIK_A] && (key[DIK_RSHIFT] || key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"A");
	iFlagNameEingabe = 1;
}
if(key[DIK_B] && (key[DIK_RSHIFT] || key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"B");
	iFlagNameEingabe = 1;
}
if(key[DIK_C] && (key[DIK_RSHIFT] || key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"C");
	iFlagNameEingabe = 1;
}
if(key[DIK_D] && (key[DIK_RSHIFT] || key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"D");
	iFlagNameEingabe = 1;
}
if(key[DIK_E] && (key[DIK_RSHIFT] || key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"E");
	iFlagNameEingabe = 1;
}
if(key[DIK_F] && (key[DIK_RSHIFT] || key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"F");
	iFlagNameEingabe = 1;
}
if(key[DIK_G] &&  (key[DIK_RSHIFT] || key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"G");
	iFlagNameEingabe = 1;
}
if(key[DIK_H] &&  (key[DIK_RSHIFT] || key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"H");
	iFlagNameEingabe = 1;
}
if(key[DIK_I] &&  (key[DIK_RSHIFT] || key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"I");
	iFlagNameEingabe = 1;
}
if(key[DIK_J] &&  (key[DIK_RSHIFT] || key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"J");
	iFlagNameEingabe = 1;
}
if(key[DIK_K] &&  (key[DIK_RSHIFT] || key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"K");
	iFlagNameEingabe = 1;
}
if(key[DIK_L] &&  (key[DIK_RSHIFT] || key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"L");
	iFlagNameEingabe = 1;
}
if(key[DIK_M] &&  (key[DIK_RSHIFT] || key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"M");
	iFlagNameEingabe = 1;
}
if(key[DIK_N] &&  (key[DIK_RSHIFT] || key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"N");
	iFlagNameEingabe = 1;
}
if(key[DIK_O] &&  (key[DIK_RSHIFT] || key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"O");
	iFlagNameEingabe = 1;
}
if(key[DIK_P] &&  (key[DIK_RSHIFT] || key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"P");
	iFlagNameEingabe = 1;
}
if(key[DIK_Q] &&  (key[DIK_RSHIFT] || key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"Q");
	iFlagNameEingabe = 1;
}
if(key[DIK_R] &&  (key[DIK_RSHIFT] || key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"R");
	iFlagNameEingabe = 1;
}
if(key[DIK_S] &&  (key[DIK_RSHIFT] || key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"S");
	iFlagNameEingabe = 1;
}
if(key[DIK_T] &&  (key[DIK_RSHIFT] || key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"T");
	iFlagNameEingabe = 1;
}
if(key[DIK_U] &&  (key[DIK_RSHIFT] || key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"U");
	iFlagNameEingabe = 1;
}
if(key[DIK_V] &&  (key[DIK_RSHIFT] || key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"V");
	iFlagNameEingabe = 1;
}
if(key[DIK_W] &&  (key[DIK_RSHIFT] || key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"W");
	iFlagNameEingabe = 1;
}
if(key[DIK_X] &&  (key[DIK_RSHIFT] || key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"X");
	iFlagNameEingabe = 1;
}
if(key[DIK_Y] &&  (key[DIK_RSHIFT] || key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"Z");
	iFlagNameEingabe = 1;
}
if(key[DIK_Z] &&  (key[DIK_RSHIFT] || key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"Y");
	iFlagNameEingabe = 1;
}
//-----------------------------------------------	


if(key[DIK_A] && !(key[DIK_RSHIFT] && key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"a");
	iFlagNameEingabe = 1;
}
if(key[DIK_B] && !(key[DIK_RSHIFT] && key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"b");
	iFlagNameEingabe = 1;
}
if(key[DIK_C] && !(key[DIK_RSHIFT] && key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"c");
	iFlagNameEingabe = 1;
}
if(key[DIK_D] && !(key[DIK_RSHIFT] && key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"d");
	iFlagNameEingabe = 1;
}
if(key[DIK_E] && !(key[DIK_RSHIFT] && key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"e");
	iFlagNameEingabe = 1;
}
if(key[DIK_F] && !(key[DIK_RSHIFT] && key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"f");
	iFlagNameEingabe = 1;
}
if(key[DIK_G] && !(key[DIK_RSHIFT] && key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"g");
	iFlagNameEingabe = 1;
}
if(key[DIK_H] && !(key[DIK_RSHIFT] && key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"h");
	iFlagNameEingabe = 1;
}
if(key[DIK_I] && !(key[DIK_RSHIFT] && key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"i");
	iFlagNameEingabe = 1;
}
if(key[DIK_J] && !(key[DIK_RSHIFT] && key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"j");
	iFlagNameEingabe = 1;
}
if(key[DIK_K] && !(key[DIK_RSHIFT] && key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"k");
	iFlagNameEingabe = 1;
}
if(key[DIK_L] && !(key[DIK_RSHIFT] && key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"l");
	iFlagNameEingabe = 1;
}
if(key[DIK_M] && !(key[DIK_RSHIFT] && key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"m");
	iFlagNameEingabe = 1;
}
if(key[DIK_N] && !(key[DIK_RSHIFT] && key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"n");
	iFlagNameEingabe = 1;
}
if(key[DIK_O] && !(key[DIK_RSHIFT] && key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"o");
	iFlagNameEingabe = 1;
}
if(key[DIK_P] && !(key[DIK_RSHIFT] && key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"p");
	iFlagNameEingabe = 1;
}
if(key[DIK_Q] && !(key[DIK_RSHIFT] && key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"q");
	iFlagNameEingabe = 1;
}
if(key[DIK_R] && !(key[DIK_RSHIFT] && key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"r");
	iFlagNameEingabe = 1;
}
if(key[DIK_S] && !(key[DIK_RSHIFT] && key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"s");
	iFlagNameEingabe = 1;
}
if(key[DIK_T] && !(key[DIK_RSHIFT] && key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"t");
	iFlagNameEingabe = 1;
}
if(key[DIK_U] && !(key[DIK_RSHIFT] && key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"u");
	iFlagNameEingabe = 1;
}
if(key[DIK_V] && !(key[DIK_RSHIFT] && key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"v");
	iFlagNameEingabe = 1;
}
if(key[DIK_W] && !(key[DIK_RSHIFT] && key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"w");
	iFlagNameEingabe = 1;
}
if(key[DIK_X] && !(key[DIK_RSHIFT] && key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"x");
	iFlagNameEingabe = 1;
}
if(key[DIK_Y] && !(key[DIK_RSHIFT] && key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"z");
	iFlagNameEingabe = 1;
}
if(key[DIK_Z] && !(key[DIK_RSHIFT] && key[DIK_LSHIFT]) && !iFlagNameEingabe)
{
	strcat(cName,"y");
	iFlagNameEingabe = 1;
}
if(	!key[DIK_A] && !key[DIK_B] && !key[DIK_C]&& !key[DIK_D] && !key[DIK_E]
&& !key[DIK_F] && !key[DIK_G] && !key[DIK_H] && !key[DIK_I] && !key[DIK_J] && !key[DIK_K]
&& !key[DIK_L] && !key[DIK_M] && !key[DIK_N] && !key[DIK_O] && !key[DIK_P] && !key[DIK_Q]
&& !key[DIK_R] && !key[DIK_S] && !key[DIK_T] && !key[DIK_U] && !key[DIK_V] && !key[DIK_W]
&& !key[DIK_X] && !key[DIK_Y] && !key[DIK_Z])
{
	iFlagNameEingabe = 0;
}

}

// Menüunterpunkt "Highscore" anzeigen
void spiel::renderhighs()
{
	poll_keyboard(key);

	sky.render(&D3DXVECTOR3(0,0,0));

	if(!key[DIK_RETURN] )
	{
		iFlagHS = 0;
		iFlagSET = 0;
	}

	set_textcolor(255,130,8);

	display_text(200,20,"HIGHSCORE");
	display_text(200,60*11,"EXIT");

	set_textcolor(255,255,255);

	Finger.move(300,650,435,721);

	Finger.render();


	for(int i=0;i<10;i++)
	{
		display_text(400,60*(i+1),"%s",highsc[i]);
	}

	if(key[DIK_RETURN] && !iFlagHS)
	{
		play_sound(16,0);
		iMenu = 7;
		iFlagHS = 1;
		iFlagSTART = 1;
	}
}
//Auswahl im Menü
void spiel::AuswahlSpielStarten()
{
	iMenu = 0;
	spieleinst();
}
void spiel::AuswahlHighscoreStarten()
{
	iMenu = 1;		
	Highscoreladen();
}

void spiel::AuswahlSettingsStarten()
{
	iMenu=2;
}
//Laden der Highscore
void spiel::Highscoreladen()
{
	ifstream f;

	f.open("Highscore\\HS.txt");

	while(!(f.eof()))
	{
		f.getline(highsc[hszaehler],255);
		hszaehler++;
	}

	int g = 0, h = 0, k =0,j=0,i=0,m=0;

	for(i=0;i<12;i++)
	{
		for(j=0;j<12;j++)
		{
			if( ((highsc[i][j] - '0') >=0) && ((highsc[i][j] - '0') <10))
			{
				c_score[k++] = highsc[i][j];
			}
		}

		i_punkte[g++] = atoi(c_score);
		k = 0;

		for(m=0;m<12;m++)
			c_score[m] = 0;
	}

	f.close();
}
// Spawne den nächsten Gegner
void spiel::gegner_spawnen()
{
	int iNxtGegnertyp;
	int iNxtFormation;
	int iNxtPosition;

	level->get_next_enemy(&iNxtGegnertyp,&iNxtFormation,&iNxtPosition);

	D3DXMATRIX matrGegnerSpawn;
	spieler->get_transform(&matrGegnerSpawn);


	list<objekt *>::iterator iterNxtGegnerobj;
	list<Gegnerfilesystem *>::iterator iterNxtGegnerfst;

	iterNxtGegnerobj = lstGegnerobj.begin();
	iterNxtGegnerfst = lstGegnerfst.begin();

	for (int i = 0; i < iNxtGegnertyp; i++)
	{
		iterNxtGegnerobj++;
		iterNxtGegnerfst++;
	}	

	// mit Vektoren ist Indexzugriff möglich, evtl noch umschreiben?

	objekt* uebergabeObjekt = *iterNxtGegnerobj;
	Gegnerfilesystem* uebergabeFilesystem = *iterNxtGegnerfst;
	Gegner* hilf = new Gegner(uebergabeObjekt,uebergabeFilesystem, matrGegnerSpawn);
	//	Gegner* hilf = new Gegner(iterNxtGegnerobj,iterNxtGegnerfst,matrGegnerSpawn);

	D3DXMatrixTranslation(&matrGegnerSpawn,0.0f,0.0f,(float)iNxtPosition);
	hilf->add_transform(&matrGegnerSpawn);
	lstGegner.push_back(hilf);
}


//Maus und Tastatureingaben verarbeiten
void spiel::verarbeite_eingaben()
{
	// Löschbox mit nach vorne verschieben
	objLoeschbox->add_transform(&matrGlobalBewegung);
	//Mauseingaben
	//_____________Mauseingaben___________________
	if(iRot)
	{
		D3DXMatrixRotationX(&matrix,iRot*D3DX_PI/180);
		iRot=0;
		spieler->add_transform(&matrix);
	}

	static short int gflag = 0;
	set_pointlight(6,&vector,0,0,0,0);
	if(poll_mouse(&maus))
	{
		if(maus.rgbButtons[0])
		{
			if(iSteps<=0)
			{
				D3DXMATRIX mCurPosSpieler;
				spieler->get_transform(&mCurPosSpieler);
				Schuss* schuss_erstellen;

				int iWaffe=spieler->get_waffe();

				for (int i=-iWaffe; i<= iWaffe; i++)
				{
					schuss_erstellen = new Schuss(*lstSchussobj.begin(),*lstSchussfst.begin(),0,i);
					D3DXMatrixScaling(&matrix,10.0f,0.7f,0.7f);
					schuss_erstellen->set_transform(&matrix);
					schuss_erstellen->add_transform(&mCurPosSpieler);

					lstSchuss.push_back(schuss_erstellen);
				}

				play_sound(0);
				spieler->get_pos(vector);
				vector.x+=10;
				vector.y-=4;
				iSteps=CPS_DELAY_ZWISCHEN_SCHUESSEN;
				set_pointlight(6,&vector,50,0.2f,0,0);
			}
		}

		if(maus.lX)
		{
			spieler->get_transform(&matrix);
			if((matrix._43 -(float) maus.lX/MausGe) >= MAPBREITE/2)
			{
				D3DXMatrixTranslation(&matrix,0,0,-matrix._43+MAPBREITE/2);
				spieler->add_transform(&matrix);
			}
			else
				if((matrix._43 -(float) maus.lX/MausGe) <= -MAPBREITE/2)
				{
					D3DXMatrixTranslation(&matrix,0,0,-matrix._43-MAPBREITE/2);
					spieler->add_transform(&matrix);
				}
				else
				{
					D3DXMatrixTranslation(&matrix,0,0,-(float)maus.lX/MausGe);
					spieler->add_transform(&matrix);
				}

				if(maus.lX>0 && iRot==0)
				{
					D3DXMatrixRotationX(&matrix,-((int)(maus.lX/3))*D3DX_PI/180);
					spieler->add_transform(&matrix);
					iRot+=((int)(maus.lX/3));
				}
				if(maus.lX<0 && iRot==0)
				{
					D3DXMatrixRotationX(&matrix,-((int)(maus.lX/3))*D3DX_PI/180);
					spieler->add_transform(&matrix);
					iRot+=((int)(maus.lX/3));
				}

		}

		if(maus.lY)
		{
			spieler->get_transform(&matrix);
			if( -(float) maus.lY/(MausGe/2) + matrix._41 > (float)(standort.x + MAXVORNE))
			{
				D3DXMatrixTranslation(&matrix,(float)(-matrix._41+(float)(standort.x + MAXVORNE)),0,0);
				spieler->add_transform(&matrix);
			}
			else
				if( -(float) maus.lY/(MausGe/2) + matrix._41 < standort.x + MAXHINTEN)
				{
					D3DXMatrixTranslation(&matrix,(float) (-matrix._41+(float)(standort.x + MAXHINTEN)),0,0);
					spieler->add_transform(&matrix);
				}
				else
				{
					D3DXMatrixTranslation(&matrix,-(float)maus.lY/(MausGe/2),0,0);
					spieler->add_transform(&matrix);
				}
		}

	}
}


//Unterfunktion die sich ums Modelle laden kümmert
int spiel::modelle_laden(char *cDir_name, list<objekt *> *lstType,float fMaxwidth)
{
	// Mist zu lange gewartet mit den Kommentaren , hälfte wieder verdrängt ^^
	char cStdModelle_count[sizeof(int)*8+1];	// Maximale Länge von Integer in binär
	char cStdModelle_namen[257];				// Länger können Dateinamen unter WinxP nicht sein oder?
	objekt *oHilf;								// für <list>pushback
	char cStdModelle_dir[60]="";
	strcpy(cStdModelle_dir,cDir_name);
	strcat(cStdModelle_dir,"\\Modelle.ini");
	int iObjcount=0;
	for(int i=0; 1 ;i++)
	{
		//Durchzählvariable der Ladbaren Modelle in der .ini Datei
		itoa(i,cStdModelle_count,10);		

		// liest aus der Modelle.ini Datei den Teil [Modelle] aus und schreibt sie in cStdModelle_namen
		// z.B. bei gegner:
		// [Modelle]
		// 0=skal.x
		// Wenn dieser Eintrag nicht gefunden wurde, wird in den Buffer 0 geschrieben
		GetPrivateProfileString("Modelle" ,cStdModelle_count,0,cStdModelle_namen,257, cStdModelle_dir);

		// Wenn 
		if(!cStdModelle_namen[0])
			return iObjcount;
		oHilf=new objekt();

		if(oHilf->load(cStdModelle_namen, cDir_name,D3DCULL_NONE))
		{
			//If Bedingung kümmert sich rein um die Skalierung
			if(fMaxwidth)
			{
				D3DXVECTOR3 vSkal1,vSkal2;
				oHilf->get_boundingbox(&vSkal1,&vSkal2);
				float fSkal;
				fSkal= abs(vSkal1.z) + abs(vSkal2.z);

				if(fSkal>=fMaxwidth)
				{
					fSkal=fMaxwidth/fSkal;
					D3DXMATRIX mRescale;
					D3DXMatrixScaling(&mRescale,fSkal,fSkal,fSkal);
					oHilf->add_transform(&mRescale);
				}
			}

			lstType->push_back(oHilf);
			iObjcount++;
		}
		else
			delete(oHilf);
	}
}
//Unterfunktion profmodloadup() dient nur zur Übersicht für die Zusammenführung
void spiel::profmodloadup()
{
	//-----Standard Modelle laden
	iAnzGelGegobj=0;
	iAnzGelLanobj=0;
	iAnzGelSchobj=0;
	iAnzGelPicobj=0;

	// Unterordner Gegner, in Liste Gegnerobj, falls Model > 10 Einheiten breit wird es runterskaliert
	iAnzGelGegobj=modelle_laden("Gegner",&lstGegnerobj,10.0f);
	// Unterordner Landschaft, Liste Landschaftobj, keine Skalierung!
	iAnzGelLanobj=modelle_laden("Landschaft",&lstLandschaftobj);
	// siehe oben
	iAnzGelSchobj=modelle_laden("Schuss",&lstSchussobj);
	// siehe oben
	iAnzGelPicobj=modelle_laden("Pickup",&lstPickupobj,4.3f);

	//----Gegnerprofile laden
	char cProfGeg[20]="Gegner\\Modelle.ini";
	char cProfGeg_count[sizeof(int)*8+1]="";
	iProfGeg_count=0;
	for(int i=0; 1 ;i++)
	{
		itoa(i,cProfGeg_count,10);	
		struct Gegnerfilesystem *gfstHelp=new Gegnerfilesystem;
		gfstHelp->iWaffe=GetPrivateProfileInt(cProfGeg_count,"Waffe",0,cProfGeg);
		gfstHelp->iHp=GetPrivateProfileInt(cProfGeg_count,"HP",0,cProfGeg);
		gfstHelp->iGeschwindigkeit=GetPrivateProfileInt(cProfGeg_count,"Geschwindigkeit",0,cProfGeg);
		gfstHelp->iModell=GetPrivateProfileInt(cProfGeg_count,"Modell",0,cProfGeg);
		gfstHelp->iPunkte=GetPrivateProfileInt(cProfGeg_count,"Punkte",0,cProfGeg);
		gfstHelp->iSchusstyp=GetPrivateProfileInt(cProfGeg_count,"Schusstyp",0,cProfGeg);
		gfstHelp->iGeschwindigkeitz=GetPrivateProfileInt(cProfGeg_count,"Geschwindigkeitz",0,cProfGeg);


		if(gfstHelp->iWaffe || gfstHelp->iHp || gfstHelp->iGeschwindigkeit || gfstHelp->iModell)
		{
			lstGegnerfst.push_back(gfstHelp);
			iProfGeg_count++;
		}
		else
		{
			delete(gfstHelp);
			break;
		}
	}

	//----Formation laden
	int j,i;
	char cProfFor[255]="Formation\\Modelle.ini";
	char cProfFor_counti[sizeof(int)*8+1]="";
	char cProfFor_countj[sizeof(int)*8+1]="";
	struct Formationfilesystem *ffstHelp;
	iProfFor_count=0;
	char ForNamex[10] = {"x"};
	char ForNamez[10] = {"z"};


	for(i=0;;i++)
	{
		itoa(i,cProfFor_counti,10);
		ffstHelp = new Formationfilesystem;

		for(j=0;;j++)
		{
			D3DXVECTOR3 *vecHilf = new D3DXVECTOR3();
			itoa(j,cProfFor_countj,10);			
			strcat(ForNamex,cProfFor_countj);
			strcat(ForNamez,cProfFor_countj);
			vecHilf->x = GetPrivateProfileInt(cProfFor_countj,ForNamex,0,cProfFor);
			vecHilf->z = GetPrivateProfileInt(cProfFor_countj,ForNamez,0,cProfFor);
			if(vecHilf->x || vecHilf->z)
			{
				ffstHelp->lstVecFormation.push_back(vecHilf);
				iProfFor_count++;

			}
			else
			{

				delete(vecHilf);
				break;
			}

			ForNamex[0] = 'x';ForNamex[1] = 0;
			ForNamez[0] = 'z';ForNamez[1] = 0;

		}

		if(ffstHelp->lstVecFormation.empty())
			break;

		lstFormationfst.push_back(ffstHelp);




	}


	//SchussProfile laden
	char cProfSch[20]="Schuss\\Modelle.ini";
	char cProfSch_count[sizeof(int)*8+1]="";
	iProfSch_count=0;
	for(int i=0; 1 ;i++)
	{
		itoa(i,cProfSch_count,10);		
		struct Schussfilesystem *sfstHelp=new Schussfilesystem;
		sfstHelp->iGeschwindigkeit=GetPrivateProfileInt(cProfSch_count,"Geschwindigkeit",0,cProfSch);
		sfstHelp->iModell=GetPrivateProfileInt(cProfSch_count,"Modell",0,cProfSch);
		sfstHelp->iStaerke=GetPrivateProfileInt(cProfSch_count,"Staerke",0,cProfSch);
		if(sfstHelp->iGeschwindigkeit || sfstHelp->iModell || sfstHelp->iStaerke)
		{
			lstSchussfst.push_back(sfstHelp);
			iProfSch_count++;
		}
		else
		{
			delete(sfstHelp);
			break;
		}
	}

	//PickupProfile laden
	char cProfPic[20]="Pickup\\Modelle.ini";
	char cProfPic_count[sizeof(int)*8+1]="";
	iProfPic_count=0;
	for(int i=0; 1 ;i++)
	{
		itoa(i,cProfPic_count,10);		
		struct Pickupfilesystem *pfstHelp=new Pickupfilesystem;
		pfstHelp->iWert=GetPrivateProfileInt(cProfPic_count,"Wert",0,cProfPic);
		pfstHelp->iModell=GetPrivateProfileInt(cProfPic_count,"Modell",0,cProfPic);
		if(pfstHelp->iWert || pfstHelp->iModell)
		{
			lstPickupfst.push_back(pfstHelp);
			iProfPic_count++;
		}
		else
		{
			delete(pfstHelp);
			break;
		}
	}

}


void spiel::behandle_partikel()
{
	/*objekt enable_part;
	enable_part.load("schuss.x","Schuss",D3DCULL_CCW);
	enable_part.set_pos(D3DXVECTOR3(769808,-9999,-99999));
	enable_part.render(0);*/

	partAntrieb1.render();
	partAntrieb2.render();

	D3DXVECTOR3 spieler_pos, lichtdir;
	spieler->get_pos(spieler_pos);

	spieler_pos.z += 3.5f;
	spieler_pos.y += 3.5f;
	spieler_pos.x -= 5.3f;
	partAntrieb1.set_posi(spieler_pos);
	spieler_pos.y += 0.2f;
	spieler_pos.z -= 6.7f;
	partAntrieb2.set_posi(spieler_pos);

	partAntrieb1.set(2); // Partikel weiterschieben
	partAntrieb2.set(2);
}

void spiel::kollisionSpielerschuesseGegner()
{
	int flag=0;
	//------------------Kollision schuesse Gegner--------------------------
	for (iterSchuss = lstSchuss.begin(); iterSchuss != lstSchuss.end();)
	{
		flag=0;
		D3DXVECTOR3 vPosTests;
		(*iterSchuss)->step();
		(*iterSchuss)->get_pos(vPosTests);
		// Gegner löschen, wenn außerhalb der Löschbox
		if( (*iterSchuss)->out(objLoeschbox) ) 
		{
			Schuss *schusshilf = *iterSchuss;
			list<Schuss *>::iterator iterhilf = iterSchuss;
			iterSchuss++;
			lstSchuss.erase(iterhilf);
			delete(schusshilf);
		}
		else //Gegner noch innerhalb der Box
		{
			for (iterGegner = lstGegner.begin();iterGegner != lstGegner.end();)
			{ 
				D3DXVECTOR3 vPosTestg,luv,roh,resc;
				(*iterGegner)->get_pos(vPosTestg);
				(*iterGegner)->get_boundingbox( &luv, &roh);
				resc=luv-roh;
				luv.x*=(10/resc.x);
				luv.y*=(10/resc.y);
				luv.z*=(10/resc.z);
				roh.x*=(10/resc.x);
				roh.y*=(10/resc.y);
				roh.z*=(10/resc.z);
				vPosTestg-=vPosTests;
				float x=(*iterSchuss)->get_Schrittweite(); // richtungsvektor des schusses
				float y=0;  //
				float z=0;  // 
				if(vPosTestg.x < (luv.x+x) && vPosTestg.z < (luv.z+y) && vPosTestg.x> (roh.x-x) && vPosTestg.z > (roh.z-z))
				{

					// KOLLISION
					//(*iterGegner)->render_box(0,123,0);
					iadd_points = (*iterGegner)->treffer((*iterSchuss)->get_staerke());
					if(iadd_points)
					{	

						Gegner *hilf=(*iterGegner);

						(*iterGegner)->get_pos(abc);
						partExplosion.set_posi(abc);
						list<Gegner *>::iterator iterGegner1=iterGegner;
						lstGegner.erase(iterGegner1);
						delete(hilf);
						if(iDead>6)
							iDead=4;
						play_sound(iDead);
						partExplosion.set(0);
						iExplo=15;
						iDead++;
						iPunkte += iadd_points;
						iadd_points = 0;
					}
					Schuss *hilf1=(*iterSchuss);
					list<Schuss *>::iterator iterSchuss1=iterSchuss;
					iterSchuss++;
					lstSchuss.erase(iterSchuss1);
					delete(hilf1);	
					flag=1;
					play_sound(3);
					break;
				}


				iterGegner++;
			}


			if(!flag)
				iterSchuss++;
		}
	}
}

void spiel::kollisionGegnerschuesseSpieler()
{
	//Kollision Schuesse Gegner
	//D3DXVECTOR3 luv,roh,vSpieleraktuell,vSchussaktuell,vSchusspos,vSpielerposdif;
	D3DXVECTOR3 vSchussaktuell;
	int flag=0;
	Schuss *hilf;
	int adsad=0;
	for(iterGegSchuss=lstGegSchuss.begin();iterGegSchuss!=lstGegSchuss.end();)
	{
		(*iterGegSchuss)->get_pos(vSchussaktuell);

		if(spieler->inbox(&vSchussaktuell))
		{//Hit
			//Hit

			if(iVidflag==1)
			{
				iVidflag=0;

				iVideo++;//(int)rand()%();
				if(iVideo>2)
					iVideo=0;

				endlosvideo.pause(1);

				switch(iVideo)
				{
				case 0:
					{
						endlosvideo.pause(1);
						video2.pause(1);
						video3.pause(1);
						//video1.play();
						video1.setpos(0,1);
						video2.setpos(0,0);
						video3.setpos(0,0);
						break;
					}
				case 1:
					{
						endlosvideo.pause(1);
						video1.pause(1);
						video3.pause(1);
						video2.setpos(0,1);
						video1.setpos(0,0);
						video3.setpos(0,0);
						//video2.play();
						break;
					}
				case 2:
					{
						endlosvideo.pause(1);
						video2.pause(1);
						video1.pause(1);
						//video3.play();
						video3.setpos(0,1);
						video1.setpos(0,0);
						video2.setpos(0,0);
						break;
					}
				}
			}

			iAufladen=0;
			iSchild=6;
			play_sound(1);

			if(spieler->treffer((*iterGegSchuss)->get_staerke()))
			{
				//Spieler zerstört
			}

			if (spieler->get_hp()<=5 && !iAlarm)
			{
				play_sound(8);
				iAlarm=1;
				iPoweronoff=0;
			}
			hilf=(*iterGegSchuss);
			delete(hilf);
			list<Schuss *>::iterator iterGegSchuss2;
			iterGegSchuss2=iterGegSchuss;
			iterGegSchuss++;
			lstGegSchuss.erase(iterGegSchuss2);
		}
		else
		{
			iterGegSchuss++;
		}
	}

	spieler->get_pos(vSpieleraltepos);	
}
void spiel::kollisionSpielerGegner()
{
	for(iterGegner=lstGegner.begin();iterGegner!=lstGegner.end();iterGegner++)
	{
		D3DXVECTOR3 vGegnerpos,vSpielerpos,gluv,groh,sroh,sluv,resc;
		(*iterGegner)->get_pos(vGegnerpos);
		(*iterGegner)->get_boundingbox(&gluv,&groh);

		spieler->get_pos(vSpielerpos);
		resc=gluv-groh;
		gluv.x*=(10/resc.x);
		gluv.y*=(10/resc.y);
		gluv.z*=(10/resc.z);
		groh.x*=(10/resc.x);
		groh.y*=(10/resc.y);
		groh.z*=(10/resc.z);
		D3DXVECTOR3 vErste,vZwote,vDritte,vVierte;
		vErste=vGegnerpos+gluv;
		vZwote=vGegnerpos+gluv;
		vDritte=vGegnerpos+groh;
		vVierte=vGegnerpos+groh;

		//float fHelp;
		//fHelp=vVierte.x;
		vVierte.x=vDritte.x;
		vDritte.z=vVierte.z;

		vErste.y=0;
		vZwote.y=0;
		vDritte.y=0;
		vVierte.y=0;

		if(spieler->inbox(&vErste) || spieler->inbox(&vZwote) || spieler->inbox(&vDritte) || spieler->inbox(&vVierte))
		{
	/*		spieler->render_box(123,0,0);
			(*iterGegner)->render_box(0,123,0);*/
		}	

		spieler->get_boundingbox(&sluv,&sroh);
		D3DXVECTOR3 vsErste,vsZwote,vsDritte,vsVierte;

		vsErste=vSpielerpos+sluv;
		vsZwote=vSpielerpos+sluv;
		vsDritte=vSpielerpos+sroh;
		vsVierte=vSpielerpos+sroh;

		float help;
		help=vsZwote.x;
		vsZwote.x=vsVierte.x;
		vsVierte.x=help;

		vsErste.y=0;
		vsZwote.y=0;
		vsDritte.y=0;
		vsVierte.y=0;

		if( kollisionSpielerGegnerauth(vErste, vDritte,vsErste) || kollisionSpielerGegnerauth(vErste,vDritte,vsZwote) || kollisionSpielerGegnerauth(vErste,vDritte,vsDritte) || kollisionSpielerGegnerauth(vErste,vDritte,vsVierte) )
		{
		/*	spieler->render_box(123,0,0);
			(*iterGegner)->render_box(0,123,0);*/
		}
	}		

}

int spiel::kollisionSpielerGegnerauth(D3DXVECTOR3 &vErste,D3DXVECTOR3 &vDritte,D3DXVECTOR3 &vsErste)
{
	if ((vErste.x<=vsErste.x && vDritte.x>=vsErste.x)||(vErste.x>=vsErste.x && vDritte.x<=vsErste.x))
		if((vErste.z<=vsErste.z && vDritte.z>=vsErste.z)||(vErste.z>=vsErste.z && vDritte.z<=vsErste.z))
			return 1;
	return 0;
}

void spiel::kollisionSpielerPickup()
{
	if(pPick)
	{

		pPick->get_pos(vector);
		if(!(objLoeschbox->inbox(&vector)))
		{
			pPick->aktion(*spieler);
			Pickup *pHelp=pPick;
			delete (pHelp);
			pPick=0;
		}
		else
		{
			if(spieler->inbox(&vector))
			{
				pPick->aktion(*spieler);
				Pickup *pHelp=pPick;
				delete (pHelp);
				pPick=0;
				play_sound(255);
			}
		}
	}
}

void spiel::createPickup()
{

	if(!pPick && !iPickupzeit)
	{
		int random=0;

		iterPickupfst=lstPickupfst.begin();
		list<objekt *>::iterator iterPickupobj=lstPickupobj.begin();

		random=rand()%(iProfPic_count);
		for(int i=0; i<random; i++ ,iterPickupfst++)
		{
		}

		for(int i=0; i<(*iterPickupfst)->iModell; i++, iterPickupobj++)
		{
		}

		pPick=new Pickup((*iterPickupobj),*iterPickupfst);
		vector=standort;
		vector.x+=350;
		vector.y=0;
		vector.z=rand()%(OFFSETZ_PICKUP)-(OFFSETZ_PICKUP/2);
		pPick->set_pos(vector);

		iPickupzeit=CPS_DELAY_PICKUPZEIT;
	}

	if(iPickupzeit==0)
	{
		iPickupzeit=CPS_DELAY_PICKUPZEIT;
	}
	else
	{
		iPickupzeit--;
	}
}




spiel sergej_faerlich;