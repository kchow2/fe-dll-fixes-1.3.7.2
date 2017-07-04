#include "..\Shared\SPMission.h"

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

	// bools
	bool
		b_first,
		b_last;

	// floats
	float
		f_first,
		f_last;

	// handles
	Handle
		h_first,

		h_last;

	// integers
	int
		i_first,
		m_TPS,
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

}

void edf2::Execute(void)
{

}
