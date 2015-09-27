#!/bin/bash

FILENAME="../data/floats.txt"

# Clear
> $FILENAME

# Generate
for i in {1..1000000}
do
    awk -v "seed=$[(RANDOM & 32767) + 32768 * (RANDOM & 32767)]" 'BEGIN { srand(seed); printf("%.5f\n", rand() * 15.0) }' >> $FILENAME
done

