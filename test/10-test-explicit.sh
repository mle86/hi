#!/bin/sh
. $(dirname "$0")/init.sh

# Tests mode -L.


assertCmd "$HI  -L 1-3  -L 9  -L 2-5  < '$HERE/samples/default.txt'"

assertLineHighlighted "firstline" "$ASSERTCMDOUTPUT"  # 1-3
assertLineHighlighted "thirdline" "$ASSERTCMDOUTPUT"  # 1-3, 2-5
assertLineHighlighted "fifthline" "$ASSERTCMDOUTPUT"  # 2-5
assertLineNotHighlighted "seventhline" "$ASSERTCMDOUTPUT"
assertLineNotHighlighted "eighthline" "$ASSERTCMDOUTPUT"
assertLineHighlighted "ninthline" "$ASSERTCMDOUTPUT"  # 9
assertLineNotHighlighted "tenthline" "$ASSERTCMDOUTPUT"


# no match:
assertCmd "$HI -L 70 -L 81-83 < '$HERE/samples/default.txt'"
assertHasNoHighlights "$ASSERTCMDOUTPUT"


success
