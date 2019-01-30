#!/bin/sh

HI="$HERE/../hi"


re_highlight="\\e\\[(?:0;|1;)?(?:1|3\\d|38;5;\\d+|38;2;\\d+;\\d+;\\d+)m"
#re_highlight="\\e\\[1;33m"
re_sgr0="\\e\\[0m"


# assertHasHighlights INPUT [ERRMSG]
#  Asserts that the INPUT string contains at least one non-empty highlight.
assertHasHighlights () {
	local input="$1"
	local errmsg="${2:-"Input did not contain at least one visible highlight!"}"
	local re="/${re_highlight}[\\s*][^\\s\\e]/"
	assertRegex "$input" "$re" "$errmsg"
}

# assertHasNoHighlights INPUT [ERRMSG]
#  Asserts that the INPUT string contains zero visible highlight.
assertHasNoHighlights () {
	local input="$1"
	local errmsg="${2:-"Input did contain highlights but shouldn't have!"}"
	local re="!/${re_highlight}/"
	assertRegex "$input" "$re" "$errmsg"
}

# assertLineHighlighted LINE INPUT [ERRMSG]
#  Asserts that INPUT contains a line that is completely highlighted.
#  LINE is expected to be a non-delimited regex matching the start of the line.
assertLineHighlighted () {
	local line="$1"
	local input="$2"
	local errmsg="${3:-"Line '$line' was not highlighted!"}"
	local re="/^${re_highlight}[^\\e]*${line}/m"
	assertRegex "$input" "$re" "$errmsg"
}

# assertLineNotHighlighted LINE INPUT [ERRMSG]
#  Asserts that INPUT contains a line that is NOT completely highlighted.
#  LINE is expected to be a non-delimited regex matching the start of the line.
assertLineNotHighlighted () {
	local line="$1"
	local input="$2"
	local errmsg="${3:-"Line '$line' was highlighted but shouldn't be!"}"
	local re="/(?:^${re_sgr0}|\\A)[^\\e]*${line}/m"
	assertRegex "$input" "$re" "$errmsg"
}

