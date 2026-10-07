dist_check_SCRIPTS = run.sh

TESTS_ENVIRONMENT = \
	abs_top_srcdir=${abs_top_srcdir} \
	abs_top_builddir=${abs_top_builddir} \
	srcdir=${srcdir}

TESTS = $(dist_check_SCRIPTS)

EXTRA_DIST = ieee1609-contributed-extension.asn1

CLEANFILES = *.[choa] *.d converter-example converter-example.mk Makefile.am.* asn_constant.h

# Remove files copied from srcdir during out-of-tree builds (distcheck)
clean-local:
	if [ "$(srcdir)" != "." ]; then \
		rm -f ieee1609-contributed-extension.asn1; \
	fi
