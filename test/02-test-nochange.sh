#!/bin/sh
. $(dirname "$0")/init.sh

# This script ensures that the program won't change its input without a reason.

originalInput="$(cat -- "$HERE/samples/default.txt")"
unknownKeyword='NX99999'  # this word in not contained in the input, so the program shouldn't change anything
assertCmd "$HI -l '$unknownKeyword' < '$HERE/samples/default.txt'"
assertEq "$ASSERTCMDOUTPUT" "$originalInput"

assertCmd "$HI -l '$unknownKeyword' <&-"
assertEmpty "$ASSERTCMDOUTPUT"

success
