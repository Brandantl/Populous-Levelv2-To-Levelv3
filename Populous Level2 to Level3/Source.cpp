#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include "Level.h"
using namespace std;

// Consts
const size_t MAGIC_SIZE = 5;
const char MAGICDEF[MAGIC_SIZE] = { 'L','E','V','L','3' };

// Vars
LevelHeaderv3 h;
SWORD alt[MAP_XZ_SIZE];
UBYTE noAccessFlags[MAP_XZ_SIZE];
struct PlayerSaveInfo pi[MAX_NUM_REAL_PLAYERS];
struct SunlightSaveInfo si;
vector<struct ThingSaveInfo> things;

// Functions
bool isLevelVersion3(std::ifstream& file);
void error(string errMsg);
bool loadLevel2(string filename);
bool saveLevel3(string filename);

int main(int argc, char* argv[]) {
	string file;

	if (argc > 1) {
		file = argv[1];
	}
	else {
		cout << "Please enter a file name (without extension): ";
		getline(cin, file);
	}

	if (!loadLevel2(file)) {
		error("Cannot open file!");
	}
	else {
		if (!saveLevel3(file)) {
			error("Cannot save file!");
		}
	}

	cout << "Completed Successfully!";
	return 0;
}

bool loadLevel2(string filename) {
	ostringstream ss;
	LevelHeader v2h;
	UBYTE dummy[MAP_XZ_SIZE*2];
	ThingSaveInfo t;

	// Load HDR
	ss << filename << ".hdr";
	ifstream HDR(ss.str(), ios::binary);

	if (HDR.is_open()) {
		HDR.read((char*)&v2h, sizeof(struct LevelHeader));
		HDR.close();
	}
	else {
		return false;
	}
	ss.str(""); // Clear stringstream

	// Convert Level2 to Level3
	memcpy(&h, &v2h, sizeof(struct LevelHeader));

	// Setup defaults
	h.MaxAltPoints = MAP_XZ_SIZE;
	h.MaxNumPlayers = MAX_NUM_REAL_PLAYERS;
	h.Version = LEVEL3_VERSION;
	h.MaxNumObjects = 0; // Not defined yet!

	ss << filename << ".dat";
	ifstream DAT(ss.str(), ios::binary);
	if (DAT.is_open()) {

		// Is file already version 3?
		if (isLevelVersion3(DAT)) {
			DAT.close();
			return false;
		}

		// Seek back top top because of MAGIC check.
		DAT.seekg(0, ios::beg);

		// Land height
		for (int i = 0; i < MAP_XZ_SIZE; i++) {
			if (!DAT.read((char*)&alt[i], sizeof(SWORD))) {
				DAT.close();
				return false;
			}
		}

		// Junk
		if (!DAT.read((char*)&dummy, ((MAP_XZ_SIZE * sizeof(UBYTE)) * 2))) { // Dont care, skip!
			DAT.close();
			return false;
		}

		// No Access Squares
		for (int i = 0; i < MAP_XZ_SIZE; i++) {
			if (!DAT.read((char*)&noAccessFlags[i], sizeof(UBYTE))) {
				DAT.close();
				return false;
			}
		}

		// PlayerInfo
		for (int i = 0; i < MAX_NUM_REAL_PLAYERS; i++) {
			if (!DAT.read((char*)&pi[i], sizeof(struct PlayerSaveInfo))) {
				DAT.close();
				return false;
			}
		}
		
		// Sunlight info
		if (!DAT.read((char*)&si, sizeof(struct SunlightSaveInfo))) {
			DAT.close();
			return false;
		}

		// Things!
		for (int i = 0; i < MAX_NUM_LEVEL_THINGS; i++) {
			if (!DAT.read((char*)&t, sizeof(struct ThingSaveInfo))) {
				DAT.close();
				return false;
			}

			if (t.Type != T_MODEL_NONE)
			{
				things.push_back(t);
			}
			else {
				break;
			}
		}

		// Set objects
		h.MaxNumObjects = things.size();

		// Make sure scripts is memset to 0.
		memset(&h.Script2, 0, sizeof(char[MAX_NUM_SCRIPT2][MAX_LENGTH_SAVE_NAMEv2]) );

		DAT.close();
	}
	else {
		return false;
	}
	return true;
}

bool saveLevel3(string filename) {
	ostringstream ss;
	ss << filename << ".dat";
	ofstream file(ss.str(), ios::trunc | ios::binary);

	if (file.is_open()) {
		file.write((char*)&MAGICDEF, MAGIC_SIZE);
		file.write((char*)&h, sizeof(struct LevelHeaderv3));
		file.write((char*)&alt, sizeof(SWORD) * h.MaxAltPoints);
		file.write((char*)&noAccessFlags, sizeof(UBYTE) * h.MaxAltPoints);
		file.write((char*)&pi, sizeof(struct PlayerSaveInfo) * h.MaxNumPlayers);
		file.write((char*)&si, sizeof(struct SunlightSaveInfo));

		for (int i = 0; i < things.size(); i++) {
			file.write((char*)&things[i], sizeof(struct ThingSaveInfo));
		}

		file.close();
	}
	else {
		return false;
	}
	return true;
}

bool isLevelVersion3(std::ifstream& file) {
	char MAGIC[MAGIC_SIZE];
	file.read((char*)&MAGIC, MAGIC_SIZE);

	for (int i = 0; i < MAGIC_SIZE; i++) {
		if (MAGIC[i] != MAGICDEF[i]) {
			return false; // MAGIC IS WRONG! 
		}
	}
	return true;
}

void error(string errMsg) {
	cerr << errMsg << "\n";
	exit(-1);
}