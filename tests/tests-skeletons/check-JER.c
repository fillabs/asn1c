#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <asn_application.h>
#include <OCTET_STRING.h>
#include <BIT_STRING.h>
#include <BOOLEAN.h>
#include <jer_decoder.h>

/* Forward declaration for OCTET_STRING JER decoders */
asn_dec_rval_t OCTET_STRING_decode_jer_utf8(
    const asn_codec_ctx_t *opt_codec_ctx,
    const asn_TYPE_descriptor_t *td,
    const asn_jer_constraints_t *constraints,
    void **structure,
    const void *ptr, size_t size
);

/*
 * Test JER (JSON Encoding Rules) decoding functionality,
 * specifically for large JSON files that exceed the default buffer size.
 * This test validates the fix for issue #248 where JER decoding fails
 * for inputs larger than 16384 bytes.
 *
 * While this test verifies that the JER decoder itself can handle large
 * JSON inputs, the actual bug was in the converter-example.c file's
 * data_decode_from_file() function which reads files in chunks.
 * The fix ensures that large JSON files are read entirely into memory
 * before parsing, preventing failures due to partial tokens at chunk boundaries.
 */

static void
test_jer_decode_boolean_literals(void) {
    static const char *valid[] = {
        "true", "false",
        "true\n", "false\n",
        " true ", "\tfalse\r\n"
    };
    static const char *invalid[] = {
        "", "t", "tr", "tru",
        "f", "fa", "fal", "fals",
        "\"true\"", "truex", "falsehood", "true false"
    };
    size_t i;

    for(i = 0; i < sizeof(valid) / sizeof(valid[0]); i++) {
        BOOLEAN_t *boolean = NULL;
        asn_dec_rval_t rval = asn_decode(NULL, ATS_JER, &asn_DEF_BOOLEAN,
                                          (void **)&boolean, valid[i],
                                          strlen(valid[i]));
        printf("BOOLEAN JER valid [%s]: code=%d, consumed=%zu\n",
               valid[i], rval.code, rval.consumed);
        assert(rval.code == RC_OK);
        assert(boolean != NULL);
        ASN_STRUCT_FREE(asn_DEF_BOOLEAN, boolean);
    }

    for(i = 0; i < sizeof(invalid) / sizeof(invalid[0]); i++) {
        BOOLEAN_t *boolean = NULL;
        asn_dec_rval_t rval = asn_decode(NULL, ATS_JER, &asn_DEF_BOOLEAN,
                                          (void **)&boolean, invalid[i],
                                          strlen(invalid[i]));
        printf("BOOLEAN JER invalid [%s]: code=%d, consumed=%zu\n",
               invalid[i], rval.code, rval.consumed);
        assert(rval.code != RC_OK);
        if(boolean) ASN_STRUCT_FREE(asn_DEF_BOOLEAN, boolean);
    }

    printf("✓ BOOLEAN JER literal validation test passed\n");
}

static void
test_jer_decode_small_json(void) {
    const char *small_json = "\"Hello, World!\"";
    OCTET_STRING_t *octet_string = NULL;
    asn_dec_rval_t rval;

    /* Test small JSON decoding using the UTF8 decoder directly */
    rval = OCTET_STRING_decode_jer_utf8(0, &asn_DEF_OCTET_STRING, NULL, 
                                        (void **)&octet_string,
                                        small_json, strlen(small_json));

    printf("Small JSON test: code=%d, consumed=%zu\n", rval.code, rval.consumed);
    assert(rval.code == RC_OK);
    assert(octet_string != NULL);
    assert(octet_string->size == 13); /* "Hello, World!" */
    assert(memcmp(octet_string->buf, "Hello, World!", 13) == 0);

    ASN_STRUCT_FREE(asn_DEF_OCTET_STRING, octet_string);
    printf("✓ Small JSON decoding test passed\n");
}

static void
test_jer_decode_large_json(void) {
    /* Create a large JSON string (> 16384 bytes to trigger the bug) */
    const size_t large_size = 20000;
    char *large_json = malloc(large_size + 10);
    assert(large_json != NULL);

    /* Create a JSON string with a large payload */
    strcpy(large_json, "\"");
    for(size_t i = 1; i < large_size - 1; i++) {
        large_json[i] = 'A' + ((i - 1) % 26); /* Cycle through alphabet, starting with A */
    }
    large_json[large_size - 1] = '"';
    large_json[large_size] = '\0';

    OCTET_STRING_t *octet_string = NULL;
    asn_dec_rval_t rval;

    /* Test large JSON decoding using the UTF8 decoder */
    rval = OCTET_STRING_decode_jer_utf8(0, &asn_DEF_OCTET_STRING, NULL,
                                        (void **)&octet_string,
                                        large_json, strlen(large_json));

    printf("Large JSON test: code=%d, consumed=%zu\n", rval.code, rval.consumed);
    assert(rval.code == RC_OK);
    assert(octet_string != NULL);
    assert(octet_string->size == large_size - 2); /* Exclude quotes */

    /* Verify first few characters are correct */
    assert(octet_string->buf[0] == 'A');
    assert(octet_string->buf[1] == 'B');
    assert(octet_string->buf[2] == 'C');

    ASN_STRUCT_FREE(asn_DEF_OCTET_STRING, octet_string);
    free(large_json);
    printf("✓ Large JSON decoding test passed\n");
}

static void
test_jer_decode_with_file(void) {
    /*
     * Test the file-based decoding by creating a temporary file.
     * The name is made unique per process so that concurrent test runs
     * (e.g. parallel `make check` and `make distcheck` CI jobs sharing
     * one host) do not race on the same /tmp file.
     */
    char temp_filename[64];
    FILE *temp_file;

    snprintf(temp_filename, sizeof(temp_filename),
             "/tmp/test_large_jer.%ld.json", (long)getpid());
    
    /* Create a large JSON file */
    temp_file = fopen(temp_filename, "w");
    assert(temp_file != NULL);
    
    fprintf(temp_file, "\"");
    for(size_t i = 0; i < 20000; i++) {
        fputc('X', temp_file);
    }
    fprintf(temp_file, "\"");
    fclose(temp_file);
    
    /* Test file-based decoding */
    temp_file = fopen(temp_filename, "r");
    assert(temp_file != NULL);
    
    /* Get file size */
    fseek(temp_file, 0, SEEK_END);
    long file_size = ftell(temp_file);
    fseek(temp_file, 0, SEEK_SET);
    
    /* Read entire file */
    char *file_content = malloc(file_size + 1);
    assert(file_content != NULL);
    size_t read_size = fread(file_content, 1, file_size, temp_file);
    assert(read_size == (size_t)file_size);
    file_content[file_size] = '\0';
    fclose(temp_file);
    
    /* Test JER decoding using the UTF8 decoder */
    OCTET_STRING_t *octet_string = NULL;
    asn_dec_rval_t rval;
    
    rval = OCTET_STRING_decode_jer_utf8(0, &asn_DEF_OCTET_STRING, NULL,
                                        (void **)&octet_string,
                                        file_content, file_size);
    
    printf("File-based test: code=%d, consumed=%zu, file_size=%ld\n", 
           rval.code, rval.consumed, file_size);
    assert(rval.code == RC_OK);
    assert(octet_string != NULL);
    assert(octet_string->size == 20000); /* Content without quotes */
    
    /* Verify content */
    for(size_t i = 0; i < 100; i++) { /* Check first 100 chars */
        assert(octet_string->buf[i] == 'X');
    }
    
    ASN_STRUCT_FREE(asn_DEF_OCTET_STRING, octet_string);
    free(file_content);
    unlink(temp_filename);
    printf("✓ File-based JSON decoding test passed\n");
}

/*
 * Test strict JER decoding of variable-length BIT STRING objects.
 *
 * Original source: regression test added for the JER duplicate-member issue.
 * Version: 2026-09-24.
 * Input parameters: none; the test supplies valid and malformed JER literals.
 * Return value: none; assertions abort the test on an unexpected result.
 * Exceptions: none; decoder failures are checked through return codes.
 * Responsible party: asn1c maintainers.
 * History: added to ensure each X.697 value/length member occurs once and
 * that unknown or malformed object members are not silently skipped.
 * Use example: run the check-JER test executable from the build tree.
 */
static void
test_jer_decode_bit_string_object(void) {
    static const char *valid[] = {
        "{\"value\":\"AB\",\"length\":8}",
        "{ \n \"value\" : \"AB\" ,\t \"length\" : 8 \n }"
    };
    static const char *invalid[] = {
        "{\"value\":\"AB\",\"length\":8,\"value\":\"CD\",\"length\":8}",
        "{\"value\":\"AB\",\"length\":8,\"length\":0}",
        "{\"value\":\"AB\",\"length\":8,\"unknown\":0}",
        "{\"value\":\"AB\",\"length\":8,}",
        "{\"value\":\"AB\",\"length\":8x}"
    };
    size_t i;

    for(i = 0; i < sizeof(valid) / sizeof(valid[0]); i++) {
        BIT_STRING_t *bits = NULL;
        asn_dec_rval_t rval = asn_decode(NULL, ATS_JER, &asn_DEF_BIT_STRING,
                                          (void **)&bits, valid[i],
                                          strlen(valid[i]));
        assert(rval.code == RC_OK);
        assert(bits != NULL);
        assert(bits->size == 1);
        assert(bits->buf[0] == 0xAB);
        assert(bits->bits_unused == 0);
        ASN_STRUCT_FREE(asn_DEF_BIT_STRING, bits);
    }

    {
        BIT_STRING_t *bits = NULL;
        const char *minified = "{\"value\":\"AB\",\"length\":8}";
        asn_dec_rval_t rval = asn_decode(NULL, ATS_JER_MINIFIED,
                                          &asn_DEF_BIT_STRING, (void **)&bits,
                                          minified, strlen(minified));
        assert(rval.code == RC_OK);
        assert(bits != NULL && bits->size == 1 && bits->buf[0] == 0xAB);
        ASN_STRUCT_FREE(asn_DEF_BIT_STRING, bits);
    }

    for(i = 0; i < sizeof(invalid) / sizeof(invalid[0]); i++) {
        BIT_STRING_t *bits = NULL;
        asn_dec_rval_t rval = asn_decode(NULL, ATS_JER, &asn_DEF_BIT_STRING,
                                          (void **)&bits, invalid[i],
                                          strlen(invalid[i]));
        assert(rval.code != RC_OK);
        if(bits) ASN_STRUCT_FREE(asn_DEF_BIT_STRING, bits);
    }

    printf("✓ BIT STRING JER object validation test passed\n");
}

int
main(void) {
    printf("Running JER (JSON Encoding Rules) decoding tests...\n");

    test_jer_decode_boolean_literals();
    test_jer_decode_small_json();
    test_jer_decode_large_json();
    test_jer_decode_with_file();
    test_jer_decode_bit_string_object();
    
    printf("✓ All JER decoding tests passed!\n");
    return 0;
}
