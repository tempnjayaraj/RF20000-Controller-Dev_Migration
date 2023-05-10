
	SECTION .text:CODE

        AAPCS INTERWORK, ROPI, RWPI_COMPATIBLE,  VFP_COMPATIBLE
        PRESERVE8

	PUBLIC meta
        PUBLIC params

        EXTERN ROM_address
	EXTERN start
	EXTERN ROPI$$Length
	EXTERN RWPI_DATA$$Length
        EXTERN RWPI_ZI$$Length
        
	RSEG header:CONST:ROOT(2)
        DATA


meta:
        DC32	ROM_address             ; ROM address, defined in application.icf
	DC32	ROPI$$Length            ; Number of RO bytes, including this header.
	DC32	RWPI_DATA$$Length       ; Number of RW bytes.
        DC32    RWPI_ZI$$Length
	DC8     'RF20000P'              ; Name of application
        DC8     __DATE__                ; Date code was complied
        DC8     __TIME__                ; Time code was complied
        DC32     __VER__                ; Version of complier
        DC16    0x0008                  ; Version of application minor/major
        DC32    start - . - 4           ; Offset from 1st byte after this header
                                        ;   to the first function.
                                        
params:
        DCD  0xFFFFFFFE                 ; wireless pair timeout seconds      

        END
