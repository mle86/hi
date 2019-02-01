#!/bin/sh
. $(dirname "$0")/init.sh

# Tests options -i/-I.


# Case-insensitive (-i) tests:
assertCmd "$HI -i -l Gld < '$HERE/samples/default.txt'"
Gld_insens="$ASSERTCMDOUTPUT"

assertLineHighlighted "seventhline" "$Gld_insens"  # contains gld
assertLineHighlighted "eighthline" "$Gld_insens"  # contains GLD
assertLineNotHighlighted "ninthline" "$Gld_insens"


# Case-sensitive (-I) tests:
assertCmd "$HI -I -l GLD < '$HERE/samples/default.txt'"
GLD_sens="$ASSERTCMDOUTPUT"

assertLineNotHighlighted "seventhline" "$GLD_sens"  # contains gld (not GLD)
assertLineHighlighted "eighthline" "$GLD_sens"  # contains GLD
assertLineNotHighlighted "ninthline" "$GLD_sens"


# No match due to case sensitivity:
assertCmd "$HI -I -l glD < '$HERE/samples/default.txt'"
assertHasNoHighlights "$ASSERTCMDOUTPUT"


success
