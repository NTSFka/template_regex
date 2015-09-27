#!/bin/bash

FILENAME="../data/dates.txt"

# Clear
> $FILENAME

for i in {1..250000}
do
	date -d "@$(($RANDOM * $RANDOM * 25))" +"%-m/%-d/%Y" >> $FILENAME
done

for i in {1..250000}
do
	date -d "@$(($RANDOM * $RANDOM * 25))" +"%-m/%d/%Y" >> $FILENAME
done

for i in {1..250000}
do
	date -d "@$(($RANDOM * $RANDOM * 25))" +"%m/%-d/%Y" >> $FILENAME
done

for i in {1..250000}
do
	date -d "@$(($RANDOM * $RANDOM * 25))" +"%m/%d/%Y" >> $FILENAME
done

