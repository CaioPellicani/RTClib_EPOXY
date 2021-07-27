#include "RTClib_EPOXY.h"
#include <time.h>
#include <stdio.h>

time_t beginTime;
time_t rtcTime;

const uint8_t daysInMonth[] PROGMEM = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30};

static uint16_t date2days(uint16_t y, uint8_t m, uint8_t d) {
  if (y >= 2000U) 
    y -= 2000U;
  uint16_t days = d;
  for (uint8_t i = 1; i < m; ++i)
    days += (*(const unsigned char *)(daysInMonth + i - 1));
  if (m > 2 && y % 4 == 0)
    ++days;
  return days + 365 * y + (y + 3) / 4 - 1;
}

static uint32_t time2ulong(uint16_t days, uint8_t h, uint8_t m, uint8_t s) {
  return ((days * 24UL + h) * 60 + m) * 60 + s;
}

static uint8_t conv2d(const char *p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}

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

DateTime::DateTime(const __FlashStringHelper *date, const __FlashStringHelper *time) {
  char buff[11];
  memcpy_P(buff, date, 11);
  yOff = conv2d(buff + 9);
  // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
  switch (buff[0]) {
  case 'J':
    m = (buff[1] == 'a') ? 1 : ((buff[2] == 'n') ? 6 : 7);
    break;
  case 'F':
    m = 2;
    break;
  case 'A':
    m = buff[2] == 'r' ? 4 : 8;
    break;
  case 'M':
    m = buff[2] == 'r' ? 3 : 5;
    break;
  case 'S':
    m = 9;
    break;
  case 'O':
    m = 10;
    break;
  case 'N':
    m = 11;
    break;
  case 'D':
    m = 12;
    break;
  }
  d = conv2d(buff + 4);
  memcpy_P(buff, time, 8);
  hh = conv2d(buff);
  mm = conv2d(buff + 3);
  ss = conv2d(buff + 6);
}

uint32_t DateTime::secondstime(void) const {
  uint32_t t;
  uint16_t days = date2days(yOff, m, d);
  t = time2ulong(days, hh, mm, ss);
  return t;
}

uint32_t DateTime::unixtime(void) const {
  uint32_t t;
  uint16_t days = date2days(yOff, m, d);
  t = time2ulong(days, hh, mm, ss);
  t += SECONDS_FROM_1970_TO_2000; // seconds from 1970 to 2000

  return t;
}

uint8_t DateTime::twelveHour() const {
  if (hh == 0 || hh == 12) { // midnight or noon
    return 12;
  } else if (hh > 12) { // 1 o'clock or later
    return hh - 12;
  } else { // morning
    return hh;
  }
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

boolean RTC_DS3231::begin() { 
    time( &beginTime );
    return true; 
}
bool RTC_DS3231::lostPower(void) { return false; }

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
    time_t diff = now - beginTime;
    now = rtcTime + diff;

    struct tm *strNow = localtime( &now );
    uint8_t ss = strNow->tm_sec;
    uint8_t mm = strNow->tm_min;
    uint8_t hh = strNow->tm_hour;
    uint8_t d  = strNow->tm_mday;
    uint8_t m  = strNow->tm_mon;
    uint16_t y = strNow->tm_year;

    return DateTime(y, m, d, hh, mm, ss);
}




