#include "..\Shared\SPMission.h"

#define AUDIO_MESSAGE_COUNT 10
#define NUM_TANKS 6
#define NUM_SURVIVORS 10

class edf01 : public SPMission 
{
public:
	edf01(void)
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
	void DeleteObject(Handle h);
	void Execute(void);
	void HandleIntro(void);
	void HandleMainState(void);
	void HandleSurvivors(void);
	void HandleSurvivorDropoff(void);
	void HandleSurvivorBeacons(void);
	void SpawnHadeanReinforcements(void);
	void CheckHadeanOverrun(void);
	void ShowPickupObjective(void);
	void CheckTanksRemaining(void);
	Handle ReplaceObject(Handle h, char* newOdf);

	// bools
	bool
		b_first,
		m_SpawnHadeanReinforcements,
		m_TooManyHadeans,
		m_HandleSurvivorPickup,
		m_HandleSurvivorDropoff,
		m_HandleSurvivorBeacons,
		m_CheckRemainingTanks,
		m_ShowObjective,
		m_MissionFailed,
		b_last;

	// floats
	float
		f_first,
		m_MissionTimer,
		m_IntroTimer,
		m_HadeanReinforcementTimer,
		m_SurvivorPickupTimer,
		m_SurvivorDropoffTimer,
		m_SurvivorBeaconTimer,
		m_ShowObjectiveTimer,
		m_FailTimer1,
		m_FailTimer2,
		m_CameraStartTime,
		f_last;

	// handles
	Handle
		h_first,
		Object_Player,
		Object_Hadean1,	//Hadean tank at "Observer1"
		Object_Hadean2,	//Hadean tank at "Observer2"
		Object_PowerSource,
		Object_PlayerTanks[NUM_TANKS],
		Object_ShultzTank,
		Object_ShultzPilot,
		Object_Dropship,
		Object_Survivors[NUM_SURVIVORS],
		Object_SafeNav,
		Object_FriendTurret3,
		Object_FriendTurret2,
		Object_APC1,
		Object_APC2,
		h_last;

	// integers
	int
		i_first,
		m_TPS,
		m_MissionState,
		m_IntroState,
		m_HadeanReinforcementState,
		m_SurvivorPickupState,
		m_SurvivorDropoffState,
		m_ShowObjectiveState,
		m_FailState1,
		m_FailState2,
		m_ScanPercentage,	//Cthonian power source scan percentage
		m_PowerScanned,	//# of power sources scanned
		m_SurvivorIndex1,
		m_SurvivorIndex2,
		m_SurvivorsWaiting,
		m_SurvivorsDroppedOff,
		m_SabresRemaining,
		m_HadeanReinforcementCount,	//if this gets over 24, player loses.
		m_SurvivorDied,	//whether a survivor has died
		i_last;

	Vector
		Position1 = { -10, 50, -10 };	//Camera position

	char
		*_Text1 = "Your vehicle is equipped with\na special scanning device.\nScan the power source by\ngetting near it.",
		*_Text2 = "Hostiles are moving to attack!!\nTake action!",
		*_Text3 = "Have your subordinate tanks pick\nup the survivors, and bring them\nback to the dropships.  Only the\nsubordinate Sabres can get them.",
		*_Text4 = "Tanks carrying survivors will be\nplaced in Group 10 automatically.\nThey can't pick you up while\ncarrying a survivor.  Don't lose\nany survivors!",
		*_Text5 = "Scan the next highlighted\npower source.",
		*_Text6 = "You've lost too many tanks\nto complete this mission.",
		*_Text7 = "That's two survivors dead.  That\nlast one was no ordinary survivor,\nbut General Hardin himself!",
		*_Text8 = "The Hadeans have overrun the\narea.  There's no hope now.",
		*_Text9 = "Congratulations.  All the\nsurvivors are safe.  General\nHardin was among them, and\nwould like to thank you.",
		*_Text10 = "A survivor was KILLED.  We\nCANNOT afford to lose another!!!",
		*_Text11 = "All the remaining survivors are\nsafe.  We lost one, regretably.";

	char buf[256];	//scratch buffer for sprintf
};
DLLBase * BuildMission(void)
{
	return new edf01();
}

void edf01::Setup(void)
{
	SetAutoGroupUnits(false);
	EnableHighTPS(m_TPS);

	//set all variables to 0/false
	memset((void*)(&i_first + 1), 0, (&i_last - &i_first - 1)*sizeof(int));
	memset((void*)(&b_first + 1), 0, (&b_last - &b_first - 1)*sizeof(bool));
	memset((void*)(&h_first + 1), 0, (&h_last - &h_first - 1)*sizeof(Handle));
	memset((void*)(&f_first + 1), 0, (&f_last - &f_first - 1)*sizeof(float));

	m_SabresRemaining = 8;	//Shultz + Player's tank + 6 henchmen

	//Preload ODFs to reduce lag spikes when things are spawned in for the first time
	PreloadODF("ivturr");
	PreloadODF("ivtank");
	PreloadODF("ivtank_e01");
	PreloadODF("ivapc");
	PreloadODF("ivserv");
	PreloadODF("evtank");
	PreloadODF("evscout");
	PreloadODF("evturr");
	PreloadODF("evmisl");

	//Preload audio
	for (int i = 0; i < AUDIO_MESSAGE_COUNT; i++){
		sprintf_s(buf, "edf01_%02d.wav", i);
		PreloadAudioMessage(buf);
	}
	PreloadAudioMessage("edf01_05A.wav");
	PreloadAudioMessage("edf01_05B.wav");
	PreloadAudioMessage("edf01_06A.wav");
}

void edf01::AddObject(Handle h)
{
	if (IsOdf(h, "ispilo") && !IsPlayer(h) && m_MissionState < 4){
		Object_ShultzPilot = h;
	}
	else if (IsOdf(h, "evtank") || IsOdf(h, "evscout")){
		m_HadeanReinforcementCount++;
		if (m_HadeanReinforcementCount > 24){
			m_TooManyHadeans = true;
		}
	}
}

void edf01::DeleteObject(Handle h)
{
	if (IsOdf(h, "evtank") || IsOdf(h, "evscout")){
		m_HadeanReinforcementCount--;
	}
	else if (IsOdf(h, "ivtank") || IsOdf(h, "ivtank_e01")){
		m_SabresRemaining--;
		m_CheckRemainingTanks = true;
	}
}



void edf01::Execute(void)
{
	HandleIntro();
	HandleMainState();
	HandleSurvivors();
	HandleSurvivorDropoff();
	HandleSurvivorBeacons();
	SpawnHadeanReinforcements();
	CheckHadeanOverrun();
	CheckTanksRemaining();
	ShowPickupObjective();
}

/*
Code for handling shultz ejecting and scanning the Cthonian power things
*/
void edf01::HandleIntro(void){
	Handle h;
	if (m_IntroTimer < GetTime()){

		switch (m_IntroState){
		case 0:
			Object_Dropship = GetHandle("DropShip");
			Object_Player = GetPlayerHandle();
			SetPerceivedTeam(Object_Player, 5);
			Object_FriendTurret3 = BuildObject("ivturr", 1, "FriendTurret3");
			SetGroup(Object_FriendTurret3, 1);
			Stop(Object_FriendTurret3, 1);
			Object_FriendTurret2 = BuildObject("ivturr", 1, "FriendTurret2");
			SetGroup(Object_FriendTurret2, 1);
			Stop(Object_FriendTurret2, 1);
			h = BuildObject("ivturr", 1, "FriendTurret1");
			Stop(h, 1);
			Object_APC1 = BuildObject("ivapc", 1, "APC1");
			Stop(Object_APC1, 1);
			Object_APC2 = BuildObject("ivapc", 1, "APC2");
			Stop(Object_APC2, 1);


			for (int i = 0; i < NUM_TANKS; i++){
				sprintf_s(buf, "Buddy%d", i+1);
				Object_PlayerTanks[i] = BuildObject("ivtank", 1, buf);
				Stop(Object_PlayerTanks[i], 1);
				SetPerceivedTeam(Object_PlayerTanks[i], 5);
			}

			Object_ShultzTank = BuildObject("ivtank", 1, "Dummy");
			Stop(Object_ShultzTank, 1);
			SetObjectiveName(Object_ShultzTank, "Schulz");
			SetObjectiveOn(Object_ShultzTank);

			m_IntroState++;
			m_IntroTimer = GetTime() + 4;
			break;

		case 1:
			AudioMessage("edf01_01.wav");	//Shultz:"Looks like these Hadeans ain't all they're cracked up to be..."
			Goto(Object_APC1, "APCDest", 1);
			Goto(Object_APC2, "APCDest", 1);
			m_IntroState++;
			m_IntroTimer = GetTime() + 17;
			break;

		case 2:
			EjectPilot(Object_ShultzTank);
			m_IntroState++;
			m_IntroTimer = GetTime() + 3;
			break;

		case 3:
			AudioMessage("edf01_02.wav");	//Shultz:"Piece of junk just blew up on me..."
			Goto(Object_ShultzPilot, Object_Dropship, 1);
			CameraReady();
			m_CameraStartTime = GetTime();
			m_IntroState++;
			break;
		case 4:
			CameraObject(Object_Dropship, Position1.x, Position1.y, Position1.z, Object_ShultzPilot);
			if (GetTime() > m_CameraStartTime + 18){
				CameraFinish();
				m_IntroState++;
				m_IntroTimer = GetTime() + 2;
			}
			break;
		case 5:
			RemoveObject(Object_ShultzPilot);
			Object_ShultzPilot = NULL;
			m_IntroState++;
			break;
		case 6:
			AddObjective(_Text1, WHITE);
			AudioMessage("edf01_03.wav");	//Stewart:"Bravo Leader, scan those power sources..."
			m_IntroState++;
			m_IntroTimer = GetTime() + 3;
			break;
		case 7:	//LOC_49
			for (int i = 0; i < NUM_TANKS; i++){
				char c = i % 2 == 0 ? 'A' : 'B';
				sprintf_s(buf, "Nav%i%c", m_PowerScanned, c);
				Goto(Object_PlayerTanks[i], buf, 1);
			}

			sprintf_s(buf, "PowerSource%i", m_PowerScanned);
			Object_PowerSource = GetHandle(buf);
			sprintf_s(buf, "Power source: %i%% scanned", m_ScanPercentage);
			SetObjectiveName(Object_PowerSource, buf);
			SetObjectiveOn(Object_PowerSource);

			m_IntroState++;
			break;
		case 8:	//LOC_58
			Object_Player = GetPlayerHandle();
			if (GetDistance(Object_Player, Object_PowerSource) < 190){
				m_ScanPercentage += 4;
				if (m_ScanPercentage >= 100){
					m_ScanPercentage = 100;
					m_IntroState++;
					m_IntroTimer = GetTime() + 1;
				}
				else{
					m_IntroTimer = GetTime() + 0.4f;	//scan percentage goes up by 4 every .4s
				}
				sprintf_s(buf, "Power source: %i%% scanned", m_ScanPercentage);
				SetObjectiveName(Object_PowerSource, buf);
			}
			break;
		case 9:
			SetObjectiveOff(Object_PowerSource);
			m_ScanPercentage = 0;
			m_PowerScanned++;
			if (m_PowerScanned > 1){
				m_SpawnHadeanReinforcements = true;
				m_IntroState++;	//intro finished
			}
			else{
				ClearObjectives();
				AddObjective(_Text5, WHITE);
				m_IntroState = 7;
			}
			break;
		}
	}
}

/*
Main mission state. Deals with aggro-ing the Hadeans and controlling the other states.
*/
void edf01::HandleMainState(void){
	Handle h;
	if (m_MissionTimer < GetTime()){
		switch (m_MissionState){
		case 0:
			m_MissionState++;
			m_MissionTimer = GetTime() + 40;
			break;
		case 1:
			Object_Hadean1 = BuildObject("evtank", 5, "Observer1");
			LookAt(Object_Hadean1, Object_Player, 0);
			Object_Hadean2 = BuildObject("evtank", 5, "Observer2");
			LookAt(Object_Hadean2, Object_Player, 0);
			m_MissionState++;
			m_MissionTimer = GetTime() + 10;
			break;
		case 2:
			RemoveObject(Object_APC1);
			RemoveObject(Object_APC2);
			m_MissionState++;
			break;
		case 3:	//LOC_136
			if (GetTime() > 200){
				m_MissionState = 5;	//LOC_150
			} 
			else if (m_PowerScanned >= 1){
				m_MissionState++;
			}
			break;
		case 4:	//LOC_140
			if (GetTime() > 200 ||
				GetTeamNum(GetWhoShotMe(Object_Hadean1)) == 1 ||
				GetTeamNum(GetWhoShotMe(Object_Hadean2)) == 1 ||
				m_ScanPercentage >= 45){
				m_MissionState++;
			}
			break;
		case 5:	//LOC_150
			m_MissionState++;
			m_MissionTimer = GetTime() + 4;
			break;
		case 6:
			AudioMessage("edf01_04.wav");	//Stewart:"Heads up, 2 hostiles closing in"
			SetPerceivedTeam(Object_Player, 1);
			for (int i = 0; i < NUM_TANKS; i++){
				SetPerceivedTeam(Object_PlayerTanks[i], 1);
			}
			ClearObjectives();
			AddObjective(_Text2, WHITE);
			m_MissionState++;
			break;
		case 7:
			//Attack() doesn't seem to work if called in the same tick as SetPerceivedTeam()	
			Attack(Object_Hadean1, Object_PlayerTanks[0], 0);
			Attack(Object_Hadean2, Object_PlayerTanks[1], 0);
			m_MissionState++;
			m_MissionTimer = GetTime() + 4;
			break;
		case 8:
			for (int i = 0; i < NUM_TANKS; i++){
				SetGroup(Object_PlayerTanks[i], 0);
				Stop(Object_PlayerTanks[i], 0);
			}
			SetObjectiveOff(Object_PowerSource);
			m_MissionState++;
			break;
		case 9:	//LOC_179
			if (!(HasPilot(Object_Hadean1) && GetTeamNum(Object_Hadean1) == 5
				&& HasPilot(Object_Hadean2) && GetTeamNum(Object_Hadean2) == 5)){
				m_SpawnHadeanReinforcements = true;
				m_HandleSurvivorPickup = true;
				m_HandleSurvivorDropoff = true;
				m_HandleSurvivorBeacons = true;
				m_ShowObjective = true;
				m_MissionState++;
				m_MissionTimer = GetTime() + 7;
			}
			break;
		case 10:
			AudioMessage("edf01_05.wav");	//Lt. Minner:"Team Alpha's dropships were shot down, require assistance..."
			m_MissionState++;
			m_MissionTimer = GetTime() + 14;
			break;
		case 11:
			AudioMessage("edf01_05A.wav");	//Stewart: "Rescue the Alpha survivors"
			Object_SafeNav = BuildObject("ibnav", 1, "SafeNav");
			SetObjectiveName(Object_SafeNav, "Survivor Dropoff");
			SetObjectiveOn(Object_SafeNav);
			h = BuildObject("ibnav", 1, "SurvivorNav1");
			SetObjectiveName(h, "Survivors");
			SetObjectiveOn(h);
			h = BuildObject("ibnav", 1, "SurvivorNav2");
			SetObjectiveName(h, "Survivors");
			SetObjectiveOn(h);
			BuildObject("evscout", 5, "Enemy2");
			BuildObject("evscout", 5, "Enemy3");
			m_MissionState++;
			m_MissionTimer = GetTime() + 140;
			break;
		case 12:	//LOC_209
			Object_Player = GetPlayerHandle();
			if (GetDistance(Object_Player, Object_Dropship) > 200){
				h = BuildObject("evturr", 5, "TurretSpawn");
				Goto(h, "Turret1", 1);
				h = BuildObject("evturr", 5, "TurretSpawn");
				Goto(h, "Turret2", 1);
				h = BuildObject("evturr", 5, "TurretSpawn");
				Goto(h, "Turret3", 1);
				Stop(Object_FriendTurret3, 0);
				Stop(Object_FriendTurret2, 0);
				h = BuildObject("ivserv", 1, "Buddy4");
				SetGroup(h, 2);
				h = BuildObject("ivserv", 1, "Buddy5");
				SetGroup(h, 3);
				h = BuildObject("ivserv", 1, "Buddy6");
				SetGroup(h, 4);
				AudioMessage("edf01_05B.wav");	//Stewart:"I've put some turrets and service trucks at your disposal..."
				m_MissionState++;	//end of main state
			}
			break;
		}
	}
}

/*
Code for handling tanks picking up survivors
*/
void edf01::HandleSurvivors(void){	//routine5
	if (m_HandleSurvivorPickup && m_SurvivorPickupTimer < GetTime()){
		switch (m_SurvivorPickupState){
		case 0:	//LOC_278
			for (int i = 0; i < NUM_SURVIVORS; i++){
				sprintf_s(buf, "Survivor%i", i);
				Object_Survivors[i] = GetHandle(buf);
				SetMaxHealth(Object_Survivors[i], 800);
				SetCurHealth(Object_Survivors[i], 800);
			}
			m_SurvivorIndex1 = 0;
			m_SurvivorPickupState++;
			break;
		case 1:	//LOC_285
			if (!IsAround(Object_Survivors[m_SurvivorIndex1])){
				//a survivor died
				if (m_SurvivorDied > 0){
					//2 survivors died
					m_SurvivorPickupState = 99;
				}
				else if (m_SurvivorsDroppedOff >= NUM_SURVIVORS - 1){
					//rescued 9 survivors and one died
					m_HandleSurvivorDropoff = false;
					ClearObjectives();
					AddObjective(_Text10, RED);
					AudioMessage("edf01_06a.wav");	//Stewart:"You've just lost a survivor..."
					m_SurvivorPickupState = 199;
					m_SurvivorPickupTimer = GetTime() + 10;
				}
				else{
					//one died, rest still need rescue
					ClearObjectives();
					AddObjective(_Text10, RED);
					AudioMessage("edf01_06a.wav");	//Stewart:"You've just lost a survivor..."
					m_SurvivorPickupState++;
				}
				m_SurvivorDied = 1;
				Object_Survivors[m_SurvivorIndex1] = Object_Dropship;	//set the dead handle to the dropship to stop it from being counted again
			}
			else if (IsOdf(Object_Survivors[m_SurvivorIndex1], "ispilo")){
				Handle h = GetNearestVehicle(Object_Survivors[m_SurvivorIndex1]);
				if (!IsPlayer(h) && 
					IsOdf(h, "ivtank") && 
					GetDistance(Object_Survivors[m_SurvivorIndex1], h) < 24){
					//tank picks up the pilot
					m_SabresRemaining++;
					RemoveObject(Object_Survivors[m_SurvivorIndex1]);
					Object_Survivors[m_SurvivorIndex1] = ReplaceObject(h, "ivtank_e01");
					AudioMessage("ivtank03.wav");	//Tank:"I've got 'im"
					sprintf_s(buf, "Has survivor %i", m_SurvivorIndex1 + 1);
					SetObjectiveName(Object_Survivors[m_SurvivorIndex1], buf);
					SetObjectiveOn(Object_Survivors[m_SurvivorIndex1]);
					SetGroup(Object_Survivors[m_SurvivorIndex1], 9);
					m_SurvivorPickupState = 3;
					m_SurvivorPickupTimer = GetTime() + 1;
				}
				else{
					m_SurvivorPickupState++;
				}
			}
			else{
				m_SurvivorPickupState++;
			}
			break;
		case 2:
			m_SurvivorIndex1++;
			if (m_SurvivorIndex1 == NUM_SURVIVORS){
				m_SurvivorIndex1 = 0;
			}
			m_SurvivorPickupState--;
			break;
		case 3:
			Follow(Object_Survivors[m_SurvivorIndex1], GetPlayerHandle(), 0);
			m_SurvivorPickupState--;
			break;
		case 99:	//LOC_335
			ClearObjectives();
			AddObjective(_Text7, RED);
			AudioMessage("edf01_07.wav");	//Stewart:"You just let another survivor die. It was Hardin..."
			FailMission(GetTime() + 15, "edf01L2.txt");
			m_SurvivorPickupState++;
			break;
		case 199:
			ClearObjectives();
			AddObjective(_Text11, GREEN);
			AudioMessage("edf01_06.wav");	//Stewart:"Good work Corber. You may be surprised to hear Gen. Hardin was among the survivors..."
			SucceedMission(GetTime() + 15, "edf01W2.txt");
			m_SurvivorPickupState++;
			break;
		}
	}
}

/*
Code for handling survivors being dropped off at SafeNav
*/
void edf01::HandleSurvivorDropoff(void){	//routine6
	if (m_HandleSurvivorDropoff && m_SurvivorDropoffTimer < GetTime()){
		switch (m_SurvivorDropoffState){
		case 0:
			Handle h = Object_Survivors[m_SurvivorIndex2];
			if (IsOdf(h, "ivtank_e01") && GetDistance(h, "SafeNav") < 85){
				m_SabresRemaining++;
				Object_Survivors[m_SurvivorIndex2] = Object_Dropship;	//so survivor doesnt 'die' if tank gets destroyed later
				Handle tank = ReplaceObject(h, "ivtank");
				Stop(tank, 0);
				SetGroup(tank, 0);
				m_SurvivorsDroppedOff++;
				sprintf_s(buf, "Survivor Dropoff: %i safe", m_SurvivorsDroppedOff);
				SetObjectiveName(Object_SafeNav, buf);
				if ((m_SurvivorsDroppedOff == 3 || m_SurvivorsDroppedOff == 7) || (m_SurvivorsDroppedOff == 9 && m_SurvivorDied == 0)){	//LOC_355
					//Hadeans that go after the dropships at the rescue zone
					Handle h = BuildObject("evtank", 5, "AttackerSpawn2");
					Goto(h, "SafeNav", 0);
					h = BuildObject("evmisl", 5, "AttackerSpawn2");
					Goto(h, "SafeNav", 0);
					h = BuildObject("evmisl", 5, "AttackerSpawn2");
					Goto(h, "SafeNav", 0);
				}
				else if (m_SurvivorsDroppedOff == 9){	//LOC_365
					//win - 1 survivor died
					ClearObjectives();
					AddObjective(_Text11, GREEN);
					AudioMessage("edf01_06.wav");
					SucceedMission(GetTime() + 15, "edf01W2.txt");
					m_HandleSurvivorDropoff = false;
				}
				else if (m_SurvivorsDroppedOff == 10){	//LOC_372
					//win - no survivors died
					AudioMessage("edf01_06.wav");	//Stewart:"Good work Corber. You may be surprised to hear Gen. Hardin was among the survivors..."
					ClearObjectives();
					AddObjective(_Text9, GREEN);
					SucceedMission(GetTime() + 15, "edf01W1.txt");
				}
				//m_SurvivorDropoffState++;
			}
			else{
				m_SurvivorIndex2++;
				if (m_SurvivorIndex2 == NUM_SURVIVORS){
					m_SurvivorIndex2 = 0;
				}
			}
			break;
		}
	}
}

/*
Code for spawning in Hadean reinforcements every so often
*/
void edf01::SpawnHadeanReinforcements(void){
	Handle h;
	if (m_SpawnHadeanReinforcements && m_HadeanReinforcementTimer < GetTime()){
		switch (m_HadeanReinforcementState){
		case 0:	//LOC_72
			m_HadeanReinforcementState++;
			m_HadeanReinforcementTimer = GetTime() + 61;
			break;
		case 1:
			h = BuildObject("evscout", 5, "AttackerSpawn1");
			Patrol(h, "EnemyPatrol1", 0);
			h = BuildObject("evscout", 5, "AttackerSpawn1");
			Patrol(h, "EnemyPatrol1", 0);
			m_HadeanReinforcementState++;
			m_HadeanReinforcementTimer = GetTime() + 67;
			break;
		case 2:
			h = BuildObject("evtank", 5, "AttackerSpawn1");
			Patrol(h, "EnemyPatrol1", 0);
			m_HadeanReinforcementState++;
			m_HadeanReinforcementTimer = GetTime() + 61;
			break;
		case 3:
			h = BuildObject("evtank", 5, "AttackerSpawn1");
			Patrol(h, "EnemyPatrol1", 0);
			m_HadeanReinforcementState++;
			break;
		case 4:	//LOC_83
			if (m_SurvivorsDroppedOff > 0){
				m_HadeanReinforcementState = 7;
			}
			else{
				m_HadeanReinforcementState++;
				m_HadeanReinforcementTimer = GetTime() + 7;
			}
			break;
		case 5:
			h = BuildObject("evscout", 5, "AttackerSpawn2");
			Patrol(h, "EnemyPatrol1", 0);
			h = BuildObject("evscout", 5, "AttackerSpawn2");
			Patrol(h, "EnemyPatrol1", 0);
			if (m_SurvivorsDroppedOff > 0){
				m_HadeanReinforcementState = 7;
			}
			else{
				m_HadeanReinforcementState++;
				m_HadeanReinforcementTimer = GetTime() + 95;
			}
			break;
		case 6:
			h = BuildObject("evtank", 5, "AttackerSpawn2");
			Patrol(h, "EnemyPatrol1", 0);
			if (m_SurvivorsDroppedOff > 0){
				m_HadeanReinforcementState++;
			}
			else{
				m_HadeanReinforcementState = 4;
			}
			break;
		case 7:	//LOC_101
			m_HadeanReinforcementState++;
			m_HadeanReinforcementTimer = GetTime() + 75;
			break;
		case 8:
			h = BuildObject("evscout", 5, "AttackerSpawn2");
			Patrol(h, "EnemyPatrol1", 0);
			h = BuildObject("evscout", 5, "AttackerSpawn2");
			Patrol(h, "EnemyPatrol1", 0);
			m_HadeanReinforcementState++;
			m_HadeanReinforcementTimer = GetTime() + 95;
			break;
		case 9:
			h = BuildObject("evtank", 5, "AttackerSpawn2");
			Patrol(h, "EnemyPatrol1", 0);
			m_HadeanReinforcementState++;
			m_HadeanReinforcementTimer = GetTime() + 75;
			break;
		case 10:
			h = BuildObject("evtank", 5, "AttackerSpawn2");
			Patrol(h, "EnemyPatrol1", 0);
			m_HadeanReinforcementState++;
			m_HadeanReinforcementTimer = GetTime() + 75;
			break;
		case 11:
			h = BuildObject("evscout", 5, "AttackerSpawn2");
			Patrol(h, "EnemyPatrol1", 0);
			h = BuildObject("evscout", 5, "AttackerSpawn2");
			Patrol(h, "EnemyPatrol1", 0);
			m_HadeanReinforcementState++;
			m_HadeanReinforcementTimer = GetTime() + 95;
			break;
		case 12:
			h = BuildObject("evtank", 5, "TurretSpawn");
			Patrol(h, "EnemyPatrol1", 0);
			m_HadeanReinforcementState++;
			m_HadeanReinforcementTimer = GetTime() + 75;
			break;
		case 13:
			h = BuildObject("evtank", 5, "TurretSpawn");
			Patrol(h, "EnemyPatrol1", 0);
			if (m_HadeanReinforcementCount < 15){
				m_HadeanReinforcementState = 7;
			}
			else{
				m_HadeanReinforcementState = 7;
				m_HadeanReinforcementTimer = GetTime() + 140;
			}
			break;
		}
	}
}

void edf01::CheckHadeanOverrun(){
	if (m_TooManyHadeans && m_SurvivorsWaiting >= 1 && m_FailTimer1 < GetTime()){
		switch (m_FailState1){
		case 0:
			ClearObjectives();
			AddObjective(_Text8, RED);
			AudioMessage("edf01_08.wav");	//Stewart:"You've taken too long! The Hadeans have control over the area." 
			m_FailTimer1 = GetTime() + 7;
			m_FailState1++;
			break;
		case 1:
			AudioMessage("edf01_10.wav");	//Stewart:"You've really bungled this one..."
			FailMission(GetTime() + 14, "edf01L3.txt");
			m_FailState1++;
			break;
		}
	}
}

//routine7: counts # of pilots and places a marker on one of them
//indicating how many still need to be rescued
void edf01::HandleSurvivorBeacons(){
	if (m_HandleSurvivorBeacons && m_SurvivorBeaconTimer < GetTime()){
		m_SurvivorsWaiting = 0;
		Handle pilot = NULL;
		for (int i = 0; i < NUM_SURVIVORS; i++){
			Handle h = Object_Survivors[i];
			if (IsOdf(h, "ispilo")){
				m_SurvivorsWaiting++;
				pilot = h;
			}
		}
		if (m_SurvivorsWaiting == 0){
			m_HandleSurvivorBeacons = false;
				
		}
		else if (m_SurvivorsWaiting == 1){
			SetObjectiveName(pilot, "Last Waiting Survivor");
			SetObjectiveOn(pilot);
		}
		else{
			sprintf_s(buf, "%i Survivors Waiting", m_SurvivorsWaiting);
			SetObjectiveName(pilot, buf);
			SetObjectiveOn(pilot);
		}
		m_SurvivorBeaconTimer = GetTime() + 2;
	}
}

//routine8
void edf01::CheckTanksRemaining(void){
	if (m_CheckRemainingTanks && m_SabresRemaining <= 1){
		if (m_FailTimer2 < GetTime()){
			switch (m_FailState2){
			case 0:
				ClearObjectives();
				AddObjective(_Text6, RED);
				AudioMessage("edf01_09.wav");	//Stewart:"Return to base, we've lost too many Sabres"
				m_FailTimer2 = GetTime() + 9;
				m_FailState2++;
				break;
			case 1:
				AudioMessage("edf01_10.wav");	//Stewart:"You've really bungled this one..."
				FailMission(GetTime() + 14, "edf01L1.txt");
				m_FailState2++;
				break;
			}
		}
	}
}

//routine9
void edf01::ShowPickupObjective(void){
	if (m_ShowObjective && m_ShowObjectiveTimer < GetTime()){
		switch (m_ShowObjectiveState){
		case 0:
			m_ShowObjectiveState++;
			m_ShowObjectiveTimer = GetTime() + 15;
			break;
		case 1:
			ClearObjectives();
			AddObjective(_Text3, WHITE);
			AddObjective(_Text4, WHITE);
			m_ShowObjectiveState = 0;
			m_ShowObjective = false;
			break;
		}
	}
	
}

/*
 Helper function to swap out an object for a different one with the same position and health.
 Used for replacing ivtank with ivtank_e01 when a tank has picked up a survivor
*/
Handle edf01::ReplaceObject(Handle h, char* newOdf){
	Matrix position;
	int team = GetTeamNum(h);
	long health = GetCurHealth(h);
	GetPosition(h, position);
	RemoveObject(h);
	Handle newGuy = BuildObject(newOdf, team, position);
	SetCurHealth(h, health);

	return newGuy;
}