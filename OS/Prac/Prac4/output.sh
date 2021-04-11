touch temp.txt
echo "" > temp.txt
g++ fcfs.cpp -lpthread
./a.out >> temp.txt
echo "">> temp.txt
echo "">> temp.txt
echo "">> temp.txt
echo "FCFS done"

g++ priority.cpp -lpthread
./a.out >> temp.txt
echo "">> temp.txt
echo "">> temp.txt
echo "">> temp.txt
echo "priority done"

g++ priority_pre.cpp -lpthread
./a.out >> temp.txt
echo "">> temp.txt
echo "">> temp.txt
echo "">> temp.txt
echo "priority_pre done"

g++ sjf.cpp -lpthread
./a.out >> temp.txt
echo "">> temp.txt
echo "">> temp.txt
echo "">> temp.txt
echo "sjf done"

g++ srtf.cpp -lpthread
./a.out >> temp.txt
echo "">> temp.txt
echo "">> temp.txt
echo "">> temp.txt
echo "srtf done"

g++ rr.cpp -lpthread
./a.out >> temp.txt
echo "">> temp.txt
echo "">> temp.txt
echo "">> temp.txt
echo "rr done"