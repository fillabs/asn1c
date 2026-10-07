# JER OPEN TYPE Fix Test

This document describes how to test the fix for the JER OPEN TYPE decoder issue
where OPEN TYPE values were not being decoded correctly per ITU-T X.697 Clause 41.

## Problem Description

Per ITU-T X.697 Clause 41, OPEN TYPE values are encoded as the contained type directly,
without any CHOICE wrapper key in JSON. However, the decoder was incorrectly attempting
to parse a CHOICE wrapper, and was also consuming parent SEQUENCE delimiters, causing
"Unexpected JSON key in SEQUENCE" and "Out of defined members" errors.

## Fix Applied

Modified `skeletons/OPEN_TYPE_jer.c`:
1. Removed CHOICE key parsing logic - OPEN TYPE decoder now decodes values directly
2. Removed finalization code that consumed parent delimiters - decoder returns immediately

The decoder now matches the encoder's ITU-T X.697 compliant behavior.

## How to Test

### Automated Test (Recommended)

1. Build the project:
   ```bash
   cd <repository_root>
   make clean
   make
   ```

2. Run the test suite:
   ```bash
   make check
   ```

### Manual Test with F1AP

1. Generate F1AP code:
   ```bash
   cd tests/f1ap-regression
   ../../asn1c/asn1c -S ../../skeletons -fcompound-names -findirect-choice F1AP-16.7.0.asn
   make -f converter-example.mk
   ```

2. Create a test JER file (test.jer):
   ```json
   {
       "initiatingMessage": {
           "procedureCode": 0,
           "criticality": "reject",
           "value": {
               "protocolIEs": []
           }
       }
   }
   ```

3. Test decoding (should succeed without errors):
   ```bash
   ./converter-example -p F1AP-PDU -ijer -otext test.jer
   ```

Before the fix, this would fail with:
```
Unexpected JSON key in SEQUENCE [[
    ]
```

After the fix, it decodes successfully.

## Related Issue

This fix addresses a JER encoder/decoder inconsistency where JER encoding produced
valid ITU-T X.697 compliant JSON but the decoder could not parse it back due to
incorrect OPEN TYPE handling.
