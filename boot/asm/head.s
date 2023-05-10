
	SECTION .text:CODE

        PRESERVE8

	PUBLIC head
        
        EXTERN BOOT_CODE$$Length
	
	DATA

head:
	DC8     'RF20000B'              ; Name of application
        DC32    BOOT_CODE$$Length
        DC8     __DATE__                ; Date code was complied
        DC8     __TIME__,0,0,0          ; Time code was complied
        DC32    __VER__                 ; Version of complier
        DC16    0x0100                ; Version of application minor/major

	END
