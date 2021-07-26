#include <AUnit.h>
#include "RTClib_EPOXY.h"

using namespace aunit;

test(RTClib_EPOXY_ds3231) {
  RTC_DS3231 rtc;
  if( rtc.begin() ){
    rtc.adjust( DateTime( 2021, 6, 21, 6, 15, 50 ) );
    
    assertEqual(rtc.now().timestamp( rtc.now().TIMESTAMP_TIME ), "06:15:50" );
    delay( 1000 );
    assertEqual(rtc.now().timestamp( rtc.now().TIMESTAMP_TIME ), "06:15:51" );
  }
}
