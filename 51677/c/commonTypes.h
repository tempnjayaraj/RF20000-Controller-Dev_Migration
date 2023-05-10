/**
 * @file commonTypes.h
 * @brief Base / Global Types definitions for bdm
 */

// c++ header 
#ifndef __COMMON_TYPES_H__
#define __COMMON_TYPES_H__

//#define RTOSSOFTWAREBUILD
#include "cpu.h"
/**
 * undefine things that don't make sense 
 */
//#undef true
//#undef false
//#undef TRUE
//#undef FALSE
//#undef bool      

/**
 * define the boolean functionality
 */
//#define true  1  /**< true ==  1 */
//#define false 0  /**< false == 0 */
//#define TRUE  1  /**< TRUE ==  1 */
//#define FALSE 0  /**< FALSE == 0 */

/**
 * MACROs to manipulate & display boolean values"
 */
#define DISPLAY_BOOLEAN_STRING(x) (x ? "TRUE" : "FALSE") /**< macros for boolean */
#define BIT_MASK(x)               (1 << x)

/**
 * typedef some shorter definitions
 */
typedef uint64_t u64;   /**< typedef to a shorter abbreviation */

#define THREAD_STACK_SIZE           (65536/4)     /**< default thread size            */

/**
 * Debug Printf MACRO
 */
#undef  DEBUG_PRINTF
#define DEBUG_PRINTF
#ifdef DEBUG_PRINTF
#define DBGF(fmt,args...) { printf(fmt, ##args);FLUSH_SLEEP; } /**< Debug Print to printf */
#define DBG(fmt,args...)  { printf(fmt, ##args); }             /**< Debug Print to printf */
#else
#define DBGF(fmt,args...) 
#define DBG(fmt,args...)  
#endif

/**
 * define sleep/yield functionality
 */
// utitilities to sleep and yield ;
#define SLEEP_uS(x)  { sched_yield(); usleep(x);  }  /**< macro for sleeping for x microseconds */
#define FLUSH_SLEEP  { fflush(NULL); SLEEP_uS(0); }  /**< fflush and then yield                 */
#define YIELD_1MS    { SLEEP_uS(1000);   }           /**< yield/sleep MS                        */
#define YIELD_2MS    { SLEEP_uS(2000);   }           /**< yield/sleep MS                        */
#define YIELD_3MS    { SLEEP_uS(3000);   }           /**< yield/sleep MS                        */
#define YIELD_4MS    { SLEEP_uS(4000);   }           /**< yield/sleep MS                        */
#define YIELD_5MS    { SLEEP_uS(5000);   }           /**< yield/sleep MS                                */
#define YIELD_10MS   { SLEEP_uS(10000);  }           /**< yield/sleep MS                                */
#define YIELD_20MS   { SLEEP_uS(20000);  }           /**< yield/sleep MS                                */
#define YIELD_25MS   { SLEEP_uS(25000);  }           /**< yield/sleep MS                                */
#define YIELD_30MS   { SLEEP_uS(30000);  }           /**< yield/sleep MS                                */
#define YIELD_40MS   { SLEEP_uS(40000);  }           /**< yield/sleep MS                                */
#define YIELD_50MS   { SLEEP_uS(50000);  }           /**< yield/sleep MS                                */
#define YIELD_100MS  { SLEEP_uS(100000); }           /**< yield/sleep MS                                */
#define YIELD_125MS  { SLEEP_uS(125000); }           /**< yield/sleep MS                                */
#define YIELD_150MS  { SLEEP_uS(150000); }           /**< yield/sleep MS                                */
#define YIELD_200MS  { SLEEP_uS(200000); }           /**< yield/sleep MS                                */
#define YIELD_250MS  { SLEEP_uS(250000); }           /**< yield/sleep MS                                */
#define YIELD_300MS  { SLEEP_uS(300000); }           /**< yield/sleep MS                                */
#define YIELD_350MS  { SLEEP_uS(350000); }           /**< yield/sleep MS                                */
#define YIELD_400MS  { SLEEP_uS(400000); }           /**< yield/sleep MS                                */
#define YIELD_450MS  { SLEEP_uS(400000); }           /**< yield/sleep MS                                */
#define YIELD_500MS  { SLEEP_uS(500000); }           /**< yield/sleep MS                                */

/**
 * defines basic numbers
 */
#define THOUSAND                   (1000LL)        /**< defines a thounsad objects       */
#define MILLION                    (1000LL*1000LL) /**< defines a million objects        */
#define BILLION                    (1000LL*1000LL*1000LL) /**< defines billion objects   */

#define THOUSAND2                  (1024LL)         /**< defines programming thousand    */
#define MILLION2                   (1024LL*1024LL)  /**< defines one million bytes       */
#define BILLION2                   (1024LL*1024LL*1024LL) /**< defines one billion bytes   */

/**
 * EASY MULTIPLIERS 
 */ 
#define MICROSECS_PER_SEC         (MILLION)               /**< easy multiplier             */
#define MILLISECS_PER_SEC         (THOUSAND)              /**< easy multiplier             */
#define NANOSECS_PER_MS           (MILLION)               /**< easy multiplier             */
#define NANOSECS_PER_SEC          (THOUSAND*MILLION)      /**< easy multiplier             */
#define MICROSECS_PER_MS          (THOUSAND)              /**< easy multiplier             */
#define NANOSECS_PER_MICRO        (THOUSAND)              /**< easy multiplier             */
#define MILLI2NANO(x)             (x * NANOSECS_PER_MS)   /**< milliseconds to nanosecs    */
#define MILLI2MICRO(x)            (x * MICROSECS_PER_MS)  /**< millisecs to microsecs      */
#define SECS2MILLI(x)             (x * MILLISECS_PER_SEC) /**< converts secs to millisecs  */
#define MILLI2SECONDS(x)          (x / MILLISECS_PER_SEC) /**< convert ms to secs          */
#define MICRO2MILLI(x)            (x / MICROSECS_PER_MS)  /**< millisecs to microsecs      */

/**
 * general defines 
 */
#define RESET_POINTER_VALUE(x) { if (x != NULL) *x = 0; }
#define SET_POINTER_VALUE(x)   { if (x != NULL) *x = 1; }

/**
 * general purpose definition
 */
#define SYSCALL(x)      (system(x))        /**< call external linux system call */
  
/**
 * generic callback definition
 */
typedef void *(*dispatch_fn_t)(void *args);        /**< prototype for a standard pthread_functions         */

/**
 * MACRO fore strerror(errno))
 */
#define COMMON_ERR_STR (strerror(errno))       /**< typedef of USB ERROR STR       */
#define UNUSED_ATTRIBUTE __attribute__((unused))

////////////////////////////////////////////////
// Device Types - from CTN Discovery protocol //
////////////////////////////////////////////////
typedef enum {
   RESERVED_TYPE = 0,
   SHAVER_CAPITAL_DEVICE,
   VISUALIZATION_DEVICE,
   COBLATION_DEVICE,
   TABLET_DEVICE,
   FLUID_MANAGEMENT_TYPE,
   MAX_CTN_DEVICE_TYPE,
} CTN_DEVICE_TYPES;

// version information we are working with
#define DEFAULT_VERSION_BYTE 0x21

#endif // __COMMON_TYPES_H__
