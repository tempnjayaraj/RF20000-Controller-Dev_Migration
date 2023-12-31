@REM This batch file has been generated by the IAR Embedded Workbench
@REM C-SPY Debugger, as an aid to preparing a command line for running
@REM the cspybat command line utility using the appropriate settings.
@REM
@REM Note that this file is generated every time a new debug session
@REM is initialized, so you may want to move or rename the file before
@REM making changes.
@REM
@REM You can launch cspybat by typing the name of this batch file followed
@REM by the name of the debug file (usually an ELF/DWARF or UBROF file).
@REM
@REM Read about available command line parameters in the C-SPY Debugging
@REM Guide. Hints about additional command line parameters that may be
@REM useful in specific cases:
@REM   --download_only   Downloads a code image without starting a debug
@REM                     session afterwards.
@REM   --silent          Omits the sign-on message.
@REM   --timeout         Limits the maximum allowed execution time.
@REM 


"C:\CAEAPPS\IAR Systems\Embedded Workbench 6.5\common\bin\cspybat" "C:\CAEAPPS\IAR Systems\Embedded Workbench 6.5\arm\bin\armproc.dll" "C:\CAEAPPS\IAR Systems\Embedded Workbench 6.5\arm\bin\armjlink.dll"  %1 --plugin "C:\CAEAPPS\IAR Systems\Embedded Workbench 6.5\arm\bin\armbat.dll" --macro "C:\CAEAPPS\IAR Systems\Embedded Workbench 6.5\arm\config\debugger\Freescale\Trace_Kxx.dmac" --flash_loader "C:\CAEAPPS\RF20000-Controller\branch\phase2_vv\boot\iar\FlashK60Fxxx128K.board" --backend -B "--endian=little" "--cpu=Cortex-M4" "--fpu=None" "-p" "C:\CAEAPPS\IAR Systems\Embedded Workbench 6.5\arm\CONFIG\debugger\Freescale\MK60FN1M0xxx15.ddf" "--semihosting" "--device=MK60FN1M0xxx15" "--drv_communication=USB0" "--jlink_speed=auto" "--jlink_initial_speed=32" "--jlink_reset_strategy=0,0" "--drv_catch_exceptions=0x000" "--drv_swo_clock_setup=74000000,1,2000000" 


