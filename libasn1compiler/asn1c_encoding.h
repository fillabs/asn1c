/*
 * Encoding control processing for asn1c compiler
 * Handles ENCODING-CONTROL directives from ASN.1 modules
 */
#ifndef ASN1C_ENCODING_H
#define ASN1C_ENCODING_H

struct asn1p_s;
struct asn1p_module_s;

/*
 * Apply encoding controls from ENCODING-CONTROL sections to type definitions.
 * This runs during the compilation phase after all types are resolved.
 * 
 * Returns: Number of encoding controls applied, or -1 on error.
 *
 * NOTE: Currently a stub implementation. Full implementation requires:
 * - Phase 4: Parse ENCODING-CONTROL body to extract individual directives
 * - Phase 5: Match directives to type definitions by name
 * - Phase 6: Generate custom encoders based on encoding type
 */
int asn1c_apply_encoding_controls(struct asn1p_s *asn, struct asn1p_module_s *mod);

/*
 * Flag for code generator to emit custom XER encoder
 * Can be added to expr->_type_flags when encoding controls are present
 */
#define TM_CUSTOM_XER_ENCODER  (1<<6)

#endif /* ASN1C_ENCODING_H */
