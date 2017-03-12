// iostream is too mainstream
#include <cstdio>
// bitch please
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <queue>
#include <stack>
#include <algorithm>
#include <cmath>
#include <iomanip>
#define dibs reserve
#define OVER9000 1234567890123456780LL
#define patkan 9
#define tisic 47
#define soclose 1e-7
#define pi 3.1415926535898
#define ALL_THE(CAKE,LIE) for(auto LIE =CAKE.begin(); LIE != CAKE.end(); LIE++)
#define chocolate win
#define ff first
#define ss second
#define abs(x) ((x < 0)?-(x):(x))
#define uint unsigned int
#include <time.h>
// mylittledoge
// wow
using namespace std;
 
struct node {
	int son[2];
	int z,k,valm;
	};
 
struct intervalac {
	vector<node> T;
	vector< set<int> > nval;
 
	void constI(int akt) {
		node n =T[akt];
		if(n.z == n.k-1) return;
		for(int i =0; i < 2; i++) {
			if(i == 0) n.k =(n.z+n.k)/2;
			else {n.z =n.k; n.k =T[akt].k;}
			T[akt].son[i] =T.size();
			T.push_back(n);
			nval.push_back(nval[0]);
			constI(T[akt].son[i]);}
		}
 
	intervalac(int N) {
		node n;
		n.son[0] =n.son[1] =-1;
		n.z =0;
		n.k =N;
		nval.resize(1);
		nval[0].insert(10000000);
		n.valm =10000000;
		T.push_back(n);
		constI(0);}
 
	void put(int akt, int pos, int val) {
		node n =T[akt];
		if(n.z == n.k-1) {
			nval[akt].insert(val);
			T[akt].valm =*nval[akt].begin();
			return;}
		if((n.z+n.k)/2 <= pos) put(n.son[1],pos,val);
		else put(n.son[0],pos,val);
		T[akt].valm =min(T[n.son[0]].valm,T[n.son[1]].valm);}
 
	void rem(int akt, int pos, int val) {
		node n =T[akt];
		if(n.z == n.k-1) {
			nval[akt].erase(val);
			T[akt].valm =*nval[akt].begin();
			return;}
		if((n.z+n.k)/2 <= pos) rem(n.son[1],pos,val);
		else rem(n.son[0],pos,val);
		T[akt].valm =min(T[n.son[0]].valm,T[n.son[1]].valm);}
 
	int get(int akt, int zac, int kon) {
		node n =T[akt];
		if(n.z >= kon || zac >= n.k) return 10000000;
		if(n.z == zac && kon == n.k) return n.valm;
		return min(get(n.son[0],zac,min(kon,(n.z+n.k)/2)),get(n.son[1],max(zac,(n.z+n.k)/2),kon));}
	};
 
void DFS(int R, vector< vector<int> > &son, vector<int> &ans, vector<int> &p, intervalac &I) {
	I.put(0,p[R],R);
	if(R > 0) ans[R-1] =I.get(0,R,I.T[0].k);
	ALL_THE(son[R],it) DFS(*it,son,ans,p,I);
	I.rem(0,p[R],R);
	}
 
int main() {
	freopen("cover.in","r",stdin);
	freopen("cover.out","w",stdout);
	cin.sync_with_stdio(0);
	cin.tie(0);
	string W;
	cin >> W;
	int N =W.length();
 
	vector< vector<int> > V1(256);
	V1[0].push_back(N);
	for(int i =0; i < N; i++) V1[W[i]].push_back(i);
	int K =5;
	while(1<<(K-1) < N) K++;
	vector<int> isB(N+1+(1<<(K-1)),0);
	vector<int> SA(N+1,N); // order of suffixes
	vector<int> Bs(N+2,N+1); // bucket start
	Bs[0] =0;
	int Btot =-1, q =0; // buckets total
	for(int j =0; j < 256; j++) {
		if(!V1[j].empty()) Bs[++Btot] =q;
		for(uint i =0; i < V1[j].size(); i++) {
			SA[q] =V1[j][i];
			isB[V1[j][i]] =Btot;
			q++;}
		}
 
	vector< pair<int,int> > tmp(N+1);
	vector<int> Bs1(N+1,0); // bucket size (tmp)
	vector<int> nxtB(N+1,0), pos(N+1,0);
	for(int i =1; i < K; i++) {
		int q =1<<(i-1);
		for(int j =0; j <= Btot; j++) Bs1[j] =0; 									// t=1
		for(int j =0; j <= N; j++) nxtB[j] =isB[SA[j]+q];							// t=12
		for(int j =0; j <= Btot; j++) if(Bs[j+1]-Bs[j] > 1) for(int k =Bs[j]; k < Bs[j+1]; k++) 
			Bs1[nxtB[k]]++;									// t=9
		// transform Bs1 to bucket start (tmp)
		int s =0;
		for(int j =0; j <= Btot; j++) {												// t=1
			int s2 =Bs1[j]+s;
			Bs1[j] =s;
			s =s2;}
		for(int j =0; j <= Btot; j++) if(Bs[j+1]-Bs[j] > 1) for(int k =Bs[j]; k < Bs[j+1]; k++) 			// t=22
			pos[k] =Bs1[nxtB[k]]++;
		for(int j =0; j <= Btot; j++) if(Bs[j+1]-Bs[j] > 1) for(int k =Bs[j]; k < Bs[j+1]; k++) {			// t=14
			tmp[pos[k]].ff =SA[k];
			tmp[pos[k]].ss =j;}
		for(int j =0; j < s; j++) pos[j] =Bs[tmp[j].ss]++;							// t=14
		for(int j =0; j < s; j++) SA[pos[j]] =tmp[j].ff;							// t=7
		for(int j =0; j <= N; j++) nxtB[j] =isB[SA[j]+q];							// t=11
		// split buckets
		Btot =0;
		Bs[0] =0;
		for(int j =1; j <= N; j++) {
			if(isB[SA[j]] != isB[SA[j-1]] || nxtB[j] != nxtB[j-1]) pos[j] =++Btot;	// t=14
			else pos[j] =pos[j-1];
		}
//		clock_t ta =clock();
		int p =0;
		for(int j =1; j <= N; j++) {												// t=19
			if(p+1 == pos[j]) Bs[++p] =j;
			isB[SA[j]] =p;}
//		clock_t tb =clock();
//		cout << i << " " << (int)(tb-ta) << endl;
		}
 
	vector<int> LCP(N+1,0);
	int L =0;
	for(int i =0; i < N; i++) {
		int k =SA[isB[i]-1];
		while(i+L < N && k+L < N && W[i+L] == W[k+L]) L++;
		LCP[isB[i]] =L;
		if(L > 0) L--;}
 
	vector<int> p(N+1,0);
	int a =0;
	while(SA[a] != 0) a++;
	int b =a;
	set< pair<int,int> > occD, occDl;
	set<int> occ;
	occ.insert(SA[a]);
	for(int i =N; i > 0; i--) {
		while(a > 0 && LCP[a] >= i) {
			a--;
			auto it =occ.lower_bound(SA[a]);
			if(it != occ.end()) {
				occD.insert(make_pair(*it-SA[a],SA[a]));
				if(*it-SA[a] > i) occDl.insert(make_pair(SA[a],*it-SA[a]));}
			if(it != occ.begin()) {
				auto jt =it; jt--;
				occD.insert(make_pair(SA[a]-*jt,*jt));
				if(SA[a]-*jt > i) occDl.insert(make_pair(*jt,SA[a]-*jt));
				if(it != occ.end()) {
					occD.erase(make_pair(*it-*jt,*jt));
					occDl.erase(make_pair(*jt,*it-*jt));}
				}
			occ.insert(SA[a]);}
		while(b < N && LCP[b+1] >= i) {
			b++;
			auto it =occ.lower_bound(SA[b]);
			if(it != occ.end()) {
				occD.insert(make_pair(*it-SA[b],SA[b]));
				if(*it-SA[b] > i) occDl.insert(make_pair(SA[b],*it-SA[b]));}
			if(it != occ.begin()) {
				auto jt =it; jt--;
				occD.insert(make_pair(SA[b]-*jt,*jt));
				if(SA[b]-*jt > i) occDl.insert(make_pair(*jt,SA[b]-*jt));
				if(it != occ.end()) {
					occD.erase(make_pair(*it-*jt,*jt));
					occDl.erase(make_pair(*jt,*it-*jt));}
				}
			occ.insert(SA[b]);}
		auto it =occD.lower_bound(make_pair(i+1,-1));
		while(it != occD.end() && it->ff == i+1) {
			occDl.insert(make_pair(it->ss,it->ff));
			it++;}
		p[i] =(occDl.empty())?N:(occDl.begin()->ff);}
 
	vector<int> prev(N+1,0);
	for(int i =2; i <= N; i++) {
		prev[i] =prev[i-1];
		while(prev[i] > 0 && W[prev[i]] != W[i-1]) prev[i] =prev[prev[i]];
		if(W[prev[i]] == W[i-1]) prev[i]++;}
 
	vector< vector<int> > son(N+1);
	for(int i =1; i <= N; i++) son[prev[i]].push_back(i);
	vector<int> ans(N);
	intervalac I(N+tisic);
	for(int i =0; i <= N; i++) p[i] +=i; // p[i]: first forbidden position for length i
	DFS(0,son,ans,p,I);
	for(int i =0; i < N; i++) cout << ans[i] << ((i == N-1)?"\n":" ");
	return 0;}
 
// look at my code
// my code is amazing
 