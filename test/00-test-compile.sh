#!/bin/sh
. $(dirname "$0")/init.sh

# This script tests if hi will compile.

assertCmd -v "make -C $HERE/../"

success
