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
NUM_PROC=$((`wc -l < "$TOP_FILE"` * 4))
echo "Running on $NUM_PROC processors..."
mpirun -np "$NUM_PROC" --hostfile "$TOP_FILE" -x LD_LIBRARY_PATH ~/sph.out
