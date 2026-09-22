# level2 – bonus patch notes

## What I wanted
Same goal as level1: make level2 accept any password, with one patch
living in the file itself, no LD_PRELOAD or runtime tricks.

## First attempt
I found the very first check in `main`, right after `scanf`:

```
131b:  cmp    -0xc(%ebp),%eax
131e:  je     1476 <main+0x1a6>   ; jumps straight to call ok()
1324:  mov    -0x40(%ebp),%ebx
1327:  call   1220 <no>
```

`1476` turned out to be the call to `ok()` at the very end of `main` —
past the '0'/'0' prefix checks, the whole decode loop, and the final
`strcmp`. So this one jump, if forced to always fire, skips every check
in the program in one go. I didn't need to touch the other three
`call no` sites at all.

## The problem
`je` only jumps if the comparison right before it says "equal", and that
comparison depends on what `scanf` returned. Typing any real string
works fine, but Ctrl+D with no input at all makes `scanf` return
something else, the jump doesn't fire, and it falls through to `no()`.
Technically not a "password", but I didn't want to leave it ambiguous.

## The fix: make the jump unconditional
Instead of `je` (conditional), I used `jmp` (always jumps, no flag
check, so it doesn't care what scanf returned).

Only catch: `je` here is 6 bytes (`0f 84` + 4-byte offset), but `jmp` is
only 5 bytes (`e9` + 4-byte offset). Writing 5 bytes into a 6-byte slot
would leave one stray byte and break the next instruction, so I padded
the extra byte with a `nop` (`90`, does nothing) to keep the total at 6
bytes and avoid shifting anything after it.

## Recomputing the offset
Offsets are counted from the address right after the jump, and since
`jmp` is 5 bytes now instead of 6, that reference point shifts by one:

```
end of new instruction = 0x131e + 5 = 0x1323
offset = target - end   = 0x1476 - 0x1323 = 0x153
```

## The patch
```bash
printf '\xe9\x53\x01\x00\x00\x90' | dd of=level2_p bs=1 seek=$((0x131e)) count=6 conv=notrunc
```

## Checking it
```
objdump -d -M intel level2_p | grep -A1 131e
131e:  e9 53 01 00 00    jmp    1476 <main+0x1a6>
1323:  90                nop
```
Exactly what I wanted: a `jmp` to `ok()`, plus one nop to fill the gap,
nothing else in the file shifted.

## Result
```bash
./level2_p
Good job.        # any password
./level2_p
^D
Good job.        # even empty input / Ctrl+D
```

## Why this is the best fix for this level
One instruction, one patch site, no dependency on any register or flag
state at the time it runs — it always lands on `ok()`, no matter what
the earlier code computed. Simpler to explain than patching all four
`call no` sites separately, and it can't be broken by an edge-case
input like empty stdin.
