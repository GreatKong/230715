#include <cstdio>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <string>
#include <vector>
#include <cmath>
#include <set>
#include <map>
#include <queue>
#include <numeric>
#include <list>
#include <cstring>
#include <cctype>
#include <sstream>

using namespace std;

typedef long long LL;
typedef long double LD;
typedef vector<int> VI;
typedef pair<int,int> PII;
typedef vector<PII> VPII;

#define MP make_pair
#define ST first
#define ND second
#define PB push_back
#define FOR(i,a,b) for( int i=(a); i<=(b); ++i)
#define FORD(i,a,b) for( int i=(a); i>=(b); --i)
#define REP(i,n) for(int i=0; i<(n); ++i)
#define ALL(X) (X).begin(),(X).end()
#define SIZE(X) (int)(X).size()
#define FOREACH(it,X) for(__typeof((X).begin()) it=(X).begin(); it!=(X).end(); ++it)

char txt[100*1000];
const LD EPS = 1E-9;
const LD STEP = 5*1E-5;

struct wie{
	int coto;
	LD wart;
	struct wie *lewy, *prawy;
};

int wiecnt;
struct wie wiebuf[100*1000];
char ops[] = {'+', '-', '*', '/'};
char bufbuf[10000];
struct wie *parsuparsu(int p, int k){
	REP(iii, 4){
		char op = ops[iii];
		int lev = 0;
		FOR(i, p, k){
			if (txt[i] == '(') lev++;
			else if(txt[i] == ')') lev--;
			else if (!lev && txt[i] == op){
				struct wie *w = &wiebuf[wiecnt++];
				w->lewy = parsuparsu(p, i-1);
				w->prawy = parsuparsu(i+1, k);
				w->coto = op;
				return w;
			}
		}
	}
	if (txt[p] == '(')
		return parsuparsu(p+1, k-1);
	struct wie *w = &wiebuf[wiecnt++];
	if (txt[p] == 'x'){
		w->coto = -1;
		return w;
	}else{
		REP(i, k-p+1)
			bufbuf[i] = txt[p+i];
		bufbuf[k-p+1] = 0;
		sscanf(bufbuf, "%Lf", &w->wart);
		w->coto = -2;
		return w;
	}
}

LD licz(struct wie *w, LD x){
	if (w->coto == -1)
		return x;
	else if (w->coto == -2)
		return w->wart;
	else{
		LD w1 = licz(w->lewy, x);
		LD w2 = licz(w->prawy, x);
		switch(w->coto){
		case '+': return w1+w2;
		case '-': return w1 - w2;
		case '*': return w1 * w2;
		case '/': return w1/w2;
		}
	}
	return (LD)0;
}

void wypisz(struct wie *w, int wc){
	if (w->coto == -1){
		REP(i, wc) printf("  ");
		printf("x\n");
	}else if (w->coto==-2){
		REP(i, wc)printf("  ");
		printf("%.03Lf\n", w->wart);
	}else{
		REP(i, wc) printf("  ");
		printf("%c\n", w->coto);
		wypisz(w->lewy, wc+1);
		wypisz(w->prawy, wc+1);
	}
}

LD H, B;
struct wie *R, *T;

bool puste(char x){ return x== ' ' || x == '\t' || x == '\n' || x == '\r' || !x; }
int main(){
#ifndef ONLINE_JUDGE
	freopen("in.txt", "r", stdin);
//	freopen("out.txt", "w", stdout);
#endif
	
	LD pi = (LD)4.0 * atanl((LD)1);
	while(1){
		wiecnt = 0;
		fgets(txt, 99999, stdin);
		sscanf(txt, "%Lf%Lf", &H, &B);
		if (H < EPS && B < EPS) break;
		fgets(txt, 99999, stdin);
		int len;
		for (len = 0; !puste(txt[len]); ++len);
		R = parsuparsu(0, len-1);
		fgets(txt, 99999, stdin);
		for (len = 0; !puste(txt[len]); ++len);
		T = parsuparsu(0, len-1);
		//wypisz(R, 0);
		//wypisz(T, 0);

		LD masa_szkla = (LD)0;
		LD srodek_szkla = (LD)0;
	
		LD sr, st;

		sr = licz(R, (LD)0);
		for (LD x = STEP/2; x <= B-STEP/2; x += STEP){
			LD r1 = sr;
			LD r2 = licz(R, x);
			LD r3 = licz(R, x + STEP/2);
			sr = r3;
			LD f, f2;
			LD p1 = r1*r1;
			LD p2 = r2*r2*4;
			LD p3 = r3*r3;
			f = (p1 + p2 + p3);
			f2 = (p1*(x-STEP/2) + p2*x + p3*(x+STEP/2));
			masa_szkla += f;
			srodek_szkla += f2;
		}
		sr = licz(R, B);
		st = sr - licz(T, B);
		for (LD x = B + STEP/2; x <= H-STEP/2; x += STEP){
			LD r1 = sr;
			LD r2 = licz(R, x);
			LD r3 = licz(R, x + STEP/2);
			sr = r3;
			LD f, f2;
			LD t1 = st;
			LD t2 = r2-licz(T, x);
			LD t3 = r3-licz(T, x+STEP/2);
			st = t3;
			LD p1 = r1*r1-t1*t1;
			LD p2 = (r2*r2-t2*t2)*4;
			LD p3 = (r3*r3-t3*t3);
			f = (p1 + p2 + p3);
			f2 = (p1 * (x-STEP/2) + p2 * x + p3 * (x+STEP/2));
			masa_szkla += f;
			srodek_szkla += f2;
		}

		LD wynik = srodek_szkla / masa_szkla;
		LD opt_wys = (LD)0;
		LD opt_obj = (LD)0;
		
		masa_szkla *= (LD)2.5;
		srodek_szkla *= (LD)2.5;

		LD masa_wody = (LD)0;
		LD srodek_wody = (LD)0;
		st = licz(R, B) - licz(T, B);
		for(LD x = B + STEP/2; x <= H - STEP/2; x += STEP){
			LD r2 = licz(R, x);
			LD r3 = licz(R, x + STEP/2);
			LD t1 = st;
			LD t2 = r2-licz(T, x);
			LD t3 = r3-licz(T, x + STEP/2);
			st = t3;

			LD p1 = t1*t1;
			LD p2 = t2*t2*4;
			LD p3 = t3*t3;
			
			LD f = (p1+p2+p3);
			LD f2 = (p1*(x-STEP/2) + p2*x + p3*(x+STEP/2));
			
			masa_wody += f;
			srodek_wody += f2;
			
			LD ttt = (srodek_wody + srodek_szkla) / (masa_wody + masa_szkla);
			if (ttt < wynik){
				wynik = ttt;
				opt_wys = x - B;
				opt_obj = masa_wody;
			}
		}
		opt_obj *= STEP * pi / 6;
		int owys = (int)floorl(opt_wys * 1000.0 + 0.5);
		int oo = (int)floorl(opt_obj + 0.5);
		printf("Pour %d.%03d litres / %d.%03d cm of water.\n", oo/1000, oo%1000, owys/1000, owys%1000);
	}
	return 0;
}