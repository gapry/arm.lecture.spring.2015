	.syntax unified
	.arch armv7-a
	.text
	.align 2
	.thumb
	.thumb_func
	.global fibonacci
	.type fibonacci, function
fibonacci:
	cmp r0, #1
	ble .L0
	mov r3, #1	@counter
	mov r1, #0	@cache Fn-2
	mov r2, #1	@cache Fn-1
.LOOP:	
	add r1, r1, r2
	/* Overflow Detection */
	/*----------------------------------*/	
	mrs r4, CPSR
	mov r4, r4, LSR #26
	and r4, #00001
	cmp r4, #1
	beq .EXIT
	/*----------------------------------*/	
	add r3, #1
	cmp r3, r0
	beq .L1
	/*----------------------------------*/	

	add r2, r1, r2
	/* Overflow Detection */
	/*----------------------------------*/	
	mrs r4, CPSR
	mov r4, r4, LSR #26
	and r4, #00001
	cmp r4, #1
	beq .EXIT
	/*----------------------------------*/	
	mrs r4, CPSR
	add r3, #1
	cmp r3, r0
	beq .L2
	/*----------------------------------*/	
	b .LOOP
.L0:
	bx lr
.L1:
	mov r0, r1
	bx lr
.L2:
	mov r0, r2
	bx lr
.EXIT:
	mov r0, #-1
	bx lr
	.size fibonacci, .-fibonacci
	.end
