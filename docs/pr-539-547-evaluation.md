# Evaluation of vlm/asn1c PRs #539-#547 

The original PRs were authored by @shakespark.

This fork was compared with the topic commits behind upstream pull requests
539 through 547. The upstream branches share an integration base, so only
each PR's topic commits were evaluated; unrelated integration-branch changes
were not imported.

| PR | Local finding | Resolution |
|---|---|---|
| #539 | Partly present. The reported 24-bit skip stride was absent because this fork already consumed one bit at a time, but unknown UPER CHOICE alternatives were still rejected. | Retained the complete-length regression and added forward-compatible CHOICE skipping. |
| #540 | Present. Unknown extensible ENUMERATED values failed decode. | Added collision-free `LONG_MAX - index` storage, bounded validation, and lossless UPER relay. |
| #541 | Present. Named extension additions could widen the PER root constraint, and NamedBitList handling was not type-specific. | Added root-only constraint construction and per-type NamedBitList trailing-zero behavior. |
| #542 | Present. A materialized extension addition equal to its DEFAULT was encoded as present. | Apply DEFAULT comparison while constructing the extension bitmap. |
| #543 | Present. SET exposed no UPER codec and several container paths could call absent codecs. | Added the canonical-tag-order SET UPER codec and null-codec guards adapted to the split skeleton layout. |
| #544 | Present. OER open-type skip returned only the length-determinant size and unknown CHOICE extensions failed. | Consume the determinant plus content, validate truncation, and skip unknown extensible CHOICE alternatives. |
| #545 | Present. Normally-small long-form encoders omitted the selector bit. | Emit the long-form selector and cap NSNNWN at the decoder's supported 65535 maximum. |
| #546 | Present. Automatic ENUMERATED numbering and a globally sorted root/extension map could swap wire ordinals. | Assign smallest-unused values and sort/search root and extension segments independently. |
| #547 | Applicable compatibility concern after #539/#540/#544. | Added `ASN_REJECT_UNKNOWN_EXTENSIONS` and a separately compiled strict-mode regression library. |

The imported regression schemas cover cross-version decoding, malformed and
truncated inputs, canonical DEFAULT behavior, SET ordering, constraint roots,
large normally-small values, and ENUMERATED numbering/layout. Test filenames
use this fork's `-gen-UPER` convention.
