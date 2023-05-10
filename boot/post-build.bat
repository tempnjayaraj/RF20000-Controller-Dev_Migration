:: ewarm elf image
set OUT=%1.out
:: binary image with address info
set HEX=%1.hex
:: raw binary image without address info
set BIN=%1.bin

:: calculate checksum of the application
ielftool --fill 0xFF;0x412-checksum_end+3 --checksum __checksum_main:2,crc16,0x0;0x412-checksum_end+3 --verbose %OUT% %OUT%

:: generate additional ouput: hex
ielftool.exe --ihex --verbose %OUT% %HEX%

:: generate additional ouput: binary
ielftool.exe --bin --verbose %OUT% %BIN%

