@ GAUSS SUMMATION BUT THE LONG, RECURSIVE WAY AROUND...
start:	LDR	R0, =n
	LDR	R0, [R0]
	BL	sum
	LDR	R1, =result
	STR	R0, [R1]
	B	start

sum:	PUSH 	{LR}
	CMP	R0, #0
	BNE	sum1
	POP	{PC}

sum1:	PUSH	{R0}
	SUB	R0, R0, #1
	BL	sum
	POP	{R1}
	ADD	R0, R0, R1
	POP	{PC}

	.ORG	0x30
n:	.WORD	96
result:	.SPACE	4
