#include "..\Shared\SPMission.h"
#include <math.h>

#define NUM_DEFENDERS 10
#define NUM_PILOTS 8

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
		Object1,	//recycler
		Object2,	//player
		Object3,	//service truck
		Object4,	//service truck
		Object5, //transport
		Object13[NUM_PILOTS],
		Object21,	//Portal1
		Object22,	//Portal2
		Object23,	//exit portal
		Object25[NUM_DEFENDERS],	//Defender
		//Object35,	//hadean attacker
		//Object36,	//hadean attacker
		//Object37,	//hadean attacker
		//Object38,	//hadean attacker
		//Object39,	//hadean attacker
		//Object40,	//hadean attacker
		//Object41,	//hadean attacker
		//Object42,	//hadean attacker
		//Object43,	//hadean attacker
		//Object44,	//hadean attacker
		Object45,	//service truck
		Object46,	//service truck
		//Object48,	//temp handle for deathbomb
		Object49,	//doesn't seem to be set. Was probably intended to be the Hadean believers vat by the repair depot
		Object50,	//empty hadean scout 1
		Object51,	//empty hadean scout 2
		Object52,	//empty hadean tank 1
		Object53,	//empty hadean tank 2
		Object54,	//empty hadean zeus 1
		Object55,	//empty hadean zeus 2
		Object56,	//hadean turret 1
		Object57,	//hadean turret 2
		Object59,	//hang glider pickup
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

	//Vector
	//	Position1 = { 0, 0, 0 },
	//	Position2 = { 0, 0, 0 },
	//	Position3 = { 0, 0, 0 };
	
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

	Object21 = GetHandle("portal1");
	Object22 = GetHandle("portal2");
	Object23 = GetHandle("exitportal");
	//for (int i = 0; i < NUM_DEFENDERS; i++){
	//	sprintf_s(buf, "defender%d", i+1);
	//	Object25[i] = GetHandle(buf);
	//}
	Object49 = GetHandle("hadeantrain");
}

void edf5::AddObject(Handle h)
{

}

void edf5::Execute(void)
{
	Object2 = GetPlayerHandle();
	
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
	Object1 = BuildObject("ivrecy", 1, "recyclerspawn");
	Goto(Object1, "recyclerpath", 1);
	SetObjectiveOn(Object1);
	Object5 = BuildObject("edf5trans", 1, "recyclerspawn");
	SetObjectiveName(Object5, "Transport");
	Goto(Object5, "recyclerpath", 1);
	Object3 = BuildObject("ivserv", 1, "recyclerspawn");
	Defend2(Object3, Object5, 1);
	Object4 = BuildObject("ivserv", 1, "recyclerspawn");
	Defend2(Object4, Object5, 1);
	Object45 = BuildObject("ivserv", 1, "turret1");
	SetGroup(Object45, 5);
	Object46 = BuildObject("ivserv", 1, "turret1");
	SetGroup(Object46, 5);
	Object25[0] = BuildObject("ivturr", 1, "turret1");
	SetGroup(Object25[0], 0);
	Object25[1] = BuildObject("ivturr", 1, "turret2");
	SetGroup(Object25[1], 0);
	Object25[2] = BuildObject("ivscout", 1, "turret3");
	SetGroup(Object25[2], 4);
	Object25[3] = BuildObject("ivscout", 1, "turret4");
	SetGroup(Object25[3], 4);
	Object25[4] = BuildObject("ivtank", 1, "tank1");
	SetGroup(Object25[4], 1);
	Object25[5] = BuildObject("ivtank", 1, "tank2");
	SetGroup(Object25[5], 1);
	Object25[6] = BuildObject("ivmisl", 1, "tank3");
	SetGroup(Object25[6], 2);
	Object25[7] = BuildObject("ivmisl", 1, "tank4");
	SetGroup(Object25[7], 2);
	Object25[8] = BuildObject("ivrbomb", 1, "tank5");
	SetGroup(Object25[8], 3);
	Object25[9] = BuildObject("ivrbomb", 1, "tank6");
	SetGroup(Object25[9], 3);
}

void edf5::HandleMainState(void){
	if (m_MissionTimer < GetTime()){
		switch (m_MissionState){
		case 0:
			//Position1 = GetPosition(Object22);
			//Position2 = GetPosition(Object21);
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
			Object59 = BuildObject("aphanglider", 1, "gliderspawn");
			Goto(Object5, "apcpathtobase", 1);
			SetObjectiveOn(Object59);
			m_DoHadeanAlarm = true;
			Object50 = BuildObject("evscout", 0, "hadeanscout1");
			KillPilot(Object50);
			Object51 = BuildObject("evscout", 0, "hadeanscout2");
			KillPilot(Object51);
			Object52 = BuildObject("evtank", 0, "hadeantank1");
			KillPilot(Object52);
			Object53 = BuildObject("evtank", 0, "hadeantank2");
			KillPilot(Object53);
			Object54 = BuildObject("evmisl", 0, "hadeanzeus1");
			KillPilot(Object54);
			Object55 = BuildObject("evmisl", 0, "hadeanzeus2");
			KillPilot(Object55);
			Object56 = GetHandle("hadeanturret1");
			SetObjectiveOn(Object56);
			Object57 = GetHandle("hadeanturret2");
			SetObjectiveOn(Object57);
			m_MissionState++;
			break;
		case 5:	//LOC_48
			if (GetDistance(Object5, "apcdeploy") < 100){
				Object13[0] = BuildObject("ispilo", 1, "apcdeploy");
				Goto(Object13[0], Object50, 1);
				m_MissionState++;
				m_MissionTimer = GetTime() + 1;
			}
			break;
		case 6:
			Object13[1] = BuildObject("ispilo", 1, "apcdeploy");
			Goto(Object13[1], Object51, 1);
			m_MissionState++;
			m_MissionTimer = GetTime() + 1;
			break;
		case 7:
			Object13[2] = BuildObject("ispilo", 1, "apcdeploy");
			Goto(Object13[2], Object52, 1);
			m_MissionState++;
			m_MissionTimer = GetTime() + 1;
			break;
		case 8:
			Object13[3] = BuildObject("ispilo", 1, "apcdeploy");
			Goto(Object13[3], Object53, 1);
			m_MissionState++;
			m_MissionTimer = GetTime() + 1;
			break;
		case 9:
			Object13[4] = BuildObject("ispilo", 1, "apcdeploy");
			Goto(Object13[4], Object54, 1);
			m_MissionState++;
			m_MissionTimer = GetTime() + 1;
			break;
		case 10:
			Object13[5] = BuildObject("ispilo", 1, "apcdeploy");
			Goto(Object13[5], Object55, 1);
			m_MissionState++;
			m_MissionTimer = GetTime() + 1;
			break;
		case 11:
			Object13[6] = BuildObject("ispilo", 1, "apcdeploy");
			Goto(Object13[6], Object56, 1);
			m_MissionState++;
			m_MissionTimer = GetTime() + 1;
			break;
		case 12:
			Object13[7] = BuildObject("ispilo", 1, "apcdeploy");
			Goto(Object13[7], Object57, 1);
			m_MissionState++;
			m_MissionTimer = GetTime() + 1;
			break;
		case 13:
			SetObjectiveOff(Object56);
			SetObjectiveOff(Object57);
			SetObjectiveOn(Object23);
			AudioMessage("edf0508.wav");	//Windex:"There's the portal up ahead on radar..."
			ClearObjectives();
			AddObjective(_Text8, WHITE);
			m_MissionState++;
			m_MissionTimer = GetTime() + 60;
			break;
		case 14:
			Goto(Object1, "recyclerexit", 1);
			Follow(Object5, Object1, 1);
			m_MissionState++;
			break;
		case 15:	//LOC_83
			if (GetDistance(Object1, Object23) < 25){
				Variable3 = 1;
				Teleport(Object1, Object22, 10);
				RemoveObject(Object1);
				Goto(Object5, "recyclerexit", 1);
				AudioMessage("edf05end.wav");	//Shultz:"Aw man, what are we getting into?"
				Attack(TeleportIn("cvtank", 2, Object23, 10), Object2, 1);
				m_MissionState++;
			}
			break;
		case 16:	//LOC_93
			if (GetDistance(Object5, Object23) < 50){
				Variable4 = 1;
				Teleport(Object5, Object22, 10);
				RemoveObject(Object5);
				Attack(TeleportIn("cvtank", 2, Object23, 10), Object2, 1);
				m_MissionState++;
			}
			break;
		case 17:	//LOC_102
			if (GetDistance(Object2, Object23) < 25){
				Teleport(Object2, Object22, 10);
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
				if (!IsPlayer(Object25[i])){
					Follow(Object25[i], Object5, 1);
				}
			}
			StartCockpitTimer(60);
			m_RecyRetreatState++;
			m_RecyRetreatTimer = GetTime() + 60;
			break;
		case 2:
			BuildObject("slagb2", 2, "blockade");
			RemoveObject(Object21);
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
			Attack(BuildObject("evscout", 2, "attackerspawn2"), Object1, 1);
			Attack(BuildObject("evscout", 2, "attackerspawn2"), Object5, 1);
			m_RecyRetreatState++;
			break;
		case 5:	//LOC_265
			Variable7 = 0;
			Variable2 = 2;
			TeleportIn("evscout", 2, Object22, 10);
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
			Attack(BuildObject("evscout", 2, "attackerspawn2"), Object1, 1);
			m_RecyRetreatState++;
			m_RecyRetreatTimer = GetTime() + 45;
			break;
		case 9:
			Attack(BuildObject("evscout", 2, "attackerspawn2"), Object1, 1);
			m_RecyRetreatState++;
			m_RecyRetreatTimer = GetTime() + 120;
			break;
		case 10:
			Attack(BuildObject("evscout", 2, "attackerspawn2"), Object1, 1);
			m_RecyRetreatState++;
			break;
		}
	}
}

void edf5::HandleIntroBattle(void){
	if (m_IntroTimer < GetTime()){
		switch (m_IntroState){
		case 0:	//LOC_142
			if (GetDistance(Object1, Object21) < 25){
				AudioMessage("edf0502.wav");	//Windex:"We found a short range portal..."
				ClearObjectives();
				AddObjective(_Text2, WHITE);
				Variable2 = 1;
				Teleport(Object1, Object22, 10);
				Stop(Object1, 1);
				Goto(Object5, "blockade", 1);
				m_IntroState++;
			}
			break;
		case 1:	//LOC_151
			if (GetDistance(Object5, "blockade") < 50){
				Teleport(Object5, Object22, 10);
				Stop(Object5, 1);
				m_RecyRetreat = true;
				Goto(Object3, "blockade", 1);
				Goto(Object4, "blockade", 1);
				m_IntroState++;
			}
			break;
		case 2:	//LOC_158
			if (GetDistance(Object3, "blockade") < 25 || 
				GetDistance(Object4, "blockade") < 25){
				Teleport(Object3, Object22, 10);
				Defend2(Object3, Object5, 1);
				Teleport(Object4, Object22, 10);
				Defend2(Object4, Object5, 1);
				m_IntroState++;
			}
			break;
		case 3:	//LOC_171
			Variable6++;
			if (Variable6 > 300 || GetDistance(Object1, Object2) < 150){
				m_IntroState++;
			}
			else {
				m_IntroTimer = GetTime() + 1;
			}			
			break;
		case 4:	//LOC_176
			SetGroup(Object3, 0);
			SetGroup(Object4, 0);
			Goto(Object1, "recyclerpath1", 1);
			Goto(Object5, "recyclerpath1", 1);
			Defend2(Object3, Object5, 0);
			Defend2(Object4, Object5, 0);
			Variable8 = 1;
			m_IntroState++;
			m_IntroTimer = GetTime() + 60;
			break;
		case 5:	//LOC_184
			if (GetDistance(Object1, "basedeploy") < 25){
				Variable2 = 4;
				m_IntroState++;
			}
			break;
		case 6:	//LOC_187
			if (GetDistance(Object2, Object1) < 150){
				m_IntroState++;
			}
			break;
		case 7:	//LOC_190
			if (GetDistance(Object5, "basedeploy") < 200){
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
			//Object2 = GetPlayerHandle();
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

//Object49 is never set!?
void edf5::HadeanAlarm(void){	//Routine6
	if (m_DoHadeanAlarm && IsAround(Object49)){
		Object2 = GetPlayerHandle();
		if (IsOdf(Object2, "ispilo") && GetDistance(Object2, Object49) < 400){
			//AudioMessage("edf05alarm.wav");	//doesn't exist!
			Vector pos = GetPosition(Object49);
			Goto(BuildObject("ESPILO", 2, pos), Object50, 2);
			Goto(BuildObject("ESPILO", 2, pos), Object51, 2);
			Goto(BuildObject("ESPILO", 2, pos), Object52, 2);
			Goto(BuildObject("ESPILO", 2, pos), Object53, 2);
			Goto(BuildObject("ESPILO", 2, pos), Object54, 2);
			Goto(BuildObject("ESPILO", 2, pos), Object55, 2);
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
	if (GetDistance(Object2, Object21) < 25){
		if (Variable2 == 0 && !m_MissionFailed){
			FailMission(GetTime() + 5, "edf5tele.txt");
			m_MissionFailed = true;
		}
		else if(Variable2 == 1){
			Teleport(Object2, Object22, 10);
			Variable2 = 3;
		}
	}
}

void edf5::CheckStuffIsAlive(void){
	if (!m_MissionFailed){
		if (!IsAround(Object1) && !Variable3){
			FailMission(GetTime() + 5, "edf5recy.txt");
			m_MissionFailed = true;
		}
		else if (!IsAround(Object5) && !Variable4){
			FailMission(GetTime() + 5, "edf5trans.txt");
			m_MissionFailed = true;
		}
		else if (GetDistance(Object2, "pointofnoreturn") < 100 && !Variable2){
			BuildObject("deathbomb", 2, "pointofnoreturn");
			FailMission(GetTime() + 25, "edf5close.txt");
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
