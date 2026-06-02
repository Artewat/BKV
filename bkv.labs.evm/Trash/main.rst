                                      1 ;--------------------------------------------------------
                                      2 ; File Created by SDCC : free open source ISO C Compiler
                                      3 ; Version 4.5.0 #15242 (Linux)
                                      4 ;--------------------------------------------------------
                                      5 	.module main
                                      6 	
                                      7 	.optsdcc -mmcs51 --model-small
                                      8 ;--------------------------------------------------------
                                      9 ; Public variables in this module
                                     10 ;--------------------------------------------------------
                                     11 	.globl _main
                                     12 ;--------------------------------------------------------
                                     13 ; special function registers
                                     14 ;--------------------------------------------------------
                                     15 	.area RSEG    (ABS,DATA)
      000000                         16 	.org 0x0000
                                     17 ;--------------------------------------------------------
                                     18 ; special function bits
                                     19 ;--------------------------------------------------------
                                     20 	.area RSEG    (ABS,DATA)
      000000                         21 	.org 0x0000
                                     22 ;--------------------------------------------------------
                                     23 ; overlayable register banks
                                     24 ;--------------------------------------------------------
                                     25 	.area REG_BANK_0	(REL,OVR,DATA)
      000000                         26 	.ds 8
                                     27 ;--------------------------------------------------------
                                     28 ; internal ram data
                                     29 ;--------------------------------------------------------
                                     30 	.area DSEG    (DATA)
                                     31 ;--------------------------------------------------------
                                     32 ; overlayable items in internal ram
                                     33 ;--------------------------------------------------------
                                     34 ;--------------------------------------------------------
                                     35 ; Stack segment in internal ram
                                     36 ;--------------------------------------------------------
                                     37 	.area SSEG
      000008                         38 __start__stack:
      000008                         39 	.ds	1
                                     40 
                                     41 ;--------------------------------------------------------
                                     42 ; indirectly addressable internal ram data
                                     43 ;--------------------------------------------------------
                                     44 	.area ISEG    (DATA)
                                     45 ;--------------------------------------------------------
                                     46 ; absolute internal ram data
                                     47 ;--------------------------------------------------------
                                     48 	.area IABS    (ABS,DATA)
                                     49 	.area IABS    (ABS,DATA)
                                     50 ;--------------------------------------------------------
                                     51 ; bit data
                                     52 ;--------------------------------------------------------
                                     53 	.area BSEG    (BIT)
                                     54 ;--------------------------------------------------------
                                     55 ; paged external ram data
                                     56 ;--------------------------------------------------------
                                     57 	.area PSEG    (PAG,XDATA)
                                     58 ;--------------------------------------------------------
                                     59 ; uninitialized external ram data
                                     60 ;--------------------------------------------------------
                                     61 	.area XSEG    (XDATA)
                                     62 ;--------------------------------------------------------
                                     63 ; absolute external ram data
                                     64 ;--------------------------------------------------------
                                     65 	.area XABS    (ABS,XDATA)
                                     66 ;--------------------------------------------------------
                                     67 ; initialized external ram data
                                     68 ;--------------------------------------------------------
                                     69 	.area XISEG   (XDATA)
                                     70 	.area HOME    (CODE)
                                     71 	.area GSINIT0 (CODE)
                                     72 	.area GSINIT1 (CODE)
                                     73 	.area GSINIT2 (CODE)
                                     74 	.area GSINIT3 (CODE)
                                     75 	.area GSINIT4 (CODE)
                                     76 	.area GSINIT5 (CODE)
                                     77 	.area GSINIT  (CODE)
                                     78 	.area GSFINAL (CODE)
                                     79 	.area CSEG    (CODE)
                                     80 ;--------------------------------------------------------
                                     81 ; interrupt vector
                                     82 ;--------------------------------------------------------
                                     83 	.area HOME    (CODE)
      000000                         84 __interrupt_vect:
      000000 02 00 4C         [24]   85 	ljmp	__sdcc_gsinit_startup
                                     86 ; restartable atomic support routines
      000003                         87 	.ds	5
      000008                         88 sdcc_atomic_exchange_rollback_start::
      000008 00               [12]   89 	nop
      000009 00               [12]   90 	nop
      00000A                         91 sdcc_atomic_exchange_pdata_impl:
      00000A E2               [24]   92 	movx	a, @r0
      00000B FB               [12]   93 	mov	r3, a
      00000C EA               [12]   94 	mov	a, r2
      00000D F2               [24]   95 	movx	@r0, a
      00000E 80 2C            [24]   96 	sjmp	sdcc_atomic_exchange_exit
      000010 00               [12]   97 	nop
      000011 00               [12]   98 	nop
      000012                         99 sdcc_atomic_exchange_xdata_impl:
      000012 E0               [24]  100 	movx	a, @dptr
      000013 FB               [12]  101 	mov	r3, a
      000014 EA               [12]  102 	mov	a, r2
      000015 F0               [24]  103 	movx	@dptr, a
      000016 80 24            [24]  104 	sjmp	sdcc_atomic_exchange_exit
      000018                        105 sdcc_atomic_compare_exchange_idata_impl:
      000018 E6               [12]  106 	mov	a, @r0
      000019 B5 02 02         [24]  107 	cjne	a, ar2, .+#5
      00001C EB               [12]  108 	mov	a, r3
      00001D F6               [12]  109 	mov	@r0, a
      00001E 22               [24]  110 	ret
      00001F 00               [12]  111 	nop
      000020                        112 sdcc_atomic_compare_exchange_pdata_impl:
      000020 E2               [24]  113 	movx	a, @r0
      000021 B5 02 02         [24]  114 	cjne	a, ar2, .+#5
      000024 EB               [12]  115 	mov	a, r3
      000025 F2               [24]  116 	movx	@r0, a
      000026 22               [24]  117 	ret
      000027 00               [12]  118 	nop
      000028                        119 sdcc_atomic_compare_exchange_xdata_impl:
      000028 E0               [24]  120 	movx	a, @dptr
      000029 B5 02 02         [24]  121 	cjne	a, ar2, .+#5
      00002C EB               [12]  122 	mov	a, r3
      00002D F0               [24]  123 	movx	@dptr, a
      00002E 22               [24]  124 	ret
      00002F                        125 sdcc_atomic_exchange_rollback_end::
                                    126 
      00002F                        127 sdcc_atomic_exchange_gptr_impl::
      00002F 30 F6 E0         [24]  128 	jnb	b.6, sdcc_atomic_exchange_xdata_impl
      000032 A8 82            [24]  129 	mov	r0, dpl
      000034 20 F5 D3         [24]  130 	jb	b.5, sdcc_atomic_exchange_pdata_impl
      000037                        131 sdcc_atomic_exchange_idata_impl:
      000037 EA               [12]  132 	mov	a, r2
      000038 C6               [12]  133 	xch	a, @r0
      000039 F5 82            [12]  134 	mov	dpl, a
      00003B 22               [24]  135 	ret
      00003C                        136 sdcc_atomic_exchange_exit:
      00003C 8B 82            [24]  137 	mov	dpl, r3
      00003E 22               [24]  138 	ret
      00003F                        139 sdcc_atomic_compare_exchange_gptr_impl::
      00003F 30 F6 E6         [24]  140 	jnb	b.6, sdcc_atomic_compare_exchange_xdata_impl
      000042 A8 82            [24]  141 	mov	r0, dpl
      000044 20 F5 D9         [24]  142 	jb	b.5, sdcc_atomic_compare_exchange_pdata_impl
      000047 80 CF            [24]  143 	sjmp	sdcc_atomic_compare_exchange_idata_impl
                                    144 ;--------------------------------------------------------
                                    145 ; global & static initialisations
                                    146 ;--------------------------------------------------------
                                    147 	.area HOME    (CODE)
                                    148 	.area GSINIT  (CODE)
                                    149 	.area GSFINAL (CODE)
                                    150 	.area GSINIT  (CODE)
                                    151 	.globl __sdcc_gsinit_startup
                                    152 	.globl __sdcc_program_startup
                                    153 	.globl __start__stack
                                    154 	.globl __mcs51_genXINIT
                                    155 	.globl __mcs51_genXRAMCLEAR
                                    156 	.globl __mcs51_genRAMCLEAR
                                    157 	.area GSFINAL (CODE)
      0000A5 02 00 49         [24]  158 	ljmp	__sdcc_program_startup
                                    159 ;--------------------------------------------------------
                                    160 ; Home
                                    161 ;--------------------------------------------------------
                                    162 	.area HOME    (CODE)
                                    163 	.area HOME    (CODE)
      000049                        164 __sdcc_program_startup:
      000049 02 00 A8         [24]  165 	ljmp	_main
                                    166 ;	return from main will return to caller
                                    167 ;--------------------------------------------------------
                                    168 ; code
                                    169 ;--------------------------------------------------------
                                    170 	.area CSEG    (CODE)
                                    171 ;------------------------------------------------------------
                                    172 ;Allocation info for local variables in function 'main'
                                    173 ;------------------------------------------------------------
                                    174 ;	main.c:3: int main()
                                    175 ;	-----------------------------------------
                                    176 ;	 function main
                                    177 ;	-----------------------------------------
      0000A8                        178 _main:
                           000007   179 	ar7 = 0x07
                           000006   180 	ar6 = 0x06
                           000005   181 	ar5 = 0x05
                           000004   182 	ar4 = 0x04
                           000003   183 	ar3 = 0x03
                           000002   184 	ar2 = 0x02
                           000001   185 	ar1 = 0x01
                           000000   186 	ar0 = 0x00
                                    187 ;	main.c:6: }
      0000A8 22               [24]  188 	ret
                                    189 	.area CSEG    (CODE)
                                    190 	.area CONST   (CODE)
                                    191 	.area XINIT   (CODE)
                                    192 	.area CABS    (ABS,CODE)
