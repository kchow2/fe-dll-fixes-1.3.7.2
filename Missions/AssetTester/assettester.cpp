#include "..\Shared\SPMission.h"

class AssetTester : public SPMission 
{
public:
	AssetTester(void)
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
		m_MissionTimer,
		//m_SpawnX,
		//m_SpawnY,
		//m_SpawnZ,
		f_last;

	// handles
	Handle
		h_first,
		m_Nav1,
		m_Nav2,
		m_CurrentHandle,
		h_last;

	// integers
	int
		i_first,
		m_TPS,
		m_MissionState,
		m_AssetIndex,
		m_AssetCount,
		i_last;

	char buf[256];	//scratch buffer for sprintf
};
DLLBase * BuildMission(void)
{
	return new AssetTester();
}

char *m_AssetOdfs[] = {
	"aaflakmine",
	"AASEEK",
	"agmin",
	"ammopack1",
	"anti",
	"antibody",
	"antibody2",
	"antidrop",
	"antidropb",
	"antistem",
	"AP8BCANN",
	"AP8BMORT",
	"AP8BROCK",
	"apaaflak",
	"APAASEEK",
	"APADDER",
	"Apamjet",
	"apammopack",
	"APANNOY",
	"APARMRAD",
	"apbang",
	"APBATON",
	"apbazo",
	"APBCHAIN",
	"APBHOLE",
	"APBOLTER",
	"apbubmine",
	"APBURST",
	"apcblast",
	"APCBOMB",
	"apchaffmine",
	"apchainmin",
	"apchavoc",
	"APCLASER",
	"apcluster",
	"apcombo1",
	"apcombo2",
	"apcomlink",
	"apcpen",
	"apcpfol",
	"apcphan",
	"APCPHCG",
	"APCPHE1",
	"APCPHE2",
	"apcphlas",
	"APCPHMN",
	"apcplasma",
	"apcpmits",
	"apcpphoto",
	"apcprktbomb",
	"apcpworm",
	"apcrapier",
	"apcrplas",
	"APCRUBEX",
	"apcslam",
	"apcvcmet",
	"apcvfafm",
	"apdarkmine",
	"apdaywreck",
	"apdisruptor",
	"apdmsatc",
	"apdragb",
	"APE7Bomb",
	"APEBURST",
	"apELC",
	"APELECT",
	"APEPLAS",
	"apfbgun",
	"apfiremine",
	"apfiremine3",
	"apfirestorm",
	"apflamemin",
	"apflaregun",
	"apflotgen",
	"apflotmine",
	"APFLRBMB",
	"APFOCUS",
	"apfootmine",
	"apfrktb",
	"apfthrow",
	"APGAU8",
	"apgaus",
	"apgoofmine",
	"apgren",
	"aphanglider",
	"APHFIRE",
	"aphmort",
	"aphover",
	"aphover8",
	"aphsnip",
	"APICER",
	"APIONC",
	"apisotope",
	"apjammine",
	"apjetp",
	"aplasercan",
	"aplasmrt",
	"APLEGION",
	"aplock",
	"apmassd",
	"apmcag",
	"apmcspormin",
	"apminfield",
	"apminfielddm",
	"APMIRV",
	"APMOON",
	"APMWRECK",
	"apneutron",
	"appackbomb",
	"apparbst",
	"APPERC",
	"appinger",
	"APPLIGHT",
	"appoptag",
	"APPROTON",
	"aprdrprobe",
	"aprepair",
	"aprktb",
	"aprover",
	"aprprkit",
	"APSAM",
	"apsatlink",
	"APSATURN",
	"APSEEKX",
	"apserv-h",
	"apserv07",
	"apshellgun",
	"apshockrock",
	"apsing",
	"apskipfjack",
	"apskipjack",
	"apslagmort",
	"apslagstab",
	"apslagstabLR",
	"APSLICER",
	"apslide",
	"APSMOKE",
	"APSNARE",
	"apsniffcan",
	"apsniffrckt",
	"apsnip",
	"apsnipbz1",
	"APSPLAS",
	"apsporemine",
	"apspotlight",
	"APSPRINK",
	"aptagcr",
	"aptagcr_a",
	"aptagcr_c",
	"aptagg",
	"aptestslag",
	"APTMK1",
	"aptorp",
	"APTURBO",
	"APUGUN",
	"apvacmine",
	"apwrck",
	"apwrck7",
	"apwrck8",
	"APXVSTAB",
	"apzapmine",
	"apzemp",
	"arcon1",
	"arcon2",
	"arcon3",
	"arcon4",
	"arcs1",
	"arcs2",
	"arcs3",
	"arcs4",
	"arcworm01",
	"arcworm02",
	"Ark2",
	"aspilo",
	"aurora1",
	"aurora2",
	"aurora3",
	"aurorabolt",
	"aurorafield",
	"avBradar",
	"avhrazdm",
	"avpueatnk2",
	"avpuemisl2",
	"avpuesct2",
	"avpufrbmb2",
	"avpufsct2",
	"avpufwalk2",
	"avpuimbk2",
	"avpuirbmb2",
	"avpuirckt2",
	"bbdest05xx",
	"bbruin04",
	"beamfence",
	"beamfence10",
	"betunnxx00",
	"bhflare",
	"BHOLE",
	"bigpool",
	"bird1sndr",
	"bird2sndr",
	"blackmine",
	"blackout04",
	"blackoutl",
	"blacksquare",
	"blightmine",
	"bones",
	"bonesbig",
	"boneshuge",
	"bonessmall",
	"braindead",
	"brain_sp",
	"breach7z",
	"bspilo",
	"bub1",
	"bub1_16",
	"bub2",
	"bubmine",
	"buzz01",
	"bvhrazdm",
	"cage",
	"cbagen",
	"Cbarmo",
	"cbcbun",
	"cbfact",
	"cbfact02",
	"cbfact03",
	"cbgtow",
	"cbgtow07",
	"cbjamm",
	"cbjamm_15",
	"cbmistur",
	"cbmtow",
	"CBOMB",
	"cbpgen",
	"cbprop01",
	"cbprop0107",
	"cbprop017",
	"cbprop02",
	"cbprop03",
	"cbprop04",
	"cbprop04s",
	"cbprop05",
	"cbprop06",
	"cbprop07",
	"cbprop08",
	"cbprop09",
	"cbprop10",
	"CBPROP6e",
	"cbrecy",
	"cbrecy02",
	"cbrecy03",
	"cbrecy_15",
	"cbsbay",
	"cbscav",
	"cbscav_xx",
	"cbscup",
	"cbtcen",
	"cbturr03",
	"cerbmine",
	"cerbpcnr01",
	"cerbpcnr02",
	"cerbpcnr03",
	"cerbpipe01",
	"cerbpipe02",
	"cerbpipeT1",
	"cerbpipeX1",
	"chaffbits",
	"chaffmine",
	"CHAINMIN",
	"chvatank",
	"CocycadL_01",
	"Cocycad_01",
	"Cocycad_02",
	"Cocycad_03",
	"Cocycad_04",
	"Cohtail_01",
	"Cohtail_02",
	"comet",
	"comlink",
	"commando",
	"commandoh",
	"commandos",
	"compy01",
	"condenser",
	"consoleprop",
	"cormetal1",
	"countdown",
	"cpfol",
	"cpgfield",
	"cpgfield2",
	"cpgorbit",
	"cpholdpull",
	"cpmitsmin1",
	"cpmitsmin2",
	"CPSNIP",
	"cpstal00",
	"cpstal01",
	"cpstal02",
	"cpstal03",
	"cpstal04",
	"cpstpool01",
	"cpstpool02",
	"cricket_sp",
	"crow01",
	"cspilo",
	"cvatank",
	"cvatank2",
	"cvatank2P",
	"cvclaw",
	"cvcomet_a",
	"cvcomet_c",
	"cvcons",
	"cvcons02",
	"cvcons03",
	"cvcons07",
	"cvcspeed",
	"cvdcar",
	"cvdcar_15",
	"cvdrbombdm",
	"cvdscoutdm",
	"cvdtankdm",
	"cvgdron",
	"Cvgorg",
	"Cvgorg4w",
	"cvhatank",
	"cvhatankP",
	"cvhrazdm",
	"cvhrbomb",
	"cvhscout",
	"cvhtank",
	"cvmdron",
	"cvmlay",
	"cvmlay02",
	"cvmlay03",
	"cvmlay8",
	"cvmlayer",
	"cvmlay_15",
	"cvpuatank",
	"cvpurbomb",
	"cvpuscout",
	"cvputank",
	"cvpuwalk",
	"cvrbomb",
	"cvrbombP",
	"cvrecy02",
	"cvrecy03",
	"cvscav",
	"cvscav02",
	"cvscav_xx",
	"cvschulz",
	"cvscout",
	"cvscout10",
	"cvscout4w",
	"cvscoutP",
	"cvscout_15",
	"cvserv",
	"cvserv02",
	"cvtalon02",
	"cvtalon02P",
	"cvtalon10",
	"cvtank",
	"cvtankP",
	"cvtank_15",
	"cvturr",
	"cvturr02",
	"cvturr02P",
	"cvturr02_xx",
	"cvturr4w",
	"cvtyrant",
	"cvwalk",
	"cvwalkP",
	"cvwra00",
	"cvwraply",
	"c_8bcann_sp",
	"c_annoy_sp",
	"c_cphe1_sp",
	"c_cphe2_sp",
	"c_cphlas_sp",
	"c_dbolt_sp",
	"c_dragb_sp",
	"c_elect_sp",
	"c_eplas_sp",
	"c_fball_sp",
	"c_focus_sp",
	"c_hlas_sp",
	"c_icer_sp",
	"c_pburst_sp",
	"c_perc_sp",
	"c_proton_sp",
	"c_shockr_sp",
	"c_slstab_sp",
	"c_torp_sp",
	"c_turbl_sp",
	"c_xvstab_sp",
	"dactyl_sp",
	"dalek",
	"darkmine",
	"darkmine2",
	"dclightmine",
	"deathbomb",
	"dfootmine",
	"Dino1sndr",
	"dmsatchel",
	"drip",
	"dummy",
	"dummy2",
	"e7bbdesty",
	"e7bbdestz",
	"e7biotnk",
	"E7Bomb",
	"e7pavement",
	"e7porock",
	"e8_df",
	"EBARMO",
	"EBARMO07",
	"ebcbun",
	"ebcbun7",
	"ebcbun_e14",
	"ebdfdr",
	"ebdturr",
	"ebdturr07",
	"ebdturredf3",
	"ebfact",
	"ebfact2",
	"ebfact3",
	"ebfact4",
	"ebfa_ih",
	"ebgt2g",
	"ebgt2g7",
	"ebgt4g",
	"ebmgen",
	"ebmgun",
	"ebnexus01",
	"ebnexus02",
	"EBPGEN",
	"ebport_e8",
	"ebrectur",
	"ebrecy",
	"ebrecy_df",
	"ebrecy_m",
	"ebrecy_xx",
	"ebre_ih",
	"ebsbay",
	"ebscav",
	"ebscup",
	"ebscup_16",
	"ebshield",
	"ebtcen",
	"ebtcen07",
	"ebtrain",
	"ebufo01",
	"ebufo02",
	"ebus",
	"edf3trans",
	"edf5trans",
	"edf7pipe01",
	"edf7pipe02",
	"edf7pipe03",
	"edf7pipe04",
	"edfjamm7",
	"electurr",
	"elecworm",
	"entermine",
	"ERROR",
	"ERROR_M",
	"erupt2b",
	"espilo",
	"ESSOLD",
	"estorm1",
	"estorm2",
	"estorm3",
	"estormrc1",
	"esufo01",
	"esufo02",
	"ESUSER",
	"ESUSER_M",
	"evartl",
	"evartl07",
	"evartl2",
	"evartl_15",
	"evatank",
	"evatankdm",
	"evatankl",
	"evatanks",
	"evatanku",
	"evatu_ih",
	"evat_ih",
	"evcons",
	"evconsfc03",
	"evconsx",
	"evcons_e14",
	"evdrone",
	"evdroptf",
	"evdrop_15a",
	"evdrop_15b",
	"evdrop_l2",
	"eVKAMI",
	"eVMISL",
	"evmisldm",
	"eVMISLU",
	"EVMISLU4w",
	"Evmisludm",
	"eVMISLW",
	"evmi_ih",
	"evmort",
	"evmort4",
	"evmortdm",
	"evmorts",
	"evnexus01",
	"evpuatank",
	"evpuatanku",
	"evpumisl",
	"evpumislu",
	"evpumort",
	"evpuscav",
	"evpuscout",
	"evpuserv",
	"evputank",
	"evputanku",
	"evputurr",
	"evrecy",
	"evrecy_df",
	"evrecy_m",
	"evscav",
	"evscav4w",
	"evscout",
	"evscout4",
	"evscoutdm",
	"evscoutu",
	"evscout_e02",
	"evscout_e13",
	"evscout_m",
	"evsc_ih",
	"evserv",
	"evsky",
	"evtank",
	"evtank4",
	"evtank4w",
	"evtankdm",
	"evtankf",
	"evtankply",
	"evtanks",
	"evtanku",
	"evtank_15",
	"evta_IH",
	"evturr",
	"evturrs",
	"evturrshell",
	"evturr_15",
	"evufo01",
	"evwalk",
	"exam",
	"explodes",
	"f2bmine",
	"facbarb1",
	"facbarb2",
	"facbarb3",
	"facdeadtree",
	"facgantry01",
	"facgantry02",
	"facgantry03",
	"fachimney1",
	"facoutlet",
	"facpipebig",
	"facpool01",
	"facpool02",
	"facpylon01",
	"facrane01",
	"facrane02",
	"facrbrseg",
	"facrbrstr",
	"facrstcnr1",
	"facrstcnr2",
	"facrstcnr3",
	"facrstp1",
	"facrstp2",
	"facrstp3",
	"facstorage",
	"facstorage2",
	"facstrag1",
	"factower1",
	"fball2a",
	"fball2c",
	"fbdummy",
	"fbeacon",
	"fbeacon2",
	"fbforg",
	"fbjamm",
	"fbkiln",
	"fbrecturr",
	"fbrecy_df",
	"fbrecy_m",
	"fbrecy_xx",
	"fbscav",
	"fbscup",
	"fbspir--",
	"fbspir",
	"fbspirfc03",
	"fbspir_fl",
	"fe01bg01",
	"fe01bg02",
	"fe01fc01",
	"fe01fc02",
	"fe01fc03",
	"fe01pt01",
	"fe01pt01s",
	"fe01pt02",
	"fe01pt02s",
	"fe01pt03",
	"fe01pt03s",
	"fe01pt04",
	"fe01pt04s",
	"fe01pt05",
	"fe01pt05s",
	"fe01pt06",
	"fe01pt06s",
	"fe01pt07",
	"fe01pt07s",
	"fe01tf02",
	"fe01wl01",
	"fe01wl02",
	"febush00",
	"fecave01",
	"FeCRuin01",
	"FeCRuin02",
	"FecRuin03",
	"FecRuin04",
	"fedeworm01",
	"fefol22",
	"fefol22b",
	"fefol23",
	"fefol23b",
	"fefol24",
	"fefol24b",
	"fegarg01",
	"fegarg02",
	"fegarg03",
	"fegarg04",
	"fegeotherm",
	"fegleaf01",
	"fegleaf02",
	"fegtree01",
	"fegtree02",
	"fegtree03",
	"fegtree04",
	"fegtree05",
	"fegtree06",
	"fegtree07",
	"fegvine01",
	"fegvine02",
	"fegvine03",
	"fegvine04",
	"fepine00",
	"fepine01",
	"fepine02",
	"fepine03",
	"fepineb00",
	"Fercar_01",
	"Fercar_02",
	"Fercar_03",
	"Fercar_04",
	"Fercar_05",
	"FeRlgt01",
	"FeRlgt02",
	"FeRpool01",
	"FeRswseg01",
	"FeRswseg02",
	"FeRswseg03",
	"festatue01",
	"firebits",
	"firefly",
	"firemine",
	"flakdisp",
	"flakmine",
	"flarebombb2",
	"flightrec",
	"flotbits",
	"flotgen",
	"flotmine",
	"fluxanchor",
	"flymine",
	"foliage1",
	"foliage2",
	"foliage3-h",
	"foliage3",
	"foliage3a",
	"foliage3b",
	"foliage3c",
	"foliage4",
	"foliage5",
	"foliage5a",
	"foliage5b",
	"foliage5c",
	"foliage6",
	"FOOTMINE",
	"forcewall01",
	"fpbones",
	"fpbones1",
	"fungus1",
	"fungus2",
	"fungus3",
	"fungus4",
	"fungus5",
	"fvartl",
	"fvatank",
	"fvatank3t",
	"fvcerbw",
	"fvcons_fl",
	"fvfish",
	"FVFURYTK",
	"fvnadir",
	"fvnadirdm",
	"fvnspeed",
	"fvpuarch",
	"fvpuatank",
	"fvpurbomb",
	"fvpuscav",
	"fvpuscout",
	"fvpusent",
	"fvpuserv",
	"fvputank",
	"fvputurr",
	"fvpuwalk",
	"FVRBOM1DM",
	"FVRBOM2DM",
	"FVRBOM3DM",
	"fvrbomb",
	"fvrbomb03",
	"FVRBOMBDM",
	"fvrecy_df",
	"fvscout",
	"fvscout3t",
	"fvscoutfc03",
	"fvscoutply",
	"fvscout_m",
	"fvscout_xx",
	"fvsent",
	"fvsentfc03",
	"fvserv",
	"fvserv_xx",
	"fvshadow",
	"fvsky00",
	"fvtank",
	"fvtank3t",
	"fvtank4w",
	"fvtankfc03",
	"fvturr",
	"fvturr_xx",
	"fvwalk3t",
	"fvwalk4w",
	"fvwalkfc03",
	"fxcannon",
	"fxtarget",
	"fxtube",
	"fxtube2",
	"g3beamd",
	"g3rocks",
	"gasbomb2",
	"glightmine",
	"goober",
	"goofmine",
	"goomine01",
	"gravanchor",
	"g_armrad_sp",
	"g_bchain_sp",
	"g_boltr_sp",
	"g_burst_sp",
	"g_claser_sp",
	"g_cphcg_sp",
	"g_cphmn_sp",
	"g_crub_sp",
	"g_eburst_sp",
	"g_gau8_sp",
	"g_gaus_sp",
	"g_hburst_sp",
	"g_lascan_sp",
	"g_shell_sp",
	"g_slicer_sp",
	"g_slice_sp",
	"g_solar_sp",
	"g_ugun_sp",
	"h10",
	"Hbbseg01",
	"hbbseg07",
	"hbbseg07d",
	"Hbbstr01",
	"Hbbstr07",
	"hbport",
	"hbportal",
	"hbportbig",
	"hbportfc03",
	"hbportnav",
	"hbportold",
	"hbportTerr",
	"herbiv01",
	"hold10",
	"hold15",
	"hold5",
	"holdit1",
	"holdit5",
	"hpsnip",
	"ibairbay",
	"ibairbay2",
	"ibarmo",
	"ibarmo_r",
	"ibarmo_s",
	"ibcrat00-h",
	"ibcrat01-h",
	"ibfact",
	"ibfactE6",
	"ibfact_r",
	"ibfact_s",
	"ibgtow",
	"ibgtow01-x",
	"ibgtoww10",
	"ibgtoww7",
	"ibpgen01-x",
	"ibportsm",
	"ibrecturr",
	"ibrecy09",
	"ibrecyE6",
	"ibrecy_16",
	"ibrecy_df",
	"ibrecy_e14",
	"ibrecy_m",
	"ibrecy_r",
	"ibrecy_s",
	"ibscav",
	"ibscup",
	"ibtel1",
	"ibwseg01-h",
	"isbrain",
	"isgren",
	"isgren1",
	"isgren2",
	"isgsnp",
	"ishsnp",
	"islase",
	"isphase",
	"isshock",
	"issnip",
	"issold1",
	"issold2",
	"issold3",
	"ivapc",
	"ivapc4w",
	"ivatankt",
	"ivbomb",
	"ivcargo",
	"ivcarrs",
	"ivcarrs_ani",
	"ivcons",
	"ivconsE6",
	"ivcons_r",
	"ivcons_s",
	"ivcorbin",
	"ivcricket",
	"ivdactyl",
	"iveisen",
	"ivewalk",
	"ivewalk3t",
	"ivewalkdm",
	"ivewalk_15",
	"ivhack",
	"ivmbike",
	"ivminelyr",
	"ivmisl",
	"ivmisl8",
	"ivnadirdm",
	"ivpinata",
	"ivpuatank",
	"ivpuewalk",
	"ivpumbike",
	"ivpumisl",
	"ivpurbomb",
	"ivpurckt",
	"ivpuscav",
	"ivpuscout",
	"ivpuserv",
	"ivputank",
	"ivputurr",
	"ivpuwalk",
	"ivrbom2dm",
	"ivrbom3dm",
	"ivrbomb",
	"ivrbombdm",
	"ivrbombdm2",
	"ivrecy09",
	"ivrecyE6",
	"ivrecy_16",
	"ivrecy_df",
	"ivrecy_e14",
	"ivrecy_i",
	"ivrecy_r",
	"ivrecy_s",
	"ivscavgun",
	"ivscout10",
	"ivscout11",
	"ivscout8",
	"ivscout_m",
	"ivserv",
	"ivshul",
	"ivshul1",
	"ivshul2",
	"ivsiren",
	"ivspbk",
	"ivspider",
	"ivspy",
	"ivtank4w",
	"ivtank8",
	"ivtanker",
	"ivtankply",
	"ivtank_e01",
	"ivtrans-r",
	"ivwalk",
	"ivwalk3t",
	"i_cerbbomb_sp",
	"jammine",
	"kaboom",
	"kamimine",
	"kingofhill2",
	"lavamin",
	"lbub1",
	"lbub2",
	"legiontank",
	"light1",
	"light10",
	"light11",
	"light2",
	"light3",
	"light4",
	"light5",
	"light6",
	"light7",
	"light8",
	"light9",
	"lightmine",
	"light_spin",
	"Lneck01",
	"log01",
	"log01nc",
	"log02",
	"log03",
	"maniace",
	"maniach",
	"maniach10",
	"maniacs",
	"maproom",
	"mapsign2",
	"mapsign3",
	"mbbeam",
	"mbbeam3",
	"mbbeam4",
	"mbfire",
	"mbrocks",
	"mbruin01-h",
	"mbruin03-h",
	"mbruin05-h",
	"mcspormina",
	"mcsporminc",
	"mcurmin",
	"megagun",
	"meteor",
	"meteorb",
	"minelyr_sp",
	"MINFIELD",
	"minfielddm",
	"MIRV",
	"mi_aseek_sp",
	"mi_bhole_sp",
	"mi_chmin_sp",
	"mi_snare_sp",
	"mlight",
	"moalg02-h",
	"mogarg01a",
	"mogarg01b",
	"mogarg01c",
	"mogarg02a",
	"mogarg02b",
	"mogarg03a",
	"mogarg03b",
	"mograss01",
	"mopalm03",
	"mopalm04",
	"mo_8bmor_sp",
	"mo_cpworm_sp",
	"mo_flbmb_sp",
	"mo_hfire_sp",
	"mo_minl_sp",
	"mo_mnfld_sp",
	"mo_neut_sp",
	"mo_plite_sp",
	"mo_scor_sp",
	"MPITalon1",
	"MPITalon2",
	"mshield",
	"mtnsnsr",
	"m_combo1_sp",
	"m_combo2_sp",
	"m_cryst01",
	"m_cryst02",
	"m_cryst03",
	"m_cryst04",
	"m_ctorp_sp",
	"m_hfire_sp",
	"m_poptag_sp",
	"m_slagmort_sp",
	"m_sprink_sp",
	"m_tagcr_a_sp",
	"m_tagcr_c_sp",
	"nadir",
	"nebula3",
	"neutronb2",
	"nexhangar",
	"nexusbolt",
	"nexustube",
	"ocruin01",
	"ocruin02",
	"ocruin03",
	"palmcanopy1",
	"palmcanopy2",
	"palmcanopy3",
	"PAPOOL01",
	"PAPOOL02",
	"pbatun01s",
	"pbatun02s",
	"pbatun03s",
	"pbatun04-h",
	"pbatun04s",
	"pbatun05blk",
	"pbatun05s",
	"pbatun06s",
	"pbcomm-h",
	"pbhall-h",
	"pbhall-x",
	"pbones-h",
	"pbsbay-h",
	"pbsbay00-h",
	"pbtele01-h",
	"pbtele01-x",
	"pbtele02-h",
	"pbtele02-x",
	"pbtele05-h",
	"pbtele05-x",
	"pbtunn01h",
	"pbtunn01s",
	"pbtunn02h",
	"pbtunn02s",
	"pbtunn03h",
	"pbtunn03s",
	"pbtunn04h",
	"pbtunn04s",
	"pbtunn05h",
	"pbtunn05s",
	"pbtunn06h",
	"pbtunn06s",
	"pbtunn07h",
	"pbtunn07s",
	"pbtwos-h",
	"pbwseg02-h",
	"pbwseg04-h",
	"pbwseg05-h",
	"pgfield",
	"pgfield2",
	"pgorbit",
	"pinata",
	"pine",
	"pine0",
	"pinger",
	"podmine",
	"poolmine",
	"poolmine2",
	"portalmine",
	"portalmine2",
	"proxmine",
	"ptera01",
	"ptera02",
	"ptera03",
	"pusmine",
	"PVTURR",
	"p_amjet_sp",
	"p_flare_sp",
	"p_fmin_sp",
	"p_hang_sp",
	"p_moon_sp",
	"p_smoke_sp",
	"radarprobe",
	"raptor01",
	"rcable1",
	"rcable2",
	"rcable3",
	"rcable4",
	"rcbwreck01",
	"rcbwreck02",
	"rcbwreck03",
	"rcbwreck04",
	"rcbwreck05",
	"rcbwreck06",
	"rcmetal",
	"rcpipe",
	"rcpipe2",
	"recfield",
	"recfield2",
	"redpool1",
	"redpool2",
	"redpool3",
	"repairmine",
	"rotest2",
	"rover",
	"rover2",
	"rwpool01",
	"r_8brock_sp",
	"r_adder_sp",
	"r_baton_sp",
	"r_clust_sp",
	"r_cpworm_sp",
	"r_ctorp_sp",
	"r_cvfafm_sp",
	"r_fires_sp",
	"r_legn_sp",
	"R_MK1_SP",
	"R_SAM_SP",
	"r_satrn_sp",
	"r_sprink_sp",
	"r_tagcr_sp",
	"r_worm_sp",
	"satchelmine",
	"satcom",
	"satlink_sp",
	"satmine",
	"scconsole",
	"scibomb",
	"scraprock01",
	"scraprock02",
	"scraprock03",
	"seeker_f",
	"sens",
	"servpod_sp",
	"sfpproxm2",
	"shldgen",
	"sign-map",
	"sign-pic",
	"sign-tsao",
	"skipanchor",
	"skipfanchor",
	"slagb2",
	"smbeast1",
	"smbeast2",
	"smbeast3",
	"smbird1",
	"smbird2",
	"smdino1",
	"smdino2",
	"smdino3",
	"smdino4",
	"smdino5",
	"smdino6",
	"smdino7",
	"smdino8",
	"smfire1",
	"smgen1",
	"smgen2",
	"smhowl1",
	"SMIRV",
	"smmachine1",
	"smmachine2",
	"smmachine3",
	"smmachine4",
	"smmachine5",
	"smmachine6",
	"smmachine7",
	"smmetal1",
	"smmetal2",
	"smmoan1",
	"smnexus",
	"SMOKEB2",
	"smoker1",
	"smoker2",
	"smoker3",
	"smoker4",
	"smpool1",
	"smpower1",
	"smroar1",
	"smsteam1",
	"smsteam2",
	"smtoxic1",
	"smwater1",
	"smwaterfall",
	"smwind1",
	"snarefield",
	"snaremine",
	"sparks1",
	"sparks2",
	"spbazo",
	"spgren",
	"spike",
	"spjetp",
	"splintb2",
	"sporebits",
	"sporemine",
	"spsnip",
	"spysign01",
	"spysign02",
	"spysign03",
	"spysign04",
	"sspilo",
	"stargeom01",
	"stars",
	"stasmine",
	"stick",
	"stickgas",
	"stickgas3",
	"stickleaf",
	"stickstem",
	"svhrazdm",
	"s_bhole_sp",
	"s_fmine_sp",
	"s_seekx_sp",
	"tanktrap1",
	"tartow01",
	"teleportin",
	"teleportout",
	"terrainown",
	"tornado",
	"tornado2",
	"toxicmine",
	"Tricer01",
	"Tricer02",
	"trigbeam",
	"trn01",
	"trn02",
	"trn03",
	"trn04",
	"trn05",
	"trpool01",
	"ttx1",
	"ttx2",
	"ttx3",
	"ttx4",
	"ttx5",
	"tunnelbulb",
	"turretprobe",
	"tvwall2",
	"T_rex01",
	"ufotube",
	"vacmine",
	"verupt",
	"vfire",
	"viewstand-save",
	"viewstand",
	"vlava",
	"volcfire1",
	"vsmoke",
	"vsmoke2",
	"w2plnt1",
	"w2plnt10",
	"w2plnt2",
	"w2plnt3",
	"w2plnt5",
	"w2plnt6",
	"w2plnt7",
	"w2plnt8",
	"w2plnt9",
	"w2struct1",
	"w2struct2",
	"w2struct3",
	"w2web01",
	"warbee01",
	"waspmsl",
	"waterfall",
	"waterfall1",
	"wormgen",
	"wormgen3",
	"wormgen5",
	"XSEEKER",
	"xx-MPITalon1",
	"xx-MPITalon2",
	"xxx-fbeacon",
	"zapmine",
	"zapmine_sp",
};

void AssetTester::Setup(void)
{	
	//set all variables to 0/false
	memset(i_array, 0, i_count*sizeof(int));
	memset(b_array, 0, b_count*sizeof(bool));
	memset(h_array, 0, h_count*sizeof(Handle));
	memset(f_array, 0, f_count*sizeof(float));
	
	SetAutoGroupUnits(false);
	EnableHighTPS(m_TPS);

	m_Nav1 = GetHandle("Nav1");
	m_Nav2 = GetHandle("Nav2");
	m_AssetCount = sizeof(m_AssetOdfs) / sizeof(m_AssetOdfs[0]);
}

void AssetTester::AddObject(Handle h)
{

}

void AssetTester::Execute(void)
{
	Handle player = GetPlayerHandle();
	if (m_MissionTimer < GetTime()){
		switch (m_MissionState){
		case 0:
			AddObjective("Drive within 10m of the nav to start.", WHITE);
			AddObjective("To pause the test, drive away from the nav.", WHITE);
			m_MissionState++;
			break;
		case 1:
			if (GetDistance(player, m_Nav1) < 10){
				PrintConsoleMessage("---STARTING---");
				sprintf_s(buf, "Asset Count: %d", m_AssetCount);
				PrintConsoleMessage(buf);
				AddObjective("---STARTING---", WHITE);
				AddObjective(buf, WHITE);
				SetObjectiveOn(m_Nav2);
				SetObjectiveName(m_Nav2, "Spawn");
				m_MissionTimer = GetTime() + 3.0f;
				m_MissionState++;
			}
			break;
		case 2:		//SPAWN NEXT ASSET
			if (GetDistance(player, m_Nav1) < 10){
				sprintf_s(buf, "Spawning in asset #%d:'%s'", m_AssetIndex, m_AssetOdfs[m_AssetIndex]);
				PrintConsoleMessage(buf);
				m_MissionState++;
			}
			break;
		case 3:
			{
				m_CurrentHandle = BuildObject(m_AssetOdfs[m_AssetIndex], 0, m_Nav2);
				m_MissionState++;
				//m_MissionTimer = GetTime() + 0.5f;
			}
			break;
		case 4:
			SetNoScrapFlagByHandle(m_CurrentHandle);
			SetLifespan(m_CurrentHandle, 0.1f);
			EjectPilot(m_CurrentHandle);	//test the explosion
			m_MissionState++;
			m_MissionTimer = GetTime() + 0.1f;
			break;
		case 5:
			sprintf_s(buf, "------------asset #%d:OK", m_AssetIndex);
			PrintConsoleMessage(buf);
			m_AssetIndex++;
			m_MissionState++;
			//m_MissionTimer = GetTime() + 0.5f;
			break;
		case 6:
			if (m_AssetIndex == m_AssetCount){
				PrintConsoleMessage("CHECKING ASSETS DONE.");
				ClearObjectives();
				AddObjective("CHECKING ASSETS DONE.", GREEN);
				SucceedMission(GetTime() + 15);
				m_MissionState++;
			}
			else{
				if (IsAround(m_CurrentHandle)){
					RemoveObject(m_CurrentHandle);
				}
				m_MissionState = 2;
			}
			break;
		}
	}
}