//Single Source Shortest Path with Dijkstra Algorithm (CLRS-Page 658)
#include<vector>
#include<iostream>
#include <fstream>
using namespace std;



using namespace std;

#define MAX	100000
#define	INF	1000000000


int dist[MAX],parent[MAX];
bool found[MAX];

//i'th index in this array, holds the index of i'th graph node index, in heap
//by i'th graph node I mean i'th index of dist. i index graph
int location[MAX];  //be zaban sade tar in araye mige har onsor dist kojaye heap hast

//for i'th  node in heap, i'th element in this array, holds the
// index in dist. opposite of location[MAX]. i index heap
int index[MAX]; //be zaban sade tar in araye mige har onsor heap kojaye graphe(dist)

//instead of using these two arrays, an alternative solution is
// to create a heap node class to hold the index and create a template class
// like this template<class T , int D> DaryMinheap{} where T is the node class mentioned
//each node class holds two values: key , index_in_graph but we will still need location[]

vector<pair<int,int> > adj[MAX];

int n,m; //n: number of vertices, m: number of edges

void swap(int *x, int *y);




class DaryMinHeap {
    //maximum capacity of the heap is equal to the constant MAX
    int *array;
    int D;      //d ary
    int size;   //size of heap


public:
    DaryMinHeap(int s, int d);
    int Parent(int i) { return (i-1)/D; }

    //k'th child of the node with the index i
    int Child(int i,int k)  { return i*D + k; }

    void DownHeapify(int i );
    void UpHeapify(int i);

    //decreases the key of i'th node in heap and updates the heap
    void DecreaseKey(int i , int key);
    int ExtractMin();

    void displayHeap();
    void initializeHeap(int* a);

    //in the D-ary heap every node has a binary heap for its children within the D-ary heap array
    //leftmost child is binary minheap's root. these two functions are used for binary heap
    int RightHeapify(int i);    //returns the new(last) index of the node that we called RightHeapify on.
    int LeftHeapify(int i);     //returns the new index of the node that we called LeftHeapify on.

    //returns the binary heap child index within the D-ary heap indices
    int binHeapFirstChild(int i);
    int binHeapSecondChild(int i);
    int binHeapParent(int i);

    //swaps two elements in Dary heap and updates location[] and index[]
    void swapInDaryheap(int i, int j);
};


DaryMinHeap::DaryMinHeap(int s , int d) {
    D=d;
    size=s;
    array = new int[MAX];
}

void DaryMinHeap::DecreaseKey(int i, int key) {

    if(key > array[i]){
        cout<<"new key is greater than current key"<<endl;
        return;
    }
    array[i] = key;         //update heap
    dist[index[i]] = key;   //update graph(dist)

    //before UpHeapify we should call LeftHeapify because the key has decreased
    //so inside Upheapify we first call LeftHeapify

    UpHeapify(i);   //location[] and index[] will be updated inside UpHeapify
}

int DaryMinHeap::ExtractMin() {
    if(size < 1){
        cout<<"heap underflow"<<endl;
        return -1;
    }
    int min_index = index[0];
    //found[v] = true outside of this function

    swapInDaryheap(0,size-1);
    size--;

    DownHeapify(0); //location[] and index[] will be updated inside DownHeapify
    return min_index;
}

void DaryMinHeap::DownHeapify(int i) {
    //right heapify
    int newIndex = RightHeapify(i);

    int child = Child(newIndex,1);      //first child is the minimum
    if(child < size and array[child] < array[newIndex]){
        swapInDaryheap(newIndex,child);
        DownHeapify(child);
    }

    //not so efficient solution
    /*for(int k=1; k<=D; k++){
        int child = Child(i,k);
        if( child < size and array[child] < array[i] ){
            if(array[child] < array[smallest]){
                smallest = child;
            }
        }
    }
    if(smallest != i){
        swapInDaryheap(i,smallest);
        DownHeapify(smallest);
    }*/
}

void DaryMinHeap::UpHeapify(int i) {
    //left heapify
    int newIndex=LeftHeapify(i);

    int father = Parent(newIndex);

    if(newIndex>0 and array[father] > array[newIndex]){
        swapInDaryheap(newIndex,father);
        newIndex= father;
        UpHeapify(newIndex);
    }
}

void DaryMinHeap::displayHeap() {
    for(int i=0; i<size; i++){
        cout<<array[i]<<", ";
    }
}

void DaryMinHeap::initializeHeap(int *a) {
    for(int i=0; i<size ; i++){
        array[i] = a[i];
    }

}

void DaryMinHeap::swapInDaryheap(int i, int j) {
    swap(&location[index[i]], &location[index[j]]); //update location[]
    swap( &index[i], &index[j]);                    //update index[]
    swap( &array[i], &array[j]);              //update heap

}

int DaryMinHeap::LeftHeapify(int i) {
    int newIndex = i;
    int leftFather = binHeapParent(newIndex);

    while(i%D > 1 and array[leftFather] > array[newIndex] ){
        swapInDaryheap(newIndex,leftFather);
        newIndex = leftFather;
    }
    return newIndex;
}

int DaryMinHeap::binHeapParent(int i) {
    int r = i%D;
    if(r == 0){
        r=D;
    }
    int a = i-r;
    r = r/2;
    r = a +r;
    return r;

}

int DaryMinHeap::binHeapFirstChild(int i) {
    int r=i%D;
    if(r > D/2){
        cout<<"wrong index for binary heap child"<<endl;
        //return -1;
    }

    int a= i-r;
    r*=2;
    r = r + a;
    return r;
}

int DaryMinHeap::binHeapSecondChild(int i) {
    int r=i%D;
    if(r > D/2){
        cout<<"wrong index for binary heap child"<<endl;
        //return  -1;
    }
    int a= i-r;
    r*=2;
    r = r + a +1;
    return r;
}

int DaryMinHeap::RightHeapify(int i) {
    int newIndex = i;
    if(newIndex%D <= D/2 and newIndex< size ){

        int l = binHeapFirstChild(newIndex);
        int r = binHeapSecondChild(newIndex);
        int smallest = newIndex;

        if( l < size and array[l]< array[newIndex]){
            smallest = l;
        }
        if( r < size and array[r] < array[smallest]){
            smallest = r;
        }
        if(smallest != newIndex){
            swapInDaryheap(smallest,newIndex);
            return RightHeapify(smallest);
        }
        return newIndex;
    }

    return newIndex;

}


void swap(int *x, int *y){
    int temp = *x;
    *x = *y;
    *y = temp;
}

void dijkstra(int s) {

    for (int i=0;i<n;i++) {
        dist[i] = INF;
        parent[i] = -1;
        found[i] = false;
        //initialize location[] in heap;
        // initially heap and dist are the same
        location[i]=i;
        index[i]=i;
    }
    dist[s] = 0;

    //create DaryMinheap here
    int d = m/n;
    DaryMinHeap daryMinHeap(n, d);
    daryMinHeap.initializeHeap(dist);


    for (int i = 0; i < n; i++) {
        //daryMinHeap.displayHeap();
        //cout<<endl;


        int v = daryMinHeap.ExtractMin();
        if (dist[v] == INF)
            break;

        //raas v ro relax mikonim
        found[v] = true;
        for (auto edge : adj[v]) {
            int to = edge.first;
            int len = edge.second;

            //update heap   //decreasekey(to)
            if (dist[v] + len < dist[to]) {
                dist[to] = dist[v] + len;
                parent[to] = v;
                daryMinHeap.DecreaseKey(location[to], dist[to]);
            }
        }
    }
}

int main(){
    /////////////////
    //Sample Input (CLRS-Page 659):
/*5 10 0
0 1 10
0 4 5
1 2 1
1 4 2
2 3 4
3 0 7
3 2 6
4 1 3
4 2 9
4 3 2*/
    /////////////////
    ofstream myfile;
    myfile.open ("example.txt");



    int source;
    cin >> n >> m >> source;

    int x,y,w;
    for (int i=0;i<m;i++){
        cin >> x >> y >> w; // edge x->y with length w
        adj[x].push_back({y,w});
    }

    dijkstra(source);
    for (int i=0;i<n;i++){
        cout << i << "\t" << ((dist[i]==INF)?-1:dist[i]) << endl;
        myfile <<  i << "\t" << ((dist[i]==INF)?-1:dist[i]) << endl;
    }
    /////////////////
    //Sample Output:
//0       0
//1       8
//2       9
//3       7
//4       5
    /////////////////
    myfile.close();
    return 0;
}
