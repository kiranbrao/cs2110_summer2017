;;======================================================================
;; CS2110                      Homework 06                   Summer 2017
;;======================================================================
;; Author: Kiran Rao
;; Date: 06/19/2017
;; Assignment: Homework 06
;; Description: An assembly program that determines the absolute value
;;              of a given number
;;======================================================================

.orig x3000

;; YOUR CODE HERE! :D
	LD R0, U
	BRN ELSE	;if U < 0, go to ELSE
	ST R0, ANSWER	;if U > 0, ANSWER = U 
	BR END		;skip to END to avoid doing ELSE branch
ELSE	NOT R1, R0	;negate U using 2's complement
	ADD R2, R1, 1	;negate U using 2's complement
	ST R2, ANSWER	;ANSWER = U
END	HALT

U       .fill -20
ANSWER  .fill 0

.end
