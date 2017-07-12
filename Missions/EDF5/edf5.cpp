#include "..\Shared\SPMission.h"
#include <math.h>

#define NUM_DEFENDERS 10
#define NUM_PILOTS 8
#define NUM_EMPTIES 6

class edf5 : public SPMission 
{
public:
	edf5(void)
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
	void OnMissionStart(void);
	void HandleMainState(void);
	void HandleRecyRetreat(void);
	void HandleIntroBattle(void);
	void SpawnAttackers(void);
	void HadeanAlarm(void);
	void SpawnBirds(void);
	void HandlePortal(void);
	void CheckStuffIsAlive(void);
	Handle TeleportIn(char *odf, int team, Handle dest, float offset);
	void Teleport(Handle h, Handle dest, float offset);
	float VectorDistance(Vector v1, Vector v2);
	float VectorLength(Vector v);
	Vector VectorMul(Vector v, float factor);
	Vector VectorAdd(Vector v1, Vector v2);
	Vector VectorSub(Vector v1, Vector v2);

	bool
		b_first,
		m_SetupDone,
		m_MissionFailed,
		m_DoHadeanAlarm,
		m_RecyRetreat,
		b_last;

	float
		f_first,
		m_MissionTimer,
		m_SpawnAttackerTimer,
		m_RecyRetreatTimer,
		m_IntroTimer,
		f_last;

	Handle
		h_first,
		m_Recycler,	//recycler
		m_Player,	//player
		m_ServiceTruck1,	//service truck
		m_ServiceTruck2,	//service truck
		m_Transport, //transport
		m_Pilots[NUM_PILOTS],
		m_Portal1,
		m_Portal2,
		m_ExitPortal,
		m_Defenders[NUM_DEFENDERS],	//edf units on plateau 
		m_ServiceTruck3,
		m_ServiceTruck4,
		m_HadeanTrain,	//doesn't seem to be set. Was probably intended to be the Hadean believers vat by the repair depot
		m_EmptyShips[NUM_EMPTIES],	//empty ships at Hadean supply outpost
		m_HadTurr1,
		m_HadTurr2,
		m_HangGliderPickup,
		h_last;

	int
		i_first,
		m_TPS,
		m_MissionState,
		m_SpawnAttackerState,
		m_RecyRetreatState,
		m_IntroState,
		Variable2,	//recycler state
		Variable3,	//recy teleported out
		Variable4,	//transport teleported out
		Variable5,	//transport reached "basedeploy"
		Variable6,	//recy rendezvous timer
		Variable7,	//player was left behind
		Variable8,	//recy went through 1st portal
		m_PilotIndex,	//for edf pilots capturing hadean ships
		i_last;

	char
		*_Text1 = "Stay here and buy time for the\nRecycler to reach safety. Do NOT\nfollow yet; the canyon is carpeted \nwith high-power trip mines.",
		*_Text2 = "The Recycler has found a short-\ndistance Portal that will buy us a\nlead. Wyndt-Essex has ordered it\nto be destroyed after we pass\nthrough.",
		*_Text3 = "The Major has allowed just one\nminute before detonation. You must\nget to the Portal NOW or find\nanother way to catch up with us.",
		*_Text4 = "We're passing through enemy lines\nnow. Defend the VENGEANCE and the\ntransport at all costs.",
		*_Text5 = "We've found a Hadean repair yard. \nLeave your tank and grab the\nglider pack. Then make a stealthy\napproach and snipe the depot's\nturrets.",
		*_Text6 = "We are dispatching the transport\nwith enough pilots to take control\nof the remaining ships.",
		*_Text7 = "Use the service trucks to restore\nthe ships to full strength.",
		*_Text8 = "The StarPortal lies just a few\nklicks to the east. Let's get\nmoving.",
		*_Text9 = "MASSIVE INCOMING ATTACK WAVE:\nMULTIPLE HADEAN WALKERS, NUMEROUS\nASSAULT VEHICLES...Lock and load,\ntroops--protect the VENGEANCE at \nall costs.",
		*_Text10 = "Find another way to rejoin the\nconvoy. Hurry--the VENGEANCE\nwon't last long without an\nescort.";
	
	char buf[256];	//scratch buffer for sprintf
};
DLLBase * BuildMission(void)
{
	return new edf5();
}

void edf5::Setup(void)
{	
	//set all variables to 0/false
	memset(i_array, 0, i_count*sizeof(int));
	memset(b_array, 0, b_count*sizeof(bool));
	memset(h_array, 0, h_count*sizeof(Handle));
	memset(f_array, 0, f_count*sizeof(float));
	
	SetAutoGroupUnits(false);
	EnableHighTPS(m_TPS);

	//Preload ODFs to reduce lag spikes when things are spawned in for the first time
	PreloadODF("ivrecy");
	PreloadODF("edf5trans");
	PreloadODF("ispilo");
	PreloadODF("evturr");
	PreloadODF("evscout");
	PreloadODF("evtank");
	PreloadODF("evmisl");
	PreloadODF("evmort");
	PreloadODF("ivewalk");
	PreloadODF("espilo");
	PreloadODF("cvtank");
	PreloadODF("ptera01");
	PreloadODF("ptera02");
	PreloadODF("ptera03");
	PreloadODF("slagb2");
	PreloadODF("deathbomb");
	PreloadODF("aphanglider");
	PreloadODF("teleportin");
	PreloadODF("teleportout");
	PreloadAudioMessage("edf0501.wav");
	PreloadAudioMessage("edf0502.wav");
	PreloadAudioMessage("edf0503.wav");
	PreloadAudioMessage("edf0504.wav");
	PreloadAudioMessage("edf0505.wav");
	PreloadAudioMessage("edf0506.wav");
	PreloadAudioMessage("edf0508.wav");
	PreloadAudioMessage("edf0509.wav");
	PreloadAudioMessage("edf0510.wav");
	PreloadAudioMessage("edf05end.wav");

	m_Portal1 = GetHandle("portal1");
	m_Portal2 = GetHandle("portal2");
	m_ExitPortal = GetHandle("exitportal");
	//for (int i = 0; i < NUM_DEFENDERS; i++){
	//	sprintf_s(buf, "defender%d", i+1);
	//	m_Defenders[i] = GetHandle(buf);
	//}
	//m_HadeanTrain = GetHandle("hadeantrain");
}

void edf5::AddObject(Handle h)
{

}

void edf5::Execute(void)
{
	m_Player = GetPlayerHandle();
	
	if (!m_SetupDone){
		OnMissionStart();
		SpawnBirds();
		m_SetupDone = true;
	}

	HandleMainState();
	HandleIntroBattle();
	SpawnAttackers();
	HandlePortal();

	if (m_RecyRetreat){
		HandleRecyRetreat();
	}
	if (m_DoHadeanAlarm){
		HadeanAlarm();
	}

	CheckStuffIsAlive();
}

void edf5::OnMissionStart(void){
	m_Recycler = BuildObject("ivrecy", 1, "recyclerspawn");
	Goto(m_Recycler, "recyclerpath", 1);
	SetObjectiveOn(m_Recycler);
	m_Transport = BuildObject("edf5trans", 1, "recyclerspawn");
	SetObjectiveName(m_Transport, "Transport");
	Goto(m_Transport, "recyclerpath", 1);
	m_ServiceTruck1 = BuildObject("ivserv", 1, "recyclerspawn");
	Defend2(m_ServiceTruck1, m_Transport, 1);
	m_ServiceTruck2 = BuildObject("ivserv", 1, "recyclerspawn");
	Defend2(m_ServiceTruck2, m_Transport, 1);
	m_ServiceTruck3 = BuildObject("ivserv", 1, "turret1");
	SetGroup(m_ServiceTruck3, 5);
	m_ServiceTruck4 = BuildObject("ivserv", 1, "turret1");
	SetGroup(m_ServiceTruck4, 5);
	m_Defenders[0] = BuildObject("ivturr", 1, "turret1");
	SetGroup(m_Defenders[0], 0);
	m_Defenders[1] = BuildObject("ivturr", 1, "turret2");
	SetGroup(m_Defenders[1], 0);
	m_Defenders[2] = BuildObject("ivscout", 1, "turret3");
	SetGroup(m_Defenders[2], 4);
	m_Defenders[3] = BuildObject("ivscout", 1, "turret4");
	SetGroup(m_Defenders[3], 4);
	m_Defenders[4] = BuildObject("ivtank", 1, "tank1");
	SetGroup(m_Defenders[4], 1);
	m_Defenders[5] = BuildObject("ivtank", 1, "tank2");
	SetGroup(m_Defenders[5], 1);
	m_Defenders[6] = BuildObject("ivmisl", 1, "tank3");
	SetGroup(m_Defenders[6], 2);
	m_Defenders[7] = BuildObject("ivmisl", 1, "tank4");
	SetGroup(m_Defenders[7], 2);
	m_Defenders[8] = BuildObject("ivrbomb", 1, "tank5");
	SetGroup(m_Defenders[8], 3);
	m_Defenders[9] = BuildObject("ivrbomb", 1, "tank6");
	SetGroup(m_Defenders[9], 3);
}

void edf5::HandleMainState(void){
	if (m_MissionTimer < GetTime()){
		switch (m_MissionState){
		case 0:
			m_MissionState++;
			m_MissionTimer = GetTime() + 5;
			break;
		case 1:
			AudioMessage("edf0501.wav");	//Windex:"The Hadeans have us surrounded..."
			ClearObjectives();
			AddObjective(_Text1, WHITE);
			m_MissionState++;
			m_MissionTimer = GetTime() + 3;
			break;
		case 2:	//LOC_20
			if (Variable2){
				m_MissionState++;
			}
			break;
		case 3:
			if (Variable5 == 1){
				m_MissionState++;
				m_MissionTimer = GetTime() + 15;
			}
			break;
		case 4:
			AudioMessage("edf0505.wav");	//O'Ryan:"We should be safe up here for a while..."
			ClearObjectives();
			AddObjective(_Text5, WHITE);
			m_HangGliderPickup = BuildObject("aphanglider", 1, "gliderspawn");
			Goto(m_Transport, "apcpathtobase", 1);
			SetObjectiveOn(m_HangGliderPickup);
			m_DoHadeanAlarm = true;
			m_EmptyShips[0] = BuildObject("evscout", 0, "hadeanscout1");
			KillPilot(m_EmptyShips[0]);
			m_EmptyShips[1] = BuildObject("evscout", 0, "hadeanscout2");
			KillPilot(m_EmptyShips[1]);
			m_EmptyShips[2] = BuildObject("evtank", 0, "hadeantank1");
			KillPilot(m_EmptyShips[2]);
			m_EmptyShips[3] = BuildObject("evtank", 0, "hadeantank2");
			KillPilot(m_EmptyShips[3]);
			m_EmptyShips[4] = BuildObject("evmisl", 0, "hadeanzeus1");
			KillPilot(m_EmptyShips[4]);
			m_EmptyShips[5] = BuildObject("evmisl", 0, "hadeanzeus2");
			KillPilot(m_EmptyShips[5]);
			m_HadTurr1 = GetHandle("hadeanturret1");
			SetObjectiveOn(m_HadTurr1);
			m_HadTurr2 = GetHandle("hadeanturret2");
			SetObjectiveOn(m_HadTurr2);
			m_MissionState++;
			break;
		case 5:	//LOC_48
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
			if (GetDistance(m_Transport, "apcdeploy") < 100){
				m_Pilots[m_PilotIndex] = BuildObject("ispilo", 1, "apcdeploy");
				Goto(m_Pilots[m_PilotIndex], m_EmptyShips[m_PilotIndex], 1);
				m_PilotIndex++;
				m_MissionState++;
				m_MissionTimer = GetTime() + 1;
			}
			break;
		case 11:
			m_Pilots[6] = BuildObject("ispilo", 1, "apcdeploy");
			Goto(m_Pilots[6], m_HadTurr1, 1);
			m_MissionState++;
			m_MissionTimer = GetTime() + 1;
			break;
		case 12:
			m_Pilots[7] = BuildObject("ispilo", 1, "apcdeploy");
			Goto(m_Pilots[7], m_HadTurr2, 1);
			m_MissionState++;
			m_MissionTimer = GetTime() + 1;
			break;
		case 13:
			SetObjectiveOff(m_HadTurr1);
			SetObjectiveOff(m_HadTurr2);
			SetObjectiveOn(m_ExitPortal);
			AudioMessage("edf0508.wav");	//Windex:"There's the portal up ahead on radar..."
			ClearObjectives();
			AddObjective(_Text8, WHITE);
			m_MissionState++;
			m_MissionTimer = GetTime() + 60;
			break;
		case 14:
			Goto(m_Recycler, "recyclerexit", 1);
			Follow(m_Transport, m_Recycler, 1);
			m_MissionState++;
			break;
		case 15:	//LOC_83
			if (GetDistance(m_Recycler, m_ExitPortal) < 25){
				Variable3 = 1;
				Teleport(m_Recycler, m_Portal2, 10);
				RemoveObject(m_Recycler);
				Goto(m_Transport, "recyclerexit", 1);
				AudioMessage("edf05end.wav");	//Shultz:"Aw man, what are we getting into?"
				Attack(TeleportIn("cvtank", 2, m_ExitPortal, 10), m_Player, 1);
				m_MissionState++;
			}
			break;
		case 16:	//LOC_93
			if (GetDistance(m_Transport, m_ExitPortal) < 50){
				Variable4 = 1;
				Teleport(m_Transport, m_Portal2, 10);
				RemoveObject(m_Transport);
				Attack(TeleportIn("cvtank", 2, m_ExitPortal, 10), m_Player, 1);
				m_MissionState++;
			}
			break;
		case 17:	//LOC_102
			if (GetDistance(m_Player, m_ExitPortal) < 25){
				Teleport(m_Player, m_Portal2, 10);
				SucceedMission(GetTime() + 1, "edf5win.des");
				m_MissionState++;
			}
			break;
		}
	}
}

void edf5::HandleRecyRetreat(void){
	if (m_RecyRetreatTimer < GetTime()){
		switch (m_RecyRetreatState){
		case 0:
			m_RecyRetreatState++;
			m_RecyRetreatTimer = GetTime() + 10;
			break;
		case 1:
			AudioMessage("edf0503.wav");	//Windex:"My engineers have deactivated the heavy mines..."
			ClearObjectives();
			AddObjective(_Text3, WHITE);
			for (int i = 0; i < NUM_DEFENDERS; i++){
				if (!IsPlayer(m_Defenders[i])){
					Follow(m_Defenders[i], m_Transport, 1);
				}
			}
			StartCockpitTimer(60);
			m_RecyRetreatState++;
			m_RecyRetreatTimer = GetTime() + 60;
			break;
		case 2:
			BuildObject("slagb2", 2, "blockade");
			RemoveObject(m_Portal1);
			m_RecyRetreatState++;
			m_RecyRetreatTimer = GetTime() + 3;
			break;
		case 3:
			HideCockpitTimer();
			if (Variable2 == 3){
				m_RecyRetreatState = 5;	//LOC_265
			}
			else{
				Variable7 = 1;
				//audio doesn't make sense here?
				AudioMessage("edf0510.wav");	//Windex:"Corporal, we don't have a few minutes..."
				ClearObjectives();
				AddObjective(_Text10, RED);
				m_RecyRetreatState++;
				m_RecyRetreatTimer = GetTime() + 60;
			}
			break;
		case 4:
			Attack(BuildObject("evscout", 2, "attackerspawn2"), m_Recycler, 1);
			Attack(BuildObject("evscout", 2, "attackerspawn2"), m_Transport, 1);
			m_RecyRetreatState++;
			break;
		case 5:	//LOC_265
			Variable7 = 0;
			Variable2 = 2;
			TeleportIn("evscout", 2, m_Portal2, 10);
			m_RecyRetreatState++;
			break;
		case 6:	//LOC_269
			if (Variable8){
				m_RecyRetreatState++;
				m_RecyRetreatTimer = GetTime() + 30;
			}
			break;
		case 7:
		case 8:
			Attack(BuildObject("evscout", 2, "attackerspawn2"), m_Recycler, 1);
			m_RecyRetreatState++;
			m_RecyRetreatTimer = GetTime() + 45;
			break;
		case 9:
			Attack(BuildObject("evscout", 2, "attackerspawn2"), m_Recycler, 1);
			m_RecyRetreatState++;
			m_RecyRetreatTimer = GetTime() + 120;
			break;
		case 10:
			Attack(BuildObject("evscout", 2, "attackerspawn2"), m_Recycler, 1);
			m_RecyRetreatState++;
			break;
		}
	}
}

void edf5::HandleIntroBattle(void){
	if (m_IntroTimer < GetTime()){
		switch (m_IntroState){
		case 0:	//LOC_142
			if (GetDistance(m_Recycler, m_Portal1) < 25){
				AudioMessage("edf0502.wav");	//Windex:"We found a short range portal..."
				ClearObjectives();
				AddObjective(_Text2, WHITE);
				Variable2 = 1;
				Teleport(m_Recycler, m_Portal2, 10);
				Stop(m_Recycler, 1);
				Goto(m_Transport, "blockade", 1);
				m_IntroState++;
			}
			break;
		case 1:	//LOC_151
			if (GetDistance(m_Transport, "blockade") < 50){
				Teleport(m_Transport, m_Portal2, 10);
				Stop(m_Transport, 1);
				m_RecyRetreat = true;
				Goto(m_ServiceTruck1, "blockade", 1);
				Goto(m_ServiceTruck2, "blockade", 1);
				m_IntroState++;
			}
			break;
		case 2:	//LOC_158
			if (GetDistance(m_ServiceTruck1, "blockade") < 25 || 
				GetDistance(m_ServiceTruck2, "blockade") < 25){
				Teleport(m_ServiceTruck1, m_Portal2, 10);
				Defend2(m_ServiceTruck1, m_Transport, 1);
				Teleport(m_ServiceTruck2, m_Portal2, 10);
				Defend2(m_ServiceTruck2, m_Transport, 1);
				m_IntroState++;
			}
			break;
		case 3:	//LOC_171
			Variable6++;
			if (Variable6 > 300 || GetDistance(m_Recycler, m_Player) < 150){
				m_IntroState++;
			}
			else {
				m_IntroTimer = GetTime() + 1;
			}			
			break;
		case 4:	//LOC_176
			SetGroup(m_ServiceTruck1, 0);
			SetGroup(m_ServiceTruck2, 0);
			Goto(m_Recycler, "recyclerpath1", 1);
			Goto(m_Transport, "recyclerpath1", 1);
			Defend2(m_ServiceTruck1, m_Transport, 0);
			Defend2(m_ServiceTruck2, m_Transport, 0);
			Variable8 = 1;
			m_IntroState++;
			m_IntroTimer = GetTime() + 60;
			break;
		case 5:	//LOC_184
			if (GetDistance(m_Recycler, "basedeploy") < 25){
				Variable2 = 4;
				m_IntroState++;
			}
			break;
		case 6:	//LOC_187
			if (GetDistance(m_Player, m_Recycler) < 150){
				m_IntroState++;
			}
			break;
		case 7:	//LOC_190
			if (GetDistance(m_Transport, "basedeploy") < 200){
				Variable5 = 1;
				m_IntroState++;
			}
			break;
		}
	}
}

void edf5::SpawnAttackers(void){	//Routine5
	if (m_SpawnAttackerTimer < GetTime()){
		switch (m_SpawnAttackerState){
		case 0:
			m_SpawnAttackerState++;
			m_SpawnAttackerTimer = GetTime() + 30;
			break;
		case 1:
			//m_Player = GetPlayerHandle();
			AddObjective(_Text9, WHITE);
			Goto(BuildObject("evatank", 2, "attackerspawn"), "tankpath", 1);
			Goto(BuildObject("evscout", 2, "attackerspawn"), "zeuspath", 1);
			Goto(BuildObject("evscout", 2, "attackerspawn"), "walkpath", 1);
			Goto(BuildObject("evscout", 2, "attackerspawn"), "mortpath", 1);
			Goto(BuildObject("ivewalk", 2, "attackerspawn"), "walkpath", 1);
			m_SpawnAttackerState++;
			m_SpawnAttackerTimer = GetTime() + 30;
			break;
		case 2:
			Goto(BuildObject("evscout", 2, "attackerspawn"), "tankpath", 1);
			Goto(BuildObject("evscout", 2, "attackerspawn"), "zeuspath", 1);
			Goto(BuildObject("evscout", 2, "attackerspawn"), "walkpath", 1);
			Goto(BuildObject("evtank", 2, "attackerspawn"), "mortpath", 1);
			Goto(BuildObject("ivewalk", 2, "attackerspawn"), "walkpath", 1);
			Goto(BuildObject("evscout", 2, "attackerspawn"), "tankpath", 1);
			m_SpawnAttackerState++;
			m_SpawnAttackerTimer = GetTime() + 30;
			break;
		case 3:
			Goto(BuildObject("evtank", 2, "attackerspawn"), "tankpath", 1);
			Goto(BuildObject("evtank", 2, "attackerspawn"), "tankpath", 1);
			Goto(BuildObject("evmisl", 2, "attackerspawn"), "zeuspath", 1);
			Goto(BuildObject("evmisl", 2, "attackerspawn"), "zeuspath", 1);
			m_SpawnAttackerState++;
			m_SpawnAttackerTimer = GetTime() + 30;
			break;
		case 4:
			Goto(BuildObject("evwalk", 2, "attackerspawn"), "walkpath", 1);
			Goto(BuildObject("evmisl", 2, "attackerspawn"), "zeuspath", 1);
			Goto(BuildObject("evmisl", 2, "attackerspawn"), "zeuspath", 1);
			Goto(BuildObject("evscout", 2, "attackerspawn"), "tankpath", 1);
			Goto(BuildObject("evmort", 2, "attackerspawn"), "mortpath", 1);
			Goto(BuildObject("evmort", 2, "attackerspawn"), "mortpath", 1);
			Goto(BuildObject("evmort", 2, "attackerspawn"), "mortpath", 1);
			Goto(BuildObject("evmort", 2, "attackerspawn"), "mortpath", 1);
			m_SpawnAttackerState++;
			break;
		}
	}
}

//m_HadeanTrain is never set!?
void edf5::HadeanAlarm(void){	//Routine6
	if (m_DoHadeanAlarm && IsAround(m_HadeanTrain)){
		m_Player = GetPlayerHandle();
		if (IsOdf(m_Player, "ispilo") && GetDistance(m_Player, m_HadeanTrain) < 400){
			//AudioMessage("edf05alarm.wav");	//doesn't exist!
			Vector pos = GetPosition(m_HadeanTrain);
			for (int i = 0; i < NUM_EMPTIES; i++){
				Goto(BuildObject("ESPILO", 2, pos), m_EmptyShips[i], 2);
			}
			m_DoHadeanAlarm = false;
		}
	}
}

void edf5::SpawnBirds(void){
	char *odfs[24] = { 
		"ptera01", 
		"ptera01", 
		"ptera01", 
		"ptera02", 
		"ptera02", 
		"ptera02", 
		"ptera03", 
		"ptera03", 
		"ptera03", 
		"ptera03", 
		"ptera03", 
		"ptera01", 
		"ptera01",
		"ptera01", 
		"ptera02", 
		"ptera02", 
		"ptera02", 
		"ptera03", 
		"ptera01", 
		"ptera01", 
		"ptera01", 
		"ptera02", 
		"ptera02", 
		"ptera01" };
	for (int i = 0; i < 8; i++){
		sprintf_s(buf, "birdpath%d", i);
		for (int j = 0; j < 3; j++){
			BuildObject(odfs[i * 3 + j], 0, buf);
		}
	}
}

void edf5::HandlePortal(void){
	if (GetDistance(m_Player, m_Portal1) < 25){
		if (Variable2 == 0 && !m_MissionFailed){
			FailMission(GetTime() + 5, "edf5tele.txt");
			m_MissionFailed = true;
		}
		else if(Variable2 == 1){
			Teleport(m_Player, m_Portal2, 10);
			Variable2 = 3;
		}
	}
}

void edf5::CheckStuffIsAlive(void){
	if (!m_MissionFailed){
		if (!IsAround(m_Recycler) && !Variable3){
			FailMission(GetTime() + 5, "edf5recy.txt");
			m_MissionFailed = true;
		}
		else if (!IsAround(m_Transport) && !Variable4){
			FailMission(GetTime() + 5, "edf5trans.txt");
			m_MissionFailed = true;
		}
		else if (GetDistance(m_Player, "pointofnoreturn") < 100 && !Variable2){
			BuildObject("deathbomb", 2, "pointofnoreturn");
			FailMission(GetTime() + 25, "edf5close.txt");
			m_MissionFailed = true;
		}
	}
}

//Teleports in a new object at <dest>, <offset> metres away.
Handle edf5::TeleportIn(char *odf, int team, Handle dest, float offset){
	Vector pos = GetPosition(dest);
	pos.x += offset;
	pos.y = TerrainFindFloor(pos.x, pos.z) + 5;
	BuildObject("teleportin", 0, pos);
	return BuildObject(odf, team, pos);
}

//Teleports h to dest with an overshoot of <offset> meters
void edf5::Teleport(Handle h, Handle dest, float offset){
	Matrix oldPos;
	GetPosition(h, oldPos);
	BuildObject("teleportout", 0, oldPos);

	Matrix m;
	GetPosition(dest, m);
	Vector v = VectorSub(GetPosition(dest), GetPosition(h));
	if (VectorLength(v) < 0.001f){
		v.x = 1;	//prevent wacky results if origin and destination are too close
	}
	Vector direction = Normalize_Vector(v);
	m.posit = VectorAdd(GetPosition(dest), VectorMul(direction, offset));
	m.posit.y = TerrainFindFloor(m.posit.x, m.posit.z) + 5.0f;
	BuildObject("teleportin", 0, m);

	SetPosition(h, m);
}

//gets the 3d distance between 2 vectors
float edf5::VectorDistance(Vector v1, Vector v2){
	float dx = (v2.x - v1.x);
	float dy = (v2.y - v1.y);
	float dz = (v2.z - v1.z);
	return sqrt(dx*dx + dy*dy + dz*dz);
}

float edf5::VectorLength(Vector v){
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

Vector edf5::VectorMul(Vector v, float factor){
	return Vector(v.x*factor, v.y*factor, v.z*factor);
}

Vector edf5::VectorAdd(Vector v1, Vector v2){
	return Vector(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vector edf5::VectorSub(Vector v1, Vector v2){
	return Vector(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}
