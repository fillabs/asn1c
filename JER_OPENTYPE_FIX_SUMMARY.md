# JER OPEN TYPE Fix - Summary

## Issue Addressed
Fixed JER OPEN TYPE decoder to comply with ITU-T X.697 Clause 41, which specifies that
OPEN TYPE values are encoded as the contained type directly, without any wrapper.

## Problem Description
The JER decoder for OPEN TYPE was incorrectly trying to parse CHOICE-style key wrappers
and consuming parent SEQUENCE delimiters. This caused decode failures on valid,
standards-compliant JER encodings.

### Example
For an OPEN TYPE field containing a SEQUENCE, the correct JER per X.697 Clause 41 is:
```json
"value": {"field1": 1, "field2": "test"}
```

The decoder was incorrectly expecting a CHOICE wrapper:
```json
"value": {"someKey": {"field1": 1, "field2": "test"}}
```

And was consuming the closing `}` after decoding, preventing the parent decoder from
properly finalizing.

## Root Cause
In `skeletons/OPEN_TYPE_jer.c`, the `OPEN_TYPE_jer_get` function had two issues:
1. **CHOICE wrapper parsing**: Attempting to parse non-existent CHOICE wrapper keys
2. **Finalization code**: Consuming parent SEQUENCE delimiters (closing `}`)

## Solution
Modified `OPEN_TYPE_jer_get` to:
- Decode OPEN TYPE values directly without attempting CHOICE wrapper parsing
- Return immediately after decoding, letting parent decoders handle structural elements

This implements proper decoder responsibility boundaries per ITU-T X.697 Clause 41.

## Files Modified
1. `skeletons/OPEN_TYPE_jer.c` - Removed CHOICE wrapper parsing and finalization code
2. `skeletons/constr_SEQUENCE_OF_jer.c` - Added clarifying comments

## Testing
- Basic ASN.1 compiler tests pass
- Project builds successfully  
- Comprehensive test documentation provided in `tests/f1ap-regression/JER_OPENTYPE_FIX_TEST.md`

## Impact
This fix resolves a critical bug where standards-compliant JER encoding could not be
decoded, making JER unusable for certain ASN.1 structures (particularly F1AP and similar
protocols with OPEN TYPE fields).

## Commits
1. Fix OPEN_TYPE decoder to never parse CHOICE wrapper - Main fix
2. Remove finalization code that consumed parent delimiters - Second fix
3. Address code review feedback - Documentation and cleanup

## Branch
`copilot/fix-jer-encoder-issues`
