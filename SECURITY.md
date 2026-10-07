# Security Policy

## Supported Versions

| Version | Supported |
| ------- | --------- |
| (master) | Yes |
| 1.4.2 and earlier | No |

Only `master` receives security fixes; tagged releases do not get patch
releases. New releases may be made out-of-schedule for some security
fixes. Build from master.

---

## Security Surfaces

### Compiler (`asn1c` binary)

The compiler is a developer tool: it runs in trusted build environments on
schemas you control. Compiler bugs are not treated as security vulnerabilities —
report them via public GitHub issues or pull requests.

### Generated code and runtime skeletons

The C code asn1c emits (`skeletons/` and type-specific generated files) runs in
production systems that parse untrusted network data (X.509 certificates, mobile
network messages, ITS vehicle communication, etc.). This is the active security
surface. Examples of in-scope vulnerabilities:

- Buffer overflow or integer overflow in a BER/DER/PER/OER/XER decoder
- Unchecked length or constraint allowing heap corruption in generated decoders
- Memory leak or use-after-free in generated encode/decode paths
- Incorrect constraint validation accepting out-of-range values
- Denial-of-service via crafted encoded messages

---

## Reporting

**Do not open a public GitHub issue for generated-code or skeleton vulnerabilities.**

Report it privately via GitHub:
https://github.com/mouse07410/asn1c/security/advisories/new
(or go to the repository's **Security** tab and click **Report a vulnerability**).

Include:

1. asn1c version or commit hash
2. Description of the vulnerability and its potential impact
3. Steps to reproduce — a minimal encoded input that triggers the issue
4. Proof-of-concept or crash output if available (stack trace, ASAN report, etc.)
5. Suggested fix or patch (optional)

---

## Response Timeline

| Milestone | Target |
| --------- | ------ |
| Acknowledgment | Within 72 hours |
| Initial assessment | Within 7 days |
| Fix or mitigation | Within 90 days |
| Public disclosure | Coordinated with reporter after fix is available |

---

## Notes for Downstream Maintainers

Fixes to the runtime skeletons are compiled into consuming applications, not
linked dynamically. A skeleton fix therefore requires downstream projects to
regenerate (or re-copy the skeletons) and rebuild — simply updating the
`asn1c` compiler binary is not sufficient. If you redistribute generated code
or vendored skeletons, track `master` and rebuild after security fixes land.
