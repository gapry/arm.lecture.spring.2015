	.syntax unified
	.arch armv7-a
	.text
	.global fibonacci
	.type fibonacci, function
@ ARM ISA
fibonacci:
    .code 32
    CMP r0, #1
    BXLE lr
    MOV r2, r0
    LDR r0, =.THUMB_MODE+1
    BX r0
@ THUMB ISA
    .code 16
.THUMB_MODE:
    MOV r0, #0  @fn-2
    MOV r1, #1  @fn-1
.LOOP:
    ADDS r0, r1
    ITT VS          @Overflow Detection
    MOVVS r0, -1
    BLXVS lr
    EORS r0, r1
    EORS r1, r0
    EORS r0, r1
    SUBS r2, #1
    BGT .LOOP
    BX lr
	.size fibonacci, .-fibonacci
	.end
