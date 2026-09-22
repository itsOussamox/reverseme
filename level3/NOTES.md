# Level 3 — How I found the password

## 1. First contact

I ran the binary directly:

```
$ ./level3
Please enter key: test
Nope.
```

A wrong input prints `Nope.` and exits. So there is a check somewhere in `main`
that rejects bad input.

## 2. Checking imports

```
$ readelf -W --dyn-syms level3 | grep FUNC
```

Imports: `printf`, `__isoc99_scanf`, `strlen`, `atoi`, `memset`, `strcmp`,
`fflush`, `exit`. Same family as level2, so I expected the same kind of
"read input, decode it, compare it" structure rather than a single
`strcmp` on raw input.

## 3. Disassembling `main`

```
$ gdb ./level3
(gdb) break main
(gdb) run
(gdb) disassemble main
```

I read it top to bottom, in blocks.

### 3.1 Prompt and input

```asm
lea 0xd0d(%rip),%rdi      ; "Please enter key: "
call printf
lea -0x40(%rbp),%rsi      ; address of input buffer
lea 0xd0e(%rip),%rdi      ; "%23s"
call __isoc99_scanf
```

`scanf("%23s", input)`. The buffer starts at `rbp-0x40`.

### 3.2 Two hardcoded character checks

```asm
movsbl -0x3f(%rbp),%ecx   ; input[1]
mov    $0x32,%eax         ; 0x32 = '2'
cmp    %ecx,%eax
je     ...                ; skip fail call if equal
call   ___syscall_malloc  ; fail path

movsbl -0x40(%rbp),%ecx   ; input[0]
mov    $0x34,%eax         ; 0x34 = '4'
cmp    %ecx,%eax
je     ...
call   ___syscall_malloc  ; fail path
```

I converted the hex constants to characters with gdb:

```gdb
(gdb) p/c 0x32
$1 = 50 '2'
(gdb) p/c 0x34
$2 = 52 '4'
```

**Conclusion: the key must start with `"42"`.**

### 3.3 Spotting the decoy fail function name

The fail path is a call to a function named `___syscall_malloc` (three
leading underscores). That name is misleading on purpose, real `malloc`
never appears anywhere in the imports (`readelf --dyn-syms` in step 2).
I confirmed it by disassembling the function itself:

```gdb
(gdb) disassemble ___syscall_malloc
```

It calls `puts("Nope.")` then `exit(1)`. So it is the fail function,
just given a name that looks like it does something else.

### 3.4 The decode loop

```asm
call   memset               ; memset(out, 0, 9)
movb   $0x2a,-0x21(%rbp)    ; out[0] = '*'   (0x2a = '*')
movq   $0x2,-0x18(%rbp)     ; k = 2
movl   $0x1,-0xc(%rbp)      ; j = 1

; loop:
call   strlen                ; strlen(out)   -> loop condition #1
cmp    $0x8,%rcx
jae    <exit loop>
; strlen(input)              -> loop condition #2 (k < strlen(input))
; tmp[0] = input[k]
; tmp[1] = input[k+1]
; tmp[2] = input[k+2]
call   atoi                  ; out[j] = atoi(tmp)
; k += 3, j += 1
jmp    <loop>
```

I recognized this shape from level2: read 3 digits at a time from the
input, convert them to a number with `atoi`, store the low byte as one
character of `out`, and repeat until `out` has 8 characters or the input
runs out. `out[0]` is fixed to `'*'` no matter what you type.

### 3.5 The switch after `strcmp`

```asm
lea    0xb93(%rip),%rsi     ; hidden string, at 0x555555556004
lea    -0x21(%rbp),%rdi     ; out
call   strcmp
mov    %eax,-0x10(%rbp)     ; save result
```

Then a long chain compares that saved result against `-2, -1, 0, 1, 2,
3, 4, 5, 0x73`. At first this looked like a complex multi-way check, but
tracing each branch's target showed only two distinct calls:

- Every branch except `== 0` calls `___syscall_malloc` (3 underscores) →
  the fail function from step 3.3.
- Only `== 0` calls a **different** function, `____syscall_malloc`
  (4 underscores) → confirmed with `disassemble ____syscall_malloc`,
  which calls `puts("Good job.")`.

So the extra branches (`-2, -1, 1, 2, 3, 4, 5, 115`) are decoys. The
only case that matters is the normal one: `strcmp(out, hidden) == 0`.
This is meant to make the logic look more complicated than it is.

## 4. Reading the hidden string

Since I already had its address from the disassembly (`0x555555556004`),
I read it directly, no need to run the program or set a breakpoint:

```gdb
(gdb) x/s 0x555555556004
0x555555556004: "********"
```

I double-checked this two ways:
- It also appeared on its own in a plain `strings level3` dump.
- I confirmed it dynamically too, by breaking on the exact `call strcmp`
  instruction inside `main` (not on the function name `strcmp`, which
  also matches unrelated calls made by the dynamic linker at startup):

  ```gdb
  (gdb) break *0x555555555475
  (gdb) run
  42101101101101101101101
  (gdb) x/s $rdi        ; -> "*eeeeeee" (my decoded input)
  (gdb) x/s $rsi        ; -> "********" (the target)
  ```

## 5. Building the password

The target is `"********"`, eight asterisks. `out[0]` is already `'*'`
for free (hardcoded). Every group of 3 digits must decode to `'*'`,
which is ASCII **42** (`0x2a`):

```
atoi("042") = 42  ->  '*'
```

7 groups are needed to fill `out[1]` through `out[7]`:

```
key = "42" + "042" * 7
    = 42042042042042042042042
```

## 6. Verifying

```
$ ./level3
Please enter key: 42042042042042042042042
Good job.
```

Confirmed.

## 7. Note on multiple valid passwords

`atoi`'s result is stored in a single byte (`char`), so any 3-digit
group congruent to 42 modulo 256 also works, e.g. `298` (`298 - 256 =
42`). This matches the subject's statement that several passwords can
be valid for the same binary.

## Summary for the defense

| Question | Answer |
|---|---|
| How does the program read input? | `scanf("%23s", input)` |
| What must the input start with? | `"42"`, checked byte by byte against `0x34` and `0x32` |
| How is the password built from input? | 3 input digits at a time → `atoi` → 1 output character, starting after the fixed `'*'` at `out[0]` |
| What is the fail function really called? | `___syscall_malloc` (3 underscores), a decoy name; it prints "Nope." and exits |
| What is the success function really called? | `____syscall_malloc` (4 underscores), also a decoy name; it prints "Good job." |
| Why the switch with many branches? | Decoy. Only `strcmp(...) == 0` is real; all other branches also call the fail function |
| What is the target string? | `"********"` |
| A valid password | `42042042042042042042042` |
