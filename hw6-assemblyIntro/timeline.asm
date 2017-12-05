;;======================================================================
;; CS2110                      Homework 06                   Summer 2017
;;======================================================================
;; Author:  Kiran Rao
;; Date: 06/19/2017
;; Assignment: Homework 06
;; Description: An assembly program that prints a historical timeline to
;;              the console
;;======================================================================

.orig x3000

;; YOUR CODE HERE! :D
	LEA R2, YEAR_ARR	;load the address of YEAR_ARR[0]
	LEA R3, EVENT_ARR	;load the address of EVENT_ARR[0]
	LD R1, N		
	ADD R1, R1, -1		;n-- from pseudocode (for indexing)
	BRNP NONZERO1		;if n != 0, go to NONZERO1
	LEA R0, BEFORE_MSG_2	;else, load "Nothing happened before "
	PUTS			;print message
	LDR R0, R2, 0		;load the value of the address stored in R2 (YEAR_ARR[0])
	PUTS			;print year
	LD R0, NEWLINE		;new line character
	OUT			;print character
	ADD R1, R1, 0		;use value of R1 (which is n) to set condition codes
	BRZ ENDIF1		;if n == 0, skip the loop and go to ENDIF1
NONZERO1 LEA R0, BEFORE_MSG_1	;if n != 0, load "In the years before "
	PUTS			;print message
	ADD R2, R2, R1		;address of YEAR_ARR[0+n] to R2
	LDR R0, R2, 0		;load the value of the address stored in YEAR_ARR[0+n]
	PUTS			;print year
	LEA R0, ELLIPSIS	;load "..."
	PUTS			;print
	LD R0, NEWLINE		;new line character
	OUT			;print character
	LD R4, N		
	ADD R4, R4, -1		;n-- in R4
	ADD R3, R3, R4		;address of EVENT_ARR[0+n] to R3
	ADD R4, R4, 0		;use n to set condition codes (counter)
WHILE1	BRNZ DONE1		;while n > 0 do WHILE1
	ADD R4, R4, -1		;n--
	ADD R3, R3, -1		;address of EVENT_ARR[currentIndex - 1] to R3
	LDR R0, R3, 0		;load the value of the address storead in EVENT_ARR[currentIndex - 1]
	PUTS			;print event
	LD R0, NEWLINE		;new line character
	OUT			;print character
	ADD R4, R4, 0		;use n to set condition codes
DONE1	BRP WHILE1		;exit loop when n <= 0
ENDIF1	LD R5, MAX		;max array index to R5
	NOT R6, R5		;negate max array index using 2's complement and store in R6
	ADD R6, R6, 1		;negate max array index using 2's complement and store in R6
	ADD R6, R6, R1		;determine difference between input n and max index (should be negative or zero)
	BRNP NONZERO2		;if difference between n and max != 0 (n!=max), go to NONZERO2
	LEA R0, AFTER_MSG_2	;else, load "And nothing happened after "
	PUTS			;print message
	LDR R0, R2, 14		;load the value of the address stored in YEAR_ARR[0+14]
	PUTS			;print year
	LD R0, NEWLINE		;new line character
	OUT			;print character
	ADD R6, R6, 0		;use difference between n and max to set condition codes
	BRZ ENDIF2		;if difference between n and max == 0 (n==max), skip loop and go to ENDIF2
NONZERO2 LEA R0, AFTER_MSG_1	;if difference between n and max != 0 (n!=max), load "And in the years after "
	PUTS			;print message
	LDR R0, R2, 0		;load the value of the address stored in YEAR_ARR[0+n]
	PUTS			;print year
	LEA R0, ELLIPSIS	;load "..."
	PUTS			;print
	LD R0, NEWLINE		;new line character
	OUT			;print character
	LD R4, N		;reset R4 to n after first loop
	ADD R4, R4, -1		;n--
	LEA R3, EVENT_ARR	;reset R3 to address of EVENT_ARR[0]
	ADD R3, R3, R4		;address of EVENT_ARR[0+n] to R3
	ADD R6, R6, 0		;use difference between n and max to set condition codes (counter)
WHILE2	BRZ DONE2		;while difference between n and max !=0 (n!=max), do WHILE2
	ADD R3, R3, 1		;address of EVENT_ARR[0+n+1] to R3
	ADD R6, R6, 1		;increment difference between n and max (bring difference closer to 0)
	LDR R0, R3, 0		;load value of the address stored in EVENT_ARR[0+n+1]
	PUTS			;print event
	LD R0, NEWLINE		;new line character
	OUT			;print character
	ADD R6, R6, 0		;use difference between n and max to set condition codes
DONE2	BRNP WHILE2		;exit loop when difference between n and max == 0 (n==max)
ENDIF2	HALT

N       .fill 9
MAX     .fill 14

NEWLINE .fill x0A

ELLIPSIS
    .stringz "..."

BEFORE_MSG_1
    .stringz "In the years before "

BEFORE_MSG_2
    .stringz "Nothing happened before "

AFTER_MSG_1
    .stringz "And in the years after "

AFTER_MSG_2
    .stringz "And nothing happened after "

YEAR_ARR
    .fill YEAR01
    .fill YEAR02
    .fill YEAR03
    .fill YEAR04
    .fill YEAR05
    .fill YEAR06
    .fill YEAR07
    .fill YEAR08
    .fill YEAR09
    .fill YEAR10
    .fill YEAR11
    .fill YEAR12
    .fill YEAR13
    .fill YEAR14
    .fill YEAR15

EVENT_ARR
    .fill EVENT01
    .fill EVENT02
    .fill EVENT03
    .fill EVENT04
    .fill EVENT05
    .fill EVENT06
    .fill EVENT07
    .fill EVENT08
    .fill EVENT09
    .fill EVENT10
    .fill EVENT11
    .fill EVENT12
    .fill EVENT13
    .fill EVENT14
    .fill EVENT15
.end

.orig x5000
YEAR01
    .stringz "1607"
YEAR02
    .stringz "1776"
YEAR03
    .stringz "1788"
YEAR04
    .stringz "1861"
YEAR05
    .stringz "1879"
YEAR06
    .stringz "1885"
YEAR07
    .stringz "1890"
YEAR08
    .stringz "1917"
YEAR09
    .stringz "1941"
YEAR10
    .stringz "1955"
YEAR11
    .stringz "1961"
YEAR12
    .stringz "1969"
YEAR13
    .stringz "1975"
YEAR14
    .stringz "1985"
YEAR15
    .stringz "1991"
EVENT01
    .stringz "John Smith founded Jamestown in 1607"
EVENT02
    .stringz "The Declaration of Independence was signed in 1776"
EVENT03
    .stringz "The Constitution was ratified in 1788"
EVENT04
    .stringz "The Civil War began in 1861"
EVENT05
    .stringz "Thomas Edison invented the lightbulb in 1879"
EVENT06
    .stringz "The Georgia Institute of Technology was founded in 1885"
EVENT07
    .stringz "Yosemite National Park was created in 1890"
EVENT08
    .stringz "The US entered WWI in 1917"
EVENT09
    .stringz "Japan attacked Pearl Harbor in 1941"
EVENT10
    .stringz "The Civil Rights movement began in 1955"
EVENT11
    .stringz "The Vietnam War began in 1961"
EVENT12
    .stringz "Apollo 11 landed on the moon in 1969"
EVENT13
    .stringz "Bill Gates founded Microsoft Corporation in 1975"
EVENT14
    .stringz "Super Mario Bros. debuted in 1985"
EVENT15
    .stringz "The Cold War formally ended in 1991"
.end
