#pragma once
#include "pch.h"
#include <unordered_set>

static unordered_set<UINT32> pauseIdList({

	702, // Trainning Master

	/* LF */
	31309102, // Azure Force. Edgar

	/* VS */
	31301001, // Flemma P1
	31301002, // Flemma P2

	/* VS Solo */
	33301001, // Flemma P1
	33301002, // Flemma P2

	/* BS */
	31310101, // Tenebris P1
	31310103, // BS Hard Phase 1

	/* BS Solo */
	32320101, // Tenebris P1

	});

static unordered_set<UINT32> endIdList({

	/* HH */
	31308001, // Raphakumba
	
	/* LF */
	31309151, // Junk Queen

	/* VS */
	31301003, // Flemma P3

	/* VS Solo */
	33301003, // Flemma P3

	/* BS */
	31310102, // Tenebris P2
	31310107, // Tenebris Hard P2

	/* BS Solo */
	32320102, // Tenebris P2

	/* RANK */
	32308101, // Raphakumba Rank
	32301001, // Flemma Rank

	});

static unordered_set<UINT32> resumeIgnoreIdList({

	/* AR */
	14000101, 14000102, 14000103, // Summoned Orb

	/* SKILL */
	1081102, 1081103, 1081104, 1081105, 1081106, 1081107, 1081108, 1081109, // Field Octagon(Ephnel)
	1091101, // Bomb Wick(Nabi)

	});

static unordered_set<UINT32> changeAggroIdList({

	701, 702, // Trainning Master

	/* HH */
	31308001, // Raphakumba

	/* LF */
	31309102, // Azure Force. Edgar
	31309151, // Junk Queen

	/* VS */
	31301001, 31301002, 31301003, // Flemma

	/* BS */
	31310101, 31310102, // Tenebris
	31310103, 31310107, // Tenebris Hard

	});

static unordered_set<UINT32> dpsIgnoreIdList({

	/* HH */
	31308106, // Hermit
	31308102, 31308103, 31308104, // Totem
	31308105, 31308112, 31308111, 31308114, 31308115, // defilement
	31308110, 31308113, 31308116, // Clone of Raphakumba

	/* LF */
	31309109, 31309154, // Queen's Sword

	/* BS */
	31310111, 31310112, // Tenebris
	31310116, 31310117, // Flame of Tenebris
	31310151, 31310152, 31310153, 31310154, // Mobs
	31310192, // Reward box

	/* BS Solo */
	32320103, 32320104, 32320105, 32320106, // Mobs
	32320107, 32320108, 32320109, 32320114, // Fake Tenebris
	32320110, 32320111, // Flame of Tenebris
	32320112,  // Hazy Beam
	32320113, 32320115, // Reward box

	/* BSVH */
	31310118, 31310119, // Desire Energy Fragment, Hazy Beam

	/* Rocco Town Hero Mob */
	34401502, 34401503, 34401504,
	34401702,
	34401802, 34401803,

	/* AR */
	14000101, 14000102, 14000103, // Summoned Orb

	/* SKILL */
	1081102, 1081103, 1081104, 1081105, 1081106, 1081107, 1081108, 1081109, // Field Octagon(Ephnel)
	1091101 // Bomb Wick(Nabi)

	});

static unordered_set<UINT32> LunarFallBossListId({

	701, 702, // Trainning Master

	/* LF */
	31309101, // Azure Force. Arculus
	31309102, // Azure Force. Edgar
	31309151, // Junk Queen

	});