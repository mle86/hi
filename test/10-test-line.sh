#!/bin/sh
. $(dirname "$0")/init.sh

# Tests mode -l.


assertCmd "$HI -l NX9991 wxq NX9992 gld NX9993 < '$HERE/samples/default.txt'"
wxq_gld="$ASSERTCMDOUTPUT"

assertLineHighlighted "firstline" "$wxq_gld"  # contains wxq
assertLineHighlighted "thirdline" "$wxq_gld"  # contains wxq AND gld
assertLineHighlighted "seventhline" "$wxq_gld"  # contains gld
assertLineNotHighlighted "secondline" "$wxq_gld"
assertLineNotHighlighted "fifthline" "$wxq_gld"


# no match:
assertCmd "$HI -l NX9991 NX9992 < '$HERE/samples/default.txt'"
assertHasNoHighlights "$ASSERTCMDOUTPUT"


success
