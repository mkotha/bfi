# Brainfuck interpreter on C Preprocessor

## Usage

 # include "bfi.h"
 
 BFI_RUN(<bf-program>, <input>)
 BFI_RUN1(<bf-program>)

A BF program must be given in a special syntax. Replace +-><.,[] in
an ordinary BF program with x_bdwrLR, respectively. The instructions
must be separated with whitespaces, so that they will not be
recognized as a single token. In place of L and R, parentheses ( and
) can also be used, provided that a ) never directly follows a (.

An input will be given as a sequence of bytes. Each byte is
represented by a 2-digit hexadecimal with lowercase letters.
Characters are encoded in ASCII.

Example:

 BFI_RUN(r (w (_) r), 0x61 0x62 0x63) /* => abc */

Letters, numbers and underscores can be written directly in an input
sequence. Pairs of parentheses can also be used, privided that the
content is not empty. There are some additional shorthands such as
0comma for comma and 0_n for the newline character. See lex.h for
detail.

Example:

 BFI_RUN(r (w (_) r), a b c (F 0comma 0space G) ) /* => abc(F, G) */
