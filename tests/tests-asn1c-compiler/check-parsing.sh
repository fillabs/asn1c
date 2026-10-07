#!/bin/sh

# Test diff(1) capabilities
diff -a . . 2>/dev/null && diffArgs="-a"		# Assume text files
diff -u . . 2>/dev/null && diffArgs="$diffArgs -u"	# Unified diff output
diff -w . . 2>/dev/null && diffArgs="$diffArgs -w"	# Number of whitespaces not relevant

finalExitCode=0

if [ "$1" != "regenerate" ]; then
    set -e
fi

LAST_FAILED=""
print_status() {
    if [ -n "${LAST_FAILED}" ]; then
        echo "Error while processing $LAST_FAILED"
    fi
}

trap print_status EXIT

top_srcdir="${top_srcdir:-../..}"
top_builddir="${top_builddir:-../..}"

for ref in ${top_srcdir}/tests/tests-asn1c-compiler/*.asn1.+*; do
	# Figure out the initial source file used to generate this output.
	src=$(echo "$ref" | sed -e 's/\.+[-_a-zA-Z0-9=]*$//')
	# Figure out compiler flags used to create the file.
	flags=$(echo "$ref" | sed -e 's/.*\.+//' | tr _ " ")
	echo "Checking $src against $ref"
	template=.tmp.check-parsing.$$
	oldversion=${template}.old
	newversion=${template}.new
	PROCESSING="$ref (from $src)"
	# Strip the version-dependent parts before comparing:
	#  - "found in <path>" varies between build environments
	#  - "asn1c-<version> (<url>)" changes with every commit
	# Both the reference file and the fresh compiler output are normalised
	# identically, so a version-string-only difference does not cause a failure.
	LC_ALL=C sed -e 's/^found in .*/found in .../' -e 's/asn1c-[^ ]* ([^)]*)/asn1c/g' -e 's/asn1c-[^ >]*/asn1c/g' < "$ref" > "$oldversion"
	ec=0
	(${top_builddir}/asn1c/asn1c -S ${top_srcdir}/skeletons -no-gen-OER -no-gen-UPER -no-gen-APER -no-gen-JER $flags "$src" | LC_ALL=C sed -e 's/^found in .*/found in .../' -e 's/asn1c-[^ ]* ([^)]*)/asn1c/g' -e 's/asn1c-[^ >]*/asn1c/g' > "$newversion") || ec=$?
	if [ $? = 0 ]; then
		diff $diffArgs "$oldversion" "$newversion" || ec=$?
	fi
	if [ $ec != 0 ]; then
		LAST_FAILED="$ref (from $src)"
		finalExitCode=$ec
	fi
	rm -f $oldversion $newversion
	if [ "$1" = "regenerate" ]; then
		${top_builddir}/asn1c/asn1c -S ${top_srcdir}/skeletons -no-gen-OER -no-gen-UPER -no-gen-APER -no-gen-JER $flags "$src" | LC_ALL=C sed -e 's/asn1c-[^ )]* ([^)]*)/asn1c-0.9.29 (http:\/\/lionet.info\/asn1c)/g' -e 's/asn1c-[^ >]*/asn1c-0.9.29/g' > "$ref"
	fi
done

exit $finalExitCode
