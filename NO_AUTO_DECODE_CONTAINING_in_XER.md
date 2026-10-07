# Copilot Chat Conversation Export: Automatic decoding of containing in XER

**User:** @mouse07410
**Thread URL:** https://github.com/copilot/c/39e6a3db-89be-4675-a5c6-e85d6c0c5b38

## @mouse07410

What changes are required to decode `CONTAINING` automatically?

## Copilot

Based on my analysis of the asn1c codebase and the issue #440, I can explain what changes would be required to decode `CONTAINING` automatically.  Here's a comprehensive answer:

## Overview of the Problem

Issue #440 highlights that: 
1. **Automatic decoding of `CONTAINING` is NOT working** in version 1.3.0
2. The output format changed from **hexadecimal** (v0.9.29) to **Base64** (v1.3.0+) for XER encoding
3. The project owner (mouse07410) has stated that **automatic `CONTAINING` decoding is "very likely impossible"**

## Current State

### How CONTAINING is Currently Handled

From the code search results, I can see that: 

1. **Parser Level** (`libasn1parser/asn1p_y.y`):
   - `CONTAINING` constraints are parsed and stored with type `ACT_CT_CTNG`
   - The contained type information is stored in `containedSubtype`

2. **Constraint Processing** (`libasn1fix/asn1fix_constraint.c`):
   - The `constraint_type_resolve()` function processes CONTAINING constraints
   - It extracts the referenced type but doesn't generate decoding logic

3. **Runtime Encoding** (skeletons):
   - OCTET STRINGs with `CONTAINING` are treated as regular OCTET STRINGs
   - **No automatic nested decoding occurs**
   - XER encoding uses Base64 by default (per X.693 standard compliance)

## What Changes Are Required

To implement automatic `CONTAINING` decoding, you would need: 

### 1. **Compiler Changes** (`libasn1compiler/`)

**Extract CONTAINING Type Information:**
```c
// In asn1c_C. c or new file
typedef struct containing_info_s {
    asn1p_expr_t *contained_type;  // The type that should be decoded
    asn1p_ref_t *type_reference;    // Reference to the type
    int is_pdu;                     // Whether it's a PDU type
} containing_info_t;

// Function to extract CONTAINING info from constraints
containing_info_t *extract_containing_info(asn1p_constraint_t *ct) {
    if (ct->type == ACT_CT_CTNG && ct->value) {
        // Extract the contained type from ct->value->value. v_type
        // Return structure with type information
    }
    return NULL;
}
```

**Generate Custom Decoder Functions:**
```c
// Generate a custom decoder that: 
// 1. Decodes outer OCTET STRING
// 2. Automatically decodes the contained type
// 3. Stores both in the structure

static int
emit_containing_decoder(arg_t *arg, containing_info_t *ci) {
    OUT("static int\n");
    OUT("%s__decode_contained(", MKID(arg->expr));
    OUT("const asn_codec_ctx_t *opt_codec_ctx,\n");
    OUT("    const asn_TYPE_descriptor_t *td,\n");
    OUT("    void **sptr, const void *buf, size_t size) {\n");
    
    // Generate code to: 
    // 1. Decode outer OCTET STRING
    OUT("    asn_dec_rval_t outer_rval;\n");
    OUT("    OCTET_STRING_t *outer_str;\n");
    OUT("    outer_rval = OCTET_STRING_decode_xer(...);\n");
    
    // 2. Decode inner type from the OCTET STRING buffer
    OUT("    asn_dec_rval_t inner_rval;\n");
    OUT("    %s_t *inner_val = NULL;\n", MKID(ci->contained_type));
    OUT("    inner_rval = %s_decode_xer(opt_codec_ctx, &asn_DEF_%s,\n",
        MKID(ci->contained_type), MKID(ci->contained_type));
    OUT("        (void **)&inner_val, NULL,\n");
    OUT("        outer_str->buf, outer_str->size);\n");
    
    // 3. Store decoded value
    OUT("    // Store both outer and inner representations\n");
    
    OUT("}\n\n");
}
```

### 2. **Type Descriptor Modifications**

**Extend Structure Definitions:**
```c
// Modify generated structures to include decoded CONTAINING value
typedef struct MyType {
    OCTET_STRING_t outer_value;  // Original OCTET STRING
    RRCReconfigurationComplete_t *contained_value;  // Decoded CONTAINING type
    // ... other fields
} MyType_t;
```

### 3. **Runtime Skeleton Changes** (`skeletons/`)

**Create New Decoder Variant:**
```c
// In OCTET_STRING_xer. c or new file
asn_dec_rval_t
OCTET_STRING_decode_xer_with_containing(
    const asn_codec_ctx_t *opt_codec_ctx,
    const asn_TYPE_descriptor_t *td,
    const asn_TYPE_descriptor_t *contained_td,  // NEW: descriptor for inner type
    void **sptr,
    const char *opt_mname,
    const void *buf_ptr,
    size_t size) {
    
    // 1. Decode outer OCTET STRING
    asn_dec_rval_t outer_rval = 
        OCTET_STRING_decode_xer_auto(opt_codec_ctx, td, sptr, 
                                      opt_mname, buf_ptr, size);
    
    if (outer_rval.code != RC_OK) {
        return outer_rval;
    }
    
    // 2. Decode contained type from the buffer
    OCTET_STRING_t *outer = *(OCTET_STRING_t **)sptr;
    void *inner_ptr = NULL;
    
    asn_dec_rval_t inner_rval = 
        contained_td->op->xer_decoder(opt_codec_ctx, contained_td,
                                      &inner_ptr, NULL,
                                      outer->buf, outer->size);
    
    // 3. Store decoded inner value (requires structure modification)
    // This is where it gets complex - need to extend the structure
    
    return outer_rval;
}
```

### 4. **Constraint Propagation**

Modify constraint checking to pass CONTAINING information through the compilation pipeline: 

```c
// In asn1fix_constraint.c
int asn1f_constraint_process_containing(arg_t *arg, asn1p_constraint_t *ct) {
    if (ct->type == ACT_CT_CTNG) {
        // Mark this expression as having CONTAINING constraint
        arg->expr->has_containing = 1;
        arg->expr->contained_type = resolve_contained_type(arg, ct);
    }
}
```

## Why It's "Very Likely Impossible"

The project maintainer noted it's very difficult because:

1. **Structure Layout Complexity**: Adding the decoded value requires modifying the generated C structures, which breaks API compatibility
2. **Circular Dependencies**: The contained type might not be defined yet during code generation
3. **Multiple Encoding Rules**: Would need implementation for BER, PER, OER, XER, JER - each has different ways to handle OCTET STRING
4. **Memory Management**: Who owns the decoded inner value? How is it freed?
5. **Error Handling**: What if the inner decoding fails?  Keep partial results?

## Workaround:  ENCODING-CONTROL

The repository owner suggests using **ENCODING-CONTROL** directives (see [ENCODING_CONTROL_STATUS. md](https://github.com/mouse07410/asn1c/blob/vlm_master/ENCODING_CONTROL_STATUS.md)):

```asn1
-- Force hexadecimal encoding instead of Base64
MyType ::= OCTET STRING (CONTAINING RRCReconfigurationComplete)
    [[XER: HEXADECIMAL]]
```

This would at least give you hexadecimal output instead of Base64, though you'd still need to manually decode the CONTAINING type in your application code.

## Manual Application-Level Solution

The most practical approach is to decode CONTAINING manually in your application:

```c
// 1. Decode outer message
UL_DCCH_Message_t *msg = NULL;
rval = xer_decode(NULL, &asn_DEF_UL_DCCH_Message, (void **)&msg, buffer, size);

// 2. Extract OCTET STRING with CONTAINING
OCTET_STRING_t *scg_response = /* navigate to nr_SCG_Response field */;

// 3. Manually decode the contained type
RRCReconfigurationComplete_t *contained = NULL;
rval = xer_decode(NULL, &asn_DEF_RRCReconfigurationComplete,
                  (void **)&contained, 
                  scg_response->buf, scg_response->size);
```

---

**Note**: The search results are limited to 10 results per query. For more comprehensive results, you can search directly on GitHub: 
- [CONTAINING in libasn1compiler](https://github.com/mouse07410/asn1c/search?q=containedSubtype+path%3Alibasn1compiler%2F&type=code)
- [XER encoding in skeletons](https://github.com/mouse07410/asn1c/search?q=encode_xer+path%3Askeletons%2F&type=code)
