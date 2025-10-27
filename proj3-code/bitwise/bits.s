# Read the following instructions carefully
#
# You will provide your solution to this part of the project by
# editing the collection of functions in this source file.
#
# Some rules from the C bitwise puzzles are still in effect for your assembly
# code here:
#  1. No global variables are allowed
#  2. You may not define or call any additional functions in this file
#  3. You may not use any floating-point assembly instructions
#
# You may assume that your machine:
#  1. Uses two's complement, 32-bit representations of integers.
#  2. Has unpredictable behavior when shifting if the shift amount
#     is less than 0 or greater than 31.

# TO AVOID GRADING SURPRISES:
#   Pay attention to the results of the call_cc script, which is run
#   as part of the provided tests.
#
#   This makes sure you have adhered to the x86-64 calling convention
#   in your assembly code. If it reports any errors, make sure to fix
#   them before you submit your code.
#
# YOU WILL RECEIVE NO CREDIT IF YOUR CODE DOES NOT PASS THIS CHECK

# bitXor - Compute x^y
#   Example: bitXor(4, 5) = 1
#   Rating: 1
.global bitXor
bitXor:

    movl %edi, %r8d
    movl %esi, %r9d

    movl %edi, %eax
    andl %esi, %eax
    notl %eax


    notl %r8d
    notl %r9d
    andl %r8d,%r9d
    notl %r9d
    andl %r9d,%eax
    ret

# bitAnd - Compute x&y
#   Example: bitAnd(6, 5) = 4
#   Rating: 1
.global bitAnd
bitAnd:
    movl %edi, %eax
    movl %esi, %r8d
    notl %r8d
    notl %eax
    orl %r8d, %eax
    notl %eax
    ret

# allOddBits - Return 1 if all odd-numbered bits in word set to 1
#   where bits are numbered from 0 (least significant) to 31 (most significant)
#   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
#   Rating: 2
.global allOddBits
allOddBits:
    movl %edi, %eax
    movl $0xAAAAAAAA, %r8d
    andl %r8d, %eax
    cmpl %r8d,%eax
    sete %al
    movzbl %al,%eax
    ret

# floatIsEqual - Compute f == g for floating point arguments f and g.
#   Both the arguments are passed as unsigned int's, but
#   they are to be interpreted as the bit-level representations of
#   single-precision floating point values.
#   If either argument is NaN, return 0.
#   +0 and -0 are considered equal.
#   Rating: 2
.global floatIsEqual
floatIsEqual:
    movl %edi,%eax
    movl %esi,%ecx
    andl $0x7FFFFFFF,%eax
    andl $0x7FFFFFFF,%ecx
    cmpl %eax,%ecx
    jne .ne
    testl %eax,%eax
    jz .zero
    cmpl %edi,%esi
    jne .ne
    cmpl $0x7F800000,%eax
    ja .ne
.zero:
    movl $1, %eax
    ret
.ne:
    movl $0, %eax
    ret

# anyEvenBit - Return 1 if any even-numbered bit in word set to 1
#   where bits are numbered from 0 (least significant) to 31 (most significant)
#   Examples anyEvenBit(0xA) = 0, anyEvenBit(0xE) = 1
#   Rating: 2
.global anyEvenBit
anyEvenBit:
    movl %edi, %eax
    andl $0x55555555, %eax
    testl %eax,%eax
    setnz %al
    movzbl %al,%eax
    ret

# isPositive - return 1 if x > 0, return 0 otherwise
#   Example: isPositive(-1) = 0.
#   Rating: 2
.global isPositive
isPositive:
    movl %edi, %r8d
    notl %r8d
    shr $31,%r8d
    testl %edi, %edi
    setnz %al
    movzbl %al,%eax
    andl %r8d,%eax
    ret

# replaceByte(x,n,c) - Replace byte n in x with c
#   Bytes numbered from 0 (least significant) to 3 (most significant)
#   Examples: replaceByte(0x12345678, 1, 0xab) = 0x1234ab78
#   You can assume 0 <= n <= 3 and 0 <= c <= 255
#   Rating: 3
.global replaceByte
replaceByte:
    movl %esi, %ecx
    movl $0xFF, %eax
    movl %edx, %r8d
    shl $3,%ecx
    shl %ecx, %eax
    shl %ecx, %r8d
    notl %eax
    andl %edi, %eax
    orl %r8d, %eax
    ret

# isLess - if x < y  then return 1, else return 0
#   Example: isLess(4,5) = 1.
#   Rating: 3
.global isLess
isLess:
    cmpl %esi,%edi
    setl %al
    movzbl %al, %eax
    ret

# rotateLeft - Rotate x to the left by n
#   Can assume that 0 <= n <= 31
#   Examples: rotateLeft(0x87654321,4) = 0x76543218
#   Rating: 3
.global rotateLeft
rotateLeft:
    movl %edi, %eax
    movl %esi, %ecx
    rol %ecx, %eax
    ret

# bitMask - Generate a mask consisting of all 1's
#   between lowbit and highbit positions
#   Examples: bitMask(5,3) = 0x38
#   Assume 0 <= lowbit <= 31, and 0 <= highbit <= 31
#   If lowbit > highbit, then mask should be all 0's
#   Rating: 3
.global bitMask
bitMask:
    movl $0xFFFFFFFF, %eax
    movl $1, %r8d
    movl %edi, %ecx
    shl %ecx,%eax
    shl $1,%eax
    movl %esi, %ecx
    shl %ecx,%r8d
    subl $1,%r8d
    orl %r8d,%eax
    notl %eax
    ret

# floatScale2 - Return bit-level equivalent of expression 2*f for
#   floating point argument f.
#   Both the argument and result are passed as unsigned int's, but
#   they are to be interpreted as the bit-level representation of
#   single-precision floating point values.
#   When argument is NaN, return argument
#   Rating: 4
.global floatScale2
floatScale2:
    movl %edi, %eax
    movl %edi, %r8d
    andl $0x7F800000, %r8d
    cmpl $0x7F800000, %r8d
    je .ruf
    testl %r8d, %r8d
    jz .rxuf
    addl $0x800000,%eax
.ruf:
    ret
.rxuf:
    movl %edi, %r8d
    shl $1,%r8d
    andl $0x80000000,%eax
    orl %r8d,%eax
    ret


# isPower2 - returns 1 if x is a power of 2, and 0 otherwise
#   Examples: isPower2(5) = 0, isPower2(8) = 1, isPower2(0) = 0
#   Note that no negative number is a power of 2.
#   Rating: 4
.global isPower2
isPower2:
    movl %edi,%r8d
    subl $1, %r8d
    andl %edi,%r8d
    testl %r8d,%r8d
    setz %cl
    movzbl %cl, %ecx
    call isPositive
    andl %ecx,%eax
    ret

