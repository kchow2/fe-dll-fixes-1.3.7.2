#include "..\Shared\SPMission.h"

#define NUM_MEGATURRETS 4
#define NUM_BLOCKAGES 7

class edf3 : public SPMission 
{
public:
	edf3(void)
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
	void HandleDinos(void);
	void RemoveRocks(void);
	void HandleEngineerTransport(void);
	void TeleportPlayer(void);
	void CheckStuffIsAlive(void);

	// bools
	bool
		b_first,
		m_MissionFailed,
		m_RemoveRocks,
		m_SpawnEngTransport,
		m_TeleportPlayer,
		m_FactoryBuilt,
		m_EngineerTransportArrived,
		m_BaseDestroyed,
		b_last;

	// floats
	float
		f_first,
		m_MissionTimer,
		m_DinoTimer,
		m_RemoveRocksTime,
		m_EngTransportTimer,
		m_PlayerTeleportTimer,
		m_OldPlayerPosX,
		m_OldPlayerPosY,
		m_OldPlayerPosZ,
		f_last;

	// handles
	Handle
		h_first,
		m_Engineer,	//engineer that walks into megapower
		m_Dropship,
		m_Player,
		m_Recycler,
		m_HadeanRecy,
		m_MegaPower,
		m_HadeanScout,
		m_CutsceneNav,
		m_CanyonNav,	//canyon blockage nav
		m_MegaPowerNav,
		m_CityRuinsNav,
		m_SatchelPickupNav,
		m_EngineerTransport,
		m_Rocks[NUM_BLOCKAGES],	//canyon blockages
		m_SatchelPickup,
		m_MegaTurretVictim,
		m_BaseNav,
		m_HadeanGtow1,
		m_HadeanGtow2,
		m_HadeanSpike1,
		m_HadeanSpike2,
		m_FlightRecorder,
		m_MegaTurrets[NUM_MEGATURRETS],
		m_Unknown1,
		m_Unknown2,
		m_DeployedSatchel,
		h_last;

	// integers
	int
		i_first,
		m_TPS,
		m_MissionState,
		m_DinoState,
		m_EngTransportState,
		m_PlayerTeleportState,
		i_last;

	Vector
		Position1 = { -1170, -15, -1278 },
		Position2 = { 1133, -1, 351 },
		Position3 = { 846, 2, 1002 },
		Position4 = { -1170, -15, -1275 },
		Position5 = { -1170, -15, -1270 },
		Position6 = { -1170, -15, -1265 },
		Position7 = { -1170, -15, -1260 },
		Position8 = { -911, 76, -726 },
		Position9 = { -467, 40, -308 },
		Position10 = { -614, 7, 590 },
		Position11 = { -127, 0, 820 },
		Position12 = { -624, 59, -1056 },
		Position13 = { 1133, -1, 351 },
		Position14 = { 662, -55, -501 },
		Position15 = { -899, 76, -723 },
		Position16 = { -761, 180, -857 },
		Position17 = { -48, -6, 768 };

	char
		*_Text1 = "Escort the Recycler until it \nreaches the deployment zone on\nthe other side of the lake.",
		*_Text2 = "The object appears to be some\nkind of data storage unit. Bring\nit back to base immediately!",
		*_Text3 = "A map inside the data pod points\nto a possible back door to the\nHadean base. Get the satchel pack\nand investigate the nav point.",
		*_Text4 = "Our map indicates a ruined city\nup ahead, but defenses may still\nbe active. Cautiously head down\nthe canyon and check it out.",
		*_Text5 = "This ruined city appears to be\npre-Hadean, possibly very early\nCthonian in origin.",
		*_Text6 = "Our sensors are showing a strong\nelectromagnetic field. Head to\nthe position shown on your radar\nand investigate.",
		*_Text7 = "Use the 'I' key of your Digital\nControl Interface to scan the\nstructure.",
		*_Text8 = "This facility powers the Hadean\nMegagun Megagun. Protect it at\nall costs while you take out the\nbase defenses.",
		*_Text9 = "Protect the engineers as they\ntravel to the Megapower plant.",
		*_Text10 = "We have dispatched a squad of\nengineers to take control of the\nMegagun's power facility.",
		*_Text11 = "The engineers have taken control\nof the Megapower facility. They\nalso reprogrammed the Hadean gate\ntowers to ally with our forces.",
		*_Text12 = "Marshal your forces and forge\nahead for a final assault on the\nHadean base.",
		*_Text13 = "A massive bolt from an off-planet\nMegagun has just decimated our\nbase. The situation is very grim,\nwith few survivors.",
		*_Text14 = "A strange pod has been dropped\njust outside the Hadean gates.\nHead over there and see if you\ncan retrieve it.";

	char buf[256];	//scratch buffer for sprintf
};
DLLBase * BuildMission(void)
{
	return new edf3();
}

void edf3::Setup(void)
{	
	//set all variables to 0/false
	memset(i_array, 0, i_count*sizeof(int));
	memset(b_array, 0, b_count*sizeof(bool));
	memset(h_array, 0, h_count*sizeof(Handle));
	memset(f_array, 0, f_count*sizeof(float));
	
	SetAutoGroupUnits(false);
	EnableHighTPS(m_TPS);

	//Preload ODFs to reduce lag spikes when things are spawned in for the first time
	PreloadODF("lneck01");
	PreloadODF("ptera01");
	PreloadODF("ptera02");
	PreloadODF("ptera03");
	PreloadODF("compy01");
	PreloadODF("raptor01");
	PreloadODF("cvhtank");
	PreloadODF("cvhscout");
	PreloadODF("ivcargo");
	PreloadAudioMessage("edf0301.wav");	
	PreloadAudioMessage("edf0302.wav");
	PreloadAudioMessage("edf0303.wav");
	PreloadAudioMessage("edf0304.wav");
	PreloadAudioMessage("edf0305.wav");
	PreloadAudioMessage("edf0306.wav");
	PreloadAudioMessage("edf0307.wav");
	PreloadAudioMessage("edf0307a.wav");
	PreloadAudioMessage("edf0308.wav");
	PreloadAudioMessage("edf0309.wav");
	PreloadAudioMessage("edf0310.wav");
	PreloadAudioMessage("edf0311.wav");
	//PreloadAudioMessage("edf0312.wav");	//doesn't exist!
	PreloadAudioMessage("edf0313.wav");
	PreloadAudioMessage("edf0314.wav");
	PreloadAudioMessage("edf0315.wav");
	PreloadAudioMessage("edf0316.wav");
	PreloadAudioMessage("edf0317.wav");
	PreloadAudioMessage("edf0318.wav");
	PreloadAudioMessage("edf0319.wav");
	PreloadAudioMessage("edf0330.wav");
	PreloadAudioMessage("edf0331.wav");
	PreloadAudioMessage("edf0332.wav");

	m_Recycler = GetHandle("myrecycler");
	m_Player = GetHandle("player");
	m_Dropship = GetHandle("dropship");
	m_HadeanGtow1 = GetHandle("gtow1");
	m_HadeanGtow2 = GetHandle("gtow2");
	m_HadeanRecy = GetHandle("theirrecycler");
	m_MegaPower = GetHandle("megapower");
	for (int i = 0; i < NUM_MEGATURRETS; i++){
		sprintf_s(buf, "megaturr%d", i + 1);
		m_MegaTurrets[i] = GetHandle(buf);
	}
	m_HadeanSpike1 = GetHandle("spike1");
	m_HadeanSpike2 = GetHandle("spike2");
	for (int i = 0; i < NUM_BLOCKAGES; i++){
		sprintf_s(buf, "blockage%d", i + 1);
		m_Rocks[i] = GetHandle(buf);
	}
	//Object43 = GetHandle("tower");
}

void edf3::AddObject(Handle h)
{
	SetSkill(h, 3);
	if (IsOdf(h, "satchel") || IsOdf(h, "satchel1")){
		m_Player = GetPlayerHandle();
		GiveWeapon(m_Player, "igjetp");
		if (GetDistance(h, m_Rocks[0]) > 100){
			FailMission(GetTime() + 30, "edf3stupid.des");
			m_MissionFailed = true;
		}
		else{
			m_RemoveRocks = true;
			m_RemoveRocksTime = GetTime() + 30;
		}
	}
	else if (IsOdf(h, "ibfact")){
		m_FactoryBuilt = true;
	}
}

void edf3::Execute(void)
{
	//Handle h;
	if (m_MissionTimer < GetTime()){
		switch (m_MissionState){
		case 0:
			Ally(2, 3);
			SetAnimation(m_Dropship, "takeoff", 1);
			StartAnimation(m_Dropship);
			Stop(m_Recycler, 1);
			m_MissionState++;
			m_MissionTimer = GetTime() + 10;
			break;
		case 1:
			RemoveObject(m_Dropship);
			SetScrap(1, 40);
			SetPlan("edf3a.aip", 2);
			ClearObjectives();
			AddObjective(_Text1, WHITE);
			AudioMessage("edf0301.wav");	//Stewart:"We're a good 2 clicks from the deployment point"
			m_BaseNav = BuildObject("ibnav", 1, "deploy");
			SetObjectiveName(m_BaseNav, "BASE SITE");
			SetObjectiveOn(m_BaseNav);
			m_MissionState++;
			m_MissionTimer = GetTime() + 20;
			break;
		case 2:
			m_HadeanScout = BuildObject("evscout", 2, Position8);
			Attack(m_HadeanScout, m_Recycler, 1);
			AudioMessage("edf0302.wav");	//Stewart:"Heads up. Hostiles incoming."
			m_MissionState++;
			m_MissionTimer = GetTime() + 10;
			break;
		case 3:	//LOC_41
			AudioMessage("edf0303.wav");	//Stewart:"Keep sharp, Corber"
			m_MissionState++;
			break;
		case 4:
			if (!IsAlive(m_HadeanScout) || GetTeamNum(m_HadeanScout) != 2){
				AudioMessage("edf0304.wav");	//Stewart:"Good shooting, lads"
				m_MissionState++;
				m_MissionTimer = GetTime() + 10;
			}
			break;
		case 5:
			m_HadeanScout = BuildObject("evscout", 2, Position9);
			Attack(m_HadeanScout, m_Recycler, 1);
			m_MissionState++;
			m_MissionTimer = GetTime() + 15;
			break;
		case 6:
			AudioMessage("edf0305.wav");	//Stewart:"Hold on Corber, here comes another attack wave"
			m_MissionState++;
			m_MissionTimer = GetTime() + 60;
			break;
		case 7:
			m_HadeanScout = BuildObject("evscout", 2, Position9);
			Attack(m_HadeanScout, m_Recycler, 1);
			m_HadeanScout = BuildObject("evscout", 2, "stage1");
			Attack(m_HadeanScout, m_Recycler, 1);
			m_MissionState++;
			break;
		case 8:	//LOC_54			
			m_MissionState++;
			m_MissionTimer = GetTime() + 60;
			break;
		case 9:
			m_HadeanScout = BuildObject("evscout", 2, "stage1");
			Attack(m_HadeanScout, m_Recycler, 1);
			m_HadeanScout = BuildObject("evscout", 2, "stage1");
			Attack(m_HadeanScout, m_Recycler, 1);
			m_MissionState++;
			break;
		case 10:	//LOC_61
			if (IsAround(m_Recycler) && !IsOdf(m_Recycler, "ivrecy")){
				SetObjectiveOff(m_BaseNav);
				RemoveObject(m_BaseNav);
				AudioMessage("edf0306.wav");	//Stewart:"Well done. We have a little breathing room now"
				SetPlan("edf3b.aip", 2);
				m_MissionState++;
			}
			break;
		case 11:	//LOC_67
			if (m_FactoryBuilt){
				m_MissionState++;
				m_MissionTimer = GetTime() + 200;
			}
			break;
		case 12:
			ClearObjectives();
			AddObjective(_Text14, WHITE);
			AudioMessage("edf0307.wav");	//Stewart:"An APC went down. Get the flight recorder..."
			BuildObject("peapc", 0, "bunker4");
			m_FlightRecorder = BuildObject("gbag01", 0, "gtow7");
			SetObjectiveName(m_FlightRecorder, "Flight Recorder");
			SetObjectiveOn(m_FlightRecorder);
			m_MissionState++;
			break;
		case 13:	//LOC_77
			m_Player = GetPlayerHandle();
			if (GetDistance(m_Player, m_FlightRecorder) < 300){
				AudioMessage("edf0307a.wav");	//Stewart:"Try to pick up that object if you can"
				m_MissionState++;
			}
			break;
		case 14:	//LOC_81
			m_Player = GetPlayerHandle();
			if (GetDistance(m_Player, m_FlightRecorder) < 5){
				//SetObjectiveOff(m_FlightRecorder);
				RemoveObject(m_FlightRecorder);
				AudioMessage("pickup01.wav");
				ClearObjectives();
				AddObjective(_Text2, WHITE);
				AudioMessage("edf0308.wav");	//Stewart:"Your cargo is extremely important. Get back to base."
				m_MissionState++;
			}
			break;
		case 15:	//LOC_90
			m_Player = GetPlayerHandle();
			if (GetDistance(m_Player, "deploy") < 150){
				AudioMessage("edf0309.wav");	//Stewart:"The flight recorder is in bad shape..."
				m_MissionState++;
				m_MissionTimer = GetTime() + 150;
			}
			break;
		case 16:
			AudioMessage("edf0310.wav");	//Stewart:"Head to canyon nav and grab satchel"
			m_MissionState++;
			m_MissionTimer = GetTime() + 5;
			break;
		case 17:
			ClearObjectives();
			AddObjective(_Text3, WHITE);
			m_CanyonNav = BuildObject("ibnav", 1, Position12);
			SetObjectiveName(m_CanyonNav, "Canyon Blockage");
			SetObjectiveOn(m_CanyonNav);
			m_SatchelPickup = BuildObject("apsatc", 1, "satchelspawn3");
			m_SatchelPickupNav = BuildObject("ibnav", 1, "satchelspawn3");
			SetObjectiveOn(m_SatchelPickupNav);
			SetObjectiveName(m_SatchelPickupNav, "pack explosive");
			m_MissionState++;
			break;
		case 18:	//LOC_106
			m_Player = GetPlayerHandle();
			if (GetDistance(m_Player, m_CanyonNav) < 100){
				SetObjectiveOff(m_CanyonNav);
				SetObjectiveOff(m_SatchelPickupNav);
				m_MissionState++;
			}
			break;
		case 19:	//LOC_111
			if (!IsAround(m_Rocks[0])){
				ClearObjectives();
				AddObjective(_Text4, WHITE);
				AudioMessage("edf0311.wav");	//Stewart:"Our map shows abandoned structures at the other end of the canyon..."
				RemoveObject(m_SatchelPickupNav);
				RemoveObject(m_CanyonNav);
				m_CityRuinsNav = BuildObject("ibnav", 1, Position14);
				SetObjectiveName(m_CityRuinsNav, "cityruins");
				SetObjectiveOn(m_CityRuinsNav);
				m_MissionState++;
			}
			break;
		case 20:	//LOC_121
			m_Player = GetPlayerHandle();
			if (GetDistance(m_Player, m_CityRuinsNav) < 150){
				RemoveObject(m_CityRuinsNav);
				ClearObjectives();
				AddObjective(_Text5, WHITE);
				//AudioMessage("edf0312.wav");	//missing?
				m_MissionState++;
				m_MissionTimer = GetTime() + 30;
			}
			break;
		case 21:
			AudioMessage("edf0330.wav");	//Cervelli:"These new enemies are emitting anomalous readings..."
			m_MissionState++;
			m_MissionTimer = GetTime() + 30;
			break;
		case 22:
			AudioMessage("edf0331.wav");	//Cervelli:"We've finally managed to get some useful readings on the unknowns..."
			m_MissionState++;
			m_MissionTimer = GetTime() + 20;
			break;
		case 23:
			AudioMessage("edf0313.wav");	//Stewart:"We've just picked up a power surge east of the city"
			m_TeleportPlayer = true;
			CameraReady();
			m_MissionState++;
			break;
		case 24:
			if (CameraPath("megacam", 1000, 1200, m_MegaPower) || CameraCancelled()){
				CameraFinish();
				m_MegaPowerNav = BuildObject("ibnav", 1, Position13);
				ClearObjectives();
				AddObjective(_Text6, WHITE);
				SetObjectiveName(m_MegaPowerNav, "power disturbance");
				SetObjectiveOn(m_MegaPowerNav);
				m_MissionState++;
			}
			break;
		case 25:	//LOC_142
			m_Player = GetPlayerHandle();
			if (GetDistance(m_Player, m_MegaPowerNav) < 50){
				SetObjectiveOff(m_MegaPowerNav);
				ClearObjectives();
				AddObjective(_Text7, WHITE);
				m_MissionState++;
			}
			break;
		case 26:
			if (IsInfo("ebmgen")){
				AudioMessage("edf0314.wav");	//Stewart:"Good work. We've got a clear scan of the structure"
				m_MissionState++;
				m_MissionTimer = GetTime() + 10;
			}
			break;
		case 27:
			m_SpawnEngTransport = true;
			m_MissionState++;
			m_MissionTimer = GetTime() + 60;
			break;
		case 28:
			AudioMessage("edf0332.wav");	//Cervelli:"The unknowns are interfering with our scans..."
			m_MissionState++;
			break;
		case 29:
			if (GetDistance(m_EngineerTransport, m_MegaPower) < 150){
				SetTeamNum(m_MegaPower, 1);
				Vector v = GetPosition(m_EngineerTransport);
				m_Engineer = BuildObject("ispilo", 1, v);
				Goto(m_Engineer, m_MegaPower, 1);
				m_MissionState++;
				m_MissionTimer = GetTime() + 30;
			}
			break;
		case 30:
			RemoveObject(m_Engineer);
			SetTeamNum(m_MegaPower, 1);
			for (int i = 0; i < NUM_MEGATURRETS; i++){
				SetTeamNum(m_MegaTurrets[i], 1);
			}
			SetPlan("edf3c.aip", 2);
			AudioMessage("edf0316.wav");	//Stewart:"You've captured the megapower. The gate defenses are on our side now..."
			SetObjectiveOff(m_EngineerTransport);
			ClearObjectives();
			AddObjective(_Text11, WHITE);
			m_MegaTurretVictim = BuildObject("evtank", 2, "spawnvictim");
			Attack(m_MegaTurretVictim, m_MegaTurrets[2], 1);
			//m_MegaTurretVictim = BuildObject("ibnav", 1, "spawnvictim");
			m_TeleportPlayer = true;
			CameraReady();
			m_MissionState++;
			break;
		case 31:
			if (!IsAround(m_MegaTurretVictim) || CameraPath("victimcam", 3000, 1800, m_MegaTurretVictim) || CameraCancelled()){
				CameraFinish();
				RemoveObject(m_MegaTurretVictim);
				m_MissionState++;
				m_MissionTimer = GetTime() + 15;
			}
			break;
		case 32:
			ClearObjectives();
			m_MissionState++;
			m_MissionTimer = GetTime() + 120;
			break;
		case 33:
			AudioMessage("edf0316a.wav");	//Cervelli:"Incoming energy beam..."
			BuildObject("kaboom", 2, "deploy");
			BuildObject("kaboom", 2, "deploy");
			BuildObject("kaboom", 2, "deploy");
			m_MissionState++;
			m_MissionTimer = GetTime() + 28;
			break;
		case 34:
			m_BaseDestroyed = true;
			ClearObjectives();
			BuildObject("g3beamd", 2, "deploy");
			AudioMessage("edf0317.wav");	//Stewart:"<PAIN> We've just been hit..."
			AddObjective(_Text13, WHITE);
			m_CutsceneNav = BuildObject("ibnav", 1, "deploy");
			m_TeleportPlayer = true;
			m_MissionState++;
			CameraReady();
			break;
		case 35:
			if (CameraPath("recyclercam", 2000, 1000, m_CutsceneNav) || CameraCancelled()){
				CameraFinish();
				RemoveObject(m_Recycler);
				RemoveObject(m_CutsceneNav);
				ClearObjectives();
				AddObjective(_Text8, WHITE);
				m_MissionState++;
				m_MissionTimer = GetTime() + 30;
			}
			break;
		case 36:
			AudioMessage("edf0318.wav");	//Cervelli:"We're evacuating survivors. Take out the Hadean base..."
			m_MissionState++;
			break;
		case 37:	//LOC_202
			if (!IsAround(m_HadeanRecy)){
				AudioMessage("edf0319.wav");	//Cervelli:"Good work commander..."
				SucceedMission(GetTime() + 23, "edf3win.des");
				m_MissionState++;
			}
			break;
		}
	}

	HandleDinos();
	CheckStuffIsAlive();
	RemoveRocks();
	HandleEngineerTransport();
	TeleportPlayer();
}

//spawn dinos, unknowns and deploy recy at deploy point
void edf3::HandleDinos(void){
	Handle h;
	if (m_DinoTimer < GetTime()){
		switch (m_DinoState){
		case 0:
			m_DinoState++;
			m_DinoTimer = GetTime() + 15;
			break;
		case 1:
			Goto(m_Recycler, "recyclerpath", 1);
			h = BuildObject("lneck01", 0, "brontopath");
			Patrol(h, "brontopath", 1);
			h = BuildObject("lneck01", 0, "brontopath2");
			Patrol(h, "brontopath2", 1);
			for (int i = 0; i < 3; i++){
				h = BuildObject("ptera01", 0, "pterosaur1");
				Patrol(h, "pterosaur1", 1);
			}
			for (int i = 0; i < 4; i++){
				h = BuildObject("ptera02", 0, "pterosaur2");
				Patrol(h, "pterosaur2", 1);
			}
			for (int i = 0; i < 5; i++){
				h = BuildObject("ptera03", 0, "pterosaur3");
				Patrol(h, "pterosaur3", 1);
			}
			for (int i = 0; i < 6; i++){
				h = BuildObject("compy01", 9, "littledino1");
				Patrol(h, "littledino1", 1);
			}
			//for (int i = 0; i < 5; i++){	//"compy.odf" does not exist!
			//	h = BuildObject("compy", 9, "littledino2");
			//	Patrol(h, "littledino2", 1);
			//}
			m_DinoState++;
			m_DinoTimer = GetTime() + 15;
			break;
		case 2:
			h = BuildObject("raptor01", 9, "dino1");
			Patrol(h, "dino1", 1);
			m_DinoState++;
			m_DinoTimer = GetTime() + 15;
			break;
		case 3:
			h = BuildObject("raptor01", 9, "dino1");
			Patrol(h, "dino1", 1);

			//spawn Cerb "Unknown" units
			//changed Cerb units from team 2 to team 3, since they 
			//were driving across the map and attacking player's base!
			h = BuildObject("cvhtank", 3, "cerberi1");
			Patrol(h, "cerberi1", 1);
			h = BuildObject("cvhtank", 3, "cerberi1");
			Patrol(h, "cerberi1", 1);
			m_Unknown1 = BuildObject("cvhscout", 3, "cerberi1");
			Patrol(m_Unknown1, "cerberi1", 1);
			m_DinoState++;
			m_DinoTimer = GetTime() + 2;
			break;
		case 4:
			m_Unknown2 = BuildObject("cvhscout", 3, "cerberi1");
			Patrol(m_Unknown2, "cerberi1", 1);
			m_DinoState++;
			m_DinoTimer = GetTime() + 2;
			break;
		case 5:
			m_Unknown2 = BuildObject("cvhscout", 3, "cerberi1");
			Patrol(m_Unknown2, "cerberi1", 1);
			m_DinoState++;
			break;
		case 6:
			if (GetCurrentCommand(m_Recycler) == CMD_NONE){
				Dropoff(m_Recycler, "deploy", 1);
				m_DinoState++;
			}
			break;
		}
	}
}

//removes the blockages in the canyon after the satchel detonates
void edf3::RemoveRocks(void){
	if (m_RemoveRocks && m_RemoveRocksTime < GetTime()){
		Vector v = GetPosition(m_Rocks[4]);
		m_DeployedSatchel = BuildObject("satchelmine", 2, v);
		for (int i = 0; i < NUM_BLOCKAGES; i++){
			RemoveObject(m_Rocks[i]);
		}
		IFace_Activate("ai.calccliffs");
		m_RemoveRocks = false;
	}
}

//code for spawning eng transport and it following the player
void edf3::HandleEngineerTransport(void){
	if (m_SpawnEngTransport && m_EngTransportTimer < GetTime()){
		m_Player = GetPlayerHandle();
		switch (m_EngTransportState){
		case 0:
			m_EngineerTransport = BuildObject("ivcargo", 1, "engineerspawn");
			Stop(m_EngineerTransport, 1);
			SetObjectiveOn(m_EngineerTransport);
			AddObjective(_Text9, WHITE);
			//Variable1 = 1;
			AudioMessage("edf0315.wav");
			m_EngTransportState++;
			m_EngTransportTimer = GetTime() + 20;
			break;
		case 1:
			m_EngineerTransportArrived = true;
			m_EngTransportState++;
			break;
		case 2:	//LOC_340
			if (GetDistance(m_EngineerTransport, m_MegaPower) < 500){
				Goto(m_EngineerTransport, m_MegaPower, 1);
				m_EngTransportState++;
			}
			else if (GetDistance(m_Player, m_EngineerTransport) > 300 && 
				GetCurrentCommand(m_EngineerTransport) != CMD_NONE){
				Stop(m_EngineerTransport, 1);
			}
			else if (GetDistance(m_Player, m_EngineerTransport) < 300 && 
				GetCurrentCommand(m_EngineerTransport) == CMD_NONE){
				Follow(m_EngineerTransport, m_Player, 1);
			}
			break;
		}
	}
}

//temporarily moves player somewhere safe during camera cutscenes
void edf3::TeleportPlayer(void){
	if (m_TeleportPlayer && m_PlayerTeleportTimer < GetTime()){
		Matrix m;
		m_Player = GetPlayerHandle();
		GetPosition(m_Player, m);

		switch (m_PlayerTeleportState){
		case 0:
			m_OldPlayerPosX = m.posit.x;
			m_OldPlayerPosY = m.posit.y;
			m_OldPlayerPosZ = m.posit.z;
			SetPosition(m_Player, "engineerspawn");
			m_PlayerTeleportState++;
			m_PlayerTeleportTimer = GetTime() + 12;
			break;
		case 1:
			m.posit.x = m_OldPlayerPosX;
			m.posit.y = m_OldPlayerPosY;
			m.posit.z = m_OldPlayerPosZ;
			SetPosition(m_Player, m);
			m_PlayerTeleportState = 0;
			m_TeleportPlayer = false;
			break;
		}
	}
	

}

void edf3::CheckStuffIsAlive(void){
	if (!m_MissionFailed){
		if (!IsAround(m_MegaPower)){
			FailMission(GetTime() + 5, "edf3mega.des");
			m_MissionFailed = true;
		}
		else if (!m_BaseDestroyed && !IsAround(m_Recycler)){
			FailMission(GetTime() + 5, "edf3recy.des");
			m_MissionFailed = true;
		}
		else if (m_EngineerTransportArrived && !IsAround(m_EngineerTransport)){
			FailMission(GetTime() + 5, "edf3eng.des");
			m_MissionFailed = true;
		}
	}
}
