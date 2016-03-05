grep -E "Size of Queue: " out.txt | grep -Eo "[0-9]+"
echo ""

grep -E "Nodes visited: " out.txt | grep -Eo "[0-9]+"
echo ""

grep -E "[0-9]+ nanoseconds" out.txt | grep -Eo "[0-9]+"
echo ""
