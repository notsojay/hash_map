/***********************************************************************
** Date: 		4/1/22
** Project :	linear_probing.h
** Programers:	Jiahao Liang, jianwen Qi
** File:		linear_probing.h
** Purpose:		The class linear_probing.h, which implement the linear_probing.h ADT
** Notes:		
***********************************************************************/

#pragma once
#ifndef _LINEAR_PROBING_H_
#define _LINEAR_PROBING_H_

#include <iostream>
#include <vector>
#include <string>

enum STATUS
{
	EMPTY = 0,
	EXIST
};

// class of entry
template<class K, class V>
class LPPair
{
public:
	LPPair(const K& = K(), const V& = V());
	const K& getKey() const;
	const V& getData() const;
	const STATUS& getStatus() const;
	void setKey(const K&);
	void setData(const V&);
	void setStatus(const STATUS&);
private:
	K key;
	V data;
	STATUS stat;
};

template<class K, class V>
LPPair<K, V>::LPPair(const K &key, const V &data):key(key), data(data), stat(EMPTY){}

template<class K, class V>
const
K&
LPPair<K, V>::getKey() const
{
	return key;
}

template<class K, class V>
const
V&
LPPair<K, V>::getData() const
{
	return data;
}

template<class K, class V>
const
STATUS&
LPPair<K, V>::getStatus() const
{
	return stat;
}

template<class K, class V>
void
LPPair<K, V>::setKey(const K& key)
{
	this->key = key;
}

template<class K, class V>
void
LPPair<K, V>::setData(const V& data)
{
	this->data = data;
}

template<class K, class V>
void
LPPair<K, V>::setStatus(const STATUS& stat)
{
	this->stat = stat;
}

template<class K>
class LPHash
{
public:
	size_t operator()(const K& key, const size_t& arrSize) const
	{
		return std::abs(key) % arrSize;
	}
};

template<>
class LPHash<std::string>
{
public:
	size_t operator()(const std::string& str, const size_t& arrSize, const size_t& a = 1) const
	{
		size_t sum = 0;
		for(auto i = str.begin(); i != str.end(); ++i)
		{
			sum += *i * a;
		}
		return sum % arrSize;
	}
};

template<class K, class V, class H = LPHash<K>>
class LPHashMap
{
public:
	class IteratorOfHashMap;
	using iterator = IteratorOfHashMap;
public:
	LPHashMap(const size_t& = 53ul);
	~LPHashMap();
	inline bool empty() const noexcept;
	inline size_t size() const noexcept;
	iterator find(const K&);
	bool insert(const K&, const V&);
	bool erase(const K&);
	size_t getNextPrime(const size_t&) const;
		 begin() const;
	iterator end() const;
	void printEverything();
private:
	inline size_t linearProbing(size_t index) const;
private:
	std::vector<LPPair<K, V>>* table;
	size_t count;
	H hash;
};

template<class K, class V, class H>
LPHashMap<K, V, H>::LPHashMap(const size_t& capacity):table(new std::vector<LPPair<K, V>>(capacity)), count(0){}

template<class K, class V, class H>
LPHashMap<K, V, H>::~LPHashMap()
{
	if(table != nullptr)
	{
		delete table;
		table = nullptr;
	}
}

template<class K, class V, class H>
inline
bool
LPHashMap<K, V, H>::empty() const noexcept
{
	return table->empty();
}

template<class K, class V, class H>
inline
size_t
LPHashMap<K, V, H>::size() const noexcept
{
	return count;
}

template<class K, class V, class H>
typename LPHashMap<K, V, H>::iterator
LPHashMap<K, V, H>::find(const K& key)
{
	size_t index = hash(key, table->size());
	size_t originHashAddr = index;
	while(table->at(index).getKey() != key) 
	{
		index = linearProbing(index);
		if(index == originHashAddr)
			return end();
	}
	if(table->at(index).getStatus() == EXIST)
		return iterator(table->begin() + index);
	else 
		return end();
}

template<class K, class V, class H>
bool
LPHashMap<K, V, H>::insert(const K& key, const V& data)
{
	size_t index = hash(key, table->size());
	while(table->at(index).getStatus() == EXIST)
	{
		index = linearProbing(index);
	}
	table->at(index).setKey(key);
	table->at(index).setData(data);
	table->at(index).setStatus(EXIST);
	count++;
	return true;
}

template<class K, class V, class H>
bool
LPHashMap<K, V, H>::erase(const K& key)
{
	iterator p = find(key);
	if(p != end())
	{
		p.curr->setStatus(EMPTY);
		count--;
		return true;
	}
	return false;
}

template<class K, class V, class H>
size_t
LPHashMap<K, V, H>::getNextPrime(const size_t&) const
{
	const size_t PRIME_SIZE = 28;
	static const unsigned long PRIME_LIST[PRIME_SIZE] =
	{
		53ul, 97ul, 193ul, 389ul, 769ul,
		1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
		49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
		1572869ul, 3145739ul, 6291469ul, 12582917ul, 24165843ul,
		50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
		1610612741ul, 3221225473ul, 4294967291ul
	};
	for(size_t i = 0; i < PRIME_SIZE; ++i)
	{
		if(PRIME_LIST[i] > count)
		{
			return PRIME_LIST[i];
		}
	}
	return PRIME_LIST[PRIME_SIZE-1];
}

template<class K, class V, class H>
typename LPHashMap<K, V, H>::iterator
LPHashMap<K, V, H>::begin() const
{
	if(empty()) 
	{
		return end();
	}
	typename std::vector<LPPair<K, V>>::iterator pos = table->begin();
	while(pos->getStatus() == EMPTY)
	{
		++pos;
	}
	return iterator(pos);
}

template<class K, class V, class H>
typename LPHashMap<K, V, H>::iterator
LPHashMap<K, V, H>::end() const
{
	return iterator(table->end());
}

template<class K, class V, class H>
inline
size_t
LPHashMap<K, V, H>::linearProbing(size_t index) const
{
	index++;
	if(index == table->size())
	{
		index = 0;
	}
	return index;
}

template<class K, class V, class H>
void
LPHashMap<K, V, H>::printEverything()
{
	for(size_t i = 0; i < table->size(); ++i)
	{
		std::cout << i << ": " << table->at(i).getKey() << '\n';
	}
}

template<class K, class V, class H>
class LPHashMap<K, V, H>::IteratorOfHashMap
{
public:
	friend class HashMap;
public:
	IteratorOfHashMap(const typename std::vector<LPPair<K, V>>::iterator& curr = typename std::vector<LPPair<K, V>>::iterator()):curr(curr){}
	const LPPair<K, V>& operator*() const;
	const LPPair<K, V>* operator->() const;
	bool operator==(const iterator&) const;
	bool operator!=(const iterator&) const;
	iterator& operator=(const iterator& p);
	iterator& operator++();
	iterator& operator++(int);
	iterator& operator--();
	iterator& operator--(int);
private:
	typename std::vector<LPPair<K, V>>::iterator curr;
};

template<class K, class V, class H>
const
LPPair<K, V>&
LPHashMap<K, V, H>::iterator::operator*() const
{
	return *curr;
}

template<class K, class V, class H>
const
LPPair<K, V>*
LPHashMap<K, V, H>::iterator::operator->() const
{
	return &(*curr);
}

template<class K, class V, class H>
bool
LPHashMap<K, V, H>::iterator::operator==(const iterator& p) const
{
	return curr == p.curr;
}

template<class K, class V, class H>
bool
LPHashMap<K, V, H>::iterator::operator!=(const iterator& p) const
{
	return curr != p.curr;
}

template<class K, class V, class H>
typename LPHashMap<K, V, H>::iterator&
LPHashMap<K, V, H>::iterator::operator=(const iterator& p)
{
	delete curr;
	curr = p.curr;
}

template<class K, class V, class H>
typename LPHashMap<K, V, H>::iterator&
LPHashMap<K, V, H>::iterator::operator++()
{
	++curr;
	while(curr->getStatus() == EMPTY)
	{
		++curr;
	}
	return *this;
}

template<class K, class V, class H>
typename LPHashMap<K, V, H>::iterator&
LPHashMap<K, V, H>::iterator::operator++(int)
{
	iterator temp(*this);
	++curr;
	while(curr->getStatus() == EMPTY)
	{
		++curr;
	}
	return temp;
}

template<class K, class V, class H>
typename LPHashMap<K, V, H>::iterator&
LPHashMap<K, V, H>::iterator::operator--()
{
	--curr;
	while(curr->getStatus() == EMPTY)
	{
		--curr;
	}
	return *this;
}

template<class K, class V, class H>
typename LPHashMap<K, V, H>::iterator&
LPHashMap<K, V, H>::iterator::operator--(int)
{
	iterator temp(*this);
	--curr;
	while(curr->getStatus() == EMPTY)
	{
		--curr;
	}
	return temp;
}

#endif