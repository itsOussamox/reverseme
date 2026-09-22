# level1 – bonus patch notes

## What I wanted
Make level1 accept any password, without touching the source or hooking
anything at runtime (no LD_PRELOAD). The change has to live in the file
itself.

## Finding the check
I disassembled the binary and looked around the `strcmp` call in `main`:

```
123c:  e8 ff fd ff ff       call   1040 <strcmp@plt>
1241:  83 f8 00             cmp    eax,0x0
1244:  0f 85 16 00 00 00    jne    1260 <main+0xa0>
124a:  8b 5d 80             mov    ebx,DWORD PTR [ebp-0x80]
```

`strcmp` compares my input with the hidden password and returns 0 if they
match. `cmp eax,0x0` checks that return value, and `jne` jumps away to the
failure path if it's *not* zero (wrong password). If it's zero, execution
just falls through to the success code right after.

So the whole password check boils down to one instruction: that `jne`.
If it never jumps, the program always falls into the success path,
no matter what I typed.

## Getting the file offset
```
readelf -S level1 | grep -A1 " .text"
[14] .text  PROGBITS  00001090  001090 ...
```
`Addr` and `Off` are the same value here, so this binary isn't relocated —
the address objdump shows me (`0x1244`) is also the exact byte offset in
the file. No conversion needed.

## The patch
The `jne` is 6 bytes long (`0f 85 16 00 00 00`), the "near" encoding of a
conditional jump. I overwrote those 6 bytes with NOPs (`0x90`, "do
nothing"), so the CPU just slides past where the jump used to be instead
of ever taking it:

```bash
cp level1 level1_patched
printf '\x90\x90\x90\x90\x90\x90' | dd of=level1_patched bs=1 seek=$((0x1244)) count=6 conv=notrunc
```

- `bs=1` — work one byte at a time, so `seek`/`count` line up with exact
  byte offsets.
- `seek=0x1244` — start writing at the offset I found above.
- `count=6` — only touch those 6 bytes, nothing more.
- `conv=notrunc` — the important one: without it `dd` would shrink the
  whole file down to just those 6 bytes instead of only patching them.

## Checking it
```bash
xxd -s 0x1244 -l 6 level1_patched
00001244: 9090 9090 9090
```
Exactly the 6 NOPs, right where I wanted them, and nothing else in the
file moved (same size, everything before/after untouched).

## Result
```bash
./level1_patched
zzzzzzzz
Good job.
```
Any input now passes, since the instruction that used to reject a wrong
password no longer exists.

## Why this counts as "patching the binary" and not LD_PRELOAD
Nothing runs alongside the program and nothing gets injected at load
time. The bytes on disk are permanently different — `xxd` on the file
proves that on its own, with the program not even running. Anyone who
runs this exact file, on any machine, gets the same "always succeeds"
behavior, which is exactly what LD_PRELOAD *can't* guarantee since it
depends on an environment variable being set at launch.
