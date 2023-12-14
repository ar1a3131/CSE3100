#ifndef __CALENDAR_H
#define __CALENDAR_H

#define SUNDAY 0
#define MONDAY 1
#define TUESDAY 2
#define WEDNESDAY 3
#define THURSDAY 4
#define FRIDAY 5
#define SATURDAY 6

#define JAN 1
#define FEB 2
#define MAR 3
#define APR 4
#define MAY 5
#define JUNE 6
#define JULY 7
#define AUG 8
#define SEP 9
#define OCT 10
#define NOV 11
#define DEC 12


int numDaysInMonth(int month, int year);
int numFirstMondays(int startYear, int endYear, int janFirstWeekday);

#endif
