#include "unity.h"
#include <stdint.h>
#include <intrinsics.h>
#include "event_buffer.h"

void setUp(void)
{
}

void tearDown(void)
{
}

// void test_put_event (void)
// {
   // event_table evt_tbl;
   // event e = NOTIFY_EVENT;
   // uint8_t return_value = put_event (&evt_tbl, e);
   // TEST_ASSERT_EQUAL(evt_tbl.buff[0],e);
// }

void test_put_event_full_index (void)
{
   event_table evt_tbl;
   event e = NOTIFY_EVENT;
   uint8_t return_value = 1;
   
   for (uint8_t i = 0; i < EVENT_BUFFER_SIZE+1; i++)
   {
      return_value = put_event (&evt_tbl, e);
   }
   // The last return value must be 0
   TEST_ASSERT_EQUAL(0,return_value);
}

void test_get_event (void)
{
   event_table evt_tbl;
   event e = NOTIFY_EVENT;
   uint8_t return_value = put_event (&evt_tbl, e);
   TEST_ASSERT_EQUAL(evt_tbl.buff[0],e);
   event returned_event = get_event (&evt_tbl);
   TEST_ASSERT_EQUAL(e, returned_event);
}

void test_get_event_null_event (void)
{
   event_table evt_tbl;
   uint8_t return_value = 1;
   event returned_event = get_event (&evt_tbl);
   TEST_ASSERT_EQUAL(NULL_EVENT, returned_event);
}

// void test_flush_buffer(void)
// {
// #if 0
// 	 event_table evt_tbl;
// 	 event e = NOTIFY_EVENT;
// 	 uint8_t return_value = put_event (&evt_tbl, e);
// 	 flush_buffer(event_table*)
// #endif
// 	 TEST_IGNORE_MESSAGE("function has no definition");
// }

void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif
}
