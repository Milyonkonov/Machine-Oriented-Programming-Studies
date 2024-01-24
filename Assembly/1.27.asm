start:	MOV	R0, #0
	LDR	R1, =ca
for:	CMP	R0, #10
	BGE	forend
	STRB	R0, [R1, R0]
	ADD	R0, R0, #1
	B 	for

forend:	MOV	R0, #5
	MOV	R2, #0xFF
	STRB	R2, [R1, R0]

ca:	.SPACE	10
