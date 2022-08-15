@echo off
flatc.exe --cpp -o include --filename-suffix _ STRUCT.fbs
flatc.exe --cpp -o include --filename-suffix _ SW_PLAYER_METADATA.fbs
flatc.exe --cpp -o include --filename-suffix _ SW_PLOTINFO.fbs
flatc.exe --cpp -o include --filename-suffix _ SW_BUFF.fbs
flatc.exe --cpp -o include --filename-suffix _ SW_DAMAGE_PLAYER.fbs
flatc.exe --cpp -o include --filename-suffix _ SW_COMBAT.fbs
flatc.exe --cpp -o include --filename-suffix _ SW_HISTORY.fbs
pause