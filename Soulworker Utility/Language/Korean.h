#pragma once

//https://i.imgur.com/dT4XLuh.png
#define STR_WORLD_NO_INFORMATION u8"공백"
#define PLAYER_NAME_CANT_FIND u8"너 누구야?"

//https://i.imgur.com/phiBmly.png
#define STR_MENU_RESUME u8"시작/정지"
#define STR_MENU_RESET u8"초기화"
#define STR_MENU_TOPMOST u8"항상 위에 두기"
#define STR_MENU_HISTORY u8"전투내역"
#define STR_MENU_OPTIONS u8"옵션"
#define STR_MENU_EXIT u8"종료"

// https://i.imgur.com/e0KEFOM.png
#define STR_MENU_FIT_COLUMN u8"칸을 크기에 맞추기"
#define STR_MENU_FIT_ALL_COLUMN u8"모든칸을 크기에 맞추기###SizeAll"
#define STR_MENU_RESET_ORDER u8"순서 초기화"

// https://i.imgur.com/0UqwGlV.png
#define STR_TABLE_NAME u8"NAME"
#define STR_TABLE_DPS u8"DPS"
#define STR_TABLE_DAMAGE_PERCENT u8"D%"
#define STR_TABLE_TOTAL_DAMAGE u8"DAMAGE"
#define STR_TABLE_TOTAL_HIT u8"HIT"
#define STR_TABLE_CRIT_RATE u8"CRIT%"
#define STR_TABLE_HIT_PER_SECOND u8"HIT/s"
#define STR_TABLE_CRIT_HIT_PER_SECOND u8"C.HIT/s"
#define STR_TABLE_SKILL_PER_SECOND u8"Skill/s"
#define STR_TABLE_MAX_COMBO u8"MAXC"
#define STR_TABLE_ATTACK_CDMG_SUM u8"공치합"
#define STR_TABLE_SOUL_GAUGE u8"SG"
#define STR_TABLE_ATTACK_SPEED u8"공속"
#define STR_TABLE_ARMOR_BREAK u8"방관"
#define STR_TABLE_STAMINA u8"스태"
#define STR_TABLE_SOUL_VAPOR u8"SV"
#define STR_TABLE_SOULSTONE_PERCENT u8"돌전체비중%"
#define STR_TABLE_SOULSTONE_PROC u8"돌확률%"
#define STR_TABLE_SOULSTONE_DAMAGE u8"돌데미지%"
#define STR_TABLE_AVERAGE_AB u8"평균방관"
#define STR_TABLE_MiSS u8"Miss"
#define STR_TABLE_MISS_RATE u8"Miss%"
#define STR_TABLE_PARTIAL u8"Partial"
#define STR_TABLE_GET_HIT_INCLUDEZERO u8"피격(0뎀포함)"
#define STR_TABLE_GET_HIT u8"피격"
#define STR_TABLE_GET_HIT_BS u8"피격(브세)"
#define STR_TABLE_ACC1_BS u8"악세1"
#define STR_TABLE_ACC2_BS u8"악세2"
#define STR_TABLE_ACC3_BS u8"악세3"

#define STR_TABLE_EVADE_RATE_A u8"회피율A"
#define STR_TABLE_EVADE_RATE_B u8"회피율B"
#define STR_TABLE_GIGA_ENLIGHTEN u8"기가인라"
#define STR_TABLE_TERA_ENLIGHTEN u8"테라인라"

#define STR_TABLE_LOSED_HP u8"잃은HP"
#define STR_TABLE_DODGE_COUNT u8"회피기"


#define STR_TABLE_DURATION u8"지속시간" // buff,debuff

// https://i.imgur.com/YG64tR1.png
#define STR_OPTION_WINDOWS_NAME u8"설정"

#define STR_OPTION_FONTSCALE u8"폰트 크기"
#define STR_OPTION_FONTSCALE_DESC u8"폰트 크기를 조정하여 테이블 크기를 조정 할 수 있습니다."
#define STR_OPTION_UNIT_1K u8"단위 설정(1K)"
#define STR_OPTION_UNIT_1M u8"단위 설정(1M)"
#define STR_OPTION_SOLO_MODE u8"1인 모드"
#define STR_OPTION_HIDE_NAME u8"이름 숨기기"

#define STR_OPTION_WINDOW_BORDER_SIZE u8"창 외곽선"
#define STR_OPTION_CELL_PADDING u8"셀 패딩"
#define STR_OPTION_COLUMN_FONT_SCALE u8"열 폰트 스케일"
#define STR_OPTION_TABLE_FONT_SCALE u8"테이블 폰트 스케일"
#define STR_OPTION_TABLE_REFRESH_TIME u8"테이블 갱신시간"
#define STR_OPTION_TEXT_OUTLINE_COLOR u8"글자 외곽선 색상"
#define STR_OPTION_ACTIVE_COLOR u8"활성화 색상"
#define STR_OPTION_INACTIVE_COLOR u8"비활성화 색상"

#define STR_CHAR_UNKNOWN u8"Unknown"
#define STR_CHAR_HARU u8"하루"
#define STR_CHAR_ERWIN u8"어윈"
#define STR_CHAR_LILY u8"릴리"
#define STR_CHAR_JIN u8"진"
#define STR_CHAR_STELLA u8"스텔라"
#define STR_CHAR_IRIS u8"이리스"
#define STR_CHAR_CHII u8"치이"
#define STR_CHAR_Ephnel u8"에프넬"
#define STR_CHAR_NABI u8"이나비"

#define STR_OPTION_RESTORE_DEFAULT_COLOR u8"기본 색상으로 바꾸기"

#define STR_OPTION_HOTKEY_DESC_1 u8"기본 일시정지/재개 키는 CTRL + END 키입니다.\n"
#define STR_OPTION_HOTKEY_DESC_2 u8"기본 초기화 키는 CTRL + DEL 키입니다.\n"
#define STR_OPTION_HOTKEY_DESC_3 u8"현재는 option.XML에서 직접 수정해야 합니다.\n"
#define STR_OPTION_HOTKEY_DESC_4 u8"각 키의 값은 DirectInput Code Table이 기준입니다.\n"
#define STR_OPTION_HOTKEY_DESC_5 u8"각 키는 key1부터 순서대로 넣되, 할당하지 않을 키는 -1로 넣어야 합니다.\n";

#define STR_OPTION_TEST_VALUE_PLAYER u8"플레이어"
#define STR_OPTION_ADD_TEST_VALUE u8"더미 데미지 추가"
#define STR_OPTION_SAVE_AND_EXIT u8"저장하고 종료하기"

#define STR_OPTION_TAB_TABLE_SETTING u8"Table 설정"
#define STR_OPTION_TAB_HOTKEY_SETTING u8"Hotkey 설정"

// https://i.imgur.com/OBnBsek.png
#define STR_DETAIL_DETAIL u8"상세정보"
#define STR_DETAIL_SKILL u8"스킬"
#define STR_DETAIL_MONSTER u8"몬스터"
#define STR_DETAIL_BUFF_AND_DEBUFF u8"버프/디버프 (미완성)1"

#define STR_UTILWINDOW_MEOW u8"애옹한접시"

#define STR_UTILWINDOW_DPSGRAPH u8"DPS 그래프"
#define STR_UTILWINDOW_DPSGRAPH_TIME_SEC u8"시간(초)"
#define STR_UTILWINDOW_ABGRAPH u8"방관 그래프"
#define STR_UTILWINDOW_ABGRAPH_TIME_SEC u8"시간(초)"
#define STR_UTILWINDOW_JQGRAPH u8"정퀸 스택 그래프"
#define STR_UTILWINDOW_JQGRAOH_TIME_SEC u8"시간(초)"

#define STR_UTILWINDOW_COMBATLOG u8"전투 로그"