靈魂行者Steam版本傷害計算器
---

本程式運作方式為通過第三方軟體擷取網路卡中的遊戲封包，並透過SWCrypt.dll進行封包解密後分析資料

目前支援的擷取封包第三方軟體: *Npcap(推薦)、WinDivert(預設，可能會導致數據異常、程式崩潰)

*Npcap: 下載Npcap installer(1.60以上)並安裝Npcap後([https://npcap.com/#download](https://npcap.com/#download))至設定內開啟



操作說明
---------------------

快捷鍵 CTRL + DEL : 清除資料

右鍵標題欄可查看更多功能

左鍵點擊表格內角色名稱可查看詳細資料

option.xml 與 imgui.ini 為使用者設定


Q&A
---------------------

Q: 無法顯示中文

A: 請將中文字體文件(.ttf/.ttc)放到Font資料夾內

Q: 升級到新版時如何保存舊版本的設定

A: 複製option.xml 與 imgui.ini 到新版本的資料夾


注意事項
---------------------
使用本程式時需於進入副本前開啟，使用本程式造成之風險請自行負責

建議盡量不要於遊戲內公開談論本程式

問題回報及功能建議群組: https://discord.gg/PN3VmS98bt

如果無法開啟，請嘗試安裝此整合包: https://github.com/abbodi1406/vcredist/releases

特別感謝 @Nyanchii、@FeAr


---
Soulworker DPS Meter for Global Server
---

This software is used third-party software to capture network raw packet and use SWCrypt.dll to decrypt packet

Supported capture software: *Npcap(Recommended), WinDivert(Default, The data will be incorrect or DPS crash)

*Npcap: You need goto setting to enable this and Download Npcap installer(Need 1.60 or above) and install ([https://npcap.com/#download](https://npcap.com/#download))



How to use
---------------------

Hotkey - CTRL + DEL : Reset

Right click title bar can display more features

Can get player details with left click table of character name row

option.xml and imgui.ini is user settings


Q&A
---------------------

Q: Can't display chinese

A: Put chinese font file to Font folder.

Q: how to keep settings when upgrading to new version

A: Copy option.xml and imgui.ini to new version folder


Notice
---------------------
You need to run dps meter before entering maze

Also, USE AT YOUR OWN RISK

I recommend not to talk about dps meter at in-game public chat

Issue and suggest: https://discord.gg/PN3VmS98bt

If you can't open, try install this: https://github.com/abbodi1406/vcredist/releases

Special thanks @Nyanchii, @FeAr


---
Below content is Readme from original repo [Park3740/SoulMeter]

국내에서 서비스 하고 있는 소울워커 RPG의 DPS meter 입니다.

이 프로그램은 클라이언트와 서버간의 주고 받는 패킷을 캡쳐하여 정보를 집계합니다.
Windivert를 기본으로 사용하고 있으며, 관리자 권한이 필요합니다.
Windivert가 문제가 있다면 대안으로 Npcap을 사용하지만, Npcap 실행을 위한 파일은 제공하지 않습니다.

User Iterface로 ImGui를 사용하지만 Outlined, Aligned Text를 위해 소스코드를 조금 수정하여 사용하였습니다.

사용시 주의사항
---------------------
1. 본 프로그램에 사용 적발 시 정지를 당할 수 있습니다. (첫 적발 시 1일 이용 정지)
2. 본 프로그램 사용으로 인한 불이익을 개발자는 책임지지 않습니다.
3. 파티, 포스인 상태로 메이즈를 이동해야 닉네임을 표시할 수 있습니다.

---------------------
개인 사정상 더 이상 업데이트가 불가능할 것 같습니다.    
다만 다른 분이 새로운 프로그램을 작성하시고 있으니 그 프로그램이 완성되면 그 링크를 올리겠습니다.    
감사합니다.
