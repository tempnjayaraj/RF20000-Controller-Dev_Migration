#include "unity.h"
#include "unity_internals.h"
#include "sensors.h"
#include "common.h"
#include "Mockled.h"
#include "Mocksystem.h"
#include "Mockevent_buffer.h"
#include "Mockwand.h"
#include "Mockgui.h"

GEN_SETTINGS current_therapy_settings;
event_table evt_tbl;
manta_prime prime;
prime_mode_t PrimeState;

#define UNIT_TEST 1

void setUp(void)
{
	ADC1_RB = 0;
   GPIOC_PDIR = 0x00;
   GPIOA_PDIR = 0x00;
   current_therapy_settings.pump.sensor_status = 0x0;
   current_therapy_settings.pump.door_present = 0x0;
   current_therapy_settings.pump.tube_present = 0x0;
   current_therapy_settings.pump.status = 0x0;

}

void tearDown(void)
{
}


void test_door_sensor_init (void)
{
   uint8_t return_value = door_sensor_init();
   TEST_ASSERT_EQUAL(1,return_value);
}

void test_tube_sensors_init(void)
{
   uint8_t return_value =tube_sensors_init();
   TEST_ASSERT_EQUAL(1,return_value);
}


/*******************************************************************************
 *                               truth table                                   *
 *******************************************************************************
 *                     tube   |  door   | stator  |                            *
 *                    present | closed  | status  | action                     *
 *                   ---------|---------|---------|---------                   *
 *                      0     |    1    |    1    |load tube                   *
 *                      0     |    0    |    0    |load tube                   *
 *                      1     |    1    |    0    |load tube                   *
 *                      1     |    0    |    0    | opendoor                   *
 *                      0     |    0    |    1    |  error                     *
 *                      1     |    0    |    1    |  error                     *
 *                      1     |    1    |    1    |   ok                       *
 *                      0     |    1    |    0    |closedoor                   *
 ******************************************************************************/

void test_poll_pump_sensors_tube0_door0_stator0 (void)
{
   GPIOC_PDIR = (1<<SUCTION_TUBE_SENSOR_SHIFT)|(1 <<DISCHARGE_TUBE_SENSOR_SHIFT); //suction , discharge = 0
   GPIOA_PDIR = (1<<DOOR_SENSOR_SHIFT); //door = 0
   current_therapy_settings.pump.stator  =  0; // <9929 is 1
   uint8_t return_value;
   return_value =  poll_pump_sensors();
   //TEST_ASSERT_EQUAL(0,(current_therapy_settings.pump.sensor_status & STATOR_SENSOR_ON));
   TEST_ASSERT_EQUAL(0,(current_therapy_settings.pump.sensor_status & DOOR_SENSOR_ON));
   TEST_ASSERT_EQUAL(0,(current_therapy_settings.pump.sensor_status & DISCHARGE_TUBE_SENSOR_ON));
   TEST_ASSERT_EQUAL(0,(current_therapy_settings.pump.sensor_status & SUCTION_TUBE_SENSOR_ON));
}

void test_poll_pump_sensors_tube0_door0_stator0_1 (void)
{
   GPIOC_PDIR = (1<<SUCTION_TUBE_SENSOR_SHIFT)|(1 <<DISCHARGE_TUBE_SENSOR_SHIFT); //suction , discharge = 0
   GPIOA_PDIR = (1<<DOOR_SENSOR_SHIFT); //door = 0
   current_therapy_settings.pump.stator  =  20000; // <9929 is 1
   uint8_t return_value;
   return_value =  poll_pump_sensors();
   //TEST_ASSERT_EQUAL(0,(current_therapy_settings.pump.sensor_status & STATOR_SENSOR_ON));
   TEST_ASSERT_EQUAL(0,(current_therapy_settings.pump.sensor_status & DOOR_SENSOR_ON));
   TEST_ASSERT_EQUAL(0,(current_therapy_settings.pump.sensor_status & DISCHARGE_TUBE_SENSOR_ON));
   TEST_ASSERT_EQUAL(0,(current_therapy_settings.pump.sensor_status & SUCTION_TUBE_SENSOR_ON));
}


void test_poll_pump_sensors_tube1_door1_stator1 (void)
{
   GPIOC_PDIR = (0<<SUCTION_TUBE_SENSOR_SHIFT)|(0 <<DISCHARGE_TUBE_SENSOR_SHIFT);
   GPIOA_PDIR = (0<<DOOR_SENSOR_SHIFT);
   current_therapy_settings.pump.stator  =  19859-1;
   uint8_t return_value;
   for (uint8_t i = 0; i < 11; i++)
   {
      return_value =  poll_pump_sensors();
   }
   //TEST_ASSERT_EQUAL(STATOR_SENSOR_ON,(current_therapy_settings.pump.sensor_status & STATOR_SENSOR_ON));
   TEST_ASSERT_EQUAL(DOOR_SENSOR_ON,(current_therapy_settings.pump.sensor_status & DOOR_SENSOR_ON));
   TEST_ASSERT_EQUAL(DISCHARGE_TUBE_SENSOR_ON,(current_therapy_settings.pump.sensor_status & DISCHARGE_TUBE_SENSOR_ON));
   TEST_ASSERT_EQUAL(SUCTION_TUBE_SENSOR_ON,(current_therapy_settings.pump.sensor_status & SUCTION_TUBE_SENSOR_ON));
}

void test_poll_pump_sensors_tube1_door1_stator1_2 (void)
{
   GPIOC_PDIR = (1<<SUCTION_TUBE_SENSOR_SHIFT)|(0 <<DISCHARGE_TUBE_SENSOR_SHIFT);
   GPIOA_PDIR = (0<<DOOR_SENSOR_SHIFT);
   current_therapy_settings.pump.stator  =  19859-1;
   uint8_t return_value;
   put_event_ExpectAndReturn(&evt_tbl,THERAPY_OFF,1);
   for (uint8_t i = 0; i < 11; i++)
   {
      return_value =  poll_pump_sensors();
   }
   TEST_ASSERT_EQUAL(0,current_therapy_settings.pump.tube_present);
}

void test_poll_pump_sensors_tube1_door1_stator1_3 (void)
{
   GPIOC_PDIR = (0<<SUCTION_TUBE_SENSOR_SHIFT)|(1 <<DISCHARGE_TUBE_SENSOR_SHIFT);
   GPIOA_PDIR = (0<<DOOR_SENSOR_SHIFT);
   current_therapy_settings.pump.stator  =  19859-1;
   uint8_t return_value;
   for (uint8_t i = 0; i < 11; i++)
   {
      return_value =  poll_pump_sensors();
   }
   TEST_ASSERT_EQUAL(0,current_therapy_settings.pump.tube_present);
}

void test_poll_pump_sensors_tube0_door1_stator0_DOOR_CLOSE (void)
{
   GPIOC_PDIR = (1<<SUCTION_TUBE_SENSOR_SHIFT)|(1 <<DISCHARGE_TUBE_SENSOR_SHIFT); //suction , discharge = 0
   GPIOA_PDIR = (0<<DOOR_SENSOR_SHIFT); //door = 1
   current_therapy_settings.pump.stator  =  19859+1;
   current_therapy_settings.pump.on = 0;
   uint8_t return_value;
   //put_event_ExpectAndReturn(&evt_tbl,THERAPY_OFF,1);
   for (uint8_t i = 0; i < 11; i++)
   {
      return_value =  poll_pump_sensors();
   }
   TEST_ASSERT_EQUAL(DOOR_CLOSE,(current_therapy_settings.pump.status&DOOR_CLOSE));
}

void test_poll_pump_sensors_tube0_door1_stator0_DOOR_OPEN (void)
{
   GPIOC_PDIR = (0<<SUCTION_TUBE_SENSOR_SHIFT)|(0 <<DISCHARGE_TUBE_SENSOR_SHIFT); //suction , discharge = 1
   GPIOA_PDIR = (1<<DOOR_SENSOR_SHIFT); //door = 0
   current_therapy_settings.pump.stator  =  19859+1;
   current_therapy_settings.pump.on = 0;
   uint8_t return_value;
   put_event_ExpectAndReturn(&evt_tbl,THERAPY_OFF,1);
   for (uint8_t i = 0; i < 11; i++)
   {
      return_value =  poll_pump_sensors();
   }
   TEST_ASSERT_EQUAL(DOOR_OPEN,(current_therapy_settings.pump.status&DOOR_OPEN));
}

void test_poll_pump_sensors_tube0_door1_stator0_DOOR_OPEN_false (void)
{
   GPIOC_PDIR = (0<<SUCTION_TUBE_SENSOR_SHIFT)|(0 <<DISCHARGE_TUBE_SENSOR_SHIFT); //suction , discharge = 1
   GPIOA_PDIR = (1<<DOOR_SENSOR_SHIFT); //door = 0
   current_therapy_settings.pump.stator  =  19859+1;
   current_therapy_settings.pump.on = 0;
   current_therapy_settings.pump.status = DOOR_OPEN;
   uint8_t return_value;
   //put_event_ExpectAndReturn(&evt_tbl,THERAPY_OFF,1);
   for (uint8_t i = 0; i < 11; i++)
   {
      return_value =  poll_pump_sensors();
   }
   TEST_ASSERT_EQUAL(DOOR_OPEN,(current_therapy_settings.pump.status&DOOR_OPEN));
}


void test_poll_pump_sensors_tube0_door1_stator0_ERROR (void)
{
   GPIOC_PDIR = (1<<SUCTION_TUBE_SENSOR_SHIFT)|(1 <<DISCHARGE_TUBE_SENSOR_SHIFT); //suction , discharge = 0
   GPIOA_PDIR = (1<<DOOR_SENSOR_SHIFT); //door = 0
   current_therapy_settings.pump.stator  =  19859-1;
   current_therapy_settings.pump.on = 0;
   uint8_t return_value;
   put_event_ExpectAndReturn(&evt_tbl,THERAPY_OFF,1);
   for (uint8_t i = 0; i < 11; i++)
   {
      return_value =  poll_pump_sensors();
   }
   TEST_ASSERT_EQUAL(TUBE_ABSENT,(current_therapy_settings.pump.status&TUBE_ABSENT));
}

void test_poll_pump_sensors_ERROR (void)
{
   GPIOC_PDIR = (1<<SUCTION_TUBE_SENSOR_SHIFT)|(1 <<DISCHARGE_TUBE_SENSOR_SHIFT); //suction , discharge = 0
   GPIOA_PDIR = (1<<DOOR_SENSOR_SHIFT); //door = 0
   current_therapy_settings.pump.stator  =  19859-1;
   current_therapy_settings.pump.on = 1;
   uint8_t return_value;
   put_event_ExpectAndReturn(&evt_tbl,SUCTION_TUBE_ERROR,1);
   for (uint8_t i = 0; i < 11; i++)
   {
      return_value =  poll_pump_sensors();
   }
   TEST_ASSERT_EQUAL(ERROR,(current_therapy_settings.pump.status&ERROR));
}



void test_dummy (void)
{
#if _BullseyeCoverage
    cov_dumpData();
#endif

}
