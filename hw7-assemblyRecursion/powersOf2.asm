;;======================================================================
;; CS2110                    Homework #07                    Summer 2017
;;======================================================================
;; Author: Kiran Rao
;; Date: 06/25/2017
;; Assignment: Homework 07 (Part 2)
;; Description: Powers of 2
;;======================================================================

.orig x3000

MAIN
LD R6, STACK

;; CALL POWERSOF2 AND STORE ANSWER HERE :D
LD R0, N
ADD R6, R6, -1
STR R0, R6, 0		; first value on STACK is parameter N
JSR POWERSOF2

;; Pop return value and arg off stack
LDR R0, R6, 0
ADD R6, R6, 2

;; Save the answer
ST R0, ANSWER

HALT

STACK .fill xF000
N .fill 4             ; Input N
ANSWER .blkw 1        ; Save your answer here

POWERSOF2

;; YOUR CODE HERE :D

ADD R6, R6, -3		; Make space for RA, OFP, RV
STR R7, R6, 1		; Store RA
STR R5, R6, 0		; Store old FP
ADD R5, R6, -1		; Setup new FP

ADD R6, R6, -2		; Make space for locals

ADD R6, R6, -5		; Make space for registers

;; Save registers on the STACK
STR R4, R6, 4
STR R3, R6, 3
STR R2, R6, 2
STR R1, R6, 1
STR R0, R6, 0

AND R1, R1, 0		
STR R1, R5, 0		; temp1 = 0
STR R1, R5, -1		; temp2 = 0

LDR R0, R5, 4		; Load N into R0

BRZ BASE0		

ADD R1, R0, -1
BRZ BASE1
BRNP RECURSE

BASE0			; if N == 0	
AND R2, R2, 0		
ADD R2, R2, 1
STR R2, R5, 3		; store 1 in RV
BR CLEANUP

BASE1			; if N == 1
AND R2, R2, 0
ADD R2, R2, 2
STR R2, R5, 3		; store 2 in RV
BR CLEANUP

RECURSE			; if N != 0 or N != 1
ADD R6, R6, -1		; Make space for new parameter
ADD R0, R0, -1		; R0 = N-1
STR R0, R6, 0		; Put parameter on the STACK
JSR POWERSOF2		; temp1 = powersOf2(N-1)
LDR R1, R6, 0		; load RV of temp1 from R6
STR R1, R5, 0		; store value of temp1 into local variable 1

ADD R6, R6, 1		; Make space for new parameter

ADD R0, R0, -1		; R0 = N-2
STR R0, R6, 0		; Put parameter on the STACK
JSR POWERSOF2		; temp2 = powersOf2(N-2)
LDR R1, R6, 0		; load RV of temp2 from R6
STR R1, R5, -1		; store value of temp2 into local variable 2
LDR R1, R5, 0		; R1 = temp1
LDR R2, R5, 0		; R2 = temp1
ADD R1, R1, R1		
ADD R1, R1, R2		; R1 = 3*temp1
LDR R2, R5, -1		; R2 = temp2
ADD R2, R2, R2		; R2 = 2*temp2
NOT R2, R2
ADD R2, R2, 1		; R2 = -2*temp2
ADD R0, R1, R2		; R0 = 3*temp1 - 2*temp2
STR R0, R5, 3		; store result of R0 into RV
ADD R6, R6, 2		; move SP to top of STACK

;;load return value in r6 into local variable
;;move r6 to top of previous stack
;;recursive call again
;;load return value in r6 into local variable 2

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
