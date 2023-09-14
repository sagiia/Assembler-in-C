; Macro definitions

mcro ADD_MACRO
add @r0,@r1
endmcro

mcro SUB_MACRO
sub @r1,@r2
endmcro

mcro MOV_MACRO
mov 10,@r2
endmcro

mcro CMP_MACRO
cmp @r3,0
endmcro

;External labels
.extern ExternalLabel1
.extern ExternalLabel2
.extern ExternalLabel3

; Entry labels
.entry EntryLabel1
.entry EntryLabel2
.entry EntryLabel3

MAIN:	mov 5,@r1
	cmp @r1,10
	bne LOOP
	clr @r2
	lea ExternalLabel1,@r3
	not @r4
	inc @r5
	dec @r6
	add @r7,@r1
	sub 2,@r1
	jmp Subroutine1
	bne ExternalLabel2
	red @r3
	prn @r4
	jsr Subroutine2
	rts
	stop

LOOP: dec @r1
	cmp @r1,0
	bne MAIN
	rts

Subroutine1: mov -1,@r5

Subroutine2: mov @r6,ExternalLabel3

; Data section
.data   5, -6, 15
.data   127, -128, 0, 255, -1
.data   1000, -1000

; String section
.string "Hello, World!"
.string "Sample string"

; Macro expansion
ADD_MACRO
MOV_MACRO
SUB_MACRO
CMP_MACRO

; Entry labels
EntryLabel1: .data 20
EntryLabel2: .data -30
EntryLabel3: .string "Entry Label"












