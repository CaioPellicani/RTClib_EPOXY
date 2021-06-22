#include "RTClib_EPOXY.h"

DateTime::DateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour,
                   uint8_t min, uint8_t sec) {
  if (year >= 2000U) year -= 2000U;
  yOff = year;
  m = month;
  d = day;
  hh = hour;
  mm = min;
  ss = sec;
}

String DateTime::timestamp(timestampOpt opt) {
  char buffer[25];

  switch (opt) {
  case TIMESTAMP_TIME:
    sprintf(buffer, "%02d:%02d:%02d", hh, mm, ss);
    break;
  case TIMESTAMP_DATE:
    sprintf(buffer, "%u-%02d-%02d", 2000U + yOff, m, d);
    break;
  default:
    sprintf(buffer, "%u-%02d-%02dT%02d:%02d:%02d", 2000U + yOff, m, d, hh, mm,
            ss);
  }
  return String(buffer);
}

boolean RTC_DS3231::begin(void) { 
    time( &this->beginTime );
    this->rtcTime = 0;
    return true; 
}

void RTC_DS3231::adjust(const DateTime &dt){
    struct tm temp;
    temp.tm_year = dt.year();
    temp.tm_mon  = dt.month();
    temp.tm_mday = dt.day();
    temp.tm_hour = dt.hour();
    temp.tm_min  = dt.minute();
    temp.tm_sec  = dt.second();
    
    rtcTime = mktime( &temp );
}

DateTime RTC_DS3231::now() {
    time_t now;
    time( &now );
    time_t diff = now - this->beginTime;
    now = this->rtcTime + diff;

    struct tm *strNow = localtime( &now );
    uint8_t ss = strNow->tm_sec;
    uint8_t mm = strNow->tm_min;
    uint8_t hh = strNow->tm_hour;
    uint8_t d  = strNow->tm_mday;
    uint8_t m  = strNow->tm_mon;
    uint16_t y = strNow->tm_year;

    return DateTime(y, m, d, hh, mm, ss);
}




