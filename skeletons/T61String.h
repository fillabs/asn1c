/*-
 * Copyright (c) 2003-2017 Lev Walkin <vlm@lionet.info>. All rights reserved.
 * Redistribution and modifications are permitted subject to BSD license.
 */
#ifndef	_T61String_H_
#define	_T61String_H_

#include <OCTET_STRING.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef OCTET_STRING_t T61String_t;	/* Implemented via OCTET STRING */

extern asn_TYPE_descriptor_t asn_DEF_T61String;
extern asn_TYPE_operation_t asn_OP_T61String;

#define T61String_free OCTET_STRING_free

#if !defined(ASN_DISABLE_PRINT_SUPPORT)
#define T61String_print OCTET_STRING_print
#endif  /* !defined(ASN_DISABLE_PRINT_SUPPORT) */

#define T61String_compare OCTET_STRING_compare

#define T61String_constraint asn_generic_unknown_constraint

#if !defined(ASN_DISABLE_BER_SUPPORT)
#define T61String_decode_ber OCTET_STRING_decode_ber
#define T61String_encode_der OCTET_STRING_encode_der
#endif  /* !defined(ASN_DISABLE_BER_SUPPORT) */

#if !defined(ASN_DISABLE_XER_SUPPORT)
#define T61String_decode_xer OCTET_STRING_decode_xer_hex
#define T61String_encode_xer OCTET_STRING_encode_xer
#endif  /* !defined(ASN_DISABLE_XER_SUPPORT) */

#if !defined(ASN_DISABLE_UPER_SUPPORT)
#define T61String_decode_uper OCTET_STRING_decode_uper
#define T61String_encode_uper OCTET_STRING_encode_uper
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) */
#if !defined(ASN_DISABLE_APER_SUPPORT)
#define T61String_decode_aper OCTET_STRING_decode_aper
#define T61String_encode_aper OCTET_STRING_encode_aper
#endif  /* !defined(ASN_DISABLE_APER_SUPPORT) */

#ifdef __cplusplus
}
#endif

#endif	/* _T61String_H_ */
