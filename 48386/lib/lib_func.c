#include "lib.h"
/*
 * This library project exports its symbols:
 * Options -> Build actions -> Post-build command line:
 * $TOOLKIT_DIR$\bin\isymexport.exe 
 *    "$TARGET_PATH$" "$PROJ_DIR$\protected_lib.symbols"
 *  --edit "$PROJ_DIR$\steering_file.txt"
 *
*/

//extern void __iar_data_init3(void);

//__root const struct lib_func libary_entry @ LIB_ENTRY_ADDR =
//{
//  
//  (lib_init_f) lib_init
//  
//};
   
__root int lib_init(void)
{
    /* Perform zero initializion and 
    *  copy initialized data from ROM to RAM */

    //__iar_data_init3();

    return 0;
}
