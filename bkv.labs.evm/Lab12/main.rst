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
                                     12 	.globl _delay_ms
                                     13 	.globl _EIPX6
                                     14 	.globl _EIPX5
                                     15 	.globl _EIPX4
                                     16 	.globl _PI2C
                                     17 	.globl _PUSB
                                     18 	.globl _EIEX6
                                     19 	.globl _EIEX5
                                     20 	.globl _EIEX4
                                     21 	.globl _EI2C
                                     22 	.globl _EUSB
                                     23 	.globl _SMOD1
                                     24 	.globl _ERESI
                                     25 	.globl _RESI
                                     26 	.globl _INT6
                                     27 	.globl _CY
                                     28 	.globl _AC
                                     29 	.globl _F0
                                     30 	.globl _RS1
                                     31 	.globl _RS0
                                     32 	.globl _OV
                                     33 	.globl _FL
                                     34 	.globl _P
                                     35 	.globl _TF2
                                     36 	.globl _EXF2
                                     37 	.globl _RCLK
                                     38 	.globl _TCLK
                                     39 	.globl _EXEN2
                                     40 	.globl _TR2
                                     41 	.globl _C_T2
                                     42 	.globl _CP_RL2
                                     43 	.globl _SM01
                                     44 	.globl _SM11
                                     45 	.globl _SM21
                                     46 	.globl _REN1
                                     47 	.globl _TB81
                                     48 	.globl _RB81
                                     49 	.globl _TI1
                                     50 	.globl _RI1
                                     51 	.globl _PS1
                                     52 	.globl _PT2
                                     53 	.globl _PS0
                                     54 	.globl _PT1
                                     55 	.globl _PX1
                                     56 	.globl _PT0
                                     57 	.globl _PX0
                                     58 	.globl _PD7
                                     59 	.globl _PD6
                                     60 	.globl _PD5
                                     61 	.globl _PD4
                                     62 	.globl _PD3
                                     63 	.globl _PD2
                                     64 	.globl _PD1
                                     65 	.globl _PD0
                                     66 	.globl _EA
                                     67 	.globl _ES1
                                     68 	.globl _ET2
                                     69 	.globl _ES0
                                     70 	.globl _ET1
                                     71 	.globl _EX1
                                     72 	.globl _ET0
                                     73 	.globl _EX0
                                     74 	.globl _PC7
                                     75 	.globl _PC6
                                     76 	.globl _PC5
                                     77 	.globl _PC4
                                     78 	.globl _PC3
                                     79 	.globl _PC2
                                     80 	.globl _PC1
                                     81 	.globl _PC0
                                     82 	.globl _SM0
                                     83 	.globl _SM1
                                     84 	.globl _SM2
                                     85 	.globl _REN
                                     86 	.globl _TB8
                                     87 	.globl _RB8
                                     88 	.globl _TI
                                     89 	.globl _RI
                                     90 	.globl _PB7
                                     91 	.globl _PB6
                                     92 	.globl _PB5
                                     93 	.globl _PB4
                                     94 	.globl _PB3
                                     95 	.globl _PB2
                                     96 	.globl _PB1
                                     97 	.globl _PB0
                                     98 	.globl _TF1
                                     99 	.globl _TR1
                                    100 	.globl _TF0
                                    101 	.globl _TR0
                                    102 	.globl _IE1
                                    103 	.globl _IT1
                                    104 	.globl _IE0
                                    105 	.globl _IT0
                                    106 	.globl _PA7
                                    107 	.globl _PA6
                                    108 	.globl _PA5
                                    109 	.globl _PA4
                                    110 	.globl _PA3
                                    111 	.globl _PA2
                                    112 	.globl _PA1
                                    113 	.globl _PA0
                                    114 	.globl _EIP
                                    115 	.globl _B
                                    116 	.globl _EIE
                                    117 	.globl _ACC
                                    118 	.globl _EICON
                                    119 	.globl _PSW
                                    120 	.globl _TH2
                                    121 	.globl _TL2
                                    122 	.globl _RCAP2H
                                    123 	.globl _RCAP2L
                                    124 	.globl _T2CON
                                    125 	.globl _SBUF1
                                    126 	.globl _SCON1
                                    127 	.globl _GPIFSGLDATLNOX
                                    128 	.globl _GPIFSGLDATLX
                                    129 	.globl _GPIFSGLDATH
                                    130 	.globl _GPIFTRIG
                                    131 	.globl _EP01STAT
                                    132 	.globl _IP
                                    133 	.globl _OEE
                                    134 	.globl _OED
                                    135 	.globl _OEC
                                    136 	.globl _OEB
                                    137 	.globl _OEA
                                    138 	.globl _IOE
                                    139 	.globl _IOD
                                    140 	.globl _AUTOPTRSETUP
                                    141 	.globl _EP68FIFOFLGS
                                    142 	.globl _EP24FIFOFLGS
                                    143 	.globl _EP2468STAT
                                    144 	.globl _IE
                                    145 	.globl _INT4CLR
                                    146 	.globl _INT2CLR
                                    147 	.globl _IOC
                                    148 	.globl _AUTOPTRL2
                                    149 	.globl _AUTOPTRH2
                                    150 	.globl _AUTOPTRL1
                                    151 	.globl _AUTOPTRH1
                                    152 	.globl _SBUF0
                                    153 	.globl _SCON0
                                    154 	.globl __XPAGE
                                    155 	.globl _EXIF
                                    156 	.globl _IOB
                                    157 	.globl _CKCON
                                    158 	.globl _TH1
                                    159 	.globl _TH0
                                    160 	.globl _TL1
                                    161 	.globl _TL0
                                    162 	.globl _TMOD
                                    163 	.globl _TCON
                                    164 	.globl _PCON
                                    165 	.globl _DPS
                                    166 	.globl _DPH1
                                    167 	.globl _DPL1
                                    168 	.globl _DPH
                                    169 	.globl _DPL
                                    170 	.globl _SP
                                    171 	.globl _IOA
                                    172 	.globl _MY_PINSD
                                    173 	.globl _MY_OED
                                    174 	.globl _MY_IFCFG
                                    175 	.globl _GPCR2
                                    176 	.globl _ECC2B2
                                    177 	.globl _ECC2B1
                                    178 	.globl _ECC2B0
                                    179 	.globl _ECC1B2
                                    180 	.globl _ECC1B1
                                    181 	.globl _ECC1B0
                                    182 	.globl _ECCRESET
                                    183 	.globl _ECCCFG
                                    184 	.globl _EP8FIFOBUF
                                    185 	.globl _EP6FIFOBUF
                                    186 	.globl _EP4FIFOBUF
                                    187 	.globl _EP2FIFOBUF
                                    188 	.globl _EP1INBUF
                                    189 	.globl _EP1OUTBUF
                                    190 	.globl _EP0BUF
                                    191 	.globl _UDMACRCQUAL
                                    192 	.globl _UDMACRCL
                                    193 	.globl _UDMACRCH
                                    194 	.globl _GPIFHOLDAMOUNT
                                    195 	.globl _FLOWSTBHPERIOD
                                    196 	.globl _FLOWSTBEDGE
                                    197 	.globl _FLOWSTB
                                    198 	.globl _FLOWHOLDOFF
                                    199 	.globl _FLOWEQ1CTL
                                    200 	.globl _FLOWEQ0CTL
                                    201 	.globl _FLOWLOGIC
                                    202 	.globl _FLOWSTATE
                                    203 	.globl _GPIFABORT
                                    204 	.globl _GPIFREADYSTAT
                                    205 	.globl _GPIFREADYCFG
                                    206 	.globl _XGPIFSGLDATLNOX
                                    207 	.globl _XGPIFSGLDATLX
                                    208 	.globl _XGPIFSGLDATH
                                    209 	.globl _EP8GPIFTRIG
                                    210 	.globl _EP8GPIFPFSTOP
                                    211 	.globl _EP8GPIFFLGSEL
                                    212 	.globl _EP6GPIFTRIG
                                    213 	.globl _EP6GPIFPFSTOP
                                    214 	.globl _EP6GPIFFLGSEL
                                    215 	.globl _EP4GPIFTRIG
                                    216 	.globl _EP4GPIFPFSTOP
                                    217 	.globl _EP4GPIFFLGSEL
                                    218 	.globl _EP2GPIFTRIG
                                    219 	.globl _EP2GPIFPFSTOP
                                    220 	.globl _EP2GPIFFLGSEL
                                    221 	.globl _GPIFTCB0
                                    222 	.globl _GPIFTCB1
                                    223 	.globl _GPIFTCB2
                                    224 	.globl _GPIFTCB3
                                    225 	.globl _GPIFADRL
                                    226 	.globl _GPIFADRH
                                    227 	.globl _GPIFCTLCFG
                                    228 	.globl _GPIFIDLECTL
                                    229 	.globl _GPIFIDLECS
                                    230 	.globl _GPIFWFSELECT
                                    231 	.globl _SETUPDAT
                                    232 	.globl _SUDPTRCTL
                                    233 	.globl _SUDPTRL
                                    234 	.globl _SUDPTRH
                                    235 	.globl _EP8FIFOBCL
                                    236 	.globl _EP8FIFOBCH
                                    237 	.globl _EP6FIFOBCL
                                    238 	.globl _EP6FIFOBCH
                                    239 	.globl _EP4FIFOBCL
                                    240 	.globl _EP4FIFOBCH
                                    241 	.globl _EP2FIFOBCL
                                    242 	.globl _EP2FIFOBCH
                                    243 	.globl _EP8FIFOFLGS
                                    244 	.globl _EP6FIFOFLGS
                                    245 	.globl _EP4FIFOFLGS
                                    246 	.globl _EP2FIFOFLGS
                                    247 	.globl _EP8CS
                                    248 	.globl _EP6CS
                                    249 	.globl _EP4CS
                                    250 	.globl _EP2CS
                                    251 	.globl _EP1INCS
                                    252 	.globl _EP1OUTCS
                                    253 	.globl _EP0CS
                                    254 	.globl _EP8BCL
                                    255 	.globl _EP8BCH
                                    256 	.globl _EP6BCL
                                    257 	.globl _EP6BCH
                                    258 	.globl _EP4BCL
                                    259 	.globl _EP4BCH
                                    260 	.globl _EP2BCL
                                    261 	.globl _EP2BCH
                                    262 	.globl _EP1INBC
                                    263 	.globl _EP1OUTBC
                                    264 	.globl _EP0BCL
                                    265 	.globl _EP0BCH
                                    266 	.globl _FNADDR
                                    267 	.globl _MICROFRAME
                                    268 	.globl _USBFRAMEL
                                    269 	.globl _USBFRAMEH
                                    270 	.globl _TOGCTL
                                    271 	.globl _WAKEUPCS
                                    272 	.globl _SUSPEND
                                    273 	.globl _USBCS
                                    274 	.globl _XAUTODAT2
                                    275 	.globl _XAUTODAT1
                                    276 	.globl _I2CTL
                                    277 	.globl _I2DAT
                                    278 	.globl _I2CS
                                    279 	.globl _PORTECFG
                                    280 	.globl _PORTCCFG
                                    281 	.globl _PORTACFG
                                    282 	.globl _INTSETUP
                                    283 	.globl _INT4IVEC
                                    284 	.globl _INT2IVEC
                                    285 	.globl _CLRERRCNT
                                    286 	.globl _ERRCNTLIM
                                    287 	.globl _USBERRIRQ
                                    288 	.globl _USBERRIE
                                    289 	.globl _GPIFIRQ
                                    290 	.globl _GPIFIE
                                    291 	.globl _EPIRQ
                                    292 	.globl _EPIE
                                    293 	.globl _USBIRQ
                                    294 	.globl _USBIE
                                    295 	.globl _NAKIRQ
                                    296 	.globl _NAKIE
                                    297 	.globl _IBNIRQ
                                    298 	.globl _IBNIE
                                    299 	.globl _EP8FIFOIRQ
                                    300 	.globl _EP8FIFOIE
                                    301 	.globl _EP6FIFOIRQ
                                    302 	.globl _EP6FIFOIE
                                    303 	.globl _EP4FIFOIRQ
                                    304 	.globl _EP4FIFOIE
                                    305 	.globl _EP2FIFOIRQ
                                    306 	.globl _EP2FIFOIE
                                    307 	.globl _OUTPKTEND
                                    308 	.globl _INPKTEND
                                    309 	.globl _EP8ISOINPKTS
                                    310 	.globl _EP6ISOINPKTS
                                    311 	.globl _EP4ISOINPKTS
                                    312 	.globl _EP2ISOINPKTS
                                    313 	.globl _EP8FIFOPFL
                                    314 	.globl _EP8FIFOPFH
                                    315 	.globl _EP6FIFOPFL
                                    316 	.globl _EP6FIFOPFH
                                    317 	.globl _EP4FIFOPFL
                                    318 	.globl _EP4FIFOPFH
                                    319 	.globl _EP2FIFOPFL
                                    320 	.globl _EP2FIFOPFH
                                    321 	.globl _EP8AUTOINLENL
                                    322 	.globl _EP8AUTOINLENH
                                    323 	.globl _EP6AUTOINLENL
                                    324 	.globl _EP6AUTOINLENH
                                    325 	.globl _EP4AUTOINLENL
                                    326 	.globl _EP4AUTOINLENH
                                    327 	.globl _EP2AUTOINLENL
                                    328 	.globl _EP2AUTOINLENH
                                    329 	.globl _EP8FIFOCFG
                                    330 	.globl _EP6FIFOCFG
                                    331 	.globl _EP4FIFOCFG
                                    332 	.globl _EP2FIFOCFG
                                    333 	.globl _EP8CFG
                                    334 	.globl _EP6CFG
                                    335 	.globl _EP4CFG
                                    336 	.globl _EP2CFG
                                    337 	.globl _EP1INCFG
                                    338 	.globl _EP1OUTCFG
                                    339 	.globl _REVCTL
                                    340 	.globl _REVID
                                    341 	.globl _FIFOPINPOLAR
                                    342 	.globl _UART230
                                    343 	.globl _BPADDRL
                                    344 	.globl _BPADDRH
                                    345 	.globl _BREAKPT
                                    346 	.globl _FIFORESET
                                    347 	.globl _PINFLAGSCD
                                    348 	.globl _PINFLAGSAB
                                    349 	.globl _IFCONFIG
                                    350 	.globl _CPUCS
                                    351 	.globl _UNIQID5
                                    352 	.globl _UNIQID4
                                    353 	.globl _UNIQID3
                                    354 	.globl _UNIQID2
                                    355 	.globl _UNIQID1
                                    356 	.globl _UNIQID0
                                    357 	.globl _RES_WAVEDATA_END
                                    358 	.globl _GPIF_WAVE_DATA
                                    359 ;--------------------------------------------------------
                                    360 ; special function registers
                                    361 ;--------------------------------------------------------
                                    362 	.area RSEG    (ABS,DATA)
      000000                        363 	.org 0x0000
                           000080   364 _IOA	=	0x0080
                           000081   365 _SP	=	0x0081
                           000082   366 _DPL	=	0x0082
                           000083   367 _DPH	=	0x0083
                           000084   368 _DPL1	=	0x0084
                           000085   369 _DPH1	=	0x0085
                           000086   370 _DPS	=	0x0086
                           000087   371 _PCON	=	0x0087
                           000088   372 _TCON	=	0x0088
                           000089   373 _TMOD	=	0x0089
                           00008A   374 _TL0	=	0x008a
                           00008B   375 _TL1	=	0x008b
                           00008C   376 _TH0	=	0x008c
                           00008D   377 _TH1	=	0x008d
                           00008E   378 _CKCON	=	0x008e
                           000090   379 _IOB	=	0x0090
                           000091   380 _EXIF	=	0x0091
                           000092   381 __XPAGE	=	0x0092
                           000098   382 _SCON0	=	0x0098
                           000099   383 _SBUF0	=	0x0099
                           00009A   384 _AUTOPTRH1	=	0x009a
                           00009B   385 _AUTOPTRL1	=	0x009b
                           00009D   386 _AUTOPTRH2	=	0x009d
                           00009E   387 _AUTOPTRL2	=	0x009e
                           0000A0   388 _IOC	=	0x00a0
                           0000A1   389 _INT2CLR	=	0x00a1
                           0000A2   390 _INT4CLR	=	0x00a2
                           0000A8   391 _IE	=	0x00a8
                           0000AA   392 _EP2468STAT	=	0x00aa
                           0000AB   393 _EP24FIFOFLGS	=	0x00ab
                           0000AC   394 _EP68FIFOFLGS	=	0x00ac
                           0000AF   395 _AUTOPTRSETUP	=	0x00af
                           0000B0   396 _IOD	=	0x00b0
                           0000B1   397 _IOE	=	0x00b1
                           0000B2   398 _OEA	=	0x00b2
                           0000B3   399 _OEB	=	0x00b3
                           0000B4   400 _OEC	=	0x00b4
                           0000B5   401 _OED	=	0x00b5
                           0000B6   402 _OEE	=	0x00b6
                           0000B8   403 _IP	=	0x00b8
                           0000BA   404 _EP01STAT	=	0x00ba
                           0000BB   405 _GPIFTRIG	=	0x00bb
                           0000BD   406 _GPIFSGLDATH	=	0x00bd
                           0000BE   407 _GPIFSGLDATLX	=	0x00be
                           0000BF   408 _GPIFSGLDATLNOX	=	0x00bf
                           0000C0   409 _SCON1	=	0x00c0
                           0000C1   410 _SBUF1	=	0x00c1
                           0000C8   411 _T2CON	=	0x00c8
                           0000CA   412 _RCAP2L	=	0x00ca
                           0000CB   413 _RCAP2H	=	0x00cb
                           0000CC   414 _TL2	=	0x00cc
                           0000CD   415 _TH2	=	0x00cd
                           0000D0   416 _PSW	=	0x00d0
                           0000D8   417 _EICON	=	0x00d8
                           0000E0   418 _ACC	=	0x00e0
                           0000E8   419 _EIE	=	0x00e8
                           0000F0   420 _B	=	0x00f0
                           0000F8   421 _EIP	=	0x00f8
                                    422 ;--------------------------------------------------------
                                    423 ; special function bits
                                    424 ;--------------------------------------------------------
                                    425 	.area RSEG    (ABS,DATA)
      000000                        426 	.org 0x0000
                           000080   427 _PA0	=	0x0080
                           000081   428 _PA1	=	0x0081
                           000082   429 _PA2	=	0x0082
                           000083   430 _PA3	=	0x0083
                           000084   431 _PA4	=	0x0084
                           000085   432 _PA5	=	0x0085
                           000086   433 _PA6	=	0x0086
                           000087   434 _PA7	=	0x0087
                           000088   435 _IT0	=	0x0088
                           000089   436 _IE0	=	0x0089
                           00008A   437 _IT1	=	0x008a
                           00008B   438 _IE1	=	0x008b
                           00008C   439 _TR0	=	0x008c
                           00008D   440 _TF0	=	0x008d
                           00008E   441 _TR1	=	0x008e
                           00008F   442 _TF1	=	0x008f
                           000090   443 _PB0	=	0x0090
                           000091   444 _PB1	=	0x0091
                           000092   445 _PB2	=	0x0092
                           000093   446 _PB3	=	0x0093
                           000094   447 _PB4	=	0x0094
                           000095   448 _PB5	=	0x0095
                           000096   449 _PB6	=	0x0096
                           000097   450 _PB7	=	0x0097
                           000098   451 _RI	=	0x0098
                           000099   452 _TI	=	0x0099
                           00009A   453 _RB8	=	0x009a
                           00009B   454 _TB8	=	0x009b
                           00009C   455 _REN	=	0x009c
                           00009D   456 _SM2	=	0x009d
                           00009E   457 _SM1	=	0x009e
                           00009F   458 _SM0	=	0x009f
                           0000A0   459 _PC0	=	0x00a0
                           0000A1   460 _PC1	=	0x00a1
                           0000A2   461 _PC2	=	0x00a2
                           0000A3   462 _PC3	=	0x00a3
                           0000A4   463 _PC4	=	0x00a4
                           0000A5   464 _PC5	=	0x00a5
                           0000A6   465 _PC6	=	0x00a6
                           0000A7   466 _PC7	=	0x00a7
                           0000A8   467 _EX0	=	0x00a8
                           0000A9   468 _ET0	=	0x00a9
                           0000AA   469 _EX1	=	0x00aa
                           0000AB   470 _ET1	=	0x00ab
                           0000AC   471 _ES0	=	0x00ac
                           0000AD   472 _ET2	=	0x00ad
                           0000AE   473 _ES1	=	0x00ae
                           0000AF   474 _EA	=	0x00af
                           0000B0   475 _PD0	=	0x00b0
                           0000B1   476 _PD1	=	0x00b1
                           0000B2   477 _PD2	=	0x00b2
                           0000B3   478 _PD3	=	0x00b3
                           0000B4   479 _PD4	=	0x00b4
                           0000B5   480 _PD5	=	0x00b5
                           0000B6   481 _PD6	=	0x00b6
                           0000B7   482 _PD7	=	0x00b7
                           0000B8   483 _PX0	=	0x00b8
                           0000B9   484 _PT0	=	0x00b9
                           0000BA   485 _PX1	=	0x00ba
                           0000BB   486 _PT1	=	0x00bb
                           0000BC   487 _PS0	=	0x00bc
                           0000BD   488 _PT2	=	0x00bd
                           0000BE   489 _PS1	=	0x00be
                           0000C0   490 _RI1	=	0x00c0
                           0000C1   491 _TI1	=	0x00c1
                           0000C2   492 _RB81	=	0x00c2
                           0000C3   493 _TB81	=	0x00c3
                           0000C4   494 _REN1	=	0x00c4
                           0000C5   495 _SM21	=	0x00c5
                           0000C6   496 _SM11	=	0x00c6
                           0000C7   497 _SM01	=	0x00c7
                           0000C8   498 _CP_RL2	=	0x00c8
                           0000C9   499 _C_T2	=	0x00c9
                           0000CA   500 _TR2	=	0x00ca
                           0000CB   501 _EXEN2	=	0x00cb
                           0000CC   502 _TCLK	=	0x00cc
                           0000CD   503 _RCLK	=	0x00cd
                           0000CE   504 _EXF2	=	0x00ce
                           0000CF   505 _TF2	=	0x00cf
                           0000D0   506 _P	=	0x00d0
                           0000D1   507 _FL	=	0x00d1
                           0000D2   508 _OV	=	0x00d2
                           0000D3   509 _RS0	=	0x00d3
                           0000D4   510 _RS1	=	0x00d4
                           0000D5   511 _F0	=	0x00d5
                           0000D6   512 _AC	=	0x00d6
                           0000D7   513 _CY	=	0x00d7
                           0000DB   514 _INT6	=	0x00db
                           0000DC   515 _RESI	=	0x00dc
                           0000DD   516 _ERESI	=	0x00dd
                           0000DF   517 _SMOD1	=	0x00df
                           0000E8   518 _EUSB	=	0x00e8
                           0000E9   519 _EI2C	=	0x00e9
                           0000EA   520 _EIEX4	=	0x00ea
                           0000EB   521 _EIEX5	=	0x00eb
                           0000EC   522 _EIEX6	=	0x00ec
                           0000F8   523 _PUSB	=	0x00f8
                           0000F9   524 _PI2C	=	0x00f9
                           0000FA   525 _EIPX4	=	0x00fa
                           0000FB   526 _EIPX5	=	0x00fb
                           0000FC   527 _EIPX6	=	0x00fc
                                    528 ;--------------------------------------------------------
                                    529 ; overlayable register banks
                                    530 ;--------------------------------------------------------
                                    531 	.area REG_BANK_0	(REL,OVR,DATA)
      000000                        532 	.ds 8
                                    533 ;--------------------------------------------------------
                                    534 ; internal ram data
                                    535 ;--------------------------------------------------------
                                    536 	.area DSEG    (DATA)
                                    537 ;--------------------------------------------------------
                                    538 ; overlayable items in internal ram
                                    539 ;--------------------------------------------------------
                                    540 	.area	OSEG    (OVR,DATA)
                                    541 ;--------------------------------------------------------
                                    542 ; Stack segment in internal ram
                                    543 ;--------------------------------------------------------
                                    544 	.area SSEG
      000008                        545 __start__stack:
      000008                        546 	.ds	1
                                    547 
                                    548 ;--------------------------------------------------------
                                    549 ; indirectly addressable internal ram data
                                    550 ;--------------------------------------------------------
                                    551 	.area ISEG    (DATA)
                                    552 ;--------------------------------------------------------
                                    553 ; absolute internal ram data
                                    554 ;--------------------------------------------------------
                                    555 	.area IABS    (ABS,DATA)
                                    556 	.area IABS    (ABS,DATA)
                                    557 ;--------------------------------------------------------
                                    558 ; bit data
                                    559 ;--------------------------------------------------------
                                    560 	.area BSEG    (BIT)
                                    561 ;--------------------------------------------------------
                                    562 ; paged external ram data
                                    563 ;--------------------------------------------------------
                                    564 	.area PSEG    (PAG,XDATA)
                                    565 ;--------------------------------------------------------
                                    566 ; uninitialized external ram data
                                    567 ;--------------------------------------------------------
                                    568 	.area XSEG    (XDATA)
                           00E400   569 _GPIF_WAVE_DATA	=	0xe400
                           00E480   570 _RES_WAVEDATA_END	=	0xe480
                           00E507   571 _UNIQID0	=	0xe507
                           00E508   572 _UNIQID1	=	0xe508
                           00E509   573 _UNIQID2	=	0xe509
                           00E50A   574 _UNIQID3	=	0xe50a
                           00E50B   575 _UNIQID4	=	0xe50b
                           00E50C   576 _UNIQID5	=	0xe50c
                           00E600   577 _CPUCS	=	0xe600
                           00E601   578 _IFCONFIG	=	0xe601
                           00E602   579 _PINFLAGSAB	=	0xe602
                           00E603   580 _PINFLAGSCD	=	0xe603
                           00E604   581 _FIFORESET	=	0xe604
                           00E605   582 _BREAKPT	=	0xe605
                           00E606   583 _BPADDRH	=	0xe606
                           00E607   584 _BPADDRL	=	0xe607
                           00E608   585 _UART230	=	0xe608
                           00E609   586 _FIFOPINPOLAR	=	0xe609
                           00E60A   587 _REVID	=	0xe60a
                           00E60B   588 _REVCTL	=	0xe60b
                           00E610   589 _EP1OUTCFG	=	0xe610
                           00E611   590 _EP1INCFG	=	0xe611
                           00E612   591 _EP2CFG	=	0xe612
                           00E613   592 _EP4CFG	=	0xe613
                           00E614   593 _EP6CFG	=	0xe614
                           00E615   594 _EP8CFG	=	0xe615
                           00E618   595 _EP2FIFOCFG	=	0xe618
                           00E619   596 _EP4FIFOCFG	=	0xe619
                           00E61A   597 _EP6FIFOCFG	=	0xe61a
                           00E61B   598 _EP8FIFOCFG	=	0xe61b
                           00E620   599 _EP2AUTOINLENH	=	0xe620
                           00E621   600 _EP2AUTOINLENL	=	0xe621
                           00E622   601 _EP4AUTOINLENH	=	0xe622
                           00E623   602 _EP4AUTOINLENL	=	0xe623
                           00E624   603 _EP6AUTOINLENH	=	0xe624
                           00E625   604 _EP6AUTOINLENL	=	0xe625
                           00E626   605 _EP8AUTOINLENH	=	0xe626
                           00E627   606 _EP8AUTOINLENL	=	0xe627
                           00E630   607 _EP2FIFOPFH	=	0xe630
                           00E631   608 _EP2FIFOPFL	=	0xe631
                           00E632   609 _EP4FIFOPFH	=	0xe632
                           00E633   610 _EP4FIFOPFL	=	0xe633
                           00E634   611 _EP6FIFOPFH	=	0xe634
                           00E635   612 _EP6FIFOPFL	=	0xe635
                           00E636   613 _EP8FIFOPFH	=	0xe636
                           00E637   614 _EP8FIFOPFL	=	0xe637
                           00E640   615 _EP2ISOINPKTS	=	0xe640
                           00E641   616 _EP4ISOINPKTS	=	0xe641
                           00E642   617 _EP6ISOINPKTS	=	0xe642
                           00E643   618 _EP8ISOINPKTS	=	0xe643
                           00E648   619 _INPKTEND	=	0xe648
                           00E649   620 _OUTPKTEND	=	0xe649
                           00E650   621 _EP2FIFOIE	=	0xe650
                           00E651   622 _EP2FIFOIRQ	=	0xe651
                           00E652   623 _EP4FIFOIE	=	0xe652
                           00E653   624 _EP4FIFOIRQ	=	0xe653
                           00E654   625 _EP6FIFOIE	=	0xe654
                           00E655   626 _EP6FIFOIRQ	=	0xe655
                           00E656   627 _EP8FIFOIE	=	0xe656
                           00E657   628 _EP8FIFOIRQ	=	0xe657
                           00E658   629 _IBNIE	=	0xe658
                           00E659   630 _IBNIRQ	=	0xe659
                           00E65A   631 _NAKIE	=	0xe65a
                           00E65B   632 _NAKIRQ	=	0xe65b
                           00E65C   633 _USBIE	=	0xe65c
                           00E65D   634 _USBIRQ	=	0xe65d
                           00E65E   635 _EPIE	=	0xe65e
                           00E65F   636 _EPIRQ	=	0xe65f
                           00E660   637 _GPIFIE	=	0xe660
                           00E661   638 _GPIFIRQ	=	0xe661
                           00E662   639 _USBERRIE	=	0xe662
                           00E663   640 _USBERRIRQ	=	0xe663
                           00E664   641 _ERRCNTLIM	=	0xe664
                           00E665   642 _CLRERRCNT	=	0xe665
                           00E666   643 _INT2IVEC	=	0xe666
                           00E667   644 _INT4IVEC	=	0xe667
                           00E668   645 _INTSETUP	=	0xe668
                           00E670   646 _PORTACFG	=	0xe670
                           00E671   647 _PORTCCFG	=	0xe671
                           00E672   648 _PORTECFG	=	0xe672
                           00E678   649 _I2CS	=	0xe678
                           00E679   650 _I2DAT	=	0xe679
                           00E67A   651 _I2CTL	=	0xe67a
                           00E67B   652 _XAUTODAT1	=	0xe67b
                           00E67C   653 _XAUTODAT2	=	0xe67c
                           00E680   654 _USBCS	=	0xe680
                           00E681   655 _SUSPEND	=	0xe681
                           00E682   656 _WAKEUPCS	=	0xe682
                           00E683   657 _TOGCTL	=	0xe683
                           00E684   658 _USBFRAMEH	=	0xe684
                           00E685   659 _USBFRAMEL	=	0xe685
                           00E686   660 _MICROFRAME	=	0xe686
                           00E687   661 _FNADDR	=	0xe687
                           00E68A   662 _EP0BCH	=	0xe68a
                           00E68B   663 _EP0BCL	=	0xe68b
                           00E68D   664 _EP1OUTBC	=	0xe68d
                           00E68F   665 _EP1INBC	=	0xe68f
                           00E690   666 _EP2BCH	=	0xe690
                           00E691   667 _EP2BCL	=	0xe691
                           00E694   668 _EP4BCH	=	0xe694
                           00E695   669 _EP4BCL	=	0xe695
                           00E698   670 _EP6BCH	=	0xe698
                           00E699   671 _EP6BCL	=	0xe699
                           00E69C   672 _EP8BCH	=	0xe69c
                           00E69D   673 _EP8BCL	=	0xe69d
                           00E6A0   674 _EP0CS	=	0xe6a0
                           00E6A1   675 _EP1OUTCS	=	0xe6a1
                           00E6A2   676 _EP1INCS	=	0xe6a2
                           00E6A3   677 _EP2CS	=	0xe6a3
                           00E6A4   678 _EP4CS	=	0xe6a4
                           00E6A5   679 _EP6CS	=	0xe6a5
                           00E6A6   680 _EP8CS	=	0xe6a6
                           00E6A7   681 _EP2FIFOFLGS	=	0xe6a7
                           00E6A8   682 _EP4FIFOFLGS	=	0xe6a8
                           00E6A9   683 _EP6FIFOFLGS	=	0xe6a9
                           00E6AA   684 _EP8FIFOFLGS	=	0xe6aa
                           00E6AB   685 _EP2FIFOBCH	=	0xe6ab
                           00E6AC   686 _EP2FIFOBCL	=	0xe6ac
                           00E6AD   687 _EP4FIFOBCH	=	0xe6ad
                           00E6AE   688 _EP4FIFOBCL	=	0xe6ae
                           00E6AF   689 _EP6FIFOBCH	=	0xe6af
                           00E6B0   690 _EP6FIFOBCL	=	0xe6b0
                           00E6B1   691 _EP8FIFOBCH	=	0xe6b1
                           00E6B2   692 _EP8FIFOBCL	=	0xe6b2
                           00E6B3   693 _SUDPTRH	=	0xe6b3
                           00E6B4   694 _SUDPTRL	=	0xe6b4
                           00E6B5   695 _SUDPTRCTL	=	0xe6b5
                           00E6B8   696 _SETUPDAT	=	0xe6b8
                           00E6C0   697 _GPIFWFSELECT	=	0xe6c0
                           00E6C1   698 _GPIFIDLECS	=	0xe6c1
                           00E6C2   699 _GPIFIDLECTL	=	0xe6c2
                           00E6C3   700 _GPIFCTLCFG	=	0xe6c3
                           00E6C4   701 _GPIFADRH	=	0xe6c4
                           00E6C5   702 _GPIFADRL	=	0xe6c5
                           00E6CE   703 _GPIFTCB3	=	0xe6ce
                           00E6CF   704 _GPIFTCB2	=	0xe6cf
                           00E6D0   705 _GPIFTCB1	=	0xe6d0
                           00E6D1   706 _GPIFTCB0	=	0xe6d1
                           00E6D2   707 _EP2GPIFFLGSEL	=	0xe6d2
                           00E6D3   708 _EP2GPIFPFSTOP	=	0xe6d3
                           00E6D4   709 _EP2GPIFTRIG	=	0xe6d4
                           00E6DA   710 _EP4GPIFFLGSEL	=	0xe6da
                           00E6DB   711 _EP4GPIFPFSTOP	=	0xe6db
                           00E6DC   712 _EP4GPIFTRIG	=	0xe6dc
                           00E6E2   713 _EP6GPIFFLGSEL	=	0xe6e2
                           00E6E3   714 _EP6GPIFPFSTOP	=	0xe6e3
                           00E6E4   715 _EP6GPIFTRIG	=	0xe6e4
                           00E6EA   716 _EP8GPIFFLGSEL	=	0xe6ea
                           00E6EB   717 _EP8GPIFPFSTOP	=	0xe6eb
                           00E6EC   718 _EP8GPIFTRIG	=	0xe6ec
                           00E6F0   719 _XGPIFSGLDATH	=	0xe6f0
                           00E6F1   720 _XGPIFSGLDATLX	=	0xe6f1
                           00E6F2   721 _XGPIFSGLDATLNOX	=	0xe6f2
                           00E6F3   722 _GPIFREADYCFG	=	0xe6f3
                           00E6F4   723 _GPIFREADYSTAT	=	0xe6f4
                           00E6F5   724 _GPIFABORT	=	0xe6f5
                           00E6C6   725 _FLOWSTATE	=	0xe6c6
                           00E6C7   726 _FLOWLOGIC	=	0xe6c7
                           00E6C8   727 _FLOWEQ0CTL	=	0xe6c8
                           00E6C9   728 _FLOWEQ1CTL	=	0xe6c9
                           00E6CA   729 _FLOWHOLDOFF	=	0xe6ca
                           00E6CB   730 _FLOWSTB	=	0xe6cb
                           00E6CC   731 _FLOWSTBEDGE	=	0xe6cc
                           00E6CD   732 _FLOWSTBHPERIOD	=	0xe6cd
                           00E60C   733 _GPIFHOLDAMOUNT	=	0xe60c
                           00E67D   734 _UDMACRCH	=	0xe67d
                           00E67E   735 _UDMACRCL	=	0xe67e
                           00E67F   736 _UDMACRCQUAL	=	0xe67f
                           00E740   737 _EP0BUF	=	0xe740
                           00E780   738 _EP1OUTBUF	=	0xe780
                           00E7C0   739 _EP1INBUF	=	0xe7c0
                           00F000   740 _EP2FIFOBUF	=	0xf000
                           00F400   741 _EP4FIFOBUF	=	0xf400
                           00F800   742 _EP6FIFOBUF	=	0xf800
                           00FC00   743 _EP8FIFOBUF	=	0xfc00
                           00E628   744 _ECCCFG	=	0xe628
                           00E629   745 _ECCRESET	=	0xe629
                           00E62A   746 _ECC1B0	=	0xe62a
                           00E62B   747 _ECC1B1	=	0xe62b
                           00E62C   748 _ECC1B2	=	0xe62c
                           00E62D   749 _ECC2B0	=	0xe62d
                           00E62E   750 _ECC2B1	=	0xe62e
                           00E62F   751 _ECC2B2	=	0xe62f
                           00E50D   752 _GPCR2	=	0xe50d
                           00E601   753 _MY_IFCFG	=	0xe601
                           00E604   754 _MY_OED	=	0xe604
                           00E607   755 _MY_PINSD	=	0xe607
                                    756 ;--------------------------------------------------------
                                    757 ; absolute external ram data
                                    758 ;--------------------------------------------------------
                                    759 	.area XABS    (ABS,XDATA)
                                    760 ;--------------------------------------------------------
                                    761 ; initialized external ram data
                                    762 ;--------------------------------------------------------
                                    763 	.area XISEG   (XDATA)
                                    764 	.area HOME    (CODE)
                                    765 	.area GSINIT0 (CODE)
                                    766 	.area GSINIT1 (CODE)
                                    767 	.area GSINIT2 (CODE)
                                    768 	.area GSINIT3 (CODE)
                                    769 	.area GSINIT4 (CODE)
                                    770 	.area GSINIT5 (CODE)
                                    771 	.area GSINIT  (CODE)
                                    772 	.area GSFINAL (CODE)
                                    773 	.area CSEG    (CODE)
                                    774 ;--------------------------------------------------------
                                    775 ; interrupt vector
                                    776 ;--------------------------------------------------------
                                    777 	.area HOME    (CODE)
      000000                        778 __interrupt_vect:
      000000 02 00 4C         [24]  779 	ljmp	__sdcc_gsinit_startup
                                    780 ; restartable atomic support routines
      000003                        781 	.ds	5
      000008                        782 sdcc_atomic_exchange_rollback_start::
      000008 00               [12]  783 	nop
      000009 00               [12]  784 	nop
      00000A                        785 sdcc_atomic_exchange_pdata_impl:
      00000A E2               [24]  786 	movx	a, @r0
      00000B FB               [12]  787 	mov	r3, a
      00000C EA               [12]  788 	mov	a, r2
      00000D F2               [24]  789 	movx	@r0, a
      00000E 80 2C            [24]  790 	sjmp	sdcc_atomic_exchange_exit
      000010 00               [12]  791 	nop
      000011 00               [12]  792 	nop
      000012                        793 sdcc_atomic_exchange_xdata_impl:
      000012 E0               [24]  794 	movx	a, @dptr
      000013 FB               [12]  795 	mov	r3, a
      000014 EA               [12]  796 	mov	a, r2
      000015 F0               [24]  797 	movx	@dptr, a
      000016 80 24            [24]  798 	sjmp	sdcc_atomic_exchange_exit
      000018                        799 sdcc_atomic_compare_exchange_idata_impl:
      000018 E6               [12]  800 	mov	a, @r0
      000019 B5 02 02         [24]  801 	cjne	a, ar2, .+#5
      00001C EB               [12]  802 	mov	a, r3
      00001D F6               [12]  803 	mov	@r0, a
      00001E 22               [24]  804 	ret
      00001F 00               [12]  805 	nop
      000020                        806 sdcc_atomic_compare_exchange_pdata_impl:
      000020 E2               [24]  807 	movx	a, @r0
      000021 B5 02 02         [24]  808 	cjne	a, ar2, .+#5
      000024 EB               [12]  809 	mov	a, r3
      000025 F2               [24]  810 	movx	@r0, a
      000026 22               [24]  811 	ret
      000027 00               [12]  812 	nop
      000028                        813 sdcc_atomic_compare_exchange_xdata_impl:
      000028 E0               [24]  814 	movx	a, @dptr
      000029 B5 02 02         [24]  815 	cjne	a, ar2, .+#5
      00002C EB               [12]  816 	mov	a, r3
      00002D F0               [24]  817 	movx	@dptr, a
      00002E 22               [24]  818 	ret
      00002F                        819 sdcc_atomic_exchange_rollback_end::
                                    820 
      00002F                        821 sdcc_atomic_exchange_gptr_impl::
      00002F 30 F6 E0         [24]  822 	jnb	b.6, sdcc_atomic_exchange_xdata_impl
      000032 A8 82            [24]  823 	mov	r0, dpl
      000034 20 F5 D3         [24]  824 	jb	b.5, sdcc_atomic_exchange_pdata_impl
      000037                        825 sdcc_atomic_exchange_idata_impl:
      000037 EA               [12]  826 	mov	a, r2
      000038 C6               [12]  827 	xch	a, @r0
      000039 F5 82            [12]  828 	mov	dpl, a
      00003B 22               [24]  829 	ret
      00003C                        830 sdcc_atomic_exchange_exit:
      00003C 8B 82            [24]  831 	mov	dpl, r3
      00003E 22               [24]  832 	ret
      00003F                        833 sdcc_atomic_compare_exchange_gptr_impl::
      00003F 30 F6 E6         [24]  834 	jnb	b.6, sdcc_atomic_compare_exchange_xdata_impl
      000042 A8 82            [24]  835 	mov	r0, dpl
      000044 20 F5 D9         [24]  836 	jb	b.5, sdcc_atomic_compare_exchange_pdata_impl
      000047 80 CF            [24]  837 	sjmp	sdcc_atomic_compare_exchange_idata_impl
                                    838 ;--------------------------------------------------------
                                    839 ; global & static initialisations
                                    840 ;--------------------------------------------------------
                                    841 	.area HOME    (CODE)
                                    842 	.area GSINIT  (CODE)
                                    843 	.area GSFINAL (CODE)
                                    844 	.area GSINIT  (CODE)
                                    845 	.globl __sdcc_gsinit_startup
                                    846 	.globl __sdcc_program_startup
                                    847 	.globl __start__stack
                                    848 	.globl __mcs51_genXINIT
                                    849 	.globl __mcs51_genXRAMCLEAR
                                    850 	.globl __mcs51_genRAMCLEAR
                                    851 	.area GSFINAL (CODE)
      0000A5 02 00 49         [24]  852 	ljmp	__sdcc_program_startup
                                    853 ;--------------------------------------------------------
                                    854 ; Home
                                    855 ;--------------------------------------------------------
                                    856 	.area HOME    (CODE)
                                    857 	.area HOME    (CODE)
      000049                        858 __sdcc_program_startup:
      000049 02 00 CA         [24]  859 	ljmp	_main
                                    860 ;	return from main will return to caller
                                    861 ;--------------------------------------------------------
                                    862 ; code
                                    863 ;--------------------------------------------------------
                                    864 	.area CSEG    (CODE)
                                    865 ;------------------------------------------------------------
                                    866 ;Allocation info for local variables in function 'delay_ms'
                                    867 ;------------------------------------------------------------
                                    868 ;ms            Allocated to registers 
                                    869 ;i             Allocated to registers r4 r5 
                                    870 ;------------------------------------------------------------
                                    871 ;	main.c:8: void delay_ms(unsigned int ms) {
                                    872 ;	-----------------------------------------
                                    873 ;	 function delay_ms
                                    874 ;	-----------------------------------------
      0000A8                        875 _delay_ms:
                           000007   876 	ar7 = 0x07
                           000006   877 	ar6 = 0x06
                           000005   878 	ar5 = 0x05
                           000004   879 	ar4 = 0x04
                           000003   880 	ar3 = 0x03
                           000002   881 	ar2 = 0x02
                           000001   882 	ar1 = 0x01
                           000000   883 	ar0 = 0x00
      0000A8 AE 82            [24]  884 	mov	r6, dpl
      0000AA AF 83            [24]  885 	mov	r7, dph
                                    886 ;	main.c:10: while(ms--) {
      0000AC                        887 00102$:
      0000AC 8E 04            [24]  888 	mov	ar4,r6
      0000AE 8F 05            [24]  889 	mov	ar5,r7
      0000B0 1E               [12]  890 	dec	r6
      0000B1 BE FF 01         [24]  891 	cjne	r6,#0xff,00136$
      0000B4 1F               [12]  892 	dec	r7
      0000B5                        893 00136$:
      0000B5 EC               [12]  894 	mov	a,r4
      0000B6 4D               [12]  895 	orl	a,r5
      0000B7 60 10            [24]  896 	jz	00108$
                                    897 ;	main.c:11: for(i = 0; i < 600; i++) {
      0000B9 7C 58            [12]  898 	mov	r4,#0x58
      0000BB 7D 02            [12]  899 	mov	r5,#0x02
      0000BD                        900 00107$:
                                    901 ;	main.c:12: __asm NOP __endasm;
      0000BD 00               [12]  902 	NOP	
      0000BE 1C               [12]  903 	dec	r4
      0000BF BC FF 01         [24]  904 	cjne	r4,#0xff,00138$
      0000C2 1D               [12]  905 	dec	r5
      0000C3                        906 00138$:
                                    907 ;	main.c:11: for(i = 0; i < 600; i++) {
      0000C3 EC               [12]  908 	mov	a,r4
      0000C4 4D               [12]  909 	orl	a,r5
      0000C5 70 F6            [24]  910 	jnz	00107$
      0000C7 80 E3            [24]  911 	sjmp	00102$
      0000C9                        912 00108$:
                                    913 ;	main.c:15: }
      0000C9 22               [24]  914 	ret
                                    915 ;------------------------------------------------------------
                                    916 ;Allocation info for local variables in function 'main'
                                    917 ;------------------------------------------------------------
                                    918 ;	main.c:17: void main(void) {
                                    919 ;	-----------------------------------------
                                    920 ;	 function main
                                    921 ;	-----------------------------------------
      0000CA                        922 _main:
                                    923 ;	main.c:19: MY_IFCFG &= ~0x03; 
      0000CA 90 E6 01         [24]  924 	mov	dptr,#_MY_IFCFG
      0000CD E0               [24]  925 	movx	a,@dptr
      0000CE 53 E0 FC         [24]  926 	anl	acc,#0xfc
      0000D1 F0               [24]  927 	movx	@dptr,a
                                    928 ;	main.c:22: MY_OED &= ~0x03; 
      0000D2 90 E6 04         [24]  929 	mov	dptr,#_MY_OED
      0000D5 E0               [24]  930 	movx	a,@dptr
      0000D6 53 E0 FC         [24]  931 	anl	acc,#0xfc
      0000D9 F0               [24]  932 	movx	@dptr,a
                                    933 ;	main.c:25: OEB = 0xFF; 
      0000DA 75 B3 FF         [24]  934 	mov	_OEB,#0xff
                                    935 ;	main.c:26: IOB = 0x00; 
      0000DD 75 90 00         [24]  936 	mov	_IOB,#0x00
                                    937 ;	main.c:29: IOB = 0x0F; 
      0000E0 75 90 0F         [24]  938 	mov	_IOB,#0x0f
                                    939 ;	main.c:30: delay_ms(1000); 
      0000E3 90 03 E8         [24]  940 	mov	dptr,#0x03e8
      0000E6 12 00 A8         [24]  941 	lcall	_delay_ms
                                    942 ;	main.c:32: while(1) {
      0000E9                        943 00114$:
                                    944 ;	main.c:35: if ((MY_PINSD & 0x01) == 0) {
      0000E9 90 E6 07         [24]  945 	mov	dptr,#_MY_PINSD
      0000EC E0               [24]  946 	movx	a,@dptr
      0000ED 20 E0 12         [24]  947 	jb	acc.0,00111$
                                    948 ;	main.c:36: IOB = 0xAA; // Зажигаем светодиоды через один (10101010)
      0000F0 75 90 AA         [24]  949 	mov	_IOB,#0xaa
                                    950 ;	main.c:37: while ((MY_PINSD & 0x01) == 0); // Ждем, пока кнопку ОТПУСТЯТ
      0000F3                        951 00101$:
      0000F3 90 E6 07         [24]  952 	mov	dptr,#_MY_PINSD
      0000F6 E0               [24]  953 	movx	a,@dptr
      0000F7 30 E0 F9         [24]  954 	jnb	acc.0,00101$
                                    955 ;	main.c:38: delay_ms(50); // Защита от дребезга
      0000FA 90 00 32         [24]  956 	mov	dptr,#0x0032
      0000FD 12 00 A8         [24]  957 	lcall	_delay_ms
      000100 80 E7            [24]  958 	sjmp	00114$
      000102                        959 00111$:
                                    960 ;	main.c:43: else if ((MY_PINSD & 0x02) == 0) {
      000102 90 E6 07         [24]  961 	mov	dptr,#_MY_PINSD
      000105 E0               [24]  962 	movx	a,@dptr
      000106 20 E1 12         [24]  963 	jb	acc.1,00108$
                                    964 ;	main.c:44: IOB = 0x55; // Зажигаем инверсный вариант (01010101)
      000109 75 90 55         [24]  965 	mov	_IOB,#0x55
                                    966 ;	main.c:45: while ((MY_PINSD & 0x02) == 0); // Ждем, пока кнопку ОТПУСТЯТ
      00010C                        967 00104$:
      00010C 90 E6 07         [24]  968 	mov	dptr,#_MY_PINSD
      00010F E0               [24]  969 	movx	a,@dptr
      000110 30 E1 F9         [24]  970 	jnb	acc.1,00104$
                                    971 ;	main.c:46: delay_ms(50); // Защита от дребезга
      000113 90 00 32         [24]  972 	mov	dptr,#0x0032
      000116 12 00 A8         [24]  973 	lcall	_delay_ms
      000119 80 CE            [24]  974 	sjmp	00114$
      00011B                        975 00108$:
                                    976 ;	main.c:51: IOB = 0x33; 
      00011B 75 90 33         [24]  977 	mov	_IOB,#0x33
                                    978 ;	main.c:54: }
      00011E 80 C9            [24]  979 	sjmp	00114$
                                    980 	.area CSEG    (CODE)
                                    981 	.area CONST   (CODE)
                                    982 	.area XINIT   (CODE)
                                    983 	.area CABS    (ABS,CODE)
