# ifndef BFI_MACHINE_H
# define BFI_MACHINE_H

/*** The Continuation Machine
 *
 * The continuation machine is a virtual machine implemented on top of CPP.
 * Its primary purpose is to emulate loop and recursion in an efficient way.
 * It has the following components:
 *
 * - The A register. It contains one or more portables.
 * - The instruction register. It contains a continuation machine instruction.
 * - The stack. It is a list of stack frames. Each stack frame is a pair of
 *   a portable and a word (usually an instruction).
 * - The output buffer. It contains a portable or the empty token sequence.
 *
 * An N-ary instruction is a word X for which a (N+3)-ary macro called
 * BFI_FINST_##X is defined. This macro is called the accompanying macro of X.
 *
 * Execution of the machine proceeds as follows:
 *
 * 1. The A register and the instruction register are initialized with the
 *  values given by the user. The stack is initialized to a the singleton
 *  list whose only frame is the pair (v, 6stop), where 'v' is the
 *  user-specified value. The output buffer is initialized to the empty token
 *  sequence.
 * 2. Repeat a single-step update.
 * 3. When the instruction register contains the sepcial instruction 6stop,
 *  the machine stops, and the contents of the A register or the output
 *  buffer are returned from the machine.
 * 4. When the stack is empty and the instruction register does not contain
 *  6stop, the behavior is undefined.
 * 5. When the number of executed instruction exceeds a limit, the machine
 *  aborts by causing a CPP error. The limit is roughly 2**33.
 *
 * The single-step update is defined as follows:
 *
 * 1. The accompanying macro of the content of the instruction register is
 *  called in the following form:
 *
 *  am(f, v_1, ... v_N, d, t)
 *
 *  where 'am' is the acompanying macro, 'f' is some word, 'v_1' through 'v_N'
 *  are the portables in the A register, 'd' is the portable component of the
 *  top frame of the stack, and 't' is the word component of the top frame.
 *
 *  For this to be a valid call, 'am' must be a (N+3)-ary macro, where N is
 *  the number of portables in the A register.
 *
 *  This call is required to produce a token sequence of the following form:
 *
 *  o f(new_v, i, p
 *
 *  where 'o' an optional portable, 'new_v' is a portable-active which expands
 *  to a comma-separated list of one or more portables, 'i' is an instruction,
 *  and 'p' is a portable-active whose expansion has the form:
 *
 *  , push_1 ... push_K
 *
 *  where each 'push_i' is an active of the form:
 *
 *  e, w))
 *
 *  where 'e' is a portable-active which expands to a portable, and 'w' is
 *  a word. Here K can be any non-negative integer.
 *
 * 2. The machine state is updated as follows:
 *
 *  - The A register contains the portables in the list 'new_v' expands to.
 *  - The instruction register contains 'i'.
 *  - The top frame of the stack is removed. Then each 'push_i' is pushed to the
 *    stack, where 'push_1' is pushed last and becomes the new top of the
 *    stack.
 *  - 'o' is appended to the output buffer.
 *
 * Remarks:
 *
 *  - An instruction can "call" itself by simply arranging that it is placed in
 *    the instruction register, immediately or at some later step. This does
 *    not cause a macro calling itself, because an instruction and its
 *    accompanying macro are different tokens.
 *  - Exactly one frame is popped from the stack in each step, whereas any
 *    number of frames can be pushed.
 *  - Stack popping is automatic, but you can cancel out the effect by pushing
 *    back the popped frame.
 *
 * The basic idea of continuation machine was taken from the Chaos and the
 * Order preprocessor libraries.
 * http://www.sourceforge.net/projects/chaos-pp/
 */

/*** The Calling Convention
 *
 * One calling convention is used throughout the BF interpreter. The machine
 * itself is largely agnostic of it, but the initial placement of 6stop on
 * the stack is compatible with this convention; when the first
 * instruction "returns", the machine exits and returns the "return value".
 *
 * - A function is represented as an instruction. A continuation (i.e. a point
 *   to which a function returns) is represented as a pair of an instruction
 *   and a portable called "environment".
 * - To call a function, do the following:
 *    + Place the instruction for the function in the instruction register.
 *    + Push a stack frame whose word component is the word component of
 *      the previous top stack frame, and whose portable component is the
 *      environment for the continuation.
 *    + Push another stack frame whose word component is the instruction
 *      for the continuation, and whose portable component is the first
 *      argument to the function.
 *    + Place the rest of the arguments in the A register.
 *  - To return from a function, do the following:
 *    + Place the return values in the A register.
 *    + Place the word component of the previous top stack frame in the
 *      instruction register.
 *  - There are other variants of function calls. Instead of pushing one
 *    continuation to the stack, you can push zero or multiple continuations.
 *    Pushing no continuation corresponds means a tail call. Pushing multiple
 *    continuations is useful when you want to apply a series of operations
 *    to the content of the A register.
 *
 *  Note that if an instruction for a continuation is stored in the Nth stack
 *  frame, the corresponding environment is stored in the (N+1)th stack frame.
 */

/*** Rationale
 *
 * Much of the strangeness of the machine's instruction interface comes from
 * efficiency requirements. In particular the current implementation ensures:
 *
 * - The content of the A register is scanned (by CPP) only twice per step.
 * - The content of the stack is scanned less than once in 10 steps on average.
 * - Any part of the output buffer is scanned only O(log(steps)) times.
 */

# include "util.h"

/* BFI_FMACHINE_O(v, f, d)
 * BFI_FMACHINE_V(v, f, d)
 *    v : value
 *    f : instruction
 *    d : value
 *  Run the continuation machine. The A register is initialized to 'v',
 *  the instruction register to 'f', and the stack top to ('d', 6stop).
 *  BFI_FMACHINE_O returns the content of the output buffer.
 *  BFI_FMACHINE_V returns the content of the A register.
 */
# define BFI_FMACHINE_O(v, f, d) BFI_FMACHINE_G(BFI_LEFT, v, f, d)()
# define BFI_FMACHINE_V(v, f, d) BFI_FMACHINE_G(BFI_RIGHT, v, f, d)
# define BFI_FMACHINE_G(m, v, f, d) m(BFI_FMACHINE_R((f)((BFI_FLINEAR_ENTRANCE_A, v, d, 6stop))(~)(6stack_error,~)))
# define BFI_FMACHINE_R(r) BFI_FMACHINE_END(BFI_FCM_OUTER_C_0(r))
# define BFI_FMACHINE_END(x) BFI_TAKE_OUTPUT(x
# define BFI_TAKE_OUTPUT(o, d) o,

/* Useful macros for writing instructions */

# define BFI_RETURN ,
# define BFI_PUSH(k) , k))
# define BFI_PASS_AUX ,
# define BFI_PUSH_MANY(x) ,x,BFI_EASY_OPEN_CONT_0
# define BFI_PUSH_END BFI_EASY_OPEN_CONT_END
# define BFI_PUSH_END_ID() BFI_PUSH_END
# define BFI_FJUMP_TO(t, r) t, BFI_PUSH_MANY_D(r)
# define BFI_FJUMP1(t) t, BFI_DEFER(BFI_COMMA)()
# define BFI_TERMINATE_PUSH(t) (t, BFI_DEFER(BFI_PUSH_END_ID)())
# define BFI_PUSH_MANY_D BFI_DEFER(BFI_PUSH_MANY)
# define BFI_PASS_AUX_D BFI_DEFER(BFI_COMMA)()
# define BFI_PUSH_D BFI_DEFER(BFI_PUSH)

/* Basic instructions */

/* 6id: unary instruction
 *  returns the second argument
 */
# define BFI_FINST_6id(f, v, d, t) f(v, t, BFI_RETURN

/* 6const: unary instruction
 *  returns the first argument
 */
# define BFI_FINST_6const(f, v, d, t) f(d, t, BFI_RETURN

/******* IMPLEMENTATION ********************************************/

/*** Representations of machine states
 *
 * During execution, the machine state is represented as an active like:
 *
 * outp drv(a, inst, BFI_RETURN)p0, w0))p1, w1)) ... pN, 6stop))~, 6stack_error))~, 6bottom))
 *
 *  outp : recent addition to the output buffer
 *  drv : a linear driver macro, e.g. BFI_FLINEAR_123
 *  a : A register contents, separated by commas
 *  inst : instruction reigster content
 *  p, w0 : top stack frame
 *  pN, 6stop : bottom stack frame
 *
 * while this "active" form is good for efficient execution, it is difficult
 * to deal with because it is not a portable. In particular, the machine state
 * needs to be passed around in order to get an execution limit that is
 * exponential in the number of underlying macros. For this reason there is an
 * alternative "portable" representation:
 *
 * (inst)((entrance, a, p0, w0))(p1)(w1, p2) ... (6stop,~)(6stack_error,~)
 *
 * where 'entrance' is either BFI_FLINEAR_ENTRANCE_A or BFI_FLINEAR_ENTRANCE_B.
 *
 * BFI_SETUP_REGS turns a portable representation into an active
 * representation. The active representation turns itself
 * into a portable representation after a certain number of steps. More
 * precisely, an active representation expands to one of the following forms:
 *
 * outp BFI_EMPTY,1) portable-rep
 * outp BFI_EMPTY,0) return-value
 *
 * The former means the computation has been suspended, and the latter means
 * it has ended. I'll refer to these forms as "intermediate results".
 */

/* BFI_FCM_{INNER,OUTER}_*
 *  These macros are used to invoke BFI_FENTER_LINEAR up to (2**25-1) times
 *  until the execution stops. All these macros expands to an intermediate
 *  result. Of these macros, the ones with _C_ in the name takes a portable
 *  repsentation, and the ones with _I_ takes an intermediate result.
 *
 * BFI_FCM_OUTER_C_{N}(z)
 *  Perform the rest of the computation and return the final result.
 *
 * BFI_FCM_INNER_C_{N}(z)
 *  Apply BFI_FENTER_LINAR at most (2**N) times and return the intermediate
 *  result.
 *
 * BFI_FCM_OUTER_C_{N}(x)
 *  Call BFI_FCM_OUTER_C_{N+1} if x is not a final state.
 *
 * BFI_FCM_INNER_C_{N}(x)
 *  Call BFI_FCM_INNER_C_{N} if x is not a final state.
 */
# define BFI_FCM_OUTER_C_0(z) BFI_FCM_OUTER_I_0(BFI_FCM_INNER_C_0(z))
# define BFI_FCM_OUTER_C_1(z) BFI_FCM_OUTER_I_1(BFI_FCM_INNER_C_1(z))
# define BFI_FCM_OUTER_C_2(z) BFI_FCM_OUTER_I_2(BFI_FCM_INNER_C_2(z))
# define BFI_FCM_OUTER_C_3(z) BFI_FCM_OUTER_I_3(BFI_FCM_INNER_C_3(z))
# define BFI_FCM_OUTER_C_4(z) BFI_FCM_OUTER_I_4(BFI_FCM_INNER_C_4(z))
# define BFI_FCM_OUTER_C_5(z) BFI_FCM_OUTER_I_5(BFI_FCM_INNER_C_5(z))
# define BFI_FCM_OUTER_C_6(z) BFI_FCM_OUTER_I_6(BFI_FCM_INNER_C_6(z))
# define BFI_FCM_OUTER_C_7(z) BFI_FCM_OUTER_I_7(BFI_FCM_INNER_C_7(z))
# define BFI_FCM_OUTER_C_8(z) BFI_FCM_OUTER_I_8(BFI_FCM_INNER_C_8(z))
# define BFI_FCM_OUTER_C_9(z) BFI_FCM_OUTER_I_9(BFI_FCM_INNER_C_9(z))
# define BFI_FCM_OUTER_C_10(z) BFI_FCM_OUTER_I_10(BFI_FCM_INNER_C_10(z))
# define BFI_FCM_OUTER_C_11(z) BFI_FCM_OUTER_I_11(BFI_FCM_INNER_C_11(z))
# define BFI_FCM_OUTER_C_12(z) BFI_FCM_OUTER_I_12(BFI_FCM_INNER_C_12(z))
# define BFI_FCM_OUTER_C_13(z) BFI_FCM_OUTER_I_13(BFI_FCM_INNER_C_13(z))
# define BFI_FCM_OUTER_C_14(z) BFI_FCM_OUTER_I_14(BFI_FCM_INNER_C_14(z))
# define BFI_FCM_OUTER_C_15(z) BFI_FCM_OUTER_I_15(BFI_FCM_INNER_C_15(z))
# define BFI_FCM_OUTER_C_16(z) BFI_FCM_OUTER_I_16(BFI_FCM_INNER_C_16(z))
# define BFI_FCM_OUTER_C_17(z) BFI_FCM_OUTER_I_17(BFI_FCM_INNER_C_17(z))
# define BFI_FCM_OUTER_C_18(z) BFI_FCM_OUTER_I_18(BFI_FCM_INNER_C_18(z))
# define BFI_FCM_OUTER_C_19(z) BFI_FCM_OUTER_I_19(BFI_FCM_INNER_C_19(z))
# define BFI_FCM_OUTER_C_20(z) BFI_FCM_OUTER_I_20(BFI_FCM_INNER_C_20(z))
# define BFI_FCM_OUTER_C_21(z) BFI_FCM_OUTER_I_21(BFI_FCM_INNER_C_21(z))
# define BFI_FCM_OUTER_C_22(z) BFI_FCM_OUTER_I_22(BFI_FCM_INNER_C_22(z))
# define BFI_FCM_OUTER_C_23(z) BFI_FCM_OUTER_I_23(BFI_FCM_INNER_C_23(z))
# define BFI_FCM_OUTER_C_24(z) BFI_FCM_OUTER_I_24(BFI_FCM_INNER_C_24(z))
# define BFI_FCM_OUTER_C_25(z) BFI_EMPTY, 0)BFI_MACHINE_OVERFLOW():z

/* If all the available execution steps are exhausted, abort by causing
 * a too-few-arguments error.
 */
# define BFI_MACHINE_OVERFLOW(a,b,c)

# define BFI_FCM_INNER_C_0(z) BFI_ENTER_LINEAR(BFI_SETUP_REGS z(6bottom, BFI_EASY_OPEN_CONT_END))
# define BFI_FCM_INNER_C_1(z) BFI_FCM_INNER_I_0(BFI_FCM_INNER_C_0(z))
# define BFI_FCM_INNER_C_2(z) BFI_FCM_INNER_I_1(BFI_FCM_INNER_C_1(z))
# define BFI_FCM_INNER_C_3(z) BFI_FCM_INNER_I_2(BFI_FCM_INNER_C_2(z))
# define BFI_FCM_INNER_C_4(z) BFI_FCM_INNER_I_3(BFI_FCM_INNER_C_3(z))
# define BFI_FCM_INNER_C_5(z) BFI_FCM_INNER_I_4(BFI_FCM_INNER_C_4(z))
# define BFI_FCM_INNER_C_6(z) BFI_FCM_INNER_I_5(BFI_FCM_INNER_C_5(z))
# define BFI_FCM_INNER_C_7(z) BFI_FCM_INNER_I_6(BFI_FCM_INNER_C_6(z))
# define BFI_FCM_INNER_C_8(z) BFI_FCM_INNER_I_7(BFI_FCM_INNER_C_7(z))
# define BFI_FCM_INNER_C_9(z) BFI_FCM_INNER_I_8(BFI_FCM_INNER_C_8(z))
# define BFI_FCM_INNER_C_10(z) BFI_FCM_INNER_I_9(BFI_FCM_INNER_C_9(z))
# define BFI_FCM_INNER_C_11(z) BFI_FCM_INNER_I_10(BFI_FCM_INNER_C_10(z))
# define BFI_FCM_INNER_C_12(z) BFI_FCM_INNER_I_11(BFI_FCM_INNER_C_11(z))
# define BFI_FCM_INNER_C_13(z) BFI_FCM_INNER_I_12(BFI_FCM_INNER_C_12(z))
# define BFI_FCM_INNER_C_14(z) BFI_FCM_INNER_I_13(BFI_FCM_INNER_C_13(z))
# define BFI_FCM_INNER_C_15(z) BFI_FCM_INNER_I_14(BFI_FCM_INNER_C_14(z))
# define BFI_FCM_INNER_C_16(z) BFI_FCM_INNER_I_15(BFI_FCM_INNER_C_15(z))
# define BFI_FCM_INNER_C_17(z) BFI_FCM_INNER_I_16(BFI_FCM_INNER_C_16(z))
# define BFI_FCM_INNER_C_18(z) BFI_FCM_INNER_I_17(BFI_FCM_INNER_C_17(z))
# define BFI_FCM_INNER_C_19(z) BFI_FCM_INNER_I_18(BFI_FCM_INNER_C_18(z))
# define BFI_FCM_INNER_C_20(z) BFI_FCM_INNER_I_19(BFI_FCM_INNER_C_19(z))
# define BFI_FCM_INNER_C_21(z) BFI_FCM_INNER_I_20(BFI_FCM_INNER_C_20(z))
# define BFI_FCM_INNER_C_22(z) BFI_FCM_INNER_I_21(BFI_FCM_INNER_C_21(z))
# define BFI_FCM_INNER_C_23(z) BFI_FCM_INNER_I_22(BFI_FCM_INNER_C_22(z))
# define BFI_FCM_INNER_C_24(z) BFI_FCM_INNER_I_23(BFI_FCM_INNER_C_23(z))

# define BFI_FCM_OUTER_I_0(x) BFI_FMACHINE_RESUME(BFI_FCM_OUTER_C_1, x)
# define BFI_FCM_OUTER_I_1(x) BFI_FMACHINE_RESUME(BFI_FCM_OUTER_C_2, x)
# define BFI_FCM_OUTER_I_2(x) BFI_FMACHINE_RESUME(BFI_FCM_OUTER_C_3, x)
# define BFI_FCM_OUTER_I_3(x) BFI_FMACHINE_RESUME(BFI_FCM_OUTER_C_4, x)
# define BFI_FCM_OUTER_I_4(x) BFI_FMACHINE_RESUME(BFI_FCM_OUTER_C_5, x)
# define BFI_FCM_OUTER_I_5(x) BFI_FMACHINE_RESUME(BFI_FCM_OUTER_C_6, x)
# define BFI_FCM_OUTER_I_6(x) BFI_FMACHINE_RESUME(BFI_FCM_OUTER_C_7, x)
# define BFI_FCM_OUTER_I_7(x) BFI_FMACHINE_RESUME(BFI_FCM_OUTER_C_8, x)
# define BFI_FCM_OUTER_I_8(x) BFI_FMACHINE_RESUME(BFI_FCM_OUTER_C_9, x)
# define BFI_FCM_OUTER_I_9(x) BFI_FMACHINE_RESUME(BFI_FCM_OUTER_C_10, x)
# define BFI_FCM_OUTER_I_10(x) BFI_FMACHINE_RESUME(BFI_FCM_OUTER_C_11, x)
# define BFI_FCM_OUTER_I_11(x) BFI_FMACHINE_RESUME(BFI_FCM_OUTER_C_12, x)
# define BFI_FCM_OUTER_I_12(x) BFI_FMACHINE_RESUME(BFI_FCM_OUTER_C_13, x)
# define BFI_FCM_OUTER_I_13(x) BFI_FMACHINE_RESUME(BFI_FCM_OUTER_C_14, x)
# define BFI_FCM_OUTER_I_14(x) BFI_FMACHINE_RESUME(BFI_FCM_OUTER_C_15, x)
# define BFI_FCM_OUTER_I_15(x) BFI_FMACHINE_RESUME(BFI_FCM_OUTER_C_16, x)
# define BFI_FCM_OUTER_I_16(x) BFI_FMACHINE_RESUME(BFI_FCM_OUTER_C_17, x)
# define BFI_FCM_OUTER_I_17(x) BFI_FMACHINE_RESUME(BFI_FCM_OUTER_C_18, x)
# define BFI_FCM_OUTER_I_18(x) BFI_FMACHINE_RESUME(BFI_FCM_OUTER_C_19, x)
# define BFI_FCM_OUTER_I_19(x) BFI_FMACHINE_RESUME(BFI_FCM_OUTER_C_20, x)
# define BFI_FCM_OUTER_I_20(x) BFI_FMACHINE_RESUME(BFI_FCM_OUTER_C_21, x)
# define BFI_FCM_OUTER_I_21(x) BFI_FMACHINE_RESUME(BFI_FCM_OUTER_C_22, x)
# define BFI_FCM_OUTER_I_22(x) BFI_FMACHINE_RESUME(BFI_FCM_OUTER_C_23, x)
# define BFI_FCM_OUTER_I_23(x) BFI_FMACHINE_RESUME(BFI_FCM_OUTER_C_24, x)
# define BFI_FCM_OUTER_I_24(x) BFI_FMACHINE_RESUME(BFI_FCM_OUTER_C_25, x)

# define BFI_FCM_INNER_I_0(x) BFI_FMACHINE_RESUME(BFI_FCM_INNER_C_0, x)
# define BFI_FCM_INNER_I_1(x) BFI_FMACHINE_RESUME(BFI_FCM_INNER_C_1, x)
# define BFI_FCM_INNER_I_2(x) BFI_FMACHINE_RESUME(BFI_FCM_INNER_C_2, x)
# define BFI_FCM_INNER_I_3(x) BFI_FMACHINE_RESUME(BFI_FCM_INNER_C_3, x)
# define BFI_FCM_INNER_I_4(x) BFI_FMACHINE_RESUME(BFI_FCM_INNER_C_4, x)
# define BFI_FCM_INNER_I_5(x) BFI_FMACHINE_RESUME(BFI_FCM_INNER_C_5, x)
# define BFI_FCM_INNER_I_6(x) BFI_FMACHINE_RESUME(BFI_FCM_INNER_C_6, x)
# define BFI_FCM_INNER_I_7(x) BFI_FMACHINE_RESUME(BFI_FCM_INNER_C_7, x)
# define BFI_FCM_INNER_I_8(x) BFI_FMACHINE_RESUME(BFI_FCM_INNER_C_8, x)
# define BFI_FCM_INNER_I_9(x) BFI_FMACHINE_RESUME(BFI_FCM_INNER_C_9, x)
# define BFI_FCM_INNER_I_10(x) BFI_FMACHINE_RESUME(BFI_FCM_INNER_C_10, x)
# define BFI_FCM_INNER_I_11(x) BFI_FMACHINE_RESUME(BFI_FCM_INNER_C_11, x)
# define BFI_FCM_INNER_I_12(x) BFI_FMACHINE_RESUME(BFI_FCM_INNER_C_12, x)
# define BFI_FCM_INNER_I_13(x) BFI_FMACHINE_RESUME(BFI_FCM_INNER_C_13, x)
# define BFI_FCM_INNER_I_14(x) BFI_FMACHINE_RESUME(BFI_FCM_INNER_C_14, x)
# define BFI_FCM_INNER_I_15(x) BFI_FMACHINE_RESUME(BFI_FCM_INNER_C_15, x)
# define BFI_FCM_INNER_I_16(x) BFI_FMACHINE_RESUME(BFI_FCM_INNER_C_16, x)
# define BFI_FCM_INNER_I_17(x) BFI_FMACHINE_RESUME(BFI_FCM_INNER_C_17, x)
# define BFI_FCM_INNER_I_18(x) BFI_FMACHINE_RESUME(BFI_FCM_INNER_C_18, x)
# define BFI_FCM_INNER_I_19(x) BFI_FMACHINE_RESUME(BFI_FCM_INNER_C_19, x)
# define BFI_FCM_INNER_I_20(x) BFI_FMACHINE_RESUME(BFI_FCM_INNER_C_20, x)
# define BFI_FCM_INNER_I_21(x) BFI_FMACHINE_RESUME(BFI_FCM_INNER_C_21, x)
# define BFI_FCM_INNER_I_22(x) BFI_FMACHINE_RESUME(BFI_FCM_INNER_C_22, x)
# define BFI_FCM_INNER_I_23(x) BFI_FMACHINE_RESUME(BFI_FCM_INNER_C_23, x)
# define BFI_FCM_INNER_I_24(x) BFI_FMACHINE_RESUME(BFI_FCM_INNER_C_24, x)

/* BFI_FMACHINE_RESUME(f, x)
 *    f : one of the BFI_FCM_*_C_* functions
 *    x : intermediate result
 *  If x is a final state, it expands to x.
 *  Otherwise it calls f to continue execution.
 *  Note that "BFI_FMACHINE_RESUME(f, x)" is not a simple macro call.
 *  Since x contains a closing parenthesis, it is a macro call followed by
 *  a portable representation or a final result, followed by a closing paren.
 *  This makes sure that BFI_FMACHINE_RESUME itself does not call f,
 *  and therfore is reentrant.
 */
# define BFI_FMACHINE_RESUME(k, o, d) o()BFI_FMACHINE_RESUME_##d(k)(
# define BFI_FMACHINE_RESUME_0(k) BFI_EMPTY, 0)BFI_REM
# define BFI_FMACHINE_RESUME_1(k) k

/* BFI_ENTER_LINEAR(BFI_SETUP_REGS z(6bottom, BFI_EASY_OPEN_CONT_END))
 *    z : portable representation
 *  Turns z into an active representation and run it at most 256 steps.
 */
# define BFI_ENTER_LINEAR(r) BFI_ENTER_LINEAR_I r
# define BFI_ENTER_LINEAR_I(f) BFI_FINST_##f
# define BFI_SETUP_REGS(f) (f)BFI_SETUP_REGS_A
# define BFI_SETUP_REGS_A(args) args)BFI_SETUP_REGS_B
# define BFI_SETUP_REGS_B(e) e,BFI_EASY_OPEN_CONT_0

/* BFI_EASY_OPEN_CONT_0 (i_1, d_1)(i_2, d_2) ... (i_N, d_N)(z, BFI_EASY_OPEN_CONT_END)
 *  Expands to:
 *    i_1))d_1,i_2))d_2 ... i_N))d_N,z
 */
# define BFI_EASY_OPEN_CONT_0(i, d) i))d,BFI_EASY_OPEN_CONT_NEXT 1)
# define BFI_EASY_OPEN_CONT_1(i, d) i))d,BFI_EASY_OPEN_CONT_NEXT 0)
# define BFI_EASY_OPEN_CONT_NEXT BFI_PCAT(BFI_EASY_OPEN_CONT_,
# define BFI_EASY_OPEN_CONT_END BFI_DEFER(BFI_EAT_2)(~

/* BFI_FLINEAR_ENTRANCE_A
 * BFI_FLINEAR_ENTRANCE_B
 * BFI_FLINEAR_{N}
 *  These are macros that are passed as the first argument to accompanying
 *  macros.
 */
# define BFI_FLINEAR_ENTRANCE_A(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_254, v ds
# define BFI_FLINEAR_ENTRANCE_B(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_254, v ds

# define BFI_FLINEAR_256(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_255, v ds
# define BFI_FLINEAR_255(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_254, v ds
# define BFI_FLINEAR_254(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_253, v ds
# define BFI_FLINEAR_253(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_252, v ds
# define BFI_FLINEAR_252(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_251, v ds
# define BFI_FLINEAR_251(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_250, v ds
# define BFI_FLINEAR_250(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_249, v ds
# define BFI_FLINEAR_249(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_248, v ds
# define BFI_FLINEAR_248(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_247, v ds
# define BFI_FLINEAR_247(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_246, v ds
# define BFI_FLINEAR_246(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_245, v ds
# define BFI_FLINEAR_245(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_244, v ds
# define BFI_FLINEAR_244(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_243, v ds
# define BFI_FLINEAR_243(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_242, v ds
# define BFI_FLINEAR_242(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_241, v ds
# define BFI_FLINEAR_241(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_240, v ds
# define BFI_FLINEAR_240(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_239, v ds
# define BFI_FLINEAR_239(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_238, v ds
# define BFI_FLINEAR_238(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_237, v ds
# define BFI_FLINEAR_237(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_236, v ds
# define BFI_FLINEAR_236(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_235, v ds
# define BFI_FLINEAR_235(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_234, v ds
# define BFI_FLINEAR_234(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_233, v ds
# define BFI_FLINEAR_233(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_232, v ds
# define BFI_FLINEAR_232(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_231, v ds
# define BFI_FLINEAR_231(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_230, v ds
# define BFI_FLINEAR_230(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_229, v ds
# define BFI_FLINEAR_229(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_228, v ds
# define BFI_FLINEAR_228(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_227, v ds
# define BFI_FLINEAR_227(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_226, v ds
# define BFI_FLINEAR_226(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_225, v ds
# define BFI_FLINEAR_225(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_224, v ds
# define BFI_FLINEAR_224(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_223, v ds
# define BFI_FLINEAR_223(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_222, v ds
# define BFI_FLINEAR_222(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_221, v ds
# define BFI_FLINEAR_221(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_220, v ds
# define BFI_FLINEAR_220(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_219, v ds
# define BFI_FLINEAR_219(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_218, v ds
# define BFI_FLINEAR_218(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_217, v ds
# define BFI_FLINEAR_217(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_216, v ds
# define BFI_FLINEAR_216(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_215, v ds
# define BFI_FLINEAR_215(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_214, v ds
# define BFI_FLINEAR_214(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_213, v ds
# define BFI_FLINEAR_213(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_212, v ds
# define BFI_FLINEAR_212(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_211, v ds
# define BFI_FLINEAR_211(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_210, v ds
# define BFI_FLINEAR_210(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_209, v ds
# define BFI_FLINEAR_209(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_208, v ds
# define BFI_FLINEAR_208(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_207, v ds
# define BFI_FLINEAR_207(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_206, v ds
# define BFI_FLINEAR_206(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_205, v ds
# define BFI_FLINEAR_205(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_204, v ds
# define BFI_FLINEAR_204(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_203, v ds
# define BFI_FLINEAR_203(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_202, v ds
# define BFI_FLINEAR_202(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_201, v ds
# define BFI_FLINEAR_201(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_200, v ds
# define BFI_FLINEAR_200(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_199, v ds
# define BFI_FLINEAR_199(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_198, v ds
# define BFI_FLINEAR_198(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_197, v ds
# define BFI_FLINEAR_197(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_196, v ds
# define BFI_FLINEAR_196(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_195, v ds
# define BFI_FLINEAR_195(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_194, v ds
# define BFI_FLINEAR_194(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_193, v ds
# define BFI_FLINEAR_193(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_192, v ds
# define BFI_FLINEAR_192(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_191, v ds
# define BFI_FLINEAR_191(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_190, v ds
# define BFI_FLINEAR_190(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_189, v ds
# define BFI_FLINEAR_189(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_188, v ds
# define BFI_FLINEAR_188(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_187, v ds
# define BFI_FLINEAR_187(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_186, v ds
# define BFI_FLINEAR_186(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_185, v ds
# define BFI_FLINEAR_185(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_184, v ds
# define BFI_FLINEAR_184(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_183, v ds
# define BFI_FLINEAR_183(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_182, v ds
# define BFI_FLINEAR_182(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_181, v ds
# define BFI_FLINEAR_181(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_180, v ds
# define BFI_FLINEAR_180(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_179, v ds
# define BFI_FLINEAR_179(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_178, v ds
# define BFI_FLINEAR_178(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_177, v ds
# define BFI_FLINEAR_177(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_176, v ds
# define BFI_FLINEAR_176(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_175, v ds
# define BFI_FLINEAR_175(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_174, v ds
# define BFI_FLINEAR_174(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_173, v ds
# define BFI_FLINEAR_173(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_172, v ds
# define BFI_FLINEAR_172(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_171, v ds
# define BFI_FLINEAR_171(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_170, v ds
# define BFI_FLINEAR_170(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_169, v ds
# define BFI_FLINEAR_169(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_168, v ds
# define BFI_FLINEAR_168(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_167, v ds
# define BFI_FLINEAR_167(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_166, v ds
# define BFI_FLINEAR_166(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_165, v ds
# define BFI_FLINEAR_165(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_164, v ds
# define BFI_FLINEAR_164(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_163, v ds
# define BFI_FLINEAR_163(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_162, v ds
# define BFI_FLINEAR_162(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_161, v ds
# define BFI_FLINEAR_161(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_160, v ds
# define BFI_FLINEAR_160(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_159, v ds
# define BFI_FLINEAR_159(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_158, v ds
# define BFI_FLINEAR_158(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_157, v ds
# define BFI_FLINEAR_157(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_156, v ds
# define BFI_FLINEAR_156(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_155, v ds
# define BFI_FLINEAR_155(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_154, v ds
# define BFI_FLINEAR_154(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_153, v ds
# define BFI_FLINEAR_153(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_152, v ds
# define BFI_FLINEAR_152(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_151, v ds
# define BFI_FLINEAR_151(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_150, v ds
# define BFI_FLINEAR_150(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_149, v ds
# define BFI_FLINEAR_149(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_148, v ds
# define BFI_FLINEAR_148(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_147, v ds
# define BFI_FLINEAR_147(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_146, v ds
# define BFI_FLINEAR_146(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_145, v ds
# define BFI_FLINEAR_145(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_144, v ds
# define BFI_FLINEAR_144(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_143, v ds
# define BFI_FLINEAR_143(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_142, v ds
# define BFI_FLINEAR_142(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_141, v ds
# define BFI_FLINEAR_141(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_140, v ds
# define BFI_FLINEAR_140(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_139, v ds
# define BFI_FLINEAR_139(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_138, v ds
# define BFI_FLINEAR_138(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_137, v ds
# define BFI_FLINEAR_137(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_136, v ds
# define BFI_FLINEAR_136(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_135, v ds
# define BFI_FLINEAR_135(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_134, v ds
# define BFI_FLINEAR_134(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_133, v ds
# define BFI_FLINEAR_133(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_132, v ds
# define BFI_FLINEAR_132(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_131, v ds
# define BFI_FLINEAR_131(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_130, v ds
# define BFI_FLINEAR_130(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_129, v ds
# define BFI_FLINEAR_129(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_128, v ds
# define BFI_FLINEAR_128(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_127, v ds
# define BFI_FLINEAR_127(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_126, v ds
# define BFI_FLINEAR_126(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_125, v ds
# define BFI_FLINEAR_125(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_124, v ds
# define BFI_FLINEAR_124(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_123, v ds
# define BFI_FLINEAR_123(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_122, v ds
# define BFI_FLINEAR_122(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_121, v ds
# define BFI_FLINEAR_121(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_120, v ds
# define BFI_FLINEAR_120(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_119, v ds
# define BFI_FLINEAR_119(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_118, v ds
# define BFI_FLINEAR_118(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_117, v ds
# define BFI_FLINEAR_117(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_116, v ds
# define BFI_FLINEAR_116(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_115, v ds
# define BFI_FLINEAR_115(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_114, v ds
# define BFI_FLINEAR_114(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_113, v ds
# define BFI_FLINEAR_113(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_112, v ds
# define BFI_FLINEAR_112(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_111, v ds
# define BFI_FLINEAR_111(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_110, v ds
# define BFI_FLINEAR_110(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_109, v ds
# define BFI_FLINEAR_109(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_108, v ds
# define BFI_FLINEAR_108(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_107, v ds
# define BFI_FLINEAR_107(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_106, v ds
# define BFI_FLINEAR_106(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_105, v ds
# define BFI_FLINEAR_105(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_104, v ds
# define BFI_FLINEAR_104(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_103, v ds
# define BFI_FLINEAR_103(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_102, v ds
# define BFI_FLINEAR_102(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_101, v ds
# define BFI_FLINEAR_101(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_100, v ds
# define BFI_FLINEAR_100(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_99, v ds
# define BFI_FLINEAR_99(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_98, v ds
# define BFI_FLINEAR_98(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_97, v ds
# define BFI_FLINEAR_97(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_96, v ds
# define BFI_FLINEAR_96(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_95, v ds
# define BFI_FLINEAR_95(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_94, v ds
# define BFI_FLINEAR_94(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_93, v ds
# define BFI_FLINEAR_93(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_92, v ds
# define BFI_FLINEAR_92(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_91, v ds
# define BFI_FLINEAR_91(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_90, v ds
# define BFI_FLINEAR_90(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_89, v ds
# define BFI_FLINEAR_89(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_88, v ds
# define BFI_FLINEAR_88(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_87, v ds
# define BFI_FLINEAR_87(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_86, v ds
# define BFI_FLINEAR_86(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_85, v ds
# define BFI_FLINEAR_85(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_84, v ds
# define BFI_FLINEAR_84(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_83, v ds
# define BFI_FLINEAR_83(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_82, v ds
# define BFI_FLINEAR_82(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_81, v ds
# define BFI_FLINEAR_81(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_80, v ds
# define BFI_FLINEAR_80(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_79, v ds
# define BFI_FLINEAR_79(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_78, v ds
# define BFI_FLINEAR_78(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_77, v ds
# define BFI_FLINEAR_77(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_76, v ds
# define BFI_FLINEAR_76(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_75, v ds
# define BFI_FLINEAR_75(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_74, v ds
# define BFI_FLINEAR_74(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_73, v ds
# define BFI_FLINEAR_73(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_72, v ds
# define BFI_FLINEAR_72(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_71, v ds
# define BFI_FLINEAR_71(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_70, v ds
# define BFI_FLINEAR_70(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_69, v ds
# define BFI_FLINEAR_69(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_68, v ds
# define BFI_FLINEAR_68(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_67, v ds
# define BFI_FLINEAR_67(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_66, v ds
# define BFI_FLINEAR_66(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_65, v ds
# define BFI_FLINEAR_65(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_64, v ds
# define BFI_FLINEAR_64(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_63, v ds
# define BFI_FLINEAR_63(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_62, v ds
# define BFI_FLINEAR_62(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_61, v ds
# define BFI_FLINEAR_61(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_60, v ds
# define BFI_FLINEAR_60(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_59, v ds
# define BFI_FLINEAR_59(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_58, v ds
# define BFI_FLINEAR_58(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_57, v ds
# define BFI_FLINEAR_57(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_56, v ds
# define BFI_FLINEAR_56(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_55, v ds
# define BFI_FLINEAR_55(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_54, v ds
# define BFI_FLINEAR_54(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_53, v ds
# define BFI_FLINEAR_53(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_52, v ds
# define BFI_FLINEAR_52(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_51, v ds
# define BFI_FLINEAR_51(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_50, v ds
# define BFI_FLINEAR_50(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_49, v ds
# define BFI_FLINEAR_49(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_48, v ds
# define BFI_FLINEAR_48(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_47, v ds
# define BFI_FLINEAR_47(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_46, v ds
# define BFI_FLINEAR_46(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_45, v ds
# define BFI_FLINEAR_45(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_44, v ds
# define BFI_FLINEAR_44(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_43, v ds
# define BFI_FLINEAR_43(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_42, v ds
# define BFI_FLINEAR_42(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_41, v ds
# define BFI_FLINEAR_41(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_40, v ds
# define BFI_FLINEAR_40(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_39, v ds
# define BFI_FLINEAR_39(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_38, v ds
# define BFI_FLINEAR_38(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_37, v ds
# define BFI_FLINEAR_37(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_36, v ds
# define BFI_FLINEAR_36(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_35, v ds
# define BFI_FLINEAR_35(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_34, v ds
# define BFI_FLINEAR_34(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_33, v ds
# define BFI_FLINEAR_33(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_32, v ds
# define BFI_FLINEAR_32(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_31, v ds
# define BFI_FLINEAR_31(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_30, v ds
# define BFI_FLINEAR_30(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_29, v ds
# define BFI_FLINEAR_29(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_28, v ds
# define BFI_FLINEAR_28(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_27, v ds
# define BFI_FLINEAR_27(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_26, v ds
# define BFI_FLINEAR_26(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_25, v ds
# define BFI_FLINEAR_25(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_24, v ds
# define BFI_FLINEAR_24(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_23, v ds
# define BFI_FLINEAR_23(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_22, v ds
# define BFI_FLINEAR_22(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_21, v ds
# define BFI_FLINEAR_21(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_20, v ds
# define BFI_FLINEAR_20(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_19, v ds
# define BFI_FLINEAR_19(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_18, v ds
# define BFI_FLINEAR_18(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_17, v ds
# define BFI_FLINEAR_17(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_16, v ds
# define BFI_FLINEAR_16(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_15, v ds
# define BFI_FLINEAR_15(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_14, v ds
# define BFI_FLINEAR_14(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_13, v ds
# define BFI_FLINEAR_13(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_12, v ds
# define BFI_FLINEAR_12(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_11, v ds
# define BFI_FLINEAR_11(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_10, v ds
# define BFI_FLINEAR_10(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_9, v ds
# define BFI_FLINEAR_9(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_8, v ds
# define BFI_FLINEAR_8(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_7, v ds
# define BFI_FLINEAR_7(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_6, v ds
# define BFI_FLINEAR_6(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_5, v ds
# define BFI_FLINEAR_5(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_4, v ds
# define BFI_FLINEAR_4(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_3, v ds
# define BFI_FLINEAR_3(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_2, v ds
# define BFI_FLINEAR_2(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_1, v ds
# define BFI_FLINEAR_1(v, f, ds) BFI_FINST_##f(BFI_FLINEAR_0, v ds
# define BFI_FLINEAR_0(v, f, ds) BFI_EMPTY,1)(f)((BFI_AVAILABLE_ENTRANCE(), v, BFI_CLEANUP_REST(~ds

/* BFI_CLEANUP_REST(~, d_1, t_1)) d_2, t_2)) d_3, t_3)) ... d_N,6bottom))
 *  Expands to:
 *    d_1, t_1))(d_2)(t_2, d_3) ... (t_N, d_N)
 */
# define BFI_CLEANUP_REST(_, d, t) d, t))(BFI_CLEANUP_REST_A(
# define BFI_CLEANUP_REST_A() BFI_CLOSE_CONT(
# define BFI_CLOSE_CONT(d, k) d)BFI_IFN(BFI_BOTTOM_TEST_##k)(BFI_EAT, BFI_CLOSE_CONT_NEXT)(k
# define BFI_CLOSE_CONT_NEXT(k) (k,BFI_CLOSE_CONT(

/* BFI_AVAILABLE_ENTRANCE()
 *  Returns either BFI_FLINEAR_ENTRANCE_A or BFI_FLINEAR_ENTRANCE_B, whichever
 *  is "free", i.e. expandable in the current context.
 */
# define BFI_AVAILABLE_ENTRANCE() BFI_IF(BFI_NULLARYP(BFI_FLINEAR_ENTRANCE_A(~, BFI_AVAILABLE_ENTRANCE_TEST, BFI_RPAREN())))(BFI_FLINEAR_ENTRANCE_A, BFI_FLINEAR_ENTRANCE_B)
# define BFI_FINST_BFI_AVAILABLE_ENTRANCE_TEST ()
# define BFI_AVAILABLE_ENTRANCE_TEST (

/* Internal instructions
 *
 * 6stop: terminate the machine
 * 6bottom: end-of-stack marker in the active representation
 * 6stack_erorr: a padding frame to prevent the stack from becoming empty
 */
# define BFI_FINST_6stop(f, v, d, t) BFI_EMPTY, 0)BFI_ID(v)BFI_EAT_CONT(
# define BFI_FINST_6bottom() /* should never be called */
# define BFI_FINST_6stack_error() /* should never be called */

/* BFI_EAT_CONT() d_1, t_1)) d_2, t_2)) ... d_N, 6bottom))
 *  Expands to nothing
 */
# define BFI_EAT_CONT() BFI_EAT_CONT_NEXT(
# define BFI_EAT_CONT_NEXT(d, k) BFI_IFN(BFI_BOTTOM_TEST_##k)(BFI_EMPTY, BFI_EAT_CONT)(

# define BFI_BOTTOM_TEST_6bottom ()

# endif
