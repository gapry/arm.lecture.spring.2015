    .syntax unified                       
    .arch armv7-m
    .text
    .global fibonacci
    .type fibonacci, function
@ arm isa 
fibonacci:
	push {lr}
	mov r0, #100	

	pop {pc}
    .size fibonacci, .-fibonacci
    .end
