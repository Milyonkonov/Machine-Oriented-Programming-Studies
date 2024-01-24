
main: 
		LDR R0, =src
		LDR R1, =dst
		MOV R2, #2
		MOV R3, #6
		BL copyelements
loop: 		B loop

		.org 0x30
dst: 		.space 16
src: 		.word 1,2,3,4,-1,-2,-3,-4

@ void copyvec(int src[], short dst[], int size)
copyvec:	PUSH	{R4, R5, LR}		@ Kallad subrutin får inte skriva över R4 eller R5, spara returaddress

		MOV	R3, #0			@ R3 <- i = 0
copyvecFOR:	CMP	R3, R2			@ >if (i >= size)
		BGE	copyvecENDFOR		@ >{ break; }
		LSL	R5, R3, #2		@ R5 <- i*4
		LDR	R4, [R0, R5]		@ R4 <- src[i] eller R4 <- [&src[0] + i*4]
		LSL	R5, R3, #1		@ R5 <- i*2
		STRH	R4, [R1, R5]		@ R4  ->dst[i] eller R4  -> [&dst[0] + i*2]
		ADD	R3, R3, #1		@ R3 <- R3+1 = i+1
		B	copyvecFOR		@ Loop

copyvecENDFOR:	POP	{R4, R5, PC}		@ Återställ R4 och R5 samt returnera (utan returvärde)

@ void copyelements(int src[], short dst[], int start, int end)
copyelements:	PUSH	{R4, LR}

		@src_start (R0)
		LSL	R4, R2, #2	@ R4 <- start*4 (4 byte per int)
		ADD	R0, R0, R4	@ R0 <- src_start = &src + start*4

		@dst_start (R1)
		LSL	R4, R2, #1	@ R4 <- start*2 (2 byte per short)
		ADD	R1, R1, R4	@ R1 <- dst_start = &dst[0] + start*2

		@size (R2)
		SUB	R2, R3, R2	@ R2 <- size = end - start;

		BL	copyvec		@ copyvec(src_start, dst_start, size)
		POP	{R4, PC}
