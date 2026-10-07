/*
 * Regression test for CBOR encoding of SEQUENCE containing OBJECT IDENTIFIER.
 *
 * Verifies that XER->CBOR conversion succeeds for a Certificate SEQUENCE that
 * contains OBJECT IDENTIFIER members (previously failing at
 * constr_SEQUENCE_cbor.c with "Failed to encode element SignatureIdentifier").
 */
#undef NDEBUG
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include <Certificate.h>
#include <cbor_encoder.h>
#include <cbor_decoder.h>

#ifndef SRCDIR
#define SRCDIR_S ".."
#else
#define STRINGIFY_MACRO2(x) #x
#define STRINGIFY_MACRO(x)  STRINGIFY_MACRO2(x)
#define SRCDIR_S STRINGIFY_MACRO(SRCDIR)
#endif

/* Accumulator for CBOR output */
struct buf_acc {
    uint8_t *data;
    size_t   len;
    size_t   cap;
};

static int
buf_writer(const void *bytes, size_t size, void *key) {
    struct buf_acc *acc = (struct buf_acc *)key;
    if(acc->len + size > acc->cap) {
        size_t nc = acc->cap ? acc->cap * 2 : 256;
        while(nc < acc->len + size) nc *= 2;
        uint8_t *p = (uint8_t *)realloc(acc->data, nc);
        if(!p) return -1;
        acc->data = p;
        acc->cap  = nc;
    }
    memcpy(acc->data + acc->len, bytes, size);
    acc->len += size;
    return 0;
}

/*
 * Read a file into a malloc'd buffer. Caller must free().
 * Returns NULL on error.
 */
static char *
read_file(const char *path, size_t *out_size) {
    FILE *f = fopen(path, "rb");
    if(!f) {
        fprintf(stderr, "Cannot open %s: %s\n", path, strerror(errno));
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    rewind(f);
    char *buf = (char *)malloc((size_t)sz + 1);
    if(!buf) { fclose(f); return NULL; }
    if(fread(buf, 1, (size_t)sz, f) != (size_t)sz) {
        free(buf); fclose(f); return NULL;
    }
    buf[sz] = '\0';
    fclose(f);
    if(out_size) *out_size = (size_t)sz;
    return buf;
}

int
main(int ac, char **av) {
    const char *xer_path = SRCDIR_S "/data-202/s1.xer";
    size_t xer_size = 0;
    char *xer_buf;
    Certificate_t *cert = NULL;
    asn_dec_rval_t drval;
    asn_enc_rval_t erval;
    struct buf_acc cbor = {NULL, 0, 0};

    (void)ac;
    (void)av;

    /* Read XER input */
    xer_buf = read_file(xer_path, &xer_size);
    if(!xer_buf) {
        fprintf(stderr, "FAIL: cannot read %s\n", xer_path);
        return 1;
    }

    /* XER-decode the Certificate */
    drval = xer_decode(NULL, &asn_DEF_Certificate, (void **)&cert,
                       xer_buf, xer_size);
    free(xer_buf);
    if(drval.code != RC_OK || !cert) {
        fprintf(stderr, "FAIL: XER decode failed (code=%d, consumed=%zu)\n",
                drval.code, drval.consumed);
        return 1;
    }
    printf("XER decode OK (consumed %zu bytes)\n", drval.consumed);

    /* CBOR-encode the Certificate */
    erval = cbor_encode(&asn_DEF_Certificate, cert, buf_writer, &cbor);
    if(erval.encoded < 0) {
        fprintf(stderr,
                "FAIL: CBOR encode failed for type '%s' "
                "(constr_SEQUENCE_cbor issue)\n",
                erval.failed_type ? erval.failed_type->name : "<unknown>");
        ASN_STRUCT_FREE(asn_DEF_Certificate, cert);
        free(cbor.data);
        return 1;
    }
    printf("CBOR encode OK (%zd bytes)\n", erval.encoded);
    assert(cbor.len > 0);
    assert((size_t)erval.encoded == cbor.len);

    /* CBOR-decode back and verify round-trip */
    {
        Certificate_t *cert2 = NULL;
        asn_dec_rval_t dr2;
        dr2 = cbor_decode(NULL, &asn_DEF_Certificate, (void **)&cert2,
                          cbor.data, cbor.len);
        if(dr2.code != RC_OK || !cert2) {
            fprintf(stderr, "FAIL: CBOR round-trip decode failed (code=%d)\n",
                    dr2.code);
            ASN_STRUCT_FREE(asn_DEF_Certificate, cert);
            free(cbor.data);
            return 1;
        }
        printf("CBOR round-trip decode OK\n");
        ASN_STRUCT_FREE(asn_DEF_Certificate, cert2);
    }

    ASN_STRUCT_FREE(asn_DEF_Certificate, cert);
    free(cbor.data);

    printf("All tests passed!\n");
    return 0;
}
