#!/bin/bash

FILENAME="../data/identifiers.txt"

# Clear
> $FILENAME

# Generate
for i in {1..1000000}
do
	tr -cd '_A-Za-z0-9' < /dev/urandom | fold -w $((($RANDOM % 20) + 1)) | head -n1 >> $FILENAME
done

