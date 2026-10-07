# Partial Decoding Bug Fix

## Issues Addressed

This fix resolves two critical issues with the partial decoding feature introduced in the `-P` option:

### Issue 1: False Positives for Large Messages

**Problem**: When decoding a large ASN.1 message (e.g., 20KB+) with the default I/O buffer size (8192 bytes), partial decoding results were incorrectly displayed even though the message was valid and complete.

**Example**: A 20000-byte NR5G RRC message would trigger partial result output during normal operation, even though decoding ultimately succeeded.

**Why it happened**: The RC_WMORE handler printed partial results whenever PER decoding needed more data, which occurred during normal multi-chunk reading from the file.

### Issue 2: Duplicate Output for Truncated Messages

**Problem**: When decoding a truncated message (e.g., last byte missing), partial results were printed twice.

**Example**:
```
=== Partial Decoding Results (RC_WMORE) ===
[structure contents]
=== End of Partial Results ===

=== Partial Decoding Results (RC_WMORE) ===
[same structure contents again]
=== End of Partial Results ===
```

**Why it happened**: 
1. RC_WMORE handler printed partial results when hitting EOF
2. After loop exit, cleanup code also printed partial results
3. No mechanism prevented duplicate printing

## Solution

The fix introduces a `partial_printed` flag and modifies the printing logic to distinguish between:
- Normal multi-chunk reading (where more data is available)
- True EOF condition (where no more data will come)

### Code Changes

**File**: `skeletons/converter-example.c`

1. **Added tracking flag** (line ~892):
```c
int partial_printed = 0;  /* Track if we already printed partial results */
```

2. **Modified RC_WMORE handler** (line ~956):
```c
if(opt_partial && structure && rd == 0 && !partial_printed) {
    fprintf(stderr, "\n=== Partial Decoding Results (RC_WMORE) ===\n");
    asn_fprint(stderr, pduType, structure);
    fprintf(stderr, "=== End of Partial Results ===\n\n");
    partial_printed = 1;  /* Mark that we've printed partial results */
}
```

Key conditions:
- `rd == 0`: No more data was read from file (EOF reached)
- `!partial_printed`: Haven't printed partial results yet

3. **Modified cleanup section** (line ~1022):
```c
if(opt_partial && structure && !partial_printed) {
    fprintf(stderr, "\n=== Partial Decoding Results ===\n");
    asn_fprint(stderr, pduType, structure);
    fprintf(stderr, "=== End of Partial Results ===\n\n");
}
```

Added check: `!partial_printed` to prevent duplicate output

## Behavior After Fix

### Large Valid Message (20KB with 8KB buffer)

```
Iteration 1: Read 8192 bytes, decode → RC_WMORE
  - rd = 8192 (not zero)
  - Don't print partial results ✓
  - Continue reading...

Iteration 2: Read 8192 bytes, decode → RC_WMORE
  - rd = 8192 (not zero)
  - Don't print partial results ✓
  - Continue reading...

Iteration 3: Read 3616 bytes, decode → RC_OK
  - Successfully decoded ✓
  - No partial results shown ✓
```

### Truncated Message (Last Byte Missing)

```
Iteration 1-N: Read chunks, decode → RC_WMORE
  - rd > 0 (reading data)
  - Don't print partial results yet

Iteration N+1: Read returns 0 (EOF), decode buffered data → RC_WMORE
  - rd = 0 (EOF reached)
  - partial_printed = 0
  - Print partial results (ONCE) ✓
  - Set partial_printed = 1

Loop may continue: rd = 0, decode → RC_WMORE
  - partial_printed = 1
  - Don't print again ✓

Cleanup section:
  - partial_printed = 1 OR structure = NULL
  - Don't print again ✓
```

### RC_FAIL Error (Constraint Violation)

```
Decode → RC_FAIL at byte 12
  - RC_WMORE handler not triggered
  - Break from loop
  - Cleanup section prints partial results (once) ✓
```

## Testing Recommendations

To verify the fix works with real-world data:

### Test 1: Large Valid Message
```bash
# Create or use a large (20KB+) valid ASN.1 message
./converter-example -iber -onull -P large-message.ber

# Expected: Success message, NO partial results
# ✓ Message decoded successfully
```

### Test 2: Truncated Message
```bash
# Create a truncated message (remove last few bytes)
./converter-example -iber -onull -P truncated-message.ber

# Expected: Partial results shown ONCE
# === Partial Decoding Results ===
# [decoded fields]
# === End of Partial Results ===
# Decode failed past byte X: Unexpected end of input
```

### Test 3: With Larger Buffer
```bash
# Use -b to increase buffer size
./converter-example -iber -onull -P -b 262144 large-message.ber

# Expected: Single-pass decoding, no partial results for valid message
```

## Backward Compatibility

✓ **Fully backward compatible**
- Default behavior unchanged (no `-P` flag = no partial results)
- All existing functionality preserved
- Only affects behavior when `-P` flag is used
- Fixes make the `-P` behavior more correct and useful

## Related Files

- `skeletons/converter-example.c` - Implementation
- `PARTIAL_DECODING.md` - User documentation
- `IMPLEMENTATION_SUMMARY.md` - Technical summary
- GitHub Issue - Original bug report

## Summary

This fix ensures that the partial decoding feature (`-P` option) works correctly by:
1. Only displaying partial results when truly needed (at EOF, not during normal reading)
2. Preventing duplicate output by tracking whether results were already shown
3. Maintaining full backward compatibility with existing code

The result is a more reliable and useful debugging tool for analyzing truncated or malformed ASN.1 messages.
