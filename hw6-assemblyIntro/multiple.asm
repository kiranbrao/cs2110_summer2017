;;======================================================================
;; CS2110                      Homework 06                   Summer 2017
;;======================================================================
;; Author: Kiran Rao
;; Date: 06/19/2017
;; Assignment: Homework 06
;; Description: An assembly program that determines whether one number
;;              is a multiple of another number
;;======================================================================

.orig x3000

;; YOUR CODE HERE! :D
	LD R0, A
	LD R1, B
	NOT R2, R1	;negate B using 2's complement
	ADD R2, R2, 1	;negate B using 2's complement
	AND R5, R5, 0	;initiate return value (0 or 1)
	ADD R3, R0, R2	;store the difference between A and B 
WHILE	BRNZ DONE	;while the difference between A and B is still positive, keep looping
	ADD R0, R0, R2	;subract B from A
	ADD R3, R0, R2	;calculate the new difference between A and B
	BRP WHILE	
DONE	ADD R4, R0, R2	;checks if a == b by determining if the new difference is 0
	BRNP ELSE	;if new difference is not 0, go to ELSE
	ADD R5, R5, 1	;return 1
	ST R5, ANSWER	;return 1
	BRZ END		;skip to the end to avoid the ELSE branch
ELSE	ST R5, ANSWER	;return 0
END	HALT

A       .fill 15
B       .fill 5
ANSWER  .fill 0

.end
