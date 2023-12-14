.PHONY: all

EXE = daysInMonthCounter firstMondayCounter

.PHONY all: $(EXE)

daysInMonthCounter: calendar.c daysInMonthCounter.c
	rm -f daysInMonthCounter
	gcc -o daysInMonthCounter calendar.c daysInMonthCounter.c

firstMondayCounter: calendar.c firstMondayCounter.c
	rm -f firstMondayCounter
	gcc -o firstMondayCounter calendar.c firstMondayCounter.c