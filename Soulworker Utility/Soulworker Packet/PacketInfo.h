#pragma once

// enum STAT_TYPE
enum StatType {
	None = 0x00,

	CurrentHP = 0x01,
	SG = 0x02,
	Stamina = 0x03,
	STR = 0x04,
	DEX = 0x05,
	INTELLIGENCE = 0x06,
	BALANCE = 0x07,
	CON = 0x08,
	LUK = 0x09,

	MaxHP = 0x0a,
	HPRegen = 0x0b,

	MaxSG = 0x0c,
	SGRegen = 0x0d,

	MaxStamina = 0x0e,
	StaminaRegen = 0x0f,

	SV = 0x10,
	MaxSV = 0x11,

	MoveSpeed = 0x12,
	AttackSpeed = 0x13,

	MinAttack = 0x14,
	MaxAttack = 0x15,
	MinMagicAttack = 0x16,
	MaxMagicAttack = 0x17,

	Defense = 0x18,
	MagicDefense = 0x19,

	Accuracy = 0x1a,
	MagicAccuracy = 0x1b,

	PartialDamage = 0x1c,

	Crit = 0x1d,
	MagicCrit = 0x1e,
	CritResist = 0x1f,
	MagicCritResist = 0x20,

	/* Use Crit, MagicCrit Instead */
	CritChance = 0x21,
	MagicCritChance = 0x22,

	CritDamage = 0x23,
	MagicCritDamage = 0x24,

	// IDK
	DAR = 0x25,

	DamageReduction = 0x26,
	MagicDamageReduction = 0x27,

	// 0x28 ~ 0x2A : EMPTY

	Evade = 0x2B,
	MagicEvade = 0x2C,

	AttackSuccessRate = 0x2D,
	MagicAttackSuccessRate = 0x2E,

	ArmorBreak = 0x2F,
	MagicArmorBreak = 0x30,

	ResistBurn = 0x31,
	ResistPoison = 0x32,
	ResistShock = 0x33,
	ResistBleed = 0x34,
	ResistStun = 0x35,
	ResistParalysis = 0x36,
	ResistSleep = 0x37,
	ResistFreeze = 0x38,
	ResistCharm = 0x39,
	ResistConfusion = 0x3A,
	ResistSilence = 0x3B,
	ResistWeakness = 0x3C,

	ItemAttack = 0x3D,
	ItemDefense = 0x3E,

	SoulStone_Light = 0x3F,
	SoulStone_Darkness = 0x40,
	SoulStone_Cool = 0x41,
	SoulStone_Abhor = 0x42,
	SoulStone_Heal = 0x43,
	SoulStone_Pain = 0x44,

	ResistSoulStone_Light = 0x45,
	ResistSoulStone_Darkness = 0x46,
	ResistSoulStone_Cool = 0x47,
	ResistSoulStone_Abhor = 0x48,
	ResistSoulStone_Heal = 0x49,
	ResistSoulStone_Pain = 0x4A,

	PvPAttack = 0x4B,
	PvPDefense = 0x4C,

	MoveSpeedNoFix = 0x4D,
	AttackSpeedNoFix = 0x4E,

	MAX_STAT,
	END
};

// enum EFFECT_STAT_TYPE
enum SpecialStatType {

	CoolTime_Rate = 0x00,

	PvPDamageIncreaseInt = 0x01,
	PvPDamageDecreaseInt = 0x02,

	NormalDamageAddRate = 0x03,
	BossDamageAddRate = 0x04,
	MeleeDamageAddRate = 0x05,
	RangedDamageAddRate = 0x06,
	AirDamageAddRate = 0x07,
	DownDamageAddRate = 0x08,

	NoramlDamagedDownRate = 0x09,
	BossDamagedDownRate = 0x0A,
	MeleeDamagedDownRate = 0x0B,
	RangedDamagedDownRate = 0x0C,

	SADamageAddRate = 0x0D,

	CostDownRate = 0x0E,

	ExpAddRate = 0x0F,
	GoldAddRate = 0x10,
	SvAddRate = 0x11,

	EnduranceFixed = 0x12,
	Gesture = 0x13,
	DamagedEffect = 0x14,

	HpAbsorbRate = 0x15,
	SgAbsorbRate = 0x16,

	AttackDebuff = 0x17,
	DamagedDebuff = 0x18,

	DamageReflectionRate = 0x19,

	SjunkDamageAddRate = 0x1A,
	DollDamageAddRate = 0x1B,
	NedDamageAddRate = 0x1C,
	RapidDamageAddRate = 0x1D,
	NihilDamageAddRate = 0x1E,
	IronDamageAddRate = 0x1F,
	NearDamageAddRate = 0x20,
	Dis6DamageAddRate = 0x21,

	SjunkDamageDownRate = 0x22,
	DollDamageDownRate = 0x23,
	NedDamageDownRate = 0x24,
	RapidDamageDownRate = 0x25,
	NihilDamageDownRate = 0x26,
	IronDamageDownRate = 0x27,
	NearDamageDownRate = 0x28,
	Dis6DamageDownRate = 0x29,
	MissDamageDownRate = 0x2A,
	AirDamageDownRate = 0x2B,
	DownDamageDownRate = 0x2C,

	StaminaAttackRate = 0x2D,

	AttackBuff = 0x2E,
	DamagedBuff = 0x2F,
	EvadeBuff = 0x30,

	SpeedIncreaseInDistrict = 0x31,
	SpeedIncreaseInMaze = 0x32,

	CitadelDamageAddRate = 0x33,
	CitadelDamageDownRate = 0x34,
	PrimalDamageAddRate = 0x35,
	PrimalDamagedDownRate = 0x36,

	ParallelMetronomeAddRate = 0x37,
	ParallelMetronomeDownRate = 0x38,

	End
};

#pragma pack(push, 1)
typedef struct _SWPACKETDAMAGE_DAMAGETYPE {
	unsigned int MISS : 1; // Gamnabit 0x01
	unsigned int unknown2 : 1;
	unsigned int CRIT : 1; // critical 0x04
	unsigned int unknown4 : 1;
	unsigned int soulstoneType : 4;

	_SWPACKETDAMAGE_DAMAGETYPE(BYTE rawData) {
		MISS = rawData & 0x01 ? 1 : 0;
		unknown2 = rawData & 0x02 ? 1 : 0;
		CRIT = rawData & 0x04 ? 1 : 0;
		unknown4 = rawData & 0x08 ? 1 : 0;
		soulstoneType = rawData >> 4;
	};
}SWPACKETDAMAGE_DAMAGETYPE;
#pragma pack(pop)