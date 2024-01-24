start:		LDR	R0, =intvec		@ R0 <- &intvec[0]
		LDR	R1, =shortvec		@ R1 <- &shortvec[0]
		MOV	R2, #8			@ R2 <- 8   (size parameter)
		BL	copyvec			@ copyvec (intvec, shortvec, 8)
		B	start

@ void copyvec (int[] dst, short[] src, int size)
copyvec:	PUSH	{R4, R5}		@ Kallad subrutin får inte skriva över R4 eller R5

		MOV	R3, #0			@ R3 <- i = 0
copyvecFOR:	CMP	R3, R2			@ >if (i >= size)
		BGE	copyvecENDFOR		@ >{ break; }
		LSL	R5, R3, #1		@ R5 <- i*2
		LDRSH	R4, [R1, R5]		@ R4 <- src[i] eller R4 <- [&src[0] + i*2]
		LSL	R5, R5, #1		@ R5 <- i*4 = R5*2
		STR	R4, [R0, R5]		@ R4 -> dst[i] eller R4  -> [&dst[0] + i*4]
		ADD	R3, R3, #1		@ R3 <- i+1 = R3+1
		B	copyvecFOR		@ Loop

copyvecENDFOR:	POP	{R4, R5}		@ Återställ R4 och R5
		BX	LR			@ return; (utan returvärde)

		.ORG	0x30
intvec:		.SPACE	32
shortvec:	.HWORD	1, 2, 3, 4, -1, -2, -3, -4
