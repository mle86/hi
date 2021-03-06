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
	local re="/${re_highlight}\\s*[^\\s\\e]/"
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
	assertContains "$input" "$line" "Line '$line' not contained in input at all (should have been highlighted)!"
	local re="/^(?:${re_sgr0})?${re_highlight}[^\\e]*${line}/m"
	assertRegex "$input" "$re" "$errmsg"
}

# assertLineNotHighlighted LINE INPUT [ERRMSG]
#  Asserts that INPUT contains a line that is NOT completely highlighted.
#  LINE is expected to be a non-delimited regex matching the start of the line.
assertLineNotHighlighted () {
	local line="$1"
	local input="$2"
	local errmsg="${3:-"Line '$line' was highlighted but shouldn't be!"}"
	assertContains "$input" "$line" "Line '$line' not contained in input at all (should have been contained but not highlighted)!"
	local re="/^(?:${re_sgr0})?${line}/m"
	assertRegex "$input" "$re" "$errmsg"
}

# assertWordHighlighted WORD INPUT [ERRMSG]
#  Asserts that INPUT contains a word that is specifically highlighted.
#  WORD is expected to be a non-delimited regex matching the word.
assertWordHighlighted () {
	local word="$1"
	local input="$2"
	local errmsg="${3:-"Word '$word' was not highlighted!"}"
	assertContains "$input" "$word" "Word '$word' not contained in input at all (should have been highlighted)!"
	local re="/${re_highlight}${word}${re_sgr0}/"
	assertRegex "$input" "$re" "$errmsg"
}

# assertWordNotHighlighted WORD INPUT [ERRMSG]
#  Asserts that INPUT contains a word that is NOT specifically highlighted.
#  WORD is expected to be a non-delimited regex matching the word.
assertWordNotHighlighted () {
	local word="$1"
	local input="$2"
	local errmsg="${3:-"Word '$word' was highlighted but shouldn't be!"}"
	assertContains "$input" "$word" "Word '$word' not contained in input at all (should have been contained but not highlighted)!"
	# is contained but not highlighted == ! ( !contained || contained and highlighted )
	local re="!/^(?!.*${word})|^.*${re_highlight}${word}${re_sgr0}/s"
	assertRegex "$input" "$re" "$errmsg"
}

# not ASSERTION...
#  Performs an assertion but silences it completely.
#  Inverts its return status and keeps it from exiting the test script.
#  If the assertion _fails_, the command returns true;
#  if the assertion _succeeds_, the command returns false (without any visible message).
#  Combine this this " || fail ERRMSG" or there won't be any visible error output!
not () {
	! ( "$@" ) >/dev/null 2>/dev/null
}

