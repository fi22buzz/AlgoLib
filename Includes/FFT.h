#pragma once
#include "Core.h"
#include "NumberTheory.h"

using C=complex<f64>;

Arr<C> toC(const Arr<i64>& a){
	Arr<C> ret;
	for(auto i:a)
		ret.pushb(C(i,0));
	return ret;
}

Arr<i64> toi(const Arr<C>& a){
	Arr<i64> ret;
	for(auto i:a)
		ret.pushb(round(i.real()));
	return ret;
}

const auto ee=exp(1);

Arr<C> fft(const Arr<C>& a, C w){
	int n=sz(a);
	if(n==1)
		return a;
	Arr<C> ev(n/2), od(n/2), ret(n);
	hfor(i,0,n)
		(i%2?od:ev)[i/2]=a[i];
	ev=fft(ev,w*w); od=fft(od,w*w);
	auto wi=C(1,0);
	hfor(i,0,n){
		ret[i]=ev[i%(n/2)]+wi*od[i%(n/2)];
		wi*=w;
	}
	return ret;
}
Arr<C> fft(const Arr<C>& a){return fft(a, pow(ee, C(0,1)*(-2*pi/sz(a))));}

// //topology's no-recursion code
// Arr<C> fft(Arr<C> a){
// 	int n=sz(a);
// 	for(int i=1,j=0;i<n;i++) {
// 		int bit=n/2;
// 		while(!((j^=bit)&bit))bit/=2;
// 		if(i<j)swap(a[i],a[j]);
// 	}
// 	for(int i=1;i<n;i*=2) {
// 		f64 x=-pi/i;
// 		C w(cos(x),sin(x));
// 		for(int j=0;j<n;j+=i*2) {
// 			C th(1,0);
// 			for (int k=0;k<i;k++) {
// 				C tmp=a[i+j+k]*th;
// 				a[i+j+k]=a[j+k]-tmp;
// 				a[j+k]+=tmp;
// 				th *= w;
// 			}
// 		}
// 	}
// 	return a;
// }

Arr<C> ffti(Arr<C> a){
	int n=sz(a);
	reverse(a.begin()+1, a.end());
	return mapp(fft(a), [n](auto x){return x/C(n,0);});
}

Arr<i64> conv(const Arr<i64>& a, const Arr<i64>& b){
	const auto& fa=fft(toC(a));
	const auto& fb=fft(toC(b));
	Arr<C> fc;
	hfor(i,0,sz(a)) fc.pushb(fa[i]*fb[i]);
	return toi(ffti(fc));
}

Arr<i64> conv_mul(Arr<i64> a, Arr<i64> b){
	int n=1<<lgc(max(sz(a), sz(b))*2);
	a.resize(n);
	b.resize(n);
	return conv(a,b);
}

Arr<i64> conv_cir(Arr<i64> a, Arr<i64> b){
	int n=max(sz(a), sz(b));
	int m=1<<lgc(n*2);
	a.resize(m);
	reverse(all(b));
	hfor(i,n,m)
		b.pushb(b[(n+i)%n]);
	reverse(all(b));
	auto res=conv(a,b);
	return {res.end()-n, res.end()};
}