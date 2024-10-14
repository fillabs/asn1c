#include <stdio.h>
#include <assert.h>

#include <OCTET_STRING.c>
#include <BIT_STRING.c>
#include <INTEGER.c>
#include <BOOLEAN.c>
#include <NULL.c>
#include <REAL.c>

#define check(td, a, b) test(__LINE__, td, a, b)

static int 
copy(asn_TYPE_descriptor_t *td, void **a, const void *b) {
    return asn_copy(td, a, b) != 0;
}

static int
compare(asn_TYPE_descriptor_t *td, void *a, void *b) {
    return td->op->compare_struct(td, a, b) != 0;
}

static int
test(int lineno, asn_TYPE_descriptor_t *td, void **a, void *b) {
    int rv; 

    rv = copy(td, a, b);
    if(rv) {
        fprintf(stderr, "%03d: copy() failed\n", lineno);
        assert(rv == 0);
    }
    rv = compare(td, *a, b);
    if(rv) {
        fprintf(stderr, "%03d: compare() failed\n", lineno);
        assert(rv == 0);
    }

    ASN_STRUCT_FREE(*td, *a);
    ASN_STRUCT_FREE_CONTENTS_ONLY(*td, b);

    return 0;
}

int
main(int ac, char **av) {
    (void)ac;
	(void)av;

    /* OCTET STRING */
    {
        OCTET_STRING_t b = { 0 };
        OCTET_STRING_fromBuf(&b, "Hello", 5);
        OCTET_STRING_t* a = NULL;
        check(&asn_DEF_OCTET_STRING, (void**)&a, &b);
    }

    /* INTEGER */
    {
        INTEGER_t b = { 0 };
        asn_ulong2INTEGER(&b, 123);
        INTEGER_t* a = NULL;
        check(&asn_DEF_INTEGER, (void**)&a, &b);
    }
    {
        INTEGER_t b = { 0 };
        INTEGER_t* a = NULL;
        check(&asn_DEF_INTEGER, (void**)&a, &b);
    }

    /* BIT STRING */
    {
        BIT_STRING_t b = { 0 };
        b.buf = MALLOC(2);
        b.size = 2;
        b.buf[0] = 0x80;
        b.buf[1] = 0x02;
        b.bits_unused = 1;
        BIT_STRING_t* a = NULL;
        check(&asn_DEF_BIT_STRING, (void**)&a, &b);
    }

    /* BOOLEAN */
    {
        BOOLEAN_t b = { 0 };
        b = 1;
        BIT_STRING_t* a = NULL;
        check(&asn_DEF_BOOLEAN, (void**)&a, &b);
    }

    /* NULL */
    {
        NULL_t b = { 0 };
        NULL_t* a = NULL;
        check(&asn_DEF_NULL, (void**)&a, &b);
    }

    /* REAL */
    {
        REAL_t b = { 0 };
        asn_double2REAL(&b, 123.456);
        REAL_t* a = NULL;
        check(&asn_DEF_REAL, (void**)&a, &b);
    }

	return 0;
}

