:: $TOOLKIT_DIR$\bin is not necessary since installation directory is added to path by default

:: export symbols
:: $TARGET_PATH$=%2, $PROJ_DIR$=%3
::isymexport.exe "$TARGET_PATH$" "$PROJ_DIR$\audio_lib.symbols" --edit "$PROJ_DIR$\sym_export.txt"
isymexport.exe "%2" "%3\audio_lib.symbols" --edit "%3\sym_export.txt"

set OUT=%1.out
set HEX=%1.hex
set BIN=%1.bin

:: calculate checksum of the application
::ielftool --fill 0xFF;0x70000-0xFFFFD --checksum __checksum_main:2,crc16,0x0;0x70000-0xFFFFD --verbose %OUT% %OUT%

:: generate additional ouput: hex
::ielftool.exe --ihex --verbose %OUT% %HEX%

:: generate additional ouput: binary
ielftool.exe --bin --verbose %OUT% %BIN%


