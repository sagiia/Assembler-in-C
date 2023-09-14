.entry LENGTH
.extern W
MAIN: mov @r3 ,LENGTH
mcro m2
    sub @r7, LENGTH
    bne STR
endmcro
LOOP: jmp L1
prn -5
bne W
sub @r1, @r4
mcro m1 
    sub @r1, @r4
    bne END
endmcro
m2
bne L3
L1: inc K
.entry LOOP
jmp W
m1
END: stop
STR: .string "abcdef"
LENGTH: .data 6,-9,15
K: .data 22
.extern L3