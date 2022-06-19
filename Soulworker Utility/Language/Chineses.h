#pragma once

//https://i.imgur.com/dT4XLuh.png
#define STR_WORLD_NO_INFORMATION u8"未知地區"
#define PLAYER_NAME_CANT_FIND u8"未知玩家?"

//https://i.imgur.com/phiBmly.png
#define STR_MENU_RESUME u8"開始/暫停"
#define STR_MENU_RESET u8"清除"
#define STR_MENU_TOPMOST u8"顯示在最上層"
#define STR_MENU_HISTORY u8"舊記錄"
#define STR_MENU_OPTIONS u8"設定"
#define STR_MENU_MEOW u8"圖表"
#define STR_MENU_EXIT u8"離開"

// https://i.imgur.com/e0KEFOM.png
#define STR_MENU_FIT_COLUMN u8"調整欄位至最適大小"
#define STR_MENU_FIT_ALL_COLUMN u8"調整所有欄位至最適大小"
#define STR_MENU_RESET_ORDER u8"重設排序"

// https://i.imgur.com/0UqwGlV.png
#define STR_TABLE_YOU u8"我"
#define STR_TABLE_NAME u8"名稱"
#define STR_TABLE_DPS u8"每秒傷害"
#define STR_TABLE_DAMAGE_PERCENT u8"傷害比"
#define STR_TABLE_TOTAL_DAMAGE u8"總傷害"
#define STR_TABLE_TOTAL_HIT u8"總擊中"
#define STR_TABLE_CRIT_RATE u8"暴擊率"
#define STR_TABLE_HIT_PER_SECOND u8"每秒擊中"
#define STR_TABLE_CRIT_HIT_PER_SECOND u8"每秒暴擊"
#define STR_TABLE_SKILL_PER_SECOND u8"每秒技能使用數"
#define STR_TABLE_MAX_COMBO u8"最高連擊"
#define STR_TABLE_ATTACK_CDMG_SUM u8"攻爆和"
#define STR_TABLE_SOUL_GAUGE u8"靈魂值"
#define STR_TABLE_ATTACK_SPEED u8"攻速"
#define STR_TABLE_ARMOR_BREAK u8"貫穿"
#define STR_TABLE_STAMINA u8"耐力"
#define STR_TABLE_SOUL_VAPOR u8"覺醒值"
#define STR_TABLE_SOULSTONE_PERCENT u8"靈魂石總傷(%)"
#define STR_TABLE_SOULSTONE_PROC u8"靈魂石機率"
#define STR_TABLE_SOULSTONE_DAMAGE u8"靈魂石傷(%)"
#define STR_TABLE_AVERAGE_AB u8"均貫"
#define STR_TABLE_MiSS u8"攻擊失敗"
#define STR_TABLE_MISS_RATE u8"攻擊失敗(%)"
#define STR_TABLE_PARTIAL u8"攻擊失敗傷"
#define STR_TABLE_GET_HIT_INCLUDEZERO u8"受擊數(含無敵)"
#define STR_TABLE_GET_HIT u8"受擊數"
#define STR_TABLE_GET_HIT_BS u8"受擊數(BS)"
#define STR_TABLE_ACC1_BS u8"76飾1型"
#define STR_TABLE_ACC2_BS u8"76飾2型"
#define STR_TABLE_ACC3_BS u8"76飾3型"

#define STR_TABLE_EVADE_RATE_A u8"迴避率 A"
#define STR_TABLE_EVADE_RATE_B u8"迴避率 B"
#define STR_TABLE_GIGA_ENLIGHTEN u8"紅啟蒙"
#define STR_TABLE_TERA_ENLIGHTEN u8"紫啟蒙"
#define STR_TABLE_LOSED_HP u8"損失血量"
#define STR_TABLE_DODGE_COUNT u8"迴避數"
#define STR_TABLE_DEATH u8"死亡"

#define STR_TABLE_DURATION u8"持續時間" // buff,debuff

// https://i.imgur.com/YG64tR1.png
#define STR_OPTION_WINDOWS_NAME u8"設定"

#define STR_OPTION_FONTSCALE u8"文字放大倍率"
#define STR_OPTION_FONTSCALE_DESC u8"可根據視窗大小調整文字放大倍率"
#define STR_OPTION_UNIT_1K u8"以千單位顯示(千)"
#define STR_OPTION_UNIT_1M u8"以百萬單位顯示(百萬)"
#define STR_OPTION_SOLO_MODE u8"單人模式 (只顯示自己的數據)"
#define STR_OPTION_HIDE_NAME u8"隱藏名稱"
#define STR_OPTION_SAVE_DATA_AND_RESET_WHEN_BOSS_DIED u8"Boss死亡時保存資料並重置"

#define STR_OPTION_WINDOW_BORDER_SIZE u8"視窗邊框大小"
#define STR_OPTION_CELL_PADDING u8"表格間距"
#define STR_OPTION_COLUMN_FONT_SCALE u8"表格欄位文字放大倍率"
#define STR_OPTION_TABLE_FONT_SCALE u8"表格內容文字放大倍率"
#define STR_OPTION_TABLE_REFRESH_TIME u8"表格內容刷新時間(毫秒)"
#define STR_OPTION_TEXT_OUTLINE_COLOR u8"文字輪廓顏色"
#define STR_OPTION_ACTIVE_COLOR u8"運作時的標題欄顏色"
#define STR_OPTION_INACTIVE_COLOR u8"停止時的標題欄顏色"

#define STR_CHAR_UNKNOWN u8"未知玩家"
#define STR_CHAR_HARU u8"哈露"
#define STR_CHAR_ERWIN u8"歐文"
#define STR_CHAR_LILY u8"莉莉"
#define STR_CHAR_JIN u8"金"
#define STR_CHAR_STELLA u8"史黛菈"
#define STR_CHAR_IRIS u8"伊莉絲"
#define STR_CHAR_CHII u8"琪"
#define STR_CHAR_Ephnel u8"艾芙妮爾"
#define STR_CHAR_NABI u8"李娜飛"

#define STR_OPTION_RESTORE_DEFAULT_COLOR u8"恢復預設顏色"

#define STR_OPTION_HOTKEY_DESC_1 u8"開始/暫停 : Ctrl + End\n"
#define STR_OPTION_HOTKEY_DESC_2 u8"清除 : CTRL + DEL\n\n"
#define STR_OPTION_HOTKEY_DESC_3 u8"您可以通過編輯 option.XML 來編輯熱鍵\n"
#define STR_OPTION_HOTKEY_DESC_4 u8"key 對應的值來自 DirectInput Code Table\n"
#define STR_OPTION_HOTKEY_DESC_5 u8"熱鍵的設定從 key1 開始 (接著 key2...)\n如不需要使用 key2/key3 請設定成 -1"

#define STR_OPTION_TEST_VALUE_PLAYER u8"玩家"
#define STR_OPTION_ADD_TEST_VALUE u8"加入測試資料"
#define STR_OPTION_SAVE_AND_EXIT u8"保存並退出設定"

#define STR_OPTION_TAB_TABLE_SETTING u8"表單"
#define STR_OPTION_TAB_HOTKEY_SETTING u8"熱鍵"

// https://i.imgur.com/OBnBsek.png
#define STR_DETAIL_DETAIL u8"詳細資料"
#define STR_DETAIL_SKILL u8"技能"
#define STR_DETAIL_MONSTER u8"怪物"
#define STR_DETAIL_BUFF_AND_DEBUFF u8"增益/減益狀態"

#define STR_UTILWINDOW_DPSGRAPH u8"DPS變化圖"
#define STR_UTILWINDOW_DPSGRAPH_TIME_SEC u8"時間(秒)"
#define STR_UTILWINDOW_ABGRAPH u8"貫穿變化圖"
#define STR_UTILWINDOW_ABGRAPH_TIME_SEC u8"時間(秒)"
#define STR_UTILWINDOW_JQGRAPH u8"女王層數變化圖"
#define STR_UTILWINDOW_JQGRAOH_TIME_SEC u8"時間(秒)"
#define STR_UTILWINDOW_BOSSHPGRAPH u8"Boss血量變化圖"
#define STR_UTILWINDOW_BOSSHPGRAPH_TIME_SEC u8"時間(秒)"
#define STR_UTILWINDOW_BOSSHPGRAPH_UNIT u8"單位(百萬)"

#define STR_UTILWINDOW_COMBATLOG u8"戰鬥紀錄"

#define STR_SPECIFICINFO_TOTAL u8"統計"
#define STR_SPECIFICINFO_USE_SKILL_COUNTS u8"使用次數"
#define STR_SPECIFICINFO_BUFF_NAME u8"效果"