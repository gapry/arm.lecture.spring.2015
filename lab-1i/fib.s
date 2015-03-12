	.syntax unified
	.arch armv7-a
	.text
	.global fibonacci
	.type fibonacci, function
@ ARM ISA
fibonacci:
    .code 32
    MOV r3, r0
    MOV r2, lr
    LDR r0, =.THUMB_MODE+1
    MOV lr, pc
    BLX r0
    @ Continue
    MOV r0, r1
    MOV lr, r2
    BX lr

@ THUMB ISA
    .code 16
.THUMB_MODE:
    MOV r5, #0
    MOV r1, #0  @fn-2
    MOV r4, #1  @fn-1
    MOV r6, #0  @fn
.L0:
    CMP r5, r3
    ITTTT LT
    ADDLT r6, r1
    ADDLT r6, r4
    MOVLT r1, r4
    MOVLT r4, r6
    ITT LT
    MOVLT r6, #0
    ADDLT r5, #1
    BLT .L0

    BX lr
	.size fibonacci, .-fibonacci
	.end
