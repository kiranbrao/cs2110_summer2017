;;======================================================================
;; CS2110                    Homework #07                    Summer 2017
;;======================================================================
;; Author: Kiran Rao
;; Date: 06/25/2017
;; Assignment: Homework 07 (Part 1)
;; Description: McCarthy 91
;;======================================================================

.orig x3000

MAIN
LD R6, STACK

;; CALL MC91 AND STORE ANSWER HERE :D
LD R0, N		
ADD R6, R6, -1
STR R0, R6, 0		; first value on STACK is paramter N
JSR MC91

;; Pop return value and arg off stack
LDR R0, R6, 0
ADD R6, R6, 2

;; Save the answer
ST R0, ANSWER

HALT

STACK .fill xF000
N .fill 99          ; Input N
ANSWER .blkw 1      ; Save your answer here

MC91

;; YOUR CODE HERE :D
ADD R6, R6, -3		; Make space for RA, OFP, RV
STR R7, R6, 1		; Store RA
STR R5, R6, 0		; Store old FP
ADD R5, R6, -1		; Setup new FP

ADD R6, R6, -5		; Make space for registers

;; Save registers on the STACK
STR R4, R6, 4
STR R3, R6, 3
STR R2, R6, 2
STR R1, R6, 1
STR R0, R6, 0

LDR R0, R5, 4		; Load N into R0
ADD R1, R0, -15		; Subtract 100 from N
ADD R1, R1, -15
ADD R1, R1, -15
ADD R1, R1, -15
ADD R1, R1, -15
ADD R1, R1, -15
ADD R1, R1, -10

BRP BASE		; if N > 100 go to base case
BRNZ RECURSE		; otherwise recurse

BASE
ADD R0, R0, -10		; N = N - 10
STR R0, R5, 3		; store N-10 in RV
BR CLEANUP

RECURSE
ADD R6, R6, -1		; Make space for N
ADD R0, R0, 11		; N = N + 11
STR R0, R6, 0		; Put parameter on the STACK
JSR MC91		; call MC91 with N + 11 as new parameter
LDR R0, R6, 0		; load return value in R6 to R0
ADD R6, R6, -1		; Make space for second N (previous RV)
STR R0, R6, 0		; Put parameter on the STACK
JSR MC91		; call MC91 with RV from MC91(N+11)
LDR R0, R6, 0		; load return value in R6 to R0
STR R0, R5, 3		; store RV in the STACK
ADD R6, R6, 2		; move SP to top of STACK

CLEANUP
;; Restore the register values
LDR R4, R6, 4
LDR R3, R6, 3
LDR R2, R6, 2
LDR R1, R6, 1
LDR R0, R6, 0

LDR R7, R5, 2		; Restore RA (R5)
ADD R6, R5, 1		; Point SP at OFP
LDR R5, R6, 0		; Restore OFP (R7)
ADD R6, R6, 2		; Point SP at RV

RET
.end
