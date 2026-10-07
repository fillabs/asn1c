dist_check_SCRIPTS = run.sh

TESTS_ENVIRONMENT = \
	abs_top_srcdir=${abs_top_srcdir} \
	abs_top_builddir=${abs_top_builddir} \
	srcdir=${srcdir}

TESTS = $(dist_check_SCRIPTS)

EXTRA_DIST = F1AP-16.7.0.asn

CLEANFILES = *.[choa] *.d converter-example converter-example.mk Makefile.am.* asn_constant.h \
  test-jer-opentype.jer

# Remove files copied from srcdir during out-of-tree builds (distcheck)
clean-local:
	if [ "$(srcdir)" != "." ]; then \
		rm -f F1AP-16.7.0.asn; \
	fi
