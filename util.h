# ifndef BFI_UTIL_H
# define BFI_UTIL_H

/* a note on types
 *
 * i use the following terms to classify pp token sequences.
 *
 * active:
 *   any token sequence. in particular, it may invoke macros when scanned.
 * portable-active:
 *   a non-empty active that does not contain unbalanced parens nor
 *   open commas.
 * value:
 *   a token sequence that contains no macro invocations.
 * portable:
 *   a portable-active that is also a value.
 * tuple:
 *   zero or more portables separated by commas, and surrounded by a
 *   pair of parens. examples of tuples include "(a)" and
 *   "(foo (bar), baz)". the number of elements in a tuple is called
 *   its arity. "()" is the nullary tuple.
 * callable:
 *   a portable to which a tuple of a certain size can be appended,
 *   with the resulting token sequence forming an active. a typical
 *   example of callable is a macro name, but almost any portable can
 *   be seen as a callable.
 * N-ary sequence:
 *   one or more N-ary tuples, placed side-by-side. an unary sequence
 *   is simply called a sequence.
 * word:
 *   an identifier or a pp-number.
 */

# define BFI_EMPTY()
# define BFI_COMMA() ,
# define BFI_LPAREN() (
# define BFI_RPAREN() )
# define BFI_SPACE() BFI_EMPTY() BFI_EMPTY()
# define BFI_HASH() BFI_HASH_I
# define BFI_HASH_I #

/* DEEER(f); f : callable
 *   returns f, disabling its invocation temporarily.
 */
# define BFI_DEFER(f) f BFI_EMPTY()
# define BFI_DEFER_1(f) f BFI_DEFER(BFI_EMPTY)()

# define BFI_EAT(x)
# define BFI_EAT_2(x, x1)
# define BFI_EAT_3(x, x1, x2)

# define BFI_REM(x) x
# define BFI_REM_2(x, x1) x, x1
# define BFI_REM_3(x, x1, x2) x, x1, x2
# define BFI_REM_4(x, x1, x2, x3) x, x1, x2, x3

# define BFI_SWAP(x, y) (y, x)

# define BFI_ID(x) x

/* BFI_CAT(x, y); x, y : portable
 *   concatenates x and y, pasting the last token of x and the first
 *   token of y.
 */
# define BFI_CAT(x, y) BFI_PCAT(x, y)

/* BFI_PCAT(x, y); x, y : value
 *   like BFI_CAT, but doesn't allow ordinary pre-expansion on its
 *   arguments.
 */
# define BFI_PCAT(x, y) x ## y

# define BFI_FST(x, y) x
# define BFI_SND(x, y) y

# define BFI_ELEM_3_0(x0, x1, x2) x0
# define BFI_ELEM_3_1(x0, x1, x2) x1
# define BFI_ELEM_3_2(x0, x1, x2) x2

# define BFI_ELEM_4_0(x0, x1, x2, x3) x0
# define BFI_ELEM_4_1(x0, x1, x2, x3) x1
# define BFI_ELEM_4_2(x0, x1, x2, x3) x2
# define BFI_ELEM_4_3(x0, x1, x2, x3) x3

/* BFI_SCAN_N(x); x : active
 *   expands x. this macro is not reentrant; x must not call BFI_SCAN_N,
 *   directly or intirectly.
 */
# define BFI_SCAN_N(x) x
/* BFI_CALL_N(f, x)
 *     f : callable
 *     x : tuple
 *   invokes f with arguments taken from x. this macro is not
 *   reentrant.
 */
# define BFI_CALL_N(f, x) f x

/* BFI_CALL_BEGIN_* macros expand to an incomplete invocation of a
 * callable. these macros are reentrant, because they do not directly
 * invoke the passed callable.
 */
# define BFI_CALL_BEGIN(f, x) f(x
# define BFI_CALL_BEGIN_0(f) f(
# define BFI_CALL_BEGIN_1(f, x) f(x
# define BFI_CALL_BEGIN_2(f, x1, x2) f(x1, x2
# define BFI_CALL_BEGIN_3(f, x1, x2, x3) f(x1, x2, x3
# define BFI_CALL_BEGIN_1C(f, x) f(x,
# define BFI_CALL_BEGIN_2C(f, x1, x2) f(x1, x2,
# define BFI_CALL_BEGIN_3C(f, x1, x2, x3) f(x1, x2, x3,

# define BFI_CALL_BEGIN_(n) BFI_CALL_BEGIN_##n

# define BFI_TUPLE(x) (x)
# define BFI_TUPLE_2(x0, x1) (x0,x1)

/* BFI_IF(b)(x, y); BFI_WHEN(b)(x); BFI_UNLESS(b)(y)
 *     b : 0 or 1
 *     x, y : value
 *   if b = 1, expands to x. otherwise, expands to y.
 *   absent arguments for BFI_WHEN and BFI_UNLESS are treated to be empty.
 */
# define BFI_IF(b) BFI_PCAT(BFI_IF_, b)
# define BFI_IF_1(x, y) x
# define BFI_IF_0(x, y) y

# define BFI_WHEN(b) BFI_PCAT(BFI_WHEN_, b)
# define BFI_WHEN_1(x) x
# define BFI_WHEN_0(x)

# define BFI_UNLESS(b) BFI_WHEN(BFI_NOT(b))

# define BFI_IFC(b) BFI_PCAT(BFI_IFC_, b)
# define BFI_IFC_1(x, y) BFI_CALL_BEGIN_ x
# define BFI_IFC_0(x, y) BFI_CALL_BEGIN_ y

# define BFI_NOT(b) BFI_IF(b)(0, 1)
# define BFI_AND(x, y) BFI_IF(x)(y, 0)

/* BFI_SWITCH((t, default))
 *     t : portable-active
 *     default : portable-active
 *   performs general conditional choice.
 *   'default' must expand to a portable. 't' must expand to one of the
 *   following:
 *
 *   - a portable. in this case, BFI_SWITCH expands to the expansion of
 *     'default'
 *   - an active of the form 'a BFI_TEST_SUCCESS(v) b', where 'v' is an active
 *     that expands to a portable, 'a' is a portable or is empty, and 'b' is a
 *     portable-active. in this case, BFI_SWITCH expands to the expansion of
 *     'v'.
 *
 * # define CASE_0 BFI_TEST_SUCCESS(zero)
 * # define CASE_1 BFI_TEST_SUCCESS(one)
 * # define F(x) BFI_SWITCH((CASE_##x, other))
 *
 * F(0) => zero
 * F(1 0) => one
 * F(2 1 0) => other
 */
# define BFI_SWITCH(p) BFI_SND p
# define BFI_SWITCH_C(p) BFI_SWITCH_CI p
# define BFI_SWITCH_CI(x, y) BFI_CALL_BEGIN_ y

# define BFI_TEST_SUCCESS(v) ~, v) BFI_EAT_2(~

# define BFI_IFN(x) BFI_SWITCH((BFI_NULLARY_TEST_D x, BFI_IF_0))
# define BFI_NULLARY_TEST_D() BFI_TEST_SUCCESS(BFI_IF_1)

/* BFI_NULLARYP(x); BFI_UNARYP(x); BFI_BINARYP(x); BFI_TERNARYP(x)
 *     x : portable
 *   tests whether x begins with a nullary, unary, binary and ternary
 *   tuple, respectively. x must not begin with a tuple of an
 *   unexpected size.
 * 
 * BFI_UNARYP((x)) => 1
 * BFI_UNARYP((x) y) => 1
 * BFI_UNARYP(x y) => 0
 * BFI_UNARYP((x, y)) => invalid
 */
# define BFI_NULLARYP(x) BFI_IFN(x)(1, 0)

# define BFI_UNARYP(x) BFI_SWITCH((BFI_UNARYP_TEST x, 0))
# define BFI_UNARYP_TEST(a) BFI_TEST_SUCCESS(1)

# define BFI_BINARYP(x) BFI_SWITCH((BFI_BINARYP_TEST x, 0))
# define BFI_BINARYP_TEST(a, b) BFI_TEST_SUCCESS(1)

# define BFI_TERNARYP(x) BFI_SWITCH((BFI_TERNARYP_TEST x, 0))
# define BFI_TERNARYP_TEST(a, b, c) BFI_TEST_SUCCESS(1)

/* BFI_LEFT(x); BFI_RIGHT(x)
 *     x : value
 *   x must be of the form "a, b", where a and b are portables.
 *   BFI_LEFT(x) expands to a, BFI_RIGHT(x) expands to b.
 */
# define BFI_LEFT(im) BFI_FST(im~)
# define BFI_RIGHT(im) BFI_SND(~im)

/* BFI_TAKE_PREFIX(x)
 *     x : portable
 *   x must begin with an unary tuple. BFI_TAKE_PREFIX(x) expands to
 *   the content of the tuple.
 */
# define BFI_TAKE_PREFIX(x) BFI_LEFT(BFI_TAKE_PREFIX_I x)
# define BFI_TAKE_PREFIX_I(x) x,

/* BFI_SEQ_TO_GUIDE(seq)
 *     seq : sequence
 *   converts seq into guide, a data structure which facilitates
 *   sequential iteration.
 *
 * BFI_SEQ_TO_GUIDE((foo)) => foo, 1))0, 0))
 * BFI_SEQ_TO_GUIDE((x)(y)(z)) => x, 1))y, 1))z, 1))0, 0))
 * BFI_SEQ_TO_GUIDE(BFI_TUPLE) => 0, 0))
 */
# define BFI_SEQ_TO_GUIDE(seq) BFI_IF(BFI_UNARYP(seq))(BFI_SEQ_TO_GUIDE_I, BFI_SEQ_TO_GUIDE_NIL() BFI_EAT)(seq)
# define BFI_SEQ_TO_GUIDE_I(seq) BFI_SEQ_TO_GUIDE_A seq(BFI_EAT BFI_LPAREN()BFI_LPAREN()BFI_LPAREN()))BFI_SEQ_TO_GUIDE_NIL()
# define BFI_SEQ_TO_GUIDE_A(x) x, 1))BFI_SEQ_TO_GUIDE_B
# define BFI_SEQ_TO_GUIDE_B(x) x, 1))BFI_SEQ_TO_GUIDE_A
# define BFI_SEQ_TO_GUIDE_NIL() 0, 0))

/* BFI_BSEQ_TO_GUIDE(bseq)
 *     bseq : binary sequence
 *   like BFI_SEQ_TO_GUIDE, but it handles a binary sequence, producing
 *   a binary guide.
 *
 * BFI_BSEQ_TO_GUIDE((x0, y0)(x1, y1)) => x0, y0, 1))x1, y1, 1))0, 0, 0))
 */
# define BFI_BSEQ_TO_GUIDE(seq) BFI_IF(BFI_BINARYP(seq))(BFI_BSEQ_TO_GUIDE_I, BFI_BSEQ_TO_GUIDE_NIL() BFI_EAT)(seq)
# define BFI_BSEQ_TO_GUIDE_I(seq) BFI_BSEQ_TO_GUIDE_A seq(BFI_EAT BFI_LPAREN()BFI_LPAREN()BFI_LPAREN(), ~))BFI_BSEQ_TO_GUIDE_NIL()
# define BFI_BSEQ_TO_GUIDE_A(x, y) x, y, 1))BFI_BSEQ_TO_GUIDE_B
# define BFI_BSEQ_TO_GUIDE_B(x, y) x, y, 1))BFI_BSEQ_TO_GUIDE_A
# define BFI_BSEQ_TO_GUIDE_NIL() 0, 0, 0))

/* BFI_BGUIDE_EAT(bg
 * BFI_BGUIDE_EAT_ID()bg
 *     bg : binary guide
 *   expands to nothing.
 */
# define BFI_BGUIDE_EAT(x, y, c) BFI_IF(c)(BFI_BGUIDE_EAT_ID, BFI_EMPTY)(
# define BFI_BGUIDE_EAT_ID() BFI_BGUIDE_EAT(

/* BFI_SEQ_FOR_EACH(f, seq, data)
 *     f : callable
 *     seq : sequence
 *     data : portable
 *   call f on each element in seq. this macro does not directly
 *   invoke f; it returns a deferred invocation.
 *
 * BFI_SEQ_FOR_EACH(MACRO, (a)(b)(c), ~) => MACRO(a, ~)MACRO(b, ~)MACRO(c, ~)
 */
# define BFI_SEQ_FOR_EACH(f, seq, data) BFI_SEQ_FOR_EACH_I(f, data, BFI_SEQ_TO_GUIDE(seq))
# define BFI_SEQ_FOR_EACH_I(f, data, g) BFI_SEQ_FOR_EACH_LOOP(f, data, g
# define BFI_SEQ_FOR_EACH_LOOP(f, data, x, c) BFI_IF(c)(BFI_SEQ_FOR_EACH_CONT, BFI_EAT_3)(f, data, x
# define BFI_SEQ_FOR_EACH_CONT(f, data, x) BFI_DEFER(f)(x, data)BFI_SEQ_FOR_EACH_LOOP(f, data,

/* BFI_RENDER(c); c : sequence
 *   executes a sequence of rendering instructions. a rendering
 *   instruction is a nullary callable that produces an arbitrary
 *   token sequence.
 *
 * BFI_RENDER( (BFI_ID(Hello)BFI_COMMA) (BFI_SPACE) (BFI_ID(world)BFI_EMPTY) ) => Hello, world
 */
# define BFI_RENDER(c) BFI_SCAN_N(BFI_SEQ_FOR_EACH(BFI_RENDER_OP, c, ~))
# define BFI_RENDER_OP(x, _) x()

/* BFI_RENDER_CLEAN(c); c : sequence
 *   executes a sequence of clean rendering instructins. a clean
 *   rendering instruction is a rendering instruction prefixed with
 *   a nullary tuple. this macro is used to ensure that the resulting
 *   token sequence does not contain too much unnecessary padding
 *   tokens, under GNU cpp.
 *
 * BFI_RENDER_CLEAN( (()BFI_ID(Hello)BFI_COMMA) (()BFI_SPACE) (()BFI_ID(world)BFI_EMPTY) ) => Hello, world
 */
# define BFI_RENDER_CLEAN(c) BFI_SCAN_N(BFI_EMPTY BFI_SCAN_N(BFI_SEQ_FOR_EACH(BFI_RENDER_CLEAN_OP, c, ~))())
# define BFI_RENDER_CLEAN_OP(x, _) x

/* Symbols
 *
 * a symbol is a word X for which an identity macro named BFI_SYM_##X is defined.
 * symbols can be tested for equality.
 *
 * BFI_SYMP(x); x : word
 *   tests if x is a symbol.
 * BFI_SYM_EQ(x, y); x, y : symbol
 *   tests if x is equal to y.
 * BFI_SYM_MATCH(x, y)
 *     x : symbol
 *     y : word
 *   tests whether y matches x. that is, y is a symbol that is equal
 *   to x.
 *
 * # define BFI_SYM_xxx(x) x
 * # define BFI_SYM_22C(x) x
 *
 * BFI_SYMP(xxx) => 1
 * BFI_SYMP(22C) => 1
 * BFI_SYMP(other) => 0
 * BFI_SYM_EQ(xxx, xxx) => 1
 * BFI_SYM_EQ(xxx, 22C) => 0
 * BFI_SYM_EQ(xxx, other) => invalid
 * BFI_SYM_MATCH(xxx, xxx) => 1
 * BFI_SYM_MATCH(xxx, 22C) => 0
 * BFI_SYM_MATCH(xxx, other) => 0
 */
# define BFI_SYMP(x) BFI_NULLARYP(BFI_PCAT(BFI_SYM_, x)(()))
# define BFI_SYM_EQ(x, y) BFI_NOT(BFI_NULLARYP(BFI_PCAT(BFI_SYM_, x)(BFI_PCAT(BFI_SYM_, y))(())))
# define BFI_SYM_MATCH(x, y) BFI_AND(BFI_SYMP(y), BFI_SYM_EQ(x, y))

# endif
