;;======================================================================
;; CS2110                    Homework #07                    Summer 2017
;;======================================================================
;; Author: Kiran Rao
;; Date: 06/25/2017
;; Assignment: Homework 07 (Part 3)
;; Description: BST height
;;======================================================================

.orig x3000

MAIN
LD R6, STACK

;; CALL HEIGHT AND STORE ANSWER HERE :D
LD R0, ARRAY		; address of ARRAY[0]
LD R1, INDEX
ADD R6, R6, -2
STR R1, R6, 1		; first parameter on STACK is INDEX
STR R0, R6, 0		; second parameter on STACK is address of ARRAY[0]
JSR HEIGHT

;; Pop return value and arg off stack
LDR R0, R6, 0
ADD R6, R6, 2

;; Save the answer
ST R0, ANSWER

HALT

STACK .fill xF000
ARRAY .fill x6000   ; Address of the array
INDEX .fill 1       ; Starting index
ANSWER .blkw 1      ; Save your answer here

HEIGHT

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

LDR R0, R5, 4		; R0 = address of ARRAY[0]
LDR R1, R5, 5		; R1 = INDEX

ADD R0, R0, R1		; R0 = address of ARRAY[INDEX]
LDR R2, R0, 0		; R2 = ARRAY[INDEX]

BRZ BASE
BRNP RECURSE

BASE			; if ARRAY[INDEX] == 0
AND R2, R2, 0
STR R2, R5, 3		; store 0 in RV
BR CLEANUP

RECURSE			; if ARRAY[INDEX] != 0
ADD R6, R6, -2		; make space for 2 parameters on STACK
ADD R1, R1, R1		; R1 = 2 * INDEX
STR R1, R6, 1		; Put new INDEX onto STACK as first parameter
LD R0, ARRAY		; R0 = address of ARRAY[0]
STR R0, R6, 0		; Put address of ARRAY[0] onto STACK as second parameter
JSR HEIGHT		; leftheight = bst_height(arr, 2*index)
LDR R2, R6, 0		; load RV of leftheight from R6
STR R2, R5, 0		; store value of leftheight in local variable 1
ADD R6, R6, 1		; make space for 2 parameters on STACK
ADD R1, R1, 1		; R1 = 2*INDEX + 1
LD R0, ARRAY		; R0 = address of ARRAY[0]
STR R0, R6, 0		; Put address of ARRAY[0] onto STACK as second parameter
STR R1, R6, 1		; Put new INDEX onto STACK as first parameter
JSR HEIGHT		; rightheight = bst_height(arr, 2*index + 1)
LDR R3, R6, 0		; load RV of rightheight from R6
STR R3, R5, -1		; store value of rightheight in local variable 2
LDR R2, R5, 0		; load leftheight from local variable 1 into R2
NOT R3, R3		
ADD R3, R3, 1		; negate value of rightheight
ADD R2, R2, R3		; value of leftheight -  rightheight into R2
BRP LEFT
BRNZ RIGHT

LEFT			; if leftheight > rightheight
LDR R0, R5, 0		; R0 = leftheight	
ADD R0, R0, 1		; R0 = leftheight + 1
STR R0, R5, 3		; store leftheight + 1 in RV
ADD R6, R6, 3		; move SP to top of STACK
BR CLEANUP

RIGHT			; if leftheight <= rightheight
LDR R0, R5, -1		; R0 = rightheight
ADD R0, R0, 1		; R0 = rightheight + 1
STR R0, R5, 3		; store rightheight +1 in RV
ADD R6, R6, 3		; move SP to top of STACK

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

;;======================================================================
;; You should not modify any code beyond this point
;;======================================================================

.orig x6000
.fill 0
.fill 8     ;row 1
.fill 3     ;row 2
.fill 10
.fill 1     ;row 3
.fill 6
.fill 0
.fill 14
.fill 0     ;row 4
.fill 0
.fill 4
.fill 7
.fill 0
.fill 0
.fill 13
.fill 0
.fill 0     ;row 5
.fill 0
.fill 0
.fill 0
.fill 0
.fill 0
.fill 0
.fill 0
.fill 0
.fill 0
.fill 0
.fill 0
.fill 0
.fill 0
.fill 0
.fill 0
.end
