#pragma once
#include "Core.h"

//Extensions: gp_hash_table, xet
#include <ext/pushb_ds/assoc_container.hpp>
#include <ext/pushb_ds/tree_policy.hpp>
using namespace __gnu_pushbds;

//Extensions: rope
#include <ext/rope>
using namespace __gnu_cxx;

template<typename T>
using xet = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

template<typename T>
struct multixet{
	xet<pair<T, int>> s;

	auto begin()const{return s.begin();}
	auto end()const{return s.end();}
	auto find(const T& v)const{
		auto it = s.lower_bound({v, 0});
		return it->fi == v ? it : s.end();
	}
	void insert(const T& v){ s.insert({v, counter++}); }
	void erase(const typename xet<pair<T, int>>::iterator& it){ s.erase(it); }
	int order_of_key(const T& v)const{ return s.order_of_key({v, counter}); }
	auto find_by_order(int ord)const{ return s.find_by_order(ord); }
	int count(const T& v)const{ return order_of_key(v)-order_of_key(v-1); }
	int size()const{ return sz(s); }
private:
	int counter = 0;
};