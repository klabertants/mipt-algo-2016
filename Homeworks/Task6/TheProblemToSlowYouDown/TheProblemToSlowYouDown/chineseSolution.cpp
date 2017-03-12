////
////  chineseSolution.cpp
////  TheProblemToSlowYouDown
////
////  Created by Дмитрий Ткаченко on 05/12/2016.
////  Copyright © 2016 BillionariesClub. All rights reserved.
////
//
//#include "string.h"
//#include "iostream"
//#include "cmath"
//
//using namespace std;
//const int MAXN = 400005 ;
//const int N = 26 ;
//char str1[MAXN];
//char str2[MAXN];
//
//struct Palindromic_Tree {
//    int slen,slen1;
//    int next[MAXN][N] ;
//    int fail[MAXN] ;
//    long long cnt[MAXN] ;
//    long long cnt1[MAXN] ;
//    int num[MAXN] ;
//    int len[MAXN] ;
//    int S[MAXN] ;
//    int last ;
//    int n ;
//    int p ;
//    int newnode ( int l ) {
//        for ( int i = 0 ; i < N ; ++ i ) next[p][i] = 0 ;
//        cnt[p] = 0 ;
//        num[p] = 0 ;
//        len[p] = l ;
//        return p ++ ;
//    }
//    void init () {
//        for(int i=0;i<=slen+slen1+4;i++)
//        {
//            cnt1[i]=cnt[i]=0;
//        }
//        
//        p = 0 ;
//        newnode (  0 ) ;
//        newnode ( -1 ) ;
//        last = 0 ;
//        n = 0 ;
//        S[n] = -1 ;
//        fail[0] = 1 ;
//    }
//    void init1()
//    {
//        last=0;
//        n=0;
//        S[n]=-1;
//        fail[n]=1;
//    }
//    int get_fail ( int x ) {
//        while(S[n-len[x]- 1]!= S[n])
//            x = fail[x] ;
//        return x ;
//    }
//    
//    void add ( int c ) {
//        c -= 'a' ;
//        S[++ n] = c ;
//        int cur = get_fail ( last ) ;
//        if ( !next[cur][c] ) {
//            int now = newnode ( len[cur] + 2 ) ;
//            fail[now] = next[get_fail ( fail[cur] )][c] ;
//            next[cur][c] = now ;
//            num[now] = num[fail[now]] + 1 ;
//        }
//        last = next[cur][c] ;
//        cnt[last] ++ ;
//    }
//    
//    void add1(int c)
//    {
//        c -= 'a' ;
//        S[++ n] = c ;
//        int cur = get_fail ( last ) ;
//        if ( !next[cur][c] ) {
//            int now = newnode ( len[cur] + 2 ) ;
//            fail[now] = next[get_fail ( fail[cur] )][c] ;
//            next[cur][c] = now ;
//            num[now] = num[fail[now]] + 1 ;
//        }
//        last = next[cur][c] ;
//        cnt1[last] ++ ;
//        
//    }
//    
//    void count () {
//        for ( int i = p - 1 ; i >= 0 ; -- i )
//            cnt[fail[i]] += cnt[i] ;
//    }
//    void count1()
//    {
//        for ( int i = p - 1 ; i >= 0 ; -- i )
//            cnt1[fail[i]] += cnt1[i] ;
//    }
//} tree;
//
//int main()
//{
//    int n;
//    scanf("%d",&n);
//    int k=1;
//    while(n--)
//    {
//        scanf("%s%s",str1,str2);
//        tree.init();
//        tree.slen=strlen(str1);
//        tree.slen1=strlen(str2);
//        for(int i=0;i<tree.slen;i++)
//            tree.add(str1[i]);
//        tree.count();
//        tree.init1();
//        for(int i=0;i<tree.slen1;i++)
//            tree.add1(str2[i]);
//        tree.count1();
//        long long ans=0;
//        for(int i=2;i<=tree.p-1;i++)
//        {
//            ans+=tree.cnt[i]*tree.cnt1[i];
//        }
//        printf("Case #%d: %lld\n",k++,ans);
//    }
//}
