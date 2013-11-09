# ifndef BFI_FORMAT_H
# define BFI_FORMAT_H

# include "util.h"

# define BFI_FORMAT_PRETTY(seq) BFI_FORMAT_PRETTY_I(BFI_SEQ_TO_GUIDE(seq))
# define BFI_FORMAT_PRETTY_I(g) BFI_FORMAT_PRETTY_LOOP(0, ~, g
# define BFI_FORMAT_PRETTY_LOOP(hold, val, ch, c) BFI_IF(c)(BFI_FORMAT_PRETTY_CONT, BFI_EAT_3)(hold, val, BFI_FORMAT_PRETTY_SHOW_##ch
# define BFI_FORMAT_PRETTY_CONT(hold, val, alnum_rep) BFI_CAT(BFI_FORMAT_PRETTY_NEXT_##hold, BFI_FST(alnum_rep))(val, BFI_SND(alnum_rep))
# define BFI_FORMAT_PRETTY_NEXT_00(v, rep) rep()BFI_FORMAT_PRETTY_LOOP(0, ~,
# define BFI_FORMAT_PRETTY_NEXT_01(v, rep) BFI_FORMAT_PRETTY_LOOP(1, rep,
# define BFI_FORMAT_PRETTY_NEXT_10(v, rep) BFI_ID(v)rep()BFI_FORMAT_PRETTY_LOOP(0, ~,
# define BFI_FORMAT_PRETTY_NEXT_11(v, rep) BFI_FORMAT_PRETTY_LOOP(1, BFI_PCAT(v, rep),

# define BFI_FORMAT_PRETTY_SHOW_0x00 0,'\0'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x01 0,'\x01'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x02 0,'\x02'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x03 0,'\x03'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x04 0,'\x04'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x05 0,'\x05'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x06 0,'\x06'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x07 0,'\a'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x08 0,'\b'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x09 0,'\t'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x0a 0,'\n'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x0b 0,'\v'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x0c 0,'\f'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x0d 0,'\r'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x0e 0,'\x0e'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x0f 0,'\x0f'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x10 0,'\x10'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x11 0,'\x11'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x12 0,'\x12'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x13 0,'\x13'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x14 0,'\x14'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x15 0,'\x15'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x16 0,'\x16'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x17 0,'\x17'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x18 0,'\x18'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x19 0,'\x19'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x1a 0,'\x1a'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x1b 0,'\x1b'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x1c 0,'\x1c'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x1d 0,'\x1d'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x1e 0,'\x1e'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x1f 0,'\x1f'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x20 0,BFI_SPACE
# define BFI_FORMAT_PRETTY_SHOW_0x21 0,!BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x22 0,'\"'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x23 0,BFI_HASH
# define BFI_FORMAT_PRETTY_SHOW_0x24 0,$BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x25 0,%BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x26 0,&BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x27 0,'\''BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x28 0,BFI_LPAREN
# define BFI_FORMAT_PRETTY_SHOW_0x29 0,BFI_RPAREN
# define BFI_FORMAT_PRETTY_SHOW_0x2a 0,*BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x2b 0,+BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x2c 0,BFI_COMMA
# define BFI_FORMAT_PRETTY_SHOW_0x2d 0,-BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x2e 0,.BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x2f 0,/BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x30 1,0
# define BFI_FORMAT_PRETTY_SHOW_0x31 1,1
# define BFI_FORMAT_PRETTY_SHOW_0x32 1,2
# define BFI_FORMAT_PRETTY_SHOW_0x33 1,3
# define BFI_FORMAT_PRETTY_SHOW_0x34 1,4
# define BFI_FORMAT_PRETTY_SHOW_0x35 1,5
# define BFI_FORMAT_PRETTY_SHOW_0x36 1,6
# define BFI_FORMAT_PRETTY_SHOW_0x37 1,7
# define BFI_FORMAT_PRETTY_SHOW_0x38 1,8
# define BFI_FORMAT_PRETTY_SHOW_0x39 1,9
# define BFI_FORMAT_PRETTY_SHOW_0x3a 0,:BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x3b 0,;BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x3c 0,<BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x3d 0,=BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x3e 0,>BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x3f 0,?BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x40 0,@BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x41 1,A
# define BFI_FORMAT_PRETTY_SHOW_0x42 1,B
# define BFI_FORMAT_PRETTY_SHOW_0x43 1,C
# define BFI_FORMAT_PRETTY_SHOW_0x44 1,D
# define BFI_FORMAT_PRETTY_SHOW_0x45 1,E
# define BFI_FORMAT_PRETTY_SHOW_0x46 1,F
# define BFI_FORMAT_PRETTY_SHOW_0x47 1,G
# define BFI_FORMAT_PRETTY_SHOW_0x48 1,H
# define BFI_FORMAT_PRETTY_SHOW_0x49 1,I
# define BFI_FORMAT_PRETTY_SHOW_0x4a 1,J
# define BFI_FORMAT_PRETTY_SHOW_0x4b 1,K
# define BFI_FORMAT_PRETTY_SHOW_0x4c 1,L
# define BFI_FORMAT_PRETTY_SHOW_0x4d 1,M
# define BFI_FORMAT_PRETTY_SHOW_0x4e 1,N
# define BFI_FORMAT_PRETTY_SHOW_0x4f 1,O
# define BFI_FORMAT_PRETTY_SHOW_0x50 1,P
# define BFI_FORMAT_PRETTY_SHOW_0x51 1,Q
# define BFI_FORMAT_PRETTY_SHOW_0x52 1,R
# define BFI_FORMAT_PRETTY_SHOW_0x53 1,S
# define BFI_FORMAT_PRETTY_SHOW_0x54 1,T
# define BFI_FORMAT_PRETTY_SHOW_0x55 1,U
# define BFI_FORMAT_PRETTY_SHOW_0x56 1,V
# define BFI_FORMAT_PRETTY_SHOW_0x57 1,W
# define BFI_FORMAT_PRETTY_SHOW_0x58 1,X
# define BFI_FORMAT_PRETTY_SHOW_0x59 1,Y
# define BFI_FORMAT_PRETTY_SHOW_0x5a 1,Z
# define BFI_FORMAT_PRETTY_SHOW_0x5b 0,[BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x5c 0,\BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x5d 0,]BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x5e 0,^BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x5f 1,_
# define BFI_FORMAT_PRETTY_SHOW_0x60 0,`BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x61 1,a
# define BFI_FORMAT_PRETTY_SHOW_0x62 1,b
# define BFI_FORMAT_PRETTY_SHOW_0x63 1,c
# define BFI_FORMAT_PRETTY_SHOW_0x64 1,d
# define BFI_FORMAT_PRETTY_SHOW_0x65 1,e
# define BFI_FORMAT_PRETTY_SHOW_0x66 1,f
# define BFI_FORMAT_PRETTY_SHOW_0x67 1,g
# define BFI_FORMAT_PRETTY_SHOW_0x68 1,h
# define BFI_FORMAT_PRETTY_SHOW_0x69 1,i
# define BFI_FORMAT_PRETTY_SHOW_0x6a 1,j
# define BFI_FORMAT_PRETTY_SHOW_0x6b 1,k
# define BFI_FORMAT_PRETTY_SHOW_0x6c 1,l
# define BFI_FORMAT_PRETTY_SHOW_0x6d 1,m
# define BFI_FORMAT_PRETTY_SHOW_0x6e 1,n
# define BFI_FORMAT_PRETTY_SHOW_0x6f 1,o
# define BFI_FORMAT_PRETTY_SHOW_0x70 1,p
# define BFI_FORMAT_PRETTY_SHOW_0x71 1,q
# define BFI_FORMAT_PRETTY_SHOW_0x72 1,r
# define BFI_FORMAT_PRETTY_SHOW_0x73 1,s
# define BFI_FORMAT_PRETTY_SHOW_0x74 1,t
# define BFI_FORMAT_PRETTY_SHOW_0x75 1,u
# define BFI_FORMAT_PRETTY_SHOW_0x76 1,v
# define BFI_FORMAT_PRETTY_SHOW_0x77 1,w
# define BFI_FORMAT_PRETTY_SHOW_0x78 1,x
# define BFI_FORMAT_PRETTY_SHOW_0x79 1,y
# define BFI_FORMAT_PRETTY_SHOW_0x7a 1,z
# define BFI_FORMAT_PRETTY_SHOW_0x7b 0,{BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x7c 0,|BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x7d 0,}BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x7e 0,~BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x7f 0,'\x7f'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x80 0,'\x80'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x81 0,'\x81'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x82 0,'\x82'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x83 0,'\x83'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x84 0,'\x84'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x85 0,'\x85'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x86 0,'\x86'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x87 0,'\x87'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x88 0,'\x88'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x89 0,'\x89'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x8a 0,'\x8a'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x8b 0,'\x8b'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x8c 0,'\x8c'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x8d 0,'\x8d'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x8e 0,'\x8e'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x8f 0,'\x8f'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x90 0,'\x90'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x91 0,'\x91'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x92 0,'\x92'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x93 0,'\x93'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x94 0,'\x94'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x95 0,'\x95'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x96 0,'\x96'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x97 0,'\x97'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x98 0,'\x98'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x99 0,'\x99'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x9a 0,'\x9a'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x9b 0,'\x9b'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x9c 0,'\x9c'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x9d 0,'\x9d'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x9e 0,'\x9e'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0x9f 0,'\x9f'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xa0 0,'\xa0'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xa1 0,'\xa1'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xa2 0,'\xa2'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xa3 0,'\xa3'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xa4 0,'\xa4'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xa5 0,'\xa5'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xa6 0,'\xa6'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xa7 0,'\xa7'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xa8 0,'\xa8'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xa9 0,'\xa9'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xaa 0,'\xaa'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xab 0,'\xab'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xac 0,'\xac'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xad 0,'\xad'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xae 0,'\xae'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xaf 0,'\xaf'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xb0 0,'\xb0'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xb1 0,'\xb1'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xb2 0,'\xb2'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xb3 0,'\xb3'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xb4 0,'\xb4'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xb5 0,'\xb5'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xb6 0,'\xb6'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xb7 0,'\xb7'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xb8 0,'\xb8'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xb9 0,'\xb9'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xba 0,'\xba'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xbb 0,'\xbb'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xbc 0,'\xbc'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xbd 0,'\xbd'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xbe 0,'\xbe'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xbf 0,'\xbf'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xc0 0,'\xc0'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xc1 0,'\xc1'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xc2 0,'\xc2'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xc3 0,'\xc3'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xc4 0,'\xc4'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xc5 0,'\xc5'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xc6 0,'\xc6'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xc7 0,'\xc7'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xc8 0,'\xc8'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xc9 0,'\xc9'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xca 0,'\xca'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xcb 0,'\xcb'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xcc 0,'\xcc'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xcd 0,'\xcd'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xce 0,'\xce'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xcf 0,'\xcf'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xd0 0,'\xd0'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xd1 0,'\xd1'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xd2 0,'\xd2'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xd3 0,'\xd3'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xd4 0,'\xd4'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xd5 0,'\xd5'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xd6 0,'\xd6'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xd7 0,'\xd7'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xd8 0,'\xd8'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xd9 0,'\xd9'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xda 0,'\xda'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xdb 0,'\xdb'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xdc 0,'\xdc'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xdd 0,'\xdd'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xde 0,'\xde'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xdf 0,'\xdf'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xe0 0,'\xe0'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xe1 0,'\xe1'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xe2 0,'\xe2'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xe3 0,'\xe3'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xe4 0,'\xe4'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xe5 0,'\xe5'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xe6 0,'\xe6'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xe7 0,'\xe7'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xe8 0,'\xe8'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xe9 0,'\xe9'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xea 0,'\xea'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xeb 0,'\xeb'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xec 0,'\xec'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xed 0,'\xed'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xee 0,'\xee'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xef 0,'\xef'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xf0 0,'\xf0'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xf1 0,'\xf1'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xf2 0,'\xf2'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xf3 0,'\xf3'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xf4 0,'\xf4'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xf5 0,'\xf5'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xf6 0,'\xf6'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xf7 0,'\xf7'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xf8 0,'\xf8'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xf9 0,'\xf9'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xfa 0,'\xfa'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xfb 0,'\xfb'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xfc 0,'\xfc'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xfd 0,'\xfd'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xfe 0,'\xfe'BFI_EMPTY
# define BFI_FORMAT_PRETTY_SHOW_0xff 0,'\xff'BFI_EMPTY

# define BFI_FORMAT_PRETTY_SHOW_0eof 0,BFI_EMPTY

# endif
