#include <AUnit.h>
#include "RTClib_EPOXY.h"

using namespace aunit;

test(RTClib_EPOXY_DateTime) { 
  DateTime tDateTime;
  tDateTime = DateTime( 2000, 1, 1, 0, 0, 5 );

  assertEqual( 5U, tDateTime.secondstime() );
  assertEqual( 946684805U, tDateTime.unixtime() );
  assertEqual( tDateTime.timestamp( tDateTime.TIMESTAMP_TIME ), "00:00:05" );
  assertEqual( tDateTime.timestamp( tDateTime.TIMESTAMP_DATE ), "2000-01-01" );
  assertEqual( tDateTime.timestamp( tDateTime.TIMESTAMP_FULL ), "2000-01-01T00:00:05" );
  assertEqual( tDateTime.year(), 2000 );
  assertEqual( tDateTime.month(), 1 );
  assertEqual( tDateTime.day(), 1 );
  assertEqual( tDateTime.hour(), 0 );
  assertEqual( tDateTime.minute(), 0 );
  assertEqual( tDateTime.second(), 5 );
  assertEqual( tDateTime.twelveHour(), 12 );
  assertEqual( tDateTime.isPM(), 0 );

  tDateTime = DateTime( 946684805U );
  assertEqual( tDateTime.timestamp( tDateTime.TIMESTAMP_FULL ), "2000-01-01T00:00:05" );
}

test(RTClib_EPOXY_ds3231) {
  RTC_DS3231 rtc;
  if( rtc.begin() ){
    rtc.adjust( DateTime( 2021, 6, 21, 6, 15, 50 ) );
    
    assertEqual(rtc.now().timestamp( rtc.now().TIMESTAMP_TIME ), "06:15:50" );
    delay( 1000 );
    assertEqual(rtc.now().timestamp( rtc.now().TIMESTAMP_TIME ), "06:15:51" );
  }
}
