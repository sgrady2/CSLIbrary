#include <iostream.h>
#include <cstdlib.h>
#include <vector.h>
#include <iterator.h>

using namespace std;

class Heap
{

	private:
	    vector <int> heap;
	    int left(int parent);
	    int right(int parent);
	    int parent(int child);
	    void heapifyup(int index);
	    void heapifydown(int index);
	public:
	    Heap()
	      {}
	    void Insert(int element);
	    void DeleteMin();
	    int ExtractMin();
	    void DisplayHeap();
	    int Size();




};

int Heap::Size()
{
return heap.size();


}

void Heap::Insert()
{
heap.push_back(element);
heapifyup(heap.size()-1);
}

void Heap::DeleteMin()
{
    if (heap.size()==0)
	{
	    return;
	}
}
