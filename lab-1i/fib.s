	.syntax unified
	.arch armv7-a
	.text
	.global fibonacci
	.type fibonacci, function
@ arm isa
fibonacci:
    .code 32
    cmp r0, #1
    bxle lr
    mov r2, r0
    ldr r0, =.thumb_mode+1
    bx r0
@ thumb isa
    .code 16
.thumb_mode:
    mov r0, #0  @fn-2
    mov r1, #1  @fn-1
.loop:
    adds r0, r1
    itt vs          @overflow detection
    movvs r0, -1
    blxvs lr
    eors r0, r1
    eors r1, r0
    eors r0, r1
    subs r2, #1
    bgt .loop
    bx lr
	.size fibonacci, .-fibonacci
	.end
