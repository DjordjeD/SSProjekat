.section mira
.global mira1,mira2
.extern mira3,mira4

.equ mira,mira1 + mira2


.word mira,mira1,mira2,0, 15, 16

labela_mira:
	mov ne_znam_gde_je_mira, 0x01
	mov 0x1, 22
	mov *%r0, simbol(%pc)

ne_znam_gde_je_mira:


labela_mira:
	
.end