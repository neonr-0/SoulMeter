#pragma once
#include "Third Party\SQLite\include\sqlite3.h"

#define SWDB MySQL::getInstance()
#define SWDBPATH "SWDB.db"

#ifdef _DEBUG
#define DEBUG_SKILL_NAME 0
#define DEBUG_MONSTER_NAME 0
#endif

class MySQL : public Singleton<MySQL> {
private:
	sqlite3* _db;
	sqlite3* _memdb;

	BOOL InitDB();
	BOOL InitMemDB();
	VOID FreeDB();

	BOOL InitSkillDB();
	BOOL InitMonsterDB();
	BOOL InitMapDB();
	BOOL InitBuffDB();
	BOOL InitSkillTimelineDB();
	BOOL InitBuffTimelineDB();

	sqlite3_stmt* _skill_stmt;
	sqlite3_stmt* _monster_stmt;
	sqlite3_stmt* _map_stmt;
	sqlite3_stmt* _buff_stmt;

public:
	MySQL();
	~MySQL();

	BOOL Init();
	BOOL GetSkillName(UINT32 skillId, _Out_ CHAR* out_buffer, _In_ SIZE_T out_buffer_length);
	BOOL GetMonsterName(UINT32 DB2, _Out_ CHAR* out_buffer, _In_ SIZE_T out_buffer_length);
	BOOL GetMonsterType(UINT32 DB2, INT32* type);
	BOOL GetMapName(UINT32 mapId, _Out_ CHAR* out_buffer, _In_ SIZE_T out_buffer_length);
	BOOL GetBuffName(UINT32 buffId, _Out_ CHAR* out_buffer, _In_ SIZE_T out_buffer_length, _Out_ CHAR* out_buffer_desc = NULL, _In_ SIZE_T out_buffer_desc_length = NULL);
};