#!/bin/sh
. $(dirname "$0")/init.sh

# Tests mode -w.


assertCmd "$HI -w NX9991 wxq NX9992 gld NX9993 < '$HERE/samples/default.txt'"
wxq_gld="$ASSERTCMDOUTPUT"

assertWordHighlighted "wxq" "$wxq_gld"
assertWordHighlighted "gld" "$wxq_gld"
assertWordNotHighlighted "firstline" "$wxq_gld"  # line contains wxq
assertWordNotHighlighted "secondline" "$wxq_gld"

assertLineNotHighlighted "firstline" "$wxq_gld"  # line contains wxq
assertLineNotHighlighted "thirdline" "$wxq_gld"  # line contains wxq AND gld


# words without boundaries:
assertCmd "$HI -w line < '$HERE/samples/default.txt'"
wwb="$ASSERTCMDOUTPUT"

assertWordNotHighlighted "wxq" "$wwb"
assertWordHighlighted "line" "$wwb"
not assertContains "$wwb" "firstline" || fail "Word 'firstline' still contained, even though 'line' should have been highlighted!"


# no match:
assertCmd "$HI -w NX9991 NX9992 < '$HERE/samples/default.txt'"
assertHasNoHighlights "$ASSERTCMDOUTPUT"


success
