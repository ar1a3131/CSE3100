#include <stdio.h>
#include "calendar.h"

int numDaysInMonth(int month, int year){
  switch(month){
  case JAN:
    return 31;
    break;
  case FEB:
    if((year % 100 == 0 && year % 400 == 0) || (year % 100 > 0 && year % 4 == 0)){
      //then a leap year
      return 29;
    }else{
      return 28;
    }
  case MAR:
    return 31;
  case APR:
    return 30;
  case MAY:
    return 31;
  case JUNE:
    return 30;
  case JULY:
    return 31;
  case AUG:
    return 31;
  case SEP:
    return 30;
  case OCT:
    return 31;
  case NOV:
    return 30;
  case DEC:
    return 31;
  default:
    return -1;
  }
}

int numFirstMondays(int startYear, int endYear, int janFirstWeekday)
{
  int numMondays = 0;
  int year = startYear;
  int weekDay = janFirstWeekday;
  int month = JAN;
  while(year <= endYear){
    if(weekDay == MONDAY){
      numMondays++;
    }
    weekDay = (weekDay + numDaysInMonth(month, year)) % 7;
    //go to the first of the next month.
    if(month == DEC){
      year++;
      month = JAN;
    }else{
      month++;
    }
  }
  return numMondays;
}
