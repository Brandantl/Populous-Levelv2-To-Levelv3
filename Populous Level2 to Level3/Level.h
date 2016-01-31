#pragma once
#include <Windows.h>

#define LEVEL3_VERSION						1
#define	MAP_SIZE_FACTOR 					2
#define MAP_X_SIZE							(256/MAP_SIZE_FACTOR)
#define MAP_Z_SIZE							(256/MAP_SIZE_FACTOR)
#define MAP_XZ_SIZE							(MAP_X_SIZE*MAP_Z_SIZE)
#define	MAX_NUM_TRIGGER_THINGS				10
#define	MAX_NUM_NORMAL_SPELLS				(32)
#define	MAX_LENGTH_SAVE_NAMEv2				(32)
#define	MAX_NUM_LEVELSv2					(30)
#define	MAX_NUM_SPECTATORS					1
#define	MAX_NUM_PLAYERS						(4+MAX_NUM_SPECTATORS)  //<RW> added one for hostbots
#define	MAX_NUM_REAL_PLAYERS				(MAX_NUM_PLAYERS-MAX_NUM_SPECTATORS)
#define	MAX_NUM_LEVEL_MARKERSv2				(256)
#define	MAX_NUM_LEVEL_THINGS				2000 // level files have 2000 things
#define	MAX_NUM_SCRIPT2						10

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//	THINGS STUFF
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#define		SPM_NORMAL				  			(0)
#define		SPM_INIT				  			(1)
#define		T_TYPE_NONE				  			(0)
#define		T_MODEL_NONE				   		(0)
#define		T_STATE_NONE						(0)

typedef int				SINT;
typedef signed long		SLONG;
typedef signed char		SBYTE;
typedef	unsigned char	UBYTE;
typedef signed short	SWORD;

struct		PlayerThings
{
	ULONG							SpellsAvailable;							// bit flagged info on spells available to player
	ULONG							BuildingsAvailable;							// bit flagged info on buildings available to player
	ULONG							BuildingsAvailableLevel;					// flagged if level temp available 
	ULONG							BuildingsAvailableOnce;						// flagged if one off available 
	union
	{
		ULONG							SpellsAvailableLevel;						// flagged if level temp available 
		ULONG							SpellsNotCharging;							// bit flagged as having been switched off by player
	};
	UBYTE							SpellsAvailableOnce[MAX_NUM_NORMAL_SPELLS];	// number of one shots available, per spell, indexed by spell model
	UWORD							VehiclesAvailable;							// bit flagged info on vehicles available to player
	UBYTE							TrainingManaOff;
	UBYTE							Flags;
};	

struct		LevelHeader
{
	struct PlayerThings				DefaultThings;							// default things for player 0 (if network game)... default things for other players will be taken from the computer players
	CHAR							Name[MAX_LENGTH_SAVE_NAMEv2];			// name of level
	UBYTE							NumPlayers;								// number of starting positions that have been defined
	UBYTE							ComputerPlayerIndex[MAX_NUM_PLAYERS - 2];	// computer player indices
	UBYTE							DefaultAllies[MAX_NUM_REAL_PLAYERS];			// starting allies on this level
	UBYTE							LevelType;								// defines colours on level
	UBYTE							ObjectsBankNum;							// defines objects bank used on level
	UBYTE							LevelFlags;								// wether to use fog, shaman omnipresence, etc.
	UBYTE							Pad[1];
	UWORD							Markers[MAX_NUM_LEVEL_MARKERSv2];		// marker points on level	
	UWORD							StartPos;								// start square to look at - can't use 0,0 ->this implies start at shaman
	UWORD							StartAngle;								// start angle for level
};	

struct		LevelHeaderv3
{
	struct PlayerThings				DefaultThings;							// default things for player 0 (if network game)... default things for other players will be taken from the computer players
	CHAR							Name[MAX_LENGTH_SAVE_NAMEv2];			// name of level
	UBYTE							NumPlayers;								// number of starting positions that have been defined
	UBYTE							ComputerPlayerIndex[MAX_NUM_PLAYERS - 2];	// computer player indices
	UBYTE							DefaultAllies[MAX_NUM_REAL_PLAYERS];			// starting allies on this level
	UBYTE							LevelType;								// defines colours on level
	UBYTE							ObjectsBankNum;							// defines objects bank used on level
	UBYTE							LevelFlags;								// wether to use fog, shaman omnipresence, etc.
	UBYTE							Pad[1];
	UWORD							Markers[MAX_NUM_LEVEL_MARKERSv2];		// marker points on level	
	UWORD							StartPos;								// start square to look at - can't use 0,0 ->this implies start at shaman
	UWORD							StartAngle;								// start angle for level
	UBYTE							Version;								// How many objects are in the level
	ULONG							MaxAltPoints;							// How many points are in the level
	ULONG							MaxNumObjects;							// How many objects are in the level
	ULONG							MaxNumPlayers;							// How many players are in the level
	CHAR							Script2[MAX_NUM_SCRIPT2][MAX_LENGTH_SAVE_NAMEv2];
};

struct MaxSaveInfo
{
	SLONG								Bluff[12];
};

struct TriggerSaveInfo
{
	UBYTE								TriggerType;
	UBYTE								CellRadius;
	UBYTE								RandomValue;
	SBYTE								NumOccurences;
	UWORD								TriggerCount;
	UWORD								EditorThingIdxs[MAX_NUM_TRIGGER_THINGS];
	SWORD								PrayTime;
	UBYTE								StartInactive;
	UBYTE								CreatePlayerOwned;
	SWORD								InactiveTime;
};

struct ShapeSaveInfo
{
	SLONG								Empty;
};

struct ShotSaveInfo
{
	SLONG								Empty;
};

struct EffectSaveInfo
{
	SLONG								Empty;
};

struct GeneralSaveInfo
{
	UBYTE								DiscoveryType;
	UBYTE								DiscoveryModel;
	UBYTE								AvailabilityType;
	UBYTE								TriggerType;
	SLONG								ManaAmt;
};

struct ScenerySaveInfo
{
	UBYTE								PortalStatus;
	UBYTE								PortalLevel;
	UBYTE								PortalType;
	SWORD								Angle;
	UBYTE								UserId;
	SWORD								IslandAlt;
	UBYTE								IslandNum;
	UBYTE								BridgeNum;
};

struct VehicleSaveInfo
{
	SLONG								Empty;
};

struct CreatureSaveInfo
{
	SLONG								Empty;
};

struct PersonSaveInfo
{
	SLONG								Empty;
};

struct BuildingSaveInfo
{
	SLONG								Angle;
};

union ThingSaveStuff
{
	struct PersonSaveInfo				PersonSI;
	struct BuildingSaveInfo				BuildingSI;
	struct CreatureSaveInfo				CreatureSI;
	struct VehicleSaveInfo				VehicleSI;
	struct ScenerySaveInfo				ScenerySI;
	struct GeneralSaveInfo				GeneralSI;
	struct EffectSaveInfo				EffectSI;
	struct ShotSaveInfo					ShotSI;
	struct ShapeSaveInfo				ShapeSI;
	struct TriggerSaveInfo				TriggerSI;
	struct MaxSaveInfo					Bluff;
};

struct	Coord2D
{
	SWORD								Xpos;
	SWORD								Zpos;
};

struct	Coord3D
{
	SWORD								Xpos;
	SWORD								Zpos;
	SWORD								Ypos;
};

struct ThingSaveInfo
{
	UBYTE								Model;
	UBYTE								Type;
	SBYTE								Owner;
	struct Coord2D						Position;
	union ThingSaveStuff				Stuff;
};

struct SunlightSaveInfo
{
	UBYTE								ShadeStart;
	UBYTE								ShadeRange;
	UBYTE								Inclination;
};

struct PlayerSaveInfo
{
	struct Coord2D						StartPos;
	SLONG								Future1;
	SLONG								Future2;
	SLONG								Future3;
};