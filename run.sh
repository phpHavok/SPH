#!/bin/bash
TOP_FILE="/opt/topology.lst"
BIN_FILE="./bin/sph.out"
if [ ! -f "$TOP_FILE" ]; then
	echo "No topology found"
	exit 1
fi
if [ ! -f "$BIN_FILE" ]; then
	echo "No binary found: run make"
	exit 1
fi
echo "Syncing latest binary..."
while read -r host; do
	scp "$BIN_FILE" "$host:~/."
done < /opt/topology.lst
NUM_PROC=$(((`wc -l < "$TOP_FILE"` - 1) * 4 + 1))
RANK_FILE=`mktemp`
echo 'rank 0=10.13.37.1 slot=0' > "$RANK_FILE"
RANK=1
grep -v '^10.13.37.1$' "$TOP_FILE" | while read -r host; do
	for slot in {0..3}; do
		echo "rank $RANK=$host slot=$slot" >> "$RANK_FILE"
		RANK=$((RANK + 1))
	done
done
echo "Running on $NUM_PROC processors..."
mpirun -np "$NUM_PROC" --hostfile "$TOP_FILE" --rankfile "$RANK_FILE" ~/sph.out
rm -f "$RANK_FILE"
