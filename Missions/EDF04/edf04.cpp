#include "..\Shared\SPMission.h"
#include <math.h>
#include <time.h>

#define AUDIO_MESSAGE_COUNT 17
#define NUM_PORTALS 6
#define NUM_GUARDIANS 4
#define NUM_KRULS 5
#define NUM_TURRETS1 4
#define NUM_TURRETS2 4

class edf04 : public SPMission 
{
public:
	edf04(void)
	{
		AllowRandomTracks(true);
		b_count = &b_last - &b_first - 1;
		b_array = &b_first + 1;

		f_count = &f_last - &f_first - 1;
		f_array = &f_first + 1;

		h_count = &h_last - &h_first - 1;
		h_array = &h_first + 1;

		i_count = &i_last - &i_first - 1;
		i_array = &i_first + 1;
	}

	void Setup(void);
	void AddObject(Handle h);
	void Execute(void);
	void HandleSetup(void);
	void DestroyMegaturrets(void);
	void SpawnKruls(void);
	void SpawnTurrets(void);
	void HadeanRetaliate(void);
	void HandleRecyclerRetreat(void);
	void CheckStuffIsAlive(void);
	Handle TeleportIn(char *odf, int team, Handle dest, float offset);
	Handle ReplaceObject(Handle h, char* newOdf);

	// bools
	bool
		b_first,
		m_MissionFailed,
		m_DestroyMegaturrets,
		m_SpawnTurrets,
		m_SpawnKruls,
		m_HadRetaliate,
		m_RecyclerRetreat,
		b_last;

	// floats
	float
		f_first,
		m_MissionTimer,
		m_SetupTimer,
		m_MegaturrTimer,
		m_CerbSpawnTimer,
		m_TurretSpawnTimer,
		m_HadRetaliateTimer,
		m_CuttingItCloseTime,
		f_last;

	// handles
	Handle
		h_first,
		Object1,	//mega gun
		Object2,	//mega guard 1
		Object3,	//mega guard 1
		Object4[6],	//portals
		Object11,	//"stick" in final megagun cutscene
		Object14,	//recy
		Object15,	//player
		Object16,	//engineer transport
		Object17,	//Drop3?
		Object18, //Engineer (pilot)
		Object28[NUM_TURRETS1],
		Object32[NUM_TURRETS2],
		Object36[NUM_KRULS],
		Object41[NUM_GUARDIANS],	//hadean base guardian turrets
		Object45,	//Base Nav
		Object46,	//Mega gun Nav
		Object47,	//Hadean Base Nav
		Object48,	//Nav Delta
		h_last;

	// integers
	int
		i_first,
		m_TPS,
		m_MissionState,
		m_SetupState,
		m_MegaturrState,
		m_TurretSpawnState,
		m_HadRetaliateState,
		m_RecyclerState,
		i_last;

	char
		*_Text1 = "Escort the Recycler to the \nindicated Nav point and deploy\nit at that site.",
		*_Text2 = "Take control of the Megagun relic\nto the east of our base, then\nescort the engineers' transport\nto the Megagun.",
		*_Text3 = "Stand by while Captain O'Ryan\nand his team of engineers attempt\nto crack the Hadean codes and take\ncontrol of the Megagun.",
		*_Text4 = "Each Megagun shot consumes 160\nmetric tons of scrap. Capture and\nhold 6 scrap pools so O'Ryan can\nblast the other Megaguns.",
		*_Text5 = "--WARNING-WARNING-WARNING--\nSkyEye reports incoming fire from\nthe Athena Megagun. Take cover!\n--WARNING-WARNING-WARNING--",
		*_Text6 = "Build up another 160 tons of\nbiometal so that O'Ryan can\ndestroy the second Megagun, on\nthe planet Troy.",
		*_Text7 = "The last Megagun, on Alexandria,\nis preparing to fire! Hurry and\ngather the necessary scrap!",
		*_Text8 = "You have failed in your primary\nobjective. Our last Recycler, the\nVENGEANCE, has been destroyed.",
		*_Text9 = "You allowed the transport with \nour engineers to be destroyed.\nWe can no longer take control of\nthe Megagun.",
		*_Text10 = "Collect resources and fortify\nyour position. Do NOT attack the\nHadean base until so ordered.",
		*_Text11 = "Escort the EDF Recycler VENGEANCE\nto the safe zone at Nav Delta.";

	char buf[256];	//scratch buffer for sprintf
};
DLLBase * BuildMission(void)
{
	return new edf04();
}

void edf04::Setup(void)
{	
	srand((unsigned int)time(0));
	
	//set all variables to 0/false
	memset(i_array, 0, i_count*sizeof(int));
	memset(b_array, 0, b_count*sizeof(bool));
	memset(h_array, 0, h_count*sizeof(Handle));
	memset(f_array, 0, f_count*sizeof(float));
	
	SetAutoGroupUnits(false);
	EnableHighTPS(m_TPS);

	m_SpawnTurrets = true;
	m_SpawnKruls = true;
	m_CerbSpawnTimer = 220;

	//Preload ODFs to reduce lag spikes when things are spawned in for the first time
	PreloadODF("cvhtank");
	PreloadODF("cvhscout");
	for (int i = 0; i < AUDIO_MESSAGE_COUNT; i++){
		sprintf_s(buf, "edf04%02d.wav", i + 1);
		PreloadAudioMessage(buf);
	}

	Object1 = GetHandle("Mega Gun");
	Object2 = GetHandle("MGD1");
	Object3 = GetHandle("MGD2");
	Object14 = GetHandle("unnamed_ivrecy");
	Object16 = GetHandle("Tran1");
	Object17 = GetHandle("Drop3");
	for (int i = 0; i < NUM_PORTALS; i++){
		sprintf_s(buf, "Portal%d", i + 1);
		Object4[i] = GetHandle(buf);
	}
	for (int i = 0; i < NUM_GUARDIANS; i++){
		sprintf_s(buf, "ET%d", i + 1);
		Object41[i] = GetHandle(buf);
	}
}

void edf04::AddObject(Handle newGuy)
{
	Handle temp;
	SetSkill(newGuy, 3);
	if (IsOdf(newGuy, "ibfact_s")){
		temp = TeleportIn("cvhtank", 6, Object4[3], 20);
		Attack(temp, newGuy, 1);
	}
	else if (IsOdf(newGuy, "ibgtow")){
		temp = TeleportIn("cvhscout", 6, Object4[1], 20);
		Attack(temp, newGuy, 1);
	}
	else if (IsOdf(newGuy, "ibpgen")){
		temp = TeleportIn("cvhscout", 6, Object4[3], 20);
		Attack(temp, newGuy, 1);
	}
	else if (IsOdf(newGuy, "ibsbay")){
		SetObjectiveOn(newGuy);		//Highlight the player's service bay
	}
}

void edf04::Execute(void)
{
	if (m_MissionTimer < GetTime()){
		switch (m_MissionState){
		case 0:
			SetScrap(1, 40);
			AudioMessage("edf0401.wav");	//Windex:"Attention Spartacus task force..."
			SetObjectiveOn(Object16);
			CameraReady();
			m_MissionState++;
			break;
		case 1:
			if (CameraPath("rcam", 1500, 2000, Object14) || CameraCancelled()){
				CameraFinish();
				AddObjective(_Text1, WHITE);
				Object45 = BuildObject("ibnav", 1, "bss");
				SetObjectiveName(Object45, "Base Site");
				SetObjectiveOn(Object45);
				SetPlan("edf04a.aip", 6);
				m_MissionState++;
			}
			break;
		case 2:	//LOC_46
			if (IsOdf(Object14, "ibrecy_s")){
				if (GetDistance(Object14, Object45) > 200){
					AudioMessage("edf0402.wav");	//Windex:"You were clearly ordered to deploy near the nav beacon..."
					FailMission(GetTime() + 10, "edf04l2.des");
					m_MissionState = 99;
				}
				else{
					RemoveObject(Object45);
					ClearObjectives();
					AddObjective(_Text10, WHITE);
					Object47 = BuildObject("ibnav", 1, "ebs");
					SetObjectiveName(Object47, "Hadean Base");
					SetObjectiveOn(Object47);
					AudioMessage("edf0403.wav");	//Skyeye:"We've pinpointed the Hadean stronghold..."
					m_MissionState++;
					m_MissionTimer = GetTime() + 10;
				}
			}
			break;
		case 3:
			AddObjective(_Text2, WHITE);
			AudioMessage("edf0404.wav");	//Skyeye:"We've found the Mega Gun location..."
			Object46 = BuildObject("ibnav", 1, "mgs");
			SetObjectiveName(Object46, "Mega Gun");
			SetObjectiveOn(Object46);
			SetPlan("edf04b.aip", 6);
			m_MissionState++;
			break;
		case 4:	//LOC_69
			Object15 = GetPlayerHandle();
			if (GetDistance(Object15, Object46) < 250){
				AudioMessage("edf0405.wav");	//O'Ryan:"Interesting, the Guardian turrets seem to be inactive..."
				m_MissionState++;
			}
			break;
		case 5:	//LOC_79
			if (GetDistance(Object16, Object1) < 75){
				Stop(Object16, 1);
				Vector pos = GetPosition(Object16);
				Object18 = BuildObject("ispilo", 1, pos);
				SetObjectiveName(Object18, "Engineer");
				SetObjectiveOn(Object18);
				SetObjectiveOff(Object1);
				SetObjectiveOff(Object46);
				SetObjectiveOff(Object16);
				Goto(Object18, Object1, 0);
				m_MissionState++;
			}
			break;
		case 6:	//LOC_90
			if (GetDistance(Object18, Object1) < 40){
				RemoveObject(Object18);
				ClearObjectives();
				AddObjective(_Text3, GREEN);
				AudioMessage("edf0406.wav");	//O'Ryan:"I'm inside the megagun..."
				m_MissionState++;
				m_MissionTimer = GetTime() + 15;
			}
			break;
		case 7:
			ClearObjectives();
			AddObjective(_Text2, GREEN);
			AddObjective(_Text4, WHITE);
			AudioMessage("edf0407.wav");	//O'Ryan:"Yes! The megagun is online..."
			SetTeamNum(Object1, 1);
			SetTeamNum(Object2, 1);
			SetTeamNum(Object3, 1);			
			m_MissionState++;
			m_MissionTimer = GetTime() + 10;
			break;
		case 8:
			AudioMessage("edf0408.wav");	//Windex:"Good work O'Ryan..."
			SetPlan("edf04c.aip", 6);
			m_MissionState++;
			break;
		case 9:	//LOC_107
			if (GetScrap(1) >= 110){
				AudioMessage("edf0409.wav");	//Skyeye:"We've just detected a massive energy spike on the planet Athena..."
				AddObjective(_Text5, RED);
				m_MissionState++;
				m_MissionTimer=GetTime() + 60;
			}
			break;
		case 10:
			BuildObject("mbbeam", 9, "mgbbb");
			BuildObject("mbfire", 9, "mgbbb");
			BuildObject("mbrocks", 9, "mgbbb");
			m_MissionState++;
			m_MissionTimer = GetTime() + 15;
			break;
		case 11:
			AudioMessage("edf0410.wav");	//Windex:"That was far too close..."
			ClearObjectives();
			AddObjective(_Text4, WHITE);
			m_MissionState++;
			break;
		case 12:	//LOC_119
			if (GetScrap(1) >= 160){
				AudioMessage("edf0411.wav");	//Skyeye:"Well done engineering crew. The first shot is away..."
				CameraReady();
				m_MissionState++;
			}
			break;
		case 13:
			if (CameraPath("cam1", 5000, 2000, Object1) || CameraCancelled()){
				CameraFinish();
				SetScrap(1, 0);
				ClearObjectives();
				AddObjective(_Text4, GREEN);
				m_HadRetaliate = true;	//RunSpeed,_Routine11,1,true
				AudioMessage("edf0413.wav");	//Skyeye:"We're picking up substantial energy pulsations.."
				m_MissionState++;
			}
			break;
		case 14:	//LOC_130
			if (GetScrap(1) >= 160){
				CameraReady();
				m_MissionState++;
			}
			break;
		case 15:
			if (CameraPath("cam2", 5000, 2000, Object1) || CameraCancelled()){
				CameraFinish();
				SetScrap(1, 0);
				ClearObjectives();
				AddObjective(_Text6, GREEN);
				m_MissionState++;
				m_MissionTimer = GetTime() + 5;
			}
			break;
		case 16:
			AudioMessage("edf0414.wav");	//Skyeye:"We just wiped out the megagun on planet Troy..."
			AddObjective(_Text7, WHITE);
			SetPlan("edf04d.aip", 6);
			m_HadRetaliate = true; //RunSpeed,_Routine11,1,true
			Goto(Object16, Object46, 1);
			m_CuttingItCloseTime = GetTime() + 1200;
			m_MissionState++;
			break;
		case 17:
			if (m_CuttingItCloseTime < GetTime()){
				AudioMessage("edf0415.wav");	//Skyeye:"The Hadeans are powering up the Alexandria Megagun..."
				StartCockpitTimer(300);
				m_MissionState++;
			}
			else if (GetScrap(1) >= 160){
				m_MissionState = 19;	//LOC_152
			}
			break;
		case 18:	//LOC_147
			if (GetCockpitTimer() == 0){
				m_MissionState = 199;
			}
			else if (GetScrap(1) >= 160){
				StopCockpitTimer();
				m_MissionState++;
			}
			break;
		case 19:	//LOC_152
			{
				AudioMessage("edf0416.wav");	//O'Ryan:"Fire!"
				m_RecyclerRetreat = true;	//RunSpeed,_Routine5,1,false
				Vector pos = GetPosition(Object1);
				pos.y += 10;
				Object11 = BuildObject("stick", 0, pos);
				m_DestroyMegaturrets = true; //RunSpeed,_Routine9,1,true
				CameraReady();
				m_MissionState++;
			}
			break;
		case 20:
			if (CameraPath("cam3", 5000, 2500, Object11) || CameraCancelled()){
				CameraFinish();
				RemoveObject(Object11);
				m_MissionState++;
			}
			break;
		case 21:
			CameraReady();
			m_MissionState++;
			break;
		case 22:
			if (CameraPathDir("mtcam", 3000, 3000) || CameraCancelled()){
				CameraFinish();
				Object11 = GetHandle("FN");
				m_MissionState++;
			}
			break;
		case 23:
			CameraReady();
			m_MissionState++;
			break;
		case 24:
			if (CameraPath("dtcam", 5000, 1750, Object11) || CameraCancelled()){
				CameraFinish();
				RemoveObject(Object11);
				SetScrap(1, 0);
				ClearObjectives();
				AddObjective(_Text11, WHITE);
				AudioMessage("edf0417.wav");
				m_SpawnTurrets = false; //RunSpeed,_Routine8,0,false
				m_SpawnKruls = false; //RunSpeed,_Routine10,0,false
				Object48 = BuildObject("ibnav", 1, "NGTE");
				SetObjectiveName(Object48, "Nav Delta");
				SetObjectiveOn(Object48);
				SetObjectiveOn(Object14);	//added objective marker to the recycler
				m_MissionState++;
			}
			break;
		case 25:	//LOC_173
			Object15 = GetPlayerHandle();
			if (GetDistance(Object14, Object48) < 30 && 
				GetDistance(Object15, Object48) < 75){
				ClearObjectives();
				AddObjective(_Text11, GREEN);
				SucceedMission(GetTime() + 10, "edf04w1.des");
				m_MissionState++;
			}
			break;
		case 199:	//LOC_180
			{
				//player took too long. Mission failed
				Vector pos = GetPosition(Object1);
				pos.y += 10;
				Object11 = BuildObject("stick", 0, pos);
				pos.y += 5;
				BuildObject("mbbeam", 9, pos);
				BuildObject("mbfire", 9, pos);
				CameraReady();
				m_MissionState++;
			}
			break;
		case 200:
			if (CameraPath("cam3", 5000, 2500, Object11) || CameraCancelled()){
				CameraFinish();
				RemoveObject(Object11);
				FailMission(GetTime() + 2, "edf04l4.des");
				m_MissionState++;
			}
			break;
		}
	}

	HandleSetup();
	if (m_SpawnKruls){
		SpawnKruls();
	}
	if (m_SpawnTurrets){
		SpawnTurrets();
	}
	if (m_DestroyMegaturrets){
		DestroyMegaturrets();
	}
	if (m_RecyclerRetreat){
		HandleRecyclerRetreat();
	}
	HadeanRetaliate();
	CheckStuffIsAlive();
}

void edf04::HandleSetup(void){
	Handle h;
	if (m_SetupTimer < GetTime()){
		switch (m_SetupState){
		case 0:
			Ally(1, 2);
			BuildObject("evturr", 6, "path_5");
			BuildObject("evturr", 6, "path_6");
			BuildObject("evturr", 6, "path_7");
			SetAnimation(Object17, "takeoff", 1);
			StartAnimation(Object17);
			m_SetupState++;
			m_SetupTimer = GetTime() + 15;
			break;
		case 1:
			RemoveObject(Object17);
			h = BuildObject("ptera02", 0, "aniwalk1");
			Patrol(h, "aniwalk1", 1);
			h = BuildObject("ptera03", 0, "aniwalk2");
			Patrol(h, "aniwalk2", 1);
			h = BuildObject("herbiv01", 0, "aniwalk3");
			Patrol(h, "aniwalk3", 1);
			BuildObject("compy01", 0, "anidrink1");
			BuildObject("compy01", 0, "anidrink3");
			BuildObject("raptor01", 0, "anidrink2");
			m_SetupState++;
			break;
		case 2:
			m_SetupState++;
			break;
		}
	}
}

void edf04::DestroyMegaturrets(void){
	if (m_MegaturrTimer < GetTime()){
		switch (m_MegaturrState){
		case 0:
			m_MegaturrState++;
			m_MegaturrTimer = GetTime() + 9;
			break;
		case 1:
			{
			Vector pos = GetPosition(Object1);
			pos.y += 15;
			BuildObject("mbbeam", 9, pos);
			BuildObject("mbfire", 9, pos);
			}
			m_MegaturrState++;
			m_MegaturrTimer = GetTime() + 14;
			break;
		case 2:
			EjectPilot(Object2);
			EjectPilot(Object3);
			m_MegaturrState++;
			m_MegaturrTimer = GetTime() + 3;
			break;
		case 3:
			EjectPilot(Object41[0]);
			m_MegaturrState++;
			m_MegaturrTimer = GetTime() + 2;
			break;
		case 4:
			EjectPilot(Object41[1]);
			m_MegaturrState++;
			m_MegaturrTimer = GetTime() + 2;
			break;
		case 5:
			EjectPilot(Object41[2]);
			m_MegaturrState++;
			m_MegaturrTimer = GetTime() + 2;
			break;
		case 6:
			EjectPilot(Object41[3]);
			m_MegaturrState++;
			break;
		}
	}
}

void edf04::SpawnKruls(void){
	char *cerbPatrolRoutes[NUM_KRULS] = { "STDpath","STDpath","STDpath","STDs1","STDs2" };
	if (m_CerbSpawnTimer < GetTime()){
		for (int i = 0; i < NUM_KRULS; i++){
			if (!IsAround(Object36[i])){
				Object36[i] = TeleportIn("cvhtank", 6, Object4[5], 30);
				Patrol(Object36[i], cerbPatrolRoutes[i], 1);
			}
		}
		m_CerbSpawnTimer = GetTime() + 220;
	}
}

void edf04::SpawnTurrets(void){
	char *turretPos1[NUM_TURRETS1] = { "ST1one", "ST1two", "ST1three", "ST1four" };
	char *turretPos2[NUM_TURRETS2] = { "ST2one", "ST2two", "ST2three", "ST2four" };
	
	if (m_TurretSpawnTimer < GetTime()){
		switch (m_TurretSpawnState){
		case 0:	//LOC_232
			m_TurretSpawnState++;
			m_TurretSpawnTimer = GetTime() + 60;
			break;
		case 1:	//LOC_238
			for (int i = 0; i < NUM_TURRETS1; i++){
				if (!IsAround(Object28[i])){
					Object28[i] = TeleportIn("evturr", 6, Object4[2], 30);
					Goto(Object28[i], turretPos1[i]);
				}
			}
			m_TurretSpawnState++;
			m_TurretSpawnTimer = GetTime() + 60;
			break;
		case 2:	//LOC_253
			for (int i = 0; i < NUM_TURRETS2; i++){
				if (!IsAround(Object32[i])){
					Object32[i] = TeleportIn("evturr", 6, Object4[4], 30);
					Goto(Object32[i], turretPos2[i]);
				}
			}
			m_TurretSpawnState = 0;
			m_TurretSpawnTimer = GetTime() + 60;
			break;
		}
	}
}

void edf04::HadeanRetaliate(void){
	char* locations[5] = { "CerbTL", "CerbBL", "CerbBR", "CerbTR", "CerbC" };
	char* odfs[5] = { "cvhscout", "cvhscout", "cvhtank", "cvhscout", "cvhtank", };
	const int portals[5] = {4,4,2,2,5};

	if (m_HadRetaliateTimer < GetTime()){
		switch (m_HadRetaliateState){
		case 0:	//LOC_325
			if (m_HadRetaliate){
				m_HadRetaliate = false;	//set this to false here, so if it gets triggered twice in a row quickly, this routine will still run twice.
				m_HadRetaliateState++;
				m_HadRetaliateTimer = GetTime() + 2;
			}	
			break;
		case 1:
			//spawn in some random cerb units to harrass player's extractors
			for (int i = 0; i < 6; i++){
				int r = rand() % 5;
				Handle h = TeleportIn(odfs[r], 6, Object4[portals[r]], 20);
				Goto(h, locations[r], 0);
			}
			Attack(TeleportIn("cvhtank", 6, Object4[3], 20), Object14, 1);
			Attack(TeleportIn("cvhscout", 6, Object4[3], 20), Object14, 1);
			Attack(TeleportIn("cvhtank", 6, Object4[3], 20), Object14, 1);
			Attack(TeleportIn("cvhscout", 6, Object4[3], 20), Object14, 1);
			m_HadRetaliateState++;
			m_HadRetaliateTimer = GetTime() + 15;
			break;
		case 2:
			//Hadean off planet megaguns target a random extractor on the map
			{
				int r = rand() % 5;
				BuildObject("mbbeam", 9, locations[r]);
				BuildObject("mbfire", 9, locations[r]);
				BuildObject("mbrocks", 9, locations[r]);
			}
			m_HadRetaliateState = 0;
			break;
		}
	}
}

void edf04::HandleRecyclerRetreat(void){
	switch (m_RecyclerState){
	case 0:
		Object14 = ReplaceObject(Object14, "ivrecy");
		SetTeamNum(Object14, 2);
		SetPlan("edf04e.aip", 6);
		m_RecyclerState++;
		break;
	case 1:
		Object15 = GetPlayerHandle();
		if (GetDistance(Object14, Object15) < 75){
			Goto(Object14, Object48, 1);
			m_RecyclerState++;
		}
		break;
	}
}

void edf04::CheckStuffIsAlive(void){
	if (!m_MissionFailed){
		if (!IsAround(Object14)){
			FailMission(GetTime() + 10, "edf04l1.des");
			m_MissionFailed = true;
		}
		else if (!IsAround(Object16)){
			ClearObjectives();
			AddObjective(_Text9, RED);
			//AudioMessage("edf04l3.wav");	//doesn't exist (that's an 'L', not a '1')
			//AudioMessage("edf0413a.wav");	//O'Ryan:"AAARRRGGGGH!"
			FailMission(GetTime() + 10, "edf04l3.des");
			m_MissionFailed = true;
		}
	}
}

//Teleports in a new object at <dest>, <offset> metres away.
Handle edf04::TeleportIn(char *odf, int team, Handle dest, float offset){
	Vector pos = GetPosition(dest);
	pos.x += offset;
	pos.y = TerrainFindFloor(pos.x, pos.z) + 5;
	BuildObject("teleportin", 0, pos);
	return BuildObject(odf, team, pos);
}

/*
Swap out an object for a different one with the same position and health.
*/
Handle edf04::ReplaceObject(Handle h, char* newOdf){
	Matrix position;
	int team = GetTeamNum(h);
	long health = GetCurHealth(h);
	GetPosition(h, position);
	RemoveObject(h);
	Handle newGuy = BuildObject(newOdf, team, position);
	SetCurHealth(h, health);

	return newGuy;
}
