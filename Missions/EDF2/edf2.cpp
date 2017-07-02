#include "..\Shared\SPMission.h"
#include <math.h>

#define NUM_PORTALS 5
#define NUM_ATTACKERS 8

class edf2 : public SPMission 
{
public:
	edf2(void)
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
	void SpawnDinos(void);
	void HandleScavTeleport(void);
	void HandleHadeanAttack(void);
	void HandleHadeanAttackWaves(void);
	void HandleStewartNag(void);
	bool DropshipMove(Handle h, float speed, Vector dest);
	void HandlePortals(void);
	void OnPortalDist(Handle portal, Handle h);
	void Teleport(Handle h, Handle dest, float offset);
	Handle ReplaceObject(Handle h, char* newOdf);
	float VectorDistance(Vector v1, Vector v2);
	float VectorLength(Vector v);
	Vector VectorMul(Vector v, float factor);
	Vector VectorAdd(Vector v1, Vector v2);
	Vector VectorSub(Vector v1, Vector v2);

	// bools
	bool
		b_first,
		m_RecyclerMove,
		m_ScavTeleported,
		m_AttackWaveDead,
		m_StewartNag,
		m_PlayerTeleported,
		m_MissionFailed,
		b_last;

	// floats
	float
		f_first,
		m_MissionTimer,
		m_ScavTeleportTimer,
		m_HadeanAttackTimer,
		m_StewartNagTimer,
		m_StewartNextNagTime,
		m_RecyclerMoveTime,
		m_HadeanAttackTime,
		f_last;

	// handles
	Handle
		h_first,
		m_StayPut,
		m_Portals[NUM_PORTALS],
		m_Recycler,
		m_Player,
		m_EnemyScout1,	//xypos that spawns right after enemy scav
		m_BaseNav,	//"Deploy Base" nav
		m_ScrapPool,
		m_HadeanScav,
		m_DropshipFlying,
		m_DropshipLanded,
		m_Attackers[NUM_ATTACKERS],
		m_Dino1,
		m_Dino2,
		h_last;

	// integers
	int
		i_first,
		m_TPS,
		m_MissionState,
		m_ScavTeleportState,
		m_HadeanAttackState,
		m_StewartNagState,
		m_StewartNagCounter,
		m_RecyTeleported,
		m_GunTowersBuilt,
		m_AttackWave,	//hadean attack wave counter
		i_last;

	Vector
		Position2 = { -450, -120, -1460 },	//flying dropship start position
		Position3 = { -590, -155, -1190 },	//flying dropship end position
		Position4 = { 420, 0, 920 };	//deploy base location

	char
		*_Text1 = "Scout ahead of the Recycler and\nmake sure this canyon is safe!",
		*_Text2 = "You've lost the Recycler and\na platoon's worth of good men.\nWe now have no hope of securing\nour eastern flank.",
		*_Text3 = "The Hadeans are alerted to our\npresence. You must act quickly to\n make it to safer ground!",
		*_Text4 = "Deploy the Recycler, and get\nsome gun towers built. Hurry--you\ndon't have much time!",
		*_Text5 = "Excellent job, Lieutenant! The\nGeneral will be pleased to hear\nthat we have a competent CO \nholding our eastern flank.\nReinforcements are en route.",
		*_Text6 = "Command has some suspicions.\nStay clear and observe the\nactivity near the structure.",
		*_Text7 = "Get your units out of the\ncanyon through that portal.\nIt may be your only hope.",
		*_Text8 = "Use the 'I' key of your Digital\nControl Interface to scan the\nstructure.",
		*_Text9 = "Try to pass through the portal\nby cruising under the arch. If\nyou succeed, try to return in the\nsame way.",
		*_Text10 = "Sensors show a major attack wave\napproaching from two directions\nand closing fast!",
		*_Text11 = "The Hadean attackers are almost\nwithin firing range!  Brace\n for an attack!",
		*_Text12 = "You cannot seem to follow a\nsimple order, Lieutenant. Return\nto base and turn in your sidearm\nand wings.";

	char buf[256];	//scratch buffer for sprintf
};
DLLBase * BuildMission(void)
{
	return new edf2();
}

void edf2::Setup(void)
{	
	//set all variables to 0/false
	memset(i_array, 0, i_count*sizeof(int));
	memset(b_array, 0, b_count*sizeof(bool));
	memset(h_array, 0, h_count*sizeof(Handle));
	memset(f_array, 0, f_count*sizeof(float));
	
	SetAutoGroupUnits(false);
	EnableHighTPS(m_TPS);

	//Preload ODFs to reduce lag spikes when things are spawned in for the first time
	PreloadODF("stayput");
	PreloadODF("evscout_e02");
	PreloadODF("evtank");
	PreloadODF("evscav");
	PreloadODF("evmislu");
	PreloadODF("evtanku");
	PreloadODF("evmort");
	PreloadAudioMessage("edf02_01.wav");	
	PreloadAudioMessage("edf02_02.wav");
	PreloadAudioMessage("edf02_03.wav");
	PreloadAudioMessage("edf02_04.wav");
	PreloadAudioMessage("edf02_05.wav");
	PreloadAudioMessage("edf02_05A.wav");
	PreloadAudioMessage("edf02_06.wav");
	PreloadAudioMessage("edf02_07.wav");
	PreloadAudioMessage("edf02_08.wav");
	PreloadAudioMessage("edf02_09.wav");
	PreloadAudioMessage("edf02_10.wav");
	PreloadAudioMessage("edf02_11.wav");
	PreloadAudioMessage("edf02_12.wav");
	PreloadAudioMessage("xemt2.wav");
}

void edf2::AddObject(Handle h)
{
	if (IsOdf(h, "ibgtow")){
		m_GunTowersBuilt++;
	}
	else if (IsOdf(h, "ibrecy")){
		RemoveObject(m_BaseNav);
	}
}

void edf2::Execute(void)
{
	Handle h;

	if (!m_MissionFailed && m_MissionTimer < GetTime()){
		switch (m_MissionState){
		case 0:
			m_Player = GetPlayerHandle();
			m_StayPut = BuildObject("stayput", 0, m_Player);
			m_Recycler = GetHandle("Recycler");
			Stop(m_Recycler, 1);
			m_DropshipLanded = GetHandle("DropShip");
			StartEarthQuake(10);
			m_DropshipFlying = BuildObject("ivdrop_fly", 0, Position2);
			m_MissionState++;
			m_MissionTimer = GetTime() + 7;
			break;
		case 1:
			AudioMessage("edf02_01.wav");	//Pilot:"That blast came awfully close..."
			m_MissionState++;
			m_MissionTimer = GetTime() + 3;
			break;
		case 2:
			CameraReady();
			SetAnimation(m_DropshipLanded, "Deploy", 1);
			StartAnimation(m_DropshipLanded);	//open the landed dropship doors while player isn't looking
			m_MissionState++;
			break;
		case 3:
			DropshipMove(m_DropshipFlying, 30.0f, Position3);
			if (CameraPath("CamPath", 5500, 3200, m_DropshipFlying) || CameraCancelled()){
				CameraFinish();
				m_MissionState++;
			}
			break;
		case 4:
			AudioMessage("xemt2.wav");	//<crashing noise>
			m_Player = GetPlayerHandle();
			StopEarthQuake();
			RemoveObject(m_DropshipFlying);
			RemoveObject(m_StayPut);
			SetPosition(m_Recycler, "RecyclerPath");
			SetVelocity(m_Recycler, Vector(0, 0, 15));
			SetPosition(m_Player, "PlacePlayer");
			SetVelocity(m_Player, Vector(0, 0, 40));
			h = BuildObject("ivtank", 1, "Escort1");
			Follow(h, m_Recycler, 0);
			SetGroup(h, 2);
			h = BuildObject("ivscout", 1, "Escort2");
			Follow(h, m_Recycler, 0);
			SetGroup(h, 1);
			h = BuildObject("ivscout", 1, "Escort3");
			Follow(h, m_Recycler, 0);
			SetGroup(h, 1);
			h = BuildObject("ivscout", 1, "Escort4");
			Follow(h, m_Recycler, 0);
			SetGroup(h, 1);
			h = BuildObject("evscout_e02", 5, "Enemy1");
			SetSkill(h, 3);
			h = BuildObject("evscout_e02", 5, "Enemy2");
			SetSkill(h, 3);
			SetGroup(m_Recycler, 0);
			SetScrap(1, 30);

			m_MissionState++;
			m_MissionTimer = GetTime() + 3;
			break;
		case 5:
			AudioMessage("edf02_02.wav");	//Stewart:"Good landing under the circumstances..."
			Goto(m_Recycler, "RecyclerPath", 1);
			m_BaseNav = BuildObject("ibnav", 1, "NavSpawn");
			SetObjectiveName(m_BaseNav, "Investigate");
			SetObjectiveOn(m_BaseNav);
			ClearObjectives();
			AddObjective(_Text1, WHITE);
			m_Portals[0] = GetHandle("Portal1");	//first portal in canyon
			m_Portals[1] = GetHandle("Portal2");	//recycler portal east of deploy zone
			m_Portals[2] = GetHandle("Portal3");	//portal beside scrap pool
			m_Portals[3] = GetHandle("Portal4");	//south east portal
			m_Portals[4] = GetHandle("Portal5");	//player's portal (southern most one)
			m_ScrapPool = GetHandle("Pool1");
			m_MissionState++;
			m_MissionTimer = GetTime() + 30;
			break;
		case 6:
			AudioMessage("edf02_03.wav");	//Stewart:"Our scanners just picked up a huge energy spike..."
			m_MissionState++;
			m_MissionTimer = GetTime() + 220;
			break;
		case 7:
			AudioMessage("edf02_04.wav");	//Stewart:"You've got enemy units in the canyon..."
			m_MissionState++;
			m_MissionTimer = GetTime() + 5;
			break;
		case 8:
			h = BuildObject("evscout_e02", 5, "Spawn2");
			Patrol(h, "Patrol2", 0);
			m_MissionState++;
			m_MissionTimer = GetTime() + 5;
			break;
		case 9:	//LOC_43
			if (GetTime() > 780){
				h = BuildObject("evtank", 5, "Spawn1");
				Goto(h, "Patrol1", 0);
				m_MissionState++;
				m_MissionTimer = GetTime() + 27;
			}
			else{
				m_MissionState++;
			}
			break;
		case 10:	//LOC_48		
			h = BuildObject("evscout_e02", 5, "Spawn1");
			Goto(h, "Patrol1", 0);
			m_MissionState--;
			m_MissionTimer = GetTime() + 48;	
			break;
		}
	}

	SpawnDinos();
	HandleScavTeleport();
	HandleHadeanAttack();
	HandleStewartNag();
	HandlePortals();
	

	//recycler moving to base after being teleported
	if (m_RecyclerMove && m_RecyclerMoveTime < GetTime()){
		ClearObjectives();
		AddObjective(_Text4, WHITE);
		m_RecyTeleported = 1;
		Goto(m_Recycler, m_BaseNav, 0);
		m_RecyclerMove = false;
	}

	//check recy is alive
	if (!m_MissionFailed && m_MissionState > 0 && !IsAround(m_Recycler)){
		AudioMessage("edf02_11.wav");	//Stewart:"You lost the recycler..."
		ClearObjectives();
		AddObjective(_Text2, RED);
		FailMission(GetTime() + 12, "edf02L1.txt");
		m_MissionFailed = true;
	}
}

void edf2::SpawnDinos(){
	if (GetTime() > 360){
		if (!IsAround(m_Dino1)){
			m_Dino1 = BuildObject("raptor01", 3, "DinoSpawn1");
			Goto(m_Dino1, "DinoPatrol1", 0);
		}
		if (!IsAround(m_Dino2)){
			m_Dino2 = BuildObject("raptor01", 3, "DinoSpawn1");
			Goto(m_Dino2, "DinoPatrol1", 0);
		}
	}
}

void edf2::HandleScavTeleport(void){
	if (m_ScavTeleportTimer < GetTime()){
		switch (m_ScavTeleportState){
		case 0:
			m_Player = GetPlayerHandle();
			if (IsAround(m_BaseNav) && GetDistance(m_Player, m_BaseNav) < 220){
				SetObjectiveOff(m_BaseNav);
				ClearObjectives();
				AddObjective(_Text6, WHITE);
				m_HadeanScav = BuildObject("evscav", 5, "EnemyScav");
				SetObjectiveName(m_HadeanScav, "Observe");
				SetObjectiveOn(m_HadeanScav);
				Goto(m_HadeanScav, m_Portals[0], 1);
				m_ScavTeleportState++;
				m_ScavTeleportTimer = GetTime() + 25;
			}
			break;
		case 1:
			m_EnemyScout1 = BuildObject("evscout", 5, "Spawn2");
			Goto(m_EnemyScout1, m_Portals[0], 0);
			m_ScavTeleportState++;
			m_ScavTeleportTimer = GetTime() + 6;
			break;
		case 2:
			if (IsAround(m_EnemyScout1)){
				SetObjectiveOn(m_EnemyScout1);
			}
			m_ScavTeleportState++;
			break;
		}
	}
}

//Scav teleport and Hadean assault waves
void edf2::HandleHadeanAttack(void){
	if (m_ScavTeleported && !m_MissionFailed && m_HadeanAttackTimer < GetTime()){
		switch(m_HadeanAttackState){
		case 0:
			m_HadeanAttackState++;
			m_HadeanAttackTimer = GetTime() + 3;
			break;
		case 1:
			AudioMessage("edf02_05.wav");	//Stewart:"Good God, that scav just vanished"
			m_HadeanAttackState++;
			m_HadeanAttackTimer = GetTime() + 7;
			break;
		case 2:
			SetObjectiveName(m_HadeanScav, "Teleported Harvester");
			SetObjectiveOff(m_HadeanScav);
			Stop(m_HadeanScav, 0);
			m_StewartNag = true;
			m_HadeanAttackState++;
			m_HadeanAttackTimer = GetTime() + 900;
			break;
		case 3:
			AudioMessage("edf02_08.wav");	//Stewart:"The first enemy squadron is closing in..."
			ClearObjectives();
			AddObjective(_Text10, WHITE);
			m_HadeanAttackTime = GetTime() + 600;
			m_HadeanAttackState++;
			break;
		case 4:	//LOC_94
			if (GetTime() > m_HadeanAttackTime){
				m_HadeanAttackState += 2;	//LOC_104
			}
			else if (m_GunTowersBuilt > 0){
				m_HadeanAttackTime += 230;
				m_HadeanAttackState++;
			}
			break;
		case 5:	//LOC_100
			if (m_GunTowersBuilt > 1 || m_HadeanAttackTime < GetTime()){
				m_HadeanAttackState++;
			}
			break;
		case 6:	//LOC_104
			m_HadeanAttackState++;
			m_HadeanAttackTimer = GetTime() + 50;
			break;
		case 7:
			AudioMessage("edf02_09.wav");	//Stewart:"The hostiles are just seconds away..."
			AddObjective(_Text11, WHITE);
			m_HadeanAttackState++;
			break;
		case 8:	//LOC_107
			m_Attackers[0] = BuildObject("evtank", 5, "Wave1");
			Attack(m_Attackers[0], m_Recycler, 1);
			m_HadeanAttackState++;
			m_HadeanAttackTimer = GetTime() + 3;
			break;
		case 9:
			m_Attackers[1] = BuildObject("evtanku", 5, "Wave1");
			Goto(m_Attackers[1], m_Recycler, 1);
			m_HadeanAttackState++;
			m_HadeanAttackTimer = GetTime() + 3;
			break;
		case 10:
			m_Attackers[2] = BuildObject("evscout_e02", 5, "Wave1");
			Goto(m_Attackers[2], m_Recycler, 1);
			m_HadeanAttackState++;
			m_HadeanAttackTimer = GetTime() + 3;
			break;
		case 11:
			m_Attackers[3] = BuildObject("evmislu", 5, "Wave1");
			Attack(m_Attackers[3], m_Recycler, 1);
			m_HadeanAttackState++;
			m_HadeanAttackTimer = GetTime() + 3;
			break;
		case 12:
			m_Attackers[4] = BuildObject("evtanku", 5, "Wave2");
			Attack(m_Attackers[4], m_Recycler, 1);
			m_HadeanAttackState++;
			m_HadeanAttackTimer = GetTime() + 3;
			break;
		case 13:
			m_Attackers[5] = BuildObject("evmislu", 5, "Wave2");
			Goto(m_Attackers[5], m_Recycler, 1);
			m_HadeanAttackState++;
			m_HadeanAttackTimer = GetTime() + 3;
			break;
		case 14:
			m_Attackers[6] = BuildObject("evscout_e02", 5, "Wave2");
			Goto(m_Attackers[6], m_Recycler, 1);
			m_HadeanAttackState++;
			m_HadeanAttackTimer = GetTime() + 3;
			break;
		case 15:
			m_Attackers[7] = BuildObject("evmort", 5, "Wave2");
			SetMaxHealth(m_Attackers[7], 2800);
			SetCurHealth(m_Attackers[7], 2800);
			Goto(m_Attackers[7], m_Recycler, 1);
			m_HadeanAttackState++;
			m_HadeanAttackTimer = GetTime() + 3;
			break;
		case 16:	//LOC_136
			m_AttackWaveDead = true;
			for (int i = 0; i < NUM_ATTACKERS; i++){
				if (IsAround(m_Attackers[i]) ||
					(GetTeamNum(m_Attackers[i]) == 5 &&
					HasPilot(m_Attackers[i]))){
					m_AttackWaveDead = false;
					break;
				}
			}
			if (m_AttackWaveDead){
				m_AttackWave++;
				if (m_AttackWave < 3){
					m_HadeanAttackState = 8;	//LOC_107
				}
				else{
					//win
					AudioMessage("edf02_10.wav");
					ClearObjectives();
					AddObjective(_Text5, GREEN);
					SucceedMission(GetTime() + 18, "edf02W1.txt");	//Stewart:"Well done. You've smashed the Hadeans..."
					m_HadeanAttackState++;
				}
			}
			break;
		}
	}
}

void edf2::HandleStewartNag(void){
	if (m_StewartNag && m_StewartNagTimer < GetTime()){
		switch (m_StewartNagState){
		case 0:	//LOC_259
			AudioMessage("EDF02_05A.wav");	//Stewart:"Try going through the portal"
			m_StewartNextNagTime = GetTime() + 30;
			ClearObjectives();
			AddObjective(_Text9, WHITE);
			m_StewartNagState++;
			m_StewartNextNagTime = GetTime() + 30;
			break;
		case 1:
			if (m_StewartNextNagTime < GetTime()){
				if (m_StewartNagCounter > 3){
					AudioMessage("EDF02_12.wav");	//Stewart:"You can't follow simple instructions..."
					FailMission(GetTime() + 12, "EDF02_L2.txt");
					m_MissionFailed = true;
					m_StewartNagState = 99;
				}
				else{
					AudioMessage("EDF02_05A.wav");	//Stewart:"Try going through the portal"
					m_StewartNagCounter++;
					m_StewartNextNagTime = GetTime() + 30;
				}
			}
			else if (m_PlayerTeleported){
				m_StewartNagState++;
				m_StewartNagTimer = GetTime() + 3;
			}
			break;
		case 2:
			AudioMessage("edf02_06.wav");	//Stewart:"Just as we suspected. It's a portal..."
			m_StewartNagState++;
			m_StewartNagTimer = GetTime() + 3;
			break;
		case 3:
			ClearObjectives();
			AddObjective(_Text7, WHITE);
			m_StewartNagState++;
			break;
		case 4:
			if (m_RecyTeleported > 0){
				m_Player = GetPlayerHandle();
				if (GetDistance(m_Player, m_Recycler) < 150){
					SetObjectiveOn(m_ScrapPool);
					m_StewartNagState++;
					m_StewartNagTimer = GetTime() + 5;
				}
			}
			break;
		case 5:
			AudioMessage("edf02_07.wav");	//Stewart:"You've got incoming Hadeans. Get some GTs up."
			m_StewartNagState++;
			break;
		}
	}
}

//checks for the closest vehicle to all portals on the map
void edf2::HandlePortals(void){
	for (int i = 0; i < NUM_PORTALS; i++){
		Handle h = GetNearestVehicle(m_Portals[i]);
		if (GetDistance(h, m_Portals[i]) < 15 && 
			!IsOdf(h, "teleportin") && 
			!IsOdf(h, "teleportout")){
			OnPortalDist(m_Portals[i], h);
		}
	}
}

//called when a unit gets in range of a portal
void edf2::OnPortalDist(Handle portal, Handle h){
	if (IsOdf(h, "ivrecy") && !m_RecyTeleported){
		SetObjectiveOff(m_Portals[0]);
		m_BaseNav = BuildObject("ibnav", 1, Position4);
		SetObjectiveName(m_BaseNav, "Deploy Base");
		SetObjectiveOn(m_BaseNav);
		Teleport(h, m_Portals[1], 30);
		m_RecyclerMove = true;
		m_RecyclerMoveTime = GetTime() + 3;
	}
	else if(portal==m_Portals[0]){
		if (IsPlayer(h)){
			m_PlayerTeleported = true;
			Teleport(h, m_Portals[3], 30);
		}
		else if (GetRandomFloat(1.0f) < 0.5f){
			Teleport(h, m_Portals[2], 30);
		}
		else{
			Teleport(h, m_Portals[4], 30);
		}

		if (!m_ScavTeleported && h == m_HadeanScav){
			m_ScavTeleported = true;
		}
		else if (GetTeamNum(h) == 5){
			Goto(h, m_Recycler, 0);
		}
	}
	else{
		Teleport(h, m_Portals[0], 30);
		if (!IsPlayer(h)){
			Goto(h, m_Portals[0], 0);
		}
	}
}

//Teleports h to dest with an overshoot of <offset> meters
void edf2::Teleport(Handle h, Handle dest, float offset){
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
	if (!IsPlayer(h)){
		Goto(h, m.posit, 0);	//so they don't try driving back to the origin portal
	}
}

//code to move the dropship for the intro cutscene
bool edf2::DropshipMove(Handle h, float speed, Vector dest){
	float dt = 1.0f / m_TPS;

	dest.y = TerrainFindFloor(dest.x, dest.z);
	
	Matrix oldPos, newPos;
	GetPosition(h, oldPos);
	if (VectorDistance(oldPos.posit, dest) < speed*dt){		
		Matrix newPos = oldPos;
		newPos.posit = dest;
		SetPosition(h, newPos);
		SetInterpolablePosition(h, &newPos, &oldPos, true);
		return true;
	}
	else{
		//linearly interpolate from object's pos to dest
		Vector direction = VectorSub(dest, oldPos.posit);
		float factor = speed * dt / VectorLength(direction);
		Vector offset = VectorMul(direction,  factor);
		Vector pos = VectorAdd(oldPos.posit, offset);
		Matrix newPos = oldPos;
		newPos.posit = pos;
		SetPosition(h, newPos);
		SetInterpolablePosition(h, &newPos, &oldPos, true);
		return false;
	}
}

//gets the 3d distance between 2 vectors
float edf2::VectorDistance(Vector v1, Vector v2){
	float dx = (v2.x - v1.x);
	float dy = (v2.y - v1.y);
	float dz = (v2.z - v1.z);
	return sqrt(dx*dx + dy*dy + dz*dz);
}

float edf2::VectorLength(Vector v){
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

Vector edf2::VectorMul(Vector v, float factor){
	return Vector(v.x*factor, v.y*factor, v.z*factor);
}

Vector edf2::VectorAdd(Vector v1, Vector v2){
	return Vector(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vector edf2::VectorSub(Vector v1, Vector v2){
	return Vector(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}