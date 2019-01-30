#!/bin/sh
. $(dirname "$0")/init.sh

# This script tests if the compiled "hi" executable
# is actually executable.

[ -x "$HI" ] || fail "binary not executable: $HI"

assertCmd "$HI <&- 2>/dev/null" 1  # expect syntax error ("no keywords")

success
