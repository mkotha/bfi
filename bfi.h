# ifndef BFI_BFI_H
# define BFI_BFI_H

# include "machine.h"
# include "octet.h"
# include "format.h"
# include "lex.h"

# define BFI_HEAD_7c(h, t) h
# define BFI_TAIL_7c(h, t) t
# define BFI_CONS_7c(h, t) BFI_CONS_7c_I(7c(h,t),
# define BFI_CONS_7c_I(x, y) 7c(y,x)
# define BFI_TAG_7c(h, t) 7c
# define BFI_HEAD_7z 0x00
# define BFI_TAIL_7z 7z
# define BFI_CONS_7z BFI_CONS_7z_I(
# define BFI_CONS_7z_I(x) BFI_IFN(BFI_TEST_ZERO_##x)(7z, 7c(x,7z))
# define BFI_TAG_7z 7z

# define BFI_UNCONS_0x017c(h, t) , h, t
# define BFI_UNCONS_0x027c(h, t) h)BFI_UNCONS_0x01##t
# define BFI_UNCONS_0x037c(h, t) h,BFI_UNCONS_0x02##t
# define BFI_UNCONS_0x047c(h, t) h,BFI_UNCONS_0x03##t

# define BFI_UNCONS_0x017z , 0x00, 7z
# define BFI_UNCONS_0x027z 0x00), 0x00, 7z
# define BFI_UNCONS_0x037z 0x00,0x00), 0x00, 7z
# define BFI_UNCONS_0x047z 0x00,0x00,0x00), 0x00, 7z

# define BFI_CONS_MANY_0x017c(t, x0) 7c(x0,t)
# define BFI_CONS_MANY_0x027c(t, x0, x1) 7c(x1,7c(x0,t))
# define BFI_CONS_MANY_0x037c(t, x0, x1, x2) 7c(x2,7c(x1,7c(x0,t)))
# define BFI_CONS_MANY_0x047c(t, x0, x1, x2, x3) 7c(x3,7c(x2,7c(x1,7c(x0,t))))

# define BFI_CONS_MANY_0x017z(_, x0) BFI_IFN(BFI_TEST_ZERO_##x0)(7z, 7c(x0,7z))
# define BFI_CONS_MANY_0x027z(_, x0, x1) BFI_IFN(BFI_TEST_ZERO_##x0)(BFI_CONS_MANY_0x017z(~, x1), 7c(x1,7c(x0,7z)))
# define BFI_CONS_MANY_0x037z(_, x0, x1, x2) BFI_IFN(BFI_TEST_ZERO_##x0)(BFI_CONS_MANY_0x027z(~, x1, x2), 7c(x2,7c(x1,7c(x0,7z))))
# define BFI_CONS_MANY_0x047z(_, x0, x1, x2, x3) BFI_IFN(BFI_TEST_ZERO_##x0)(BFI_CONS_MANY_0x037z(~, x1, x2, x3), 7c(x3,7c(x2,7c(x1,7c(x0,7z)))))

# define BFI_MEM_FLIP(img) BFI_MEM_FLIP_I(img)
# define BFI_MEM_FLIP_I(r, c, l) l, c, r

# define BFI_MOVE_FORWARD(n, l, c, r) BFI_MOVE_FORWARD_I(n, l, c, BFI_UNCONS_##n##r)
# define BFI_MOVE_FORWARD_I(n, l, c, g) BFI_CAT(BFI_CONS_MANY_##n, BFI_TAG_##l)(l,c,g

# define BFI_CONS(x, xs) BFI_CONS_##xs x)
# define BFI_HEAD(xs) BFI_HEAD_##xs
# define BFI_TAIL(xs) BFI_TAIL_##xs

# define BFI_BF_NEXT(l, c, r) BFI_CONS_##l c), BFI_HEAD_##r, BFI_TAIL_##r
# define BFI_BF_PREV(l, c, r) BFI_TAIL_##l, BFI_HEAD_##l, BFI_CONS_##r c)
# define BFI_BF_READ(l, c, r, i) BFI_BF_READ_I(BFI_TAKE_PREFIX(i), i, l, c, r)
# define BFI_BF_READ_I(hd, i, l, c, r) BFI_IFN(BFI_PCAT(BFI_TEST_EOF_, hd))(BFI_REM_4(l, c, r, i), BFI_REM_4(l, hd, r, BFI_EAT i))

# define BFI_TEST_EOF_0eof ()

# define BFI_FINST_6next(f, l, c, r, i, d, t) f(BFI_BF_NEXT(l, c, r) BFI_COMMA() i, t, BFI_RETURN
# define BFI_FINST_6prev(f, l, c, r, i, d, t) f(BFI_BF_PREV(l, c, r) BFI_COMMA(), t, BFI_RETURN
# define BFI_FINST_6next1(f, l, c, r, i, d, t) BFI_FINST_##t(f, BFI_CONS(c, l), BFI_HEAD(r), BFI_TAIL(r), i, BFI_EMPTY(
# define BFI_FINST_6prev1(f, l, c, r, i, d, t) BFI_FINST_##t(f, BFI_TAIL(l), BFI_HEAD(l), BFI_CONS(c, r), i, BFI_EMPTY(

# define BFI_FINST_6nextA(f, l, c, r, i, d, t) BFI_CALL_BEGIN_2(BFI_FINST_##t, f, BFI_MOVE_FORWARD(d, l, c, r)), i, BFI_EMPTY(
# define BFI_FINST_6nextB(f, l, c, r, i, d, t) BFI_CALL_BEGIN_2(BFI_FINST_##t, f, BFI_MOVE_FORWARD(d, l, c, r)), i, BFI_EMPTY(
# define BFI_FINST_6prevA(f, l, c, r, i, d, t) BFI_CALL_BEGIN_2(BFI_FINST_##t, f, BFI_MEM_FLIP(BFI_MOVE_FORWARD(d, r, c, l))), i, BFI_EMPTY(
# define BFI_FINST_6prevB(f, l, c, r, i, d, t) BFI_CALL_BEGIN_2(BFI_FINST_##t, f, BFI_MEM_FLIP(BFI_MOVE_FORWARD(d, r, c, l))), i, BFI_EMPTY(

# define BFI_FINST_6inc(f, l, c, r, i, d, t) BFI_FINST_##t(f, l, BFI_PCAT(BFI_ADD_0x01_, c), r, i, BFI_EMPTY(
# define BFI_FINST_6dec(f, l, c, r, i, d, t) BFI_FINST_##t(f, l, BFI_PCAT(BFI_ADD_0xff_, c), r, i, BFI_EMPTY(
# define BFI_FINST_6loop(f, l, c, r, i, d, t) BFI_IFN(BFI_PCAT(BFI_TEST_ZERO_, c))(BFI_LOOP_ENDF, BFI_LOOP_CONTF)(f, l, c, r, i, d, t, BFI_FJUMP_TO d)
# define BFI_FINST_6loopA(f, l, c, r, i, d, t) BFI_IFN(BFI_PCAT(BFI_TEST_ZERO_, c))(BFI_LOOP_ENDF_A, BFI_LOOP_CONTF_A)(f, l, c, r, i, d, t, BFI_FJUMP_TO d)
# define BFI_LOOP_ENDF(f, l, c, r, i, d, t, ds) f(l BFI_COMMA() c BFI_COMMA() r BFI_COMMA() i, t, BFI_RETURN
# define BFI_LOOP_ENDF_A(f, l, c, r, i, d, t, ds) BFI_FINST_##t(f, l, c, r, i, BFI_EMPTY(
# define BFI_LOOP_CONTF(f, l, c, r, i, d, t, ds) f(l BFI_COMMA() c BFI_COMMA() r BFI_COMMA() i, ds(6loop, d)(t, BFI_PUSH_END)
# define BFI_LOOP_CONTF_A(f, l, c, r, i, d, t, ds) f(l BFI_COMMA() c BFI_COMMA() r BFI_COMMA() i, ds(6loopA, d)(t, BFI_PUSH_END)
# define BFI_LOOP_ONCEF(f, l, c, r, i, d, t, ds) f(l BFI_COMMA() c BFI_COMMA() r BFI_COMMA() i, ds(t, BFI_PUSH_END)
# define BFI_FINST_6if(f, l, c, r, i, d, t) BFI_IFN(BFI_PCAT(BFI_TEST_ZERO_, c))(BFI_LOOP_ENDF_A, BFI_LOOP_ONCEF)(f, l, c, r, i, d, t, BFI_FJUMP_TO d)
# define BFI_FINST_6write(f, l, c, r, i, d, t) (c)f(l BFI_COMMA() c BFI_COMMA() r BFI_COMMA() i, t, BFI_RETURN
# define BFI_FINST_6writeA(f, l, c, r, i, d, t) (c)BFI_FINST_##t(f, l, c, r, i, BFI_EMPTY(
# define BFI_FINST_6writeB(f, l, c, r, i, d, t) (c)BFI_FINST_##t(f, l, c, r, i, BFI_EMPTY(
# define BFI_FINST_6read(f, l, c, r, i, d, t) f(BFI_BF_READ(l, c, r, i), t, BFI_RETURN
# define BFI_FINST_6add(f, l, c, r, i, d, t) BFI_FINST_##t(f, l, BFI_ADD(d, c), r, i, BFI_EMPTY(
# define BFI_FINST_6senter(f, l, c, r, i, d, t) BFI_FINST_##t(f, l, 0x00, r, (i)c, BFI_EMPTY(
# define BFI_FINST_6sexit(f, l, c, r, i, d, t) BFI_FINST_##t(f, l, c, r, BFI_TAKE_PREFIX(i), BFI_EMPTY(
# define BFI_FINST_6clear(f, l, c, r, i, d, t) f(l BFI_COMMA() 0x00 BFI_COMMA() r BFI_COMMA() i, t, BFI_RETURN
# define BFI_FINST_6clearA(f, l, c, r, i, d, t) BFI_FINST_##t(f, l, 0x00, r, i, BFI_EMPTY(
# define BFI_FINST_6sadd(f, l, c, r, i, d, t) BFI_FINST_##t(f, l, BFI_ADD(BFI_MUL(d, BFI_EAT i), c), r, i, BFI_EMPTY(

# define BFI_EXEC BFI_EXEC_RAW
# define BFI_EXEC_RAW(code, input) BFI_FMACHINE_O(BFI_INITIAL_STATE(input), 6exec, code)(0eof)
# define BFI_FINST_6exec(f, v, d, t) BFI_EXEC_I(f, v, BFI_FJUMP_TO d)(6tuple_4, ~)(t, BFI_PUSH_END)
# define BFI_EXEC_I(f, v, jd) f(BFI_REM_4 v, jd
# define BFI_INITIAL_STATE(input) (7z, 0x00, 7z, input)

# define BFI_FINST_6tuple_4(f, x1, x2, x3, x4, d, t) f((x1, x2, x3, x4), t, BFI_RETURN
# define BFI_FINST_6id_4(f, x1, x2, x3, x4, d, t) f(x1 BFI_COMMA() x2 BFI_COMMA() x3 BFI_COMMA() x4, t, BFI_RETURN

# define BFI_TRANSLATE(src) BFI_GEN_FLAT(BFI_LEX(src))

# define BFI_ADDITIVEP(x) BFI_NULLARYP(BFI_ADDITIVE_TEST_##x)
# define BFI_ADDITIVE_TEST_6inc ()
# define BFI_ADDITIVE_TEST_6dec ()
# define BFI_ADDITIVE_TEST_6add ()

# define BFI_ADDITIVE_AMOUNT(x) BFI_ADDITIVE_AMOUNT_##x
# define BFI_ADDITIVE_AMOUNT_6inc 0x01
# define BFI_ADDITIVE_AMOUNT_6dec 0xff
# define BFI_ADDITIVE_AMOUNT_6add(n) n

# define BFI_GEN_FLAT_TOK_0x78 BFI_TEST_SUCCESS((1C)(BFI_GEN_FLAT_ADD, 6inc))
# define BFI_GEN_FLAT_TOK_0x5f BFI_TEST_SUCCESS((1C)(BFI_GEN_FLAT_ADD, 6dec))
# define BFI_GEN_FLAT_TOK_0x62 BFI_TEST_SUCCESS((1C)(BFI_GEN_FLAT_ADD, 6next))
# define BFI_GEN_FLAT_TOK_0x64 BFI_TEST_SUCCESS((1C)(BFI_GEN_FLAT_ADD, 6prev))
# define BFI_GEN_FLAT_TOK_0x72 BFI_TEST_SUCCESS((1C)(BFI_GEN_FLAT_ADD, 6read))
# define BFI_GEN_FLAT_TOK_0x77 BFI_TEST_SUCCESS((1C)(BFI_GEN_FLAT_ADD, 6write))
# define BFI_GEN_FLAT_TOK_0x4c BFI_TEST_SUCCESS((0)(BFI_GEN_FLAT_PUSH))
# define BFI_GEN_FLAT_TOK_0x28 BFI_TEST_SUCCESS((0)(BFI_GEN_FLAT_PUSH))
# define BFI_GEN_FLAT_TOK_0x52 BFI_TEST_SUCCESS((0)(BFI_GEN_FLAT_POP))
# define BFI_GEN_FLAT_TOK_0x29 BFI_TEST_SUCCESS((0)(BFI_GEN_FLAT_POP))

# define BFI_GEN_FLAT(tokens) BFI_GEN_FLAT_I(BFI_SEQ_TO_GUIDE(tokens))
# define BFI_GEN_FLAT_I(g) BFI_GEN_FLAT_LOOP(6id_4, g
# define BFI_GEN_FLAT_LOOP(last, tok, c) BFI_GEN_FLAT_LOOP##c(last, BFI_SWITCH((BFI_GEN_FLAT_TOK_##tok, (0)(BFI_GEN_FLAT_IGNORE)))
# define BFI_GEN_FLAT_LOOP0(last, _) BFI_UNLESS(BFI_NULLARYP(BFI_TEST_ERASABLE_##last))((last))
# define BFI_GEN_FLAT_LOOP1(last, prof) BFI_CALL_BEGIN_ prof last)
# define BFI_GEN_FLAT_IGNORE(last) BFI_GEN_FLAT_LOOP(last,
# define BFI_GEN_FLAT_ADD(op, last) \
  BFI_IFC(BFI_AND(BFI_ADDITIVEP(last), BFI_ADDITIVEP(op)))((1C)(BFI_GEN_FLAT_LOOP, 6add(BFI_ADD(BFI_ADDITIVE_AMOUNT(last), BFI_ADDITIVE_AMOUNT(op)))), \
  BFI_IFN(BFI_TEST_REDUCEABLE_##last)((1C)(BFI_GEN_FLAT_LOOP, op), \
  BFI_IFN(BFI_TEST_REDUCEABLE_##op)((1C)(BFI_GEN_FLAT_LOOP, last), \
    (1C)((last)BFI_GEN_FLAT_LOOP, op))))
# define BFI_GEN_FLAT_PUSH(last) BFI_UNLESS(BFI_NULLARYP(BFI_TEST_REDUCEABLE_##last))((last))(6loop)BFI_GEN_FLAT_LOOP(6id_4,
# define BFI_GEN_FLAT_POP(last) BFI_UNLESS(BFI_NULLARYP(BFI_TEST_ERASABLE_##last))((last))(6end(6loop))BFI_GEN_FLAT_LOOP(6skip,

# define BFI_TEST_REDUCEABLE_6id_4 ()
# define BFI_TEST_REDUCEABLE_6skip ()
# define BFI_TEST_ERASABLE_6skip ()

# define BFI_RUN(code, input) BFI_FORMAT_PRETTY(BFI_EXEC(BFI_BUILD(BFI_OPTIMIZE(BFI_TRANSLATE(code))), BFI_LEX(input)))
# define BFI_RUN1(code) BFI_RUN(code, 0eof)

# define BFI_LOOPMUL(code) BFI_SCAN_N(BFI_LOOPMUL_I(BFI_SEQ_TO_GUIDE(code)))
# define BFI_LOOPMUL_I(g) BFI_LOOPMUL_GO(g

# define BFI_LOOPMUL_GO(i, c) BFI_IF(c)(BFI_LOOPMUL_PROCESS, BFI_EAT)(i
# define BFI_LOOPMUL_PROCESS(i) BFI_IFN(BFI_TEST_LOOP_##i)(BFI_LOOPMUL_LOOP, BFI_LOOPMUL_OTHER)(i)
# define BFI_LOOPMUL_OTHER(i) (i)BFI_LOOPMUL_GO(
# define BFI_LOOPMUL_LOOP(i) BFI_DEFER(BFI_LOOPMUL_CHOOSE)((0r)(i)BFI_LOOPMUL_PARALLEL(0x00, 0x00,

# define BFI_LOOPMUL_PARALLEL(pos, inc, i, c) BFI_SWITCH((BFI_LOOPMUL_DISPATCH_##i, BFI_LOOPMUL_PARALLEL_OTHER))(pos, inc, i
# define BFI_LOOPMUL_PARALLEL_OTHER(pos, inc, i) , 0)BFI_LOOPMUL_PROCESS(i)
# define BFI_LOOPMUL_PARALLEL_ABORT(i, j, c) , 0)(i)BFI_LOOPMUL_GO(j, c)
# define BFI_LOOPMUL_PARALLEL_ADDITIVE(pos, inc, i) BFI_IFC(BFI_NULLARYP(BFI_TEST_ZERO_##pos))( \
  (2)((0r)(i)BFI_LOOPMUL_PARALLEL, pos, BFI_ADD(inc, BFI_ADDITIVE_AMOUNT(i))), \
  (2)((0c)(i, 6sadd(BFI_ADDITIVE_AMOUNT(i)))BFI_LOOPMUL_PARALLEL, pos, inc)),
# define BFI_LOOPMUL_PARALLEL_NEXT(pos, inc, i) (0b)(i)BFI_IFC(BFI_NULLARYP(BFI_TEST_7F_##pos))((1)(BFI_LOOPMUL_PARALLEL_ABORT, i), (2)(BFI_LOOPMUL_PARALLEL, BFI_ADD_0x01_##pos, inc)),
# define BFI_LOOPMUL_PARALLEL_PREV(pos, inc, i) (0b)(i)BFI_IFC(BFI_NULLARYP(BFI_TEST_80_##pos))((1)(BFI_LOOPMUL_PARALLEL_ABORT, i), (2)(BFI_LOOPMUL_PARALLEL, BFI_ADD_0xff_##pos, inc)),
# define BFI_LOOPMUL_PARALLEL_END(pos, inc, i) BFI_IF(BFI_AND(BFI_NULLARYP(BFI_TEST_ZERO_##pos), BFI_NULLARYP(BFI_TEST_FF_##inc)))(BFI_LOOPMUL_PARALLEL_FINISH, BFI_LOOPMUL_PARALLEL_ABORT)(i,
# define BFI_LOOPMUL_PARALLEL_FINISH(i, j, c) , 1)BFI_LOOPMUL_GO(j, c)

# define BFI_LOOPMUL_DISPATCH_6inc BFI_TEST_SUCCESS(BFI_LOOPMUL_PARALLEL_ADDITIVE)
# define BFI_LOOPMUL_DISPATCH_6dec BFI_TEST_SUCCESS(BFI_LOOPMUL_PARALLEL_ADDITIVE)
# define BFI_LOOPMUL_DISPATCH_6add BFI_TEST_SUCCESS(BFI_LOOPMUL_PARALLEL_ADDITIVE)
# define BFI_LOOPMUL_DISPATCH_6next BFI_TEST_SUCCESS(BFI_LOOPMUL_PARALLEL_NEXT)
# define BFI_LOOPMUL_DISPATCH_6prev BFI_TEST_SUCCESS(BFI_LOOPMUL_PARALLEL_PREV)
# define BFI_LOOPMUL_DISPATCH_6end BFI_TEST_SUCCESS(BFI_LOOPMUL_PARALLEL_END)

# define BFI_LOOPMUL_CHOOSE(d, c) BFI_LOOPMUL_CHOOSE_##c d(0e)

# define BFI_LOOPMUL_CHOOSE_1 (6senter)BFI_LOOPMUL_CHOOSE_1_NEXT
# define BFI_LOOPMUL_CHOOSE_1_NEXT(i) BFI_LOOPMUL_CHOOSE_1_##i
# define BFI_LOOPMUL_CHOOSE_1_0r(_) BFI_LOOPMUL_CHOOSE_1_NEXT
# define BFI_LOOPMUL_CHOOSE_1_0c(x, y) (y)BFI_LOOPMUL_CHOOSE_1_NEXT
# define BFI_LOOPMUL_CHOOSE_1_0b(x) (x)BFI_LOOPMUL_CHOOSE_1_NEXT
# define BFI_LOOPMUL_CHOOSE_1_0e (6sexit)

# define BFI_LOOPMUL_CHOOSE_0 BFI_LOOPMUL_CHOOSE_0_NEXT
# define BFI_LOOPMUL_CHOOSE_0_NEXT(i) BFI_LOOPMUL_CHOOSE_0_##i
# define BFI_LOOPMUL_CHOOSE_0_0r(x) (x)BFI_LOOPMUL_CHOOSE_0_NEXT
# define BFI_LOOPMUL_CHOOSE_0_0c(x, y) (x)BFI_LOOPMUL_CHOOSE_0_NEXT
# define BFI_LOOPMUL_CHOOSE_0_0b(x) (x)BFI_LOOPMUL_CHOOSE_0_NEXT
# define BFI_LOOPMUL_CHOOSE_0_0e

# define BFI_TEST_LOOP_6loop ()
# define BFI_TEST_END_6end ()

# define BFI_PEEPHOLE(code) BFI_PEEPHOLE_I(BFI_SEQ_TO_GUIDE(code))
# define BFI_PEEPHOLE_I(g) BFI_PEEPHOLE_GO(g
# define BFI_PEEPHOLE_GO(i, c) BFI_IF(c)(BFI_PEEPHOLE_PROCESS, BFI_EAT)(i
# define BFI_PEEPHOLE_PROCESS(i) BFI_SWITCH((BFI_PEEPHOLE_DISPATCH_##i, BFI_PEEPHOLE_OTHER))(i)
# define BFI_PEEPHOLE_OTHER(i) (i)BFI_PEEPHOLE_GO(
# define BFI_PEEPHOLE_MOVE(i) BFI_PEEPHOLE_ON_MOVE(0, i, 0x01,
# define BFI_PEEPHOLE_ALTERNATE(i) (i##A)BFI_PEEPHOLE_ON_ALTERNATE(i,
# define BFI_PEEPHOLE_END(i) (6end(6loopA))BFI_PEEPHOLE_ON_END(

# define BFI_PEEPHOLE_ON_MOVE(cont, base, cur, i, c) BFI_PEEPHOLE_ON_MOVE_##c(cont, base, cur, i
# define BFI_PEEPHOLE_ON_MOVE_0(cont, base, cur, i) BFI_PEEPHOLE_MOVE_CREATE(cont, base, cur)
# define BFI_PEEPHOLE_ON_MOVE_1(cont, base, cur, i) BFI_IFC(BFI_SYM_MATCH(base, i))( \
  (3)(BFI_CAT(BFI_PEEPHOLE_MOVE_ADD_, BFI_NULLARYP(BFI_TEST_PEEPHOLE_MOVE_MAX_##cur)), cont, base, cur), \
  (1)(BFI_PEEPHOLE_MOVE_CREATE(cont, base, cur)BFI_PEEPHOLE_PROCESS, i)))
# define BFI_PEEPHOLE_MOVE_CREATE(cont, base, cur) (BFI_IFN(BFI_TEST_ONE_##cur)(base##1, BFI_IF(cont)(base##B, base##A)(cur)))
# define BFI_PEEPHOLE_MOVE_ADD_0(cont, base, cur) BFI_PEEPHOLE_ON_MOVE(cont, base, BFI_ADD_0x01_##cur,
# define BFI_PEEPHOLE_MOVE_ADD_1(cont, base, cur) BFI_PEEPHOLE_MOVE_CREATE(cont, base, cur)BFI_PEEPHOLE_ON_MOVE(BFI_NOT(cont), base, 0x01,

# define BFI_TEST_PEEPHOLE_MOVE_MAX_0x04 ()

# define BFI_PEEPHOLE_ON_ALTERNATE(base, i, c) BFI_IF(c)(BFI_PEEPHOLE_ALTERNATE_SWITCH, BFI_EAT_2)(base, i
# define BFI_PEEPHOLE_ALTERNATE_SWITCH(base, i) BFI_IFC(BFI_SYM_MATCH(base, i))((1)(BFI_PEEPHOLE_OTHER, BFI_PCAT(i, B)), (1)(BFI_PEEPHOLE_PROCESS, i)))

# define BFI_PEEPHOLE_ON_END(i, c) BFI_IF(c)(BFI_PEEPHOLE_END_SWITCH, BFI_EAT)(i
# define BFI_PEEPHOLE_END_SWITCH(i) BFI_IFN(BFI_TEST_LOOP_##i)(BFI_PEEPHOLE_END_LOOP, BFI_IFN(BFI_TEST_END_##i)(BFI_PEEPHOLE_END_END, BFI_PEEPHOLE_PROCESS))(i)
# define BFI_PEEPHOLE_END_LOOP(_) BFI_PEEPHOLE_EAT_LOOP(~,
# define BFI_PEEPHOLE_END_END(_) (6end(6if))BFI_PEEPHOLE_ON_END(

# define BFI_PEEPHOLE_EAT_LOOP(n, i, c) BFI_IF(c)(BFI_PEEPHOLE_EAT_LOOP_NEXT, BFI_EAT_2)(n, i
# define BFI_PEEPHOLE_EAT_LOOP_NEXT(n, i) BFI_IFC(BFI_NULLARYP(BFI_TEST_END_##i))( \
  BFI_IFN(n)((1C)(BFI_PEEPHOLE_EAT_LOOP, BFI_EMPTY n), (0)(BFI_PEEPHOLE_ON_END)), \
  (1C)(BFI_PEEPHOLE_EAT_LOOP, BFI_IFN(BFI_TEST_LOOP_##i)(()n, n)))

# define BFI_PEEPHOLE_DISPATCH_6next BFI_TEST_SUCCESS(BFI_PEEPHOLE_MOVE)
# define BFI_PEEPHOLE_DISPATCH_6prev BFI_TEST_SUCCESS(BFI_PEEPHOLE_MOVE)
# define BFI_PEEPHOLE_DISPATCH_6write BFI_TEST_SUCCESS(BFI_PEEPHOLE_ALTERNATE)
# define BFI_PEEPHOLE_DISPATCH_6end BFI_TEST_SUCCESS(BFI_PEEPHOLE_END)

# define BFI_OPTIMIZE(code) BFI_PEEPHOLE(BFI_LOOPMUL(code))

# define BFI_SYM_6next(x) x
# define BFI_SYM_6prev(x) x
# define BFI_SYM_6write(x) x

# define BFI_BUILD(code) BFI_UNFLATTEN(BFI_FIX_LOOP(code))

# define BFI_FIX_LOOP(code) BFI_SEQ_REVERSE(BFI_FIX_LOOP_I(BFI_SEQ_TO_GUIDE(BFI_SEQ_REVERSE(code))))
# define BFI_FIX_LOOP_I(g) BFI_FIX_LOOP_GO(~, g
# define BFI_FIX_LOOP_GO(s, i, c) BFI_IFC(c)(BFI_SWITCH((BFI_FIX_LOOP_DISPATCH_##i, (0)(BFI_FIX_LOOP_OTHER))), (0)(BFI_EAT_2))s, i
# define BFI_FIX_LOOP_OTHER(s, i) (i)BFI_FIX_LOOP_GO(s,
# define BFI_FIX_LOOP_END(k, s, i) (i)BFI_FIX_LOOP_GO((k, s),
# define BFI_FIX_LOOP_BEGIN(s, i) (BFI_FST s)BFI_FIX_LOOP_GO(BFI_SND s,

# define BFI_FIX_LOOP_DISPATCH_6end(k) BFI_TEST_SUCCESS((1C)(BFI_FIX_LOOP_END, k))
# define BFI_FIX_LOOP_DISPATCH_6loop BFI_TEST_SUCCESS((0)(BFI_FIX_LOOP_BEGIN))

# define BFI_UNFLATTEN(code) BFI_RENDER_CLEAN(BFI_SCAN_N(BFI_SEQ_FOR_EACH(BFI_UNFLATTEN_OP, code, ~)))
# define BFI_UNFLATTEN_OP(x, _) (BFI_SWITCH((BFI_UNFLATTEN_DISPATCH_##x, ()(x,~)BFI_EMPTY)))

# define BFI_UNFLATTEN_DISPATCH_6inc BFI_TEST_SUCCESS(()(6inc,0x01)BFI_EMPTY)
# define BFI_UNFLATTEN_DISPATCH_6dec BFI_TEST_SUCCESS(()(6dec,0xff)BFI_EMPTY)
# define BFI_UNFLATTEN_DISPATCH_6add(d) BFI_TEST_SUCCESS(()(6add,d)BFI_EMPTY)
# define BFI_UNFLATTEN_DISPATCH_6next BFI_TEST_SUCCESS(()(6next,~)BFI_EMPTY)
# define BFI_UNFLATTEN_DISPATCH_6next1 BFI_TEST_SUCCESS(()(6next1,~)BFI_EMPTY)
# define BFI_UNFLATTEN_DISPATCH_6nextA(d) BFI_TEST_SUCCESS(()(6nextA,d)BFI_EMPTY)
# define BFI_UNFLATTEN_DISPATCH_6nextB(d) BFI_TEST_SUCCESS(()(6nextB,d)BFI_EMPTY)
# define BFI_UNFLATTEN_DISPATCH_6prev BFI_TEST_SUCCESS(()(6prev,~)BFI_EMPTY)
# define BFI_UNFLATTEN_DISPATCH_6prev1 BFI_TEST_SUCCESS(()(6prev1,~)BFI_EMPTY)
# define BFI_UNFLATTEN_DISPATCH_6prevA(d) BFI_TEST_SUCCESS(()(6prevA,d)BFI_EMPTY)
# define BFI_UNFLATTEN_DISPATCH_6prevB(d) BFI_TEST_SUCCESS(()(6prevB,d)BFI_EMPTY)
# define BFI_UNFLATTEN_DISPATCH_6read BFI_TEST_SUCCESS(()(6read,~)BFI_EMPTY)
# define BFI_UNFLATTEN_DISPATCH_6write BFI_TEST_SUCCESS(()(6write,~)BFI_EMPTY)
# define BFI_UNFLATTEN_DISPATCH_6writeA BFI_TEST_SUCCESS(()(6writeA,~)BFI_EMPTY)
# define BFI_UNFLATTEN_DISPATCH_6writeB BFI_TEST_SUCCESS(()(6writeB,~)BFI_EMPTY)
# define BFI_UNFLATTEN_DISPATCH_6loop BFI_TEST_SUCCESS(()BFI_LOOP_OPENER)
# define BFI_UNFLATTEN_DISPATCH_6loopA BFI_TEST_SUCCESS(()BFI_LOOP_OPENER_A)
# define BFI_UNFLATTEN_DISPATCH_6if BFI_TEST_SUCCESS(()BFI_LOOP_OPENER_IF)
# define BFI_UNFLATTEN_DISPATCH_6senter BFI_TEST_SUCCESS(()(6senter,~)BFI_EMPTY)
# define BFI_UNFLATTEN_DISPATCH_6sexit BFI_TEST_SUCCESS(()(6sexit,~)BFI_EMPTY)
# define BFI_UNFLATTEN_DISPATCH_6sadd(d) BFI_TEST_SUCCESS(()(6sadd,d)BFI_EMPTY)
# define BFI_UNFLATTEN_DISPATCH_6clear BFI_TEST_SUCCESS(()(6clear,~)BFI_EMPTY)
# define BFI_UNFLATTEN_DISPATCH_6clearA BFI_TEST_SUCCESS(()(6clearA,~)BFI_EMPTY)
# define BFI_UNFLATTEN_DISPATCH_6end BFI_TEST_SUCCESS(()BFI_RPAREN)

# define BFI_LOOP_OPENER() (6loop,
# define BFI_LOOP_OPENER_A() (6loopA,
# define BFI_LOOP_OPENER_IF() (6if,

# define BFI_FINST_6seq_reverse(f, seq, _, t) BFI_CALL_BEGIN(f, BFI_SEQ_REVERSE_START(BFI_SEQ_TO_GUIDE(seq))(t))

# define BFI_SEQ_REVERSE_START(g) BFI_SEQ_REVERSE_NEXT(BFI_SEQ_REVERSE_MA, g
# define BFI_SEQ_REVERSE_NEXT(p, x, c) p##c(x
# define BFI_SEQ_REVERSE_MA0(x) BFI_TUPLE, BFI_FJUMP1
# define BFI_SEQ_REVERSE_MA1(x) ((x), BFI_SEQ_REVERSE_NEXT(BFI_SEQ_REVERSE_MB,
# define BFI_SEQ_REVERSE_MB0(x) ~, 1)(~, ~, 0), 6swap_flatten, BFI_PASS_AUX_D ~ BFI_PUSH_D
# define BFI_SEQ_REVERSE_MB1(x) (x), 2)BFI_SEQ_REVERSE_NEXT(BFI_SEQ_REVERSE_MC,
# define BFI_SEQ_REVERSE_MC0(x) (~, ~, 0), 6swap_flatten, BFI_PASS_AUX_D ~ BFI_PUSH_D
# define BFI_SEQ_REVERSE_MC1(x) ((x), BFI_SEQ_REVERSE_NEXT(BFI_SEQ_REVERSE_MD,
# define BFI_SEQ_REVERSE_MD0(x) ~, 1), 6tseq_reverse, BFI_PUSH_MANY_D(~)(6swap_flatten, ~)BFI_TERMINATE_PUSH
# define BFI_SEQ_REVERSE_MD1(x) (x), 2)BFI_SEQ_REVERSE_NEXT(BFI_SEQ_REVERSE_ME,
# define BFI_SEQ_REVERSE_ME0(x) (~, ~, 0), 6tseq_reverse, BFI_PUSH_MANY_D(~)(6swap_flatten, ~)BFI_TERMINATE_PUSH
# define BFI_SEQ_REVERSE_ME1(x) ((x), BFI_SEQ_REVERSE_NEXT(BFI_SEQ_REVERSE_MD,

# define BFI_FINST_6swap_flatten(f, ts, _, t) f(BFI_TSEQ_SWAP_FLATTEN1_A ts, t, BFI_RETURN
# define BFI_FINST_6swap_flatten_t(f, ts, _, t) f(BFI_TSEQ_SWAP_FLATTEN1_A ts(~, ~, 0), t, BFI_RETURN

# define BFI_TSEQ_SWAP_FLATTEN1_A(x, y, c) BFI_TSEQ_SWAP_FLATTEN1_A##c(x, y)
# define BFI_TSEQ_SWAP_FLATTEN1_A1(x, y) x BFI_TSEQ_SWAP_FLATTEN1_B0
# define BFI_TSEQ_SWAP_FLATTEN1_A2(x, y) y x BFI_TSEQ_SWAP_FLATTEN1_B0
# define BFI_TSEQ_SWAP_FLATTEN1_B0(x, y, k) BFI_TSEQ_SWAP_FLATTEN1_C##k(1, x, y)
# define BFI_TSEQ_SWAP_FLATTEN1_B1(x, y, k) BFI_TSEQ_SWAP_FLATTEN1_C##k(0, x, y)
# define BFI_TSEQ_SWAP_FLATTEN1_C0(d, x, y)
# define BFI_TSEQ_SWAP_FLATTEN1_C1(d, x, y) x BFI_TSEQ_SWAP_FLATTEN1_B##d
# define BFI_TSEQ_SWAP_FLATTEN1_C2(d, x, y) y x BFI_TSEQ_SWAP_FLATTEN1_B##d

# define BFI_FINST_6tseq_reverse(f, ts, _, t) BFI_CALL_BEGIN(f, BFI_TSEQ_REVERSE_A ts)(t, BFI_PUSH_END)

# define BFI_TSEQ_REVERSE_A(x, y, c) ((x, y, 2), BFI_TSEQ_REVERSE_B
# define BFI_TSEQ_REVERSE_B(x, y, c) BFI_TSEQ_REVERSE_B##c(x, y)
# define BFI_TSEQ_REVERSE_B0(x, y) ~, 1)(~, ~, 0), 6swap_flatten_t, BFI_PUSH_MANY_D(~)
# define BFI_TSEQ_REVERSE_B1(x, y) (x, ~, 1), 2)(~, ~, 0), 6swap_flatten_t, BFI_PUSH_MANY_D(~)
# define BFI_TSEQ_REVERSE_B2(x, y) (x, y, 2), 2)BFI_TSEQ_REVERSE_C
# define BFI_TSEQ_REVERSE_C(x, y, c) BFI_TSEQ_REVERSE_C##c(x, y)
# define BFI_TSEQ_REVERSE_C0(x, y) (~, ~, 0), 6swap_flatten_t, BFI_PUSH_MANY_D(~)
# define BFI_TSEQ_REVERSE_C1(x, y) ((x, ~, 1), ~, 1), 6tseq_reverse, BFI_PUSH_MANY_D(~)(6swap_flatten_t, ~)
# define BFI_TSEQ_REVERSE_C2(x, y) ((x, y, 2), BFI_TSEQ_REVERSE_D
# define BFI_TSEQ_REVERSE_D(x, y, c) BFI_TSEQ_REVERSE_D##c(x, y)
# define BFI_TSEQ_REVERSE_D0(x, y) ~, 1), 6tseq_reverse, BFI_PUSH_MANY_D(~)(6swap_flatten_t, ~)
# define BFI_TSEQ_REVERSE_D1(x, y) (x, ~, 1), 2)(~, ~, 0), 6tseq_reverse, BFI_PUSH_MANY_D(~)(6swap_flatten_t, ~)
# define BFI_TSEQ_REVERSE_D2(x, y) (x, y, 2), 2)BFI_TSEQ_REVERSE_E
# define BFI_TSEQ_REVERSE_E(x, y, c) BFI_TSEQ_REVERSE_E##c(x, y)
# define BFI_TSEQ_REVERSE_E0(x, y) (~, ~, 0), 6tseq_reverse, BFI_PUSH_MANY_D(~)(6swap_flatten_t, ~)
# define BFI_TSEQ_REVERSE_E1(x, y) ((x, ~, 1), ~, 1), 6tseq_reverse, BFI_PUSH_MANY_D(~)(6swap_flatten_t, ~)
# define BFI_TSEQ_REVERSE_E2(x, y) ((x, y, 2), BFI_TSEQ_REVERSE_D

# define BFI_SEQ_REVERSE(seq) BFI_FMACHINE_V(seq, 6seq_reverse, ~)

# define BFI_TEST_FF_0xff ()
# define BFI_TEST_7F_0x7f ()
# define BFI_TEST_80_0x80 ()
# define BFI_TEST_ZERO_0x00 ()
# define BFI_TEST_ONE_0x01 ()

# define BFI_FINST_6put(f, _, v, t) (v)f(~, t, BFI_RETURN

# endif
