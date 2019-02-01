#!/bin/sh
. $(dirname "$0")/init.sh

# Tests mode -w with words containing spaces.


assertCmd "$HI -w 'tzp gld' < '$HERE/samples/default.txt'"
tzpgld="$ASSERTCMDOUTPUT"

assertWordHighlighted "tzp gld" "$tzpgld"
assertWordNotHighlighted "gld" "$tzpgld"

assertHasHighlights   "$(printf '%s' "$tzpgld" | grep -m1 "seventhline")"  # line contains "tzp gld"
assertHasNoHighlights "$(printf '%s' "$tzpgld" | grep -m1 "thirdline")"  # line contains only "gld"
assertHasNoHighlights "$(printf '%s' "$tzpgld" | grep -m1 "secondline")"


# no match:
assertCmd "$HI -w 'gld tzp' < '$HERE/samples/default.txt'"
assertHasNoHighlights "$ASSERTCMDOUTPUT"


success
