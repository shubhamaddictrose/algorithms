
// USE THIS WHEN RANGE UPDATE (DONE BY EXTERNAL VALUE) AND RANGE QUERY IS TO BE DONE...............

#include<bits/stdc++.h>
using namespace std;
#define gc getchar_unlocked
#define rep(i,j,k) for(i=j; i<k; i++)
#define rrep(i,j,k) for(i=j; i>=k; i--)
#define tr(c,it) for(auto it = (c).begin(); it != (c).end(); it++)
#define ZERO(a) memset(a,0,sizeof(a))
#define lowbit(x) (x&(-x))
#define sz(a) int((a).size()) 
#define pb push_back 
#define all(c) (c).begin(),(c).end()
#define fin scan_int()
#define fout(x) fast_out(x)
#define sc(x) scanf("%d",&x)
#define sc2(x,y) scanf("%d%d",&x,&y)
#define sc3(x,y,z) scanf("%d%d%d",&x,&y,&z)
#define scl(x) scanf("%lld",&x)
#define scl2(x,y) scanf("%lld%lld",&x,&y)
#define scl3(x,y,z) scanf("%lld%lld%lld",&x,&y,&z)
#define scstr(x) scanf("%s",x)
#define pd(x) printf("%d",x)
#define pd2(x,y) printf("%d %d",x,y)
#define pll(x) printf("%lld",x)
#define pll2(x,y) printf("%lld %lld",x,y)
#define pstr(x) printf("%s",x)
#define newl printf("\n")
#define xx first
#define yy second
#define FAST ios_base::sync_with_stdio(false);cin.tie(NULL);
#define compress(s) sort(all(s)); s.resize(unique(s.begin(),s.end())-s.begin());
#define strtoint(s,n) if (!(istringstream(s) >> n) ) n = 0;
#define mod 1000000007
typedef long long ll;
typedef vector<int> vi;
typedef vector<string> vs;
typedef vector<vi> vvi;
typedef pair<string,int> psi; 
typedef pair<int,int> pii;
inline int scan_int() {
   int NR=0,sign=1;    char c=gc();
    while( c < 48 || c > 57 ){if(c=='-')sign=0; c=gc();}
    while(c>47 && c< 58)
    {    NR = (NR << 3) + (NR << 1) + (c - 48);    c=gc();    }
    return (sign?NR:(-NR));}
inline void fast_out(ll x){
         register char buffor[35]; register ll i=0;
         do{buffor[i++]=(x%10)+'0';
               x/=10;
            } while(x); i--;
            while(i>=0) putchar(buffor[i--]);
            putchar('\n');
       }

struct SegmentTreeNode {
	int start, end; // this node is responsible for the segment [start...end]
	ll total, pendingUpdate;
	
	SegmentTreeNode() : total(0), pendingUpdate(0) {}
	
	void assignLeaf(ll value) {
		total = value;
	}
	
	void merge(SegmentTreeNode& left, SegmentTreeNode& right) {
		total = left.total + right.total;
		if (left.pendingUpdate > 0)
			total += left.pendingUpdate * (left.end - left.start + 1);
		if (right.pendingUpdate > 0)
			total += right.pendingUpdate * (right.end - right.start + 1);
	}
	
	ll query() {
		return total;
	}
	
	bool hasPendingUpdate() {
		return pendingUpdate != 0;
	}
	
	void applyPendingUpdate() {
		total += (end - start + 1) * pendingUpdate;
		pendingUpdate = 0;
	}
	
	void addUpdate(ll value) {
		pendingUpdate += value;
	}
	
	ll getPendingUpdate() {
		return pendingUpdate;
	}
};

template<class InputType, class UpdateType, class OutputType>
class SegmentTree {
	SegmentTreeNode* nodes;
	int N;
	
public:
	SegmentTree(InputType arr[], int N) {
		this->N = N;
		nodes = new SegmentTreeNode[getSegmentTreeSize(N)];
		buildTree(arr, 1, 0, N-1);
	}
	
	~SegmentTree() {
		delete[] nodes;
	}
	
	// get the value associated with the segment [start...end]
	OutputType query(int start, int end) {
		SegmentTreeNode result = query(1, start, end);
		return result.query();
	}
	
	// range update: update the range [start...end] by value
	// Exactly what is meant by an update is determined by the
	// problem statement and that logic is captured in segment tree node
	void update(int start, int end, UpdateType value) {
		update(1, start, end, value);
	}
	
private:
	void buildTree(InputType arr[], int stIndex, int start, int end) {
		// nodes[stIndex] is responsible for the segment [start...end]
		nodes[stIndex].start = start, nodes[stIndex].end = end;
		
		if (start == end) {
			// a leaf node is responsible for a segment containing only 1 element
			nodes[stIndex].assignLeaf(arr[start]);
			return;
		}
		
		int mid = (start + end) / 2,
			leftChildIndex = 2 * stIndex,
			rightChildIndex = leftChildIndex + 1;
			
		buildTree(arr, leftChildIndex, start, mid);
		buildTree(arr, rightChildIndex, mid + 1, end);
		nodes[stIndex].merge(nodes[leftChildIndex], nodes[rightChildIndex]);
	}
	
	int getSegmentTreeSize(int N) {
		int size = 1;
		for (; size < N; size <<= 1);
		return size << 1;
	}
	
	SegmentTreeNode query(int stIndex, int start, int end) {
		if (nodes[stIndex].start == start && nodes[stIndex].end == end) {
			SegmentTreeNode result = nodes[stIndex];
			if (result.hasPendingUpdate())
				result.applyPendingUpdate();
			return result;
		}
		
		int mid = (nodes[stIndex].start + nodes[stIndex].end) >> 1,
			leftChildIndex = stIndex << 1,
			rightChildIndex = leftChildIndex + 1;			
		SegmentTreeNode result;
		
		if (start > mid)
			result = query(rightChildIndex, start, end);
		else if (end <= mid)
			result = query(leftChildIndex, start, end);
		else {
			SegmentTreeNode leftResult = query(leftChildIndex, start, mid),
							rightResult = query(rightChildIndex, mid+1, end);
			result.start = leftResult.start;
			result.end = rightResult.end;
			result.merge(leftResult, rightResult);
		}
		
		if (nodes[stIndex].hasPendingUpdate()) {
			result.addUpdate(nodes[stIndex].getPendingUpdate());
			result.applyPendingUpdate();
		}		
		return result;
	}
	
	void update(int stIndex, int start, int end, UpdateType value) {
		if (nodes[stIndex].start == start && nodes[stIndex].end == end) {
			nodes[stIndex].addUpdate(value);
			return;
		}
		
		int mid = (nodes[stIndex].start + nodes[stIndex].end) >> 1,
			leftChildIndex = stIndex << 1,
			rightChildIndex = leftChildIndex + 1;
			
		if (start > mid)
			update(rightChildIndex, start, end, value);
		else if (end <= mid)
			update(leftChildIndex, start, end, value);
		else {
			update(leftChildIndex, start, mid, value);
			update(rightChildIndex, mid+1, end, value);
		}
		nodes[stIndex].merge(nodes[leftChildIndex], nodes[rightChildIndex]);
	}
};

int A[100005];

int main() {
	int T, N, C, i, p, q, v;
	
	scanf("%d", &T);
	while (T--) {
		scanf("%d %d", &N, &C);
		SegmentTree<int,ll,ll> st(A, N);

		while (C--) {
			scanf("%d", &i);			
			if (i == 0) {
				scanf("%d %d %d", &p, &q, &v);
				st.update(p-1, q-1, v);
			} else {
				scanf("%d %d", &p, &q);
				printf("%lld\n", st.query(p-1, q-1));
			}
		}
	}	
	
	return 0;
}