# ENCODING-CONTROL Support Status

This document summarizes the ENCODING-CONTROL and encoding instruction support
implemented by this tree.

## Supported Instructions

| Encoding | Instruction | Supported targets | Notes |
|----------|-------------|-------------------|-------|
| XER | `BASE64` | `OCTET STRING` | Bare `[BASE64]` remains a XER instruction for compatibility. |
| XER | `hexadecimal` legacy form | `OCTET STRING` | Legacy `Type OCTET STRING ::= hexadecimal` form remains accepted. |
| XER | `utf8` legacy form | `OCTET STRING` | Legacy `Type OCTET STRING ::= utf8` form remains accepted. |
| XER | `TEXT` | `BOOLEAN`, `ENUMERATED`, named-number `INTEGER`, named-bit `BIT STRING` | Value remaps use `TEXT Type.value AS "wire-name"`. |
| XER | `DECIMAL` | `REAL` | Requires `GLOBAL-DEFAULTS MODIFIED-ENCODINGS` in the XER control section. |
| XER | `GLOBAL-DEFAULTS MODIFIED-ENCODINGS` | module-level XER control | Enables modified XER encodings such as `DECIMAL`. |
| JER | `BASE64` | `OCTET STRING` | Use `[JER:BASE64]` for bracketed type prefixes. Bare `[BASE64]` is XER. |
| JER | `TEXT` | `ENUMERATED` named values | Use `TEXT EnumType.value AS "json-string"`. |
| JER | `NAME` | members of `SEQUENCE`, `SET`, and `CHOICE` | Changes JSON member keys only; C fields and XER XML tags are unchanged. |

The implementation intentionally does not add unrelated XER or JER
instructions such as XER `ATTRIBUTE`, `UNTAGGED`, `USE-NIL`, or JER `ARRAY`,
`OBJECT`, and `UNWRAPPED`.

## Accepted Syntax

Bracketed type prefixes:

```asn1
Flag  ::= [TEXT] BOOLEAN
Mode  ::= [XER:TEXT] ENUMERATED { idle(0), busy(1) }
Ratio ::= [DECIMAL] REAL
Blob  ::= [JER:BASE64] OCTET STRING
```

`ENCODING-CONTROL` sections:

```asn1
ENCODING-CONTROL XER
    GLOBAL-DEFAULTS MODIFIED-ENCODINGS
    DECIMAL Ratio
    TEXT Count.one AS "uno"
END

ENCODING-CONTROL JER
    BASE64 Blob
    TEXT Mode.busy AS "occupied"
    NAME Packet.payload AS "payload64"
END
```

Legacy XER OCTET STRING controls are still accepted:

```asn1
ENCODING-CONTROL XER
    BinaryData OCTET STRING ::= hexadecimal
    TextData   OCTET STRING ::= utf8
END
```

## Semantics And Diagnostics

The compiler applies encoding controls before C generation and rejects
incompatible or ambiguous instructions.

- XER `DECIMAL` is valid only for `REAL` and requires
  `GLOBAL-DEFAULTS MODIFIED-ENCODINGS`.
- XER `TEXT` is valid for `BOOLEAN`, `ENUMERATED`, `INTEGER` with named
  numbers, and `BIT STRING` with named bits.
- JER `BASE64` is valid only for `OCTET STRING`.
- JER `TEXT` is valid only for named values of `ENUMERATED`.
- JER `NAME` is valid only for members of `SEQUENCE`, `SET`, and `CHOICE`.
- Duplicate final JER member names in the same constructed type are rejected.
- Unknown type, member, or named-value targets are rejected.

Schema-level instructions are authoritative. For example, an XER
`hexadecimal` instruction masks a runtime `XER_F_BASE64` flag for that type,
and JER `BASE64` is pinned by generated type operations.

## Runtime Behavior

Custom operation tables are generated for instructed types. The generated
wrappers call skeleton support for shared codecs and emit type-local code when
the descriptor layout requires it.

- XER text booleans encode as `true` or `false` text, not empty XML elements.
- XER text enumerations and JER text enumerations use the instructed wire
  string for matching named values.
- XER named-number `INTEGER` and named-bit `BIT STRING` text encodings emit and
  accept the configured names.
- JER `BASE64` OCTET STRING values encode as JSON strings using Base64 and
  reject malformed Base64 during decoding.
- JER `NAME` member keys are used by `SEQUENCE`, `SET`, and `CHOICE` JER
  encoders and decoders through per-member JER constraints.

## Tests

Coverage was added in:

- `tests/tests-asn1c-compiler/210-encoding-instructions-OK.asn1`
- `tests/tests-asn1c-compiler/210-encoding-instructions-OK.asn1.+-P_-gen-JER`
- `tests/tests-asn1c-smoke/check-encoding-instructions.sh`

Run the targeted checks:

```sh
tests/tests-asn1c-smoke/check-encoding-instructions.sh
cd tests/tests-asn1c-compiler && ./check-parsing.sh
```

The smoke test generates instructed schemas, compiles the generated C, checks
exact XER/JER output strings, performs JER round trips, and verifies bad-value
and bad-applicability failures.
