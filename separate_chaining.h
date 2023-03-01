/***********************************************************************
** Date: 		4/1/22
** Project :	hash_map.h
** Programers:	Jiahao Liang, jianwen Qi
** File:		hash_map.h
** Purpose:		The class hash_map.h, which implement the hash_map.h ADT
** Notes:		
***********************************************************************/

#pragma once
#ifndef _SPARATE_CHAINING_
#define _SPARATE_CHAINING_

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <utility>
#include <iterator>
#include <cstddef>
#include <stdexcept>
#include <cmath>

// class of entry
template<class K, class V>
class SCPair
{
public:
	SCPair(const K& = K(), const V& = V());
	const K& getKey() const;
	const V& getData() const;
	void setData(const V&);
	
private:
	K key;
	V data;
};

template<class K, class V>
SCPair<K, V>::SCPair(const K &key, const V &data):key(key), data(data){}

template<class K, class V>
const
K&
SCPair<K, V>::getKey() const
{
	return key;
}

template<class K, class V>
const
V&
SCPair<K, V>::getData() const
{
	return data;
}

template<class K, class V>
void
SCPair<K, V>::setData(const V& data)
{
	this->data = data;
}

template<class K>
class SCHash
{
public:
	size_t operator()(const K& key, const size_t& arrSize) const
	{
		return std::abs(key * 3 + 2) % arrSize;
	}
};

template<>
class SCHash<std::string>
{
public:
	size_t operator()(const std::string& str, const size_t& arrSize) const
	{
		size_t sum = 0;
		for(auto i = str.begin(); i != str.end(); ++i)
		{
			sum += *i;
		}
		return sum % arrSize;
	}
};

template<class K, class V, class H = SCHash<K>>
class SCHashMap
{
public:
	class IteratorOfHashMap;
	using iterator = IteratorOfHashMap;
	using entry = SCPair<K, V>;
	using bucket = std::list<entry>;
	using bktArray = std::vector<bucket>;
	using BItor = typename std::vector<bucket>::iterator;		// vector::iterator
	using EItor = typename bucket::iterator;					// list::iterator
	
public:
	SCHashMap(const size_t& = 101u);
	~SCHashMap();
	inline bool empty() const noexcept;
	inline int size() const noexcept;
	iterator insert(const K&, const V&);
	iterator find(const K&);
	void erase(const K&);
	void erase(const iterator&);
	void clear();
	iterator begin();
	inline iterator end();
	void printEverything();
	
protected:
	static void nextEntry(iterator&);
	static bool endOfBucket(const iterator&);
	static bool beginOfBucket(const iterator&);
	iterator finder(const K&);
	iterator inserter(const iterator&, const entry&);
	void eraser(const iterator&);
	
private:
	bktArray* table;		// bucket array
	size_t entryCount;	// number of entries
	H hash;				// the hash comparator
};

template<class K, class V, class H>
SCHashMap<K, V, H>::SCHashMap(const size_t& capacity):table(new bktArray(capacity)), entryCount(0){}

template<class K, class V, class H>
SCHashMap<K, V, H>::~SCHashMap()
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
SCHashMap<K, V, H>::empty() const noexcept
{
	return entryCount == 0;
}

template<class K, class V, class H>
inline
int
SCHashMap<K, V, H>::size() const noexcept
{
	return entryCount;
}

template<class K, class V, class H>
void
SCHashMap<K, V, H>::nextEntry(iterator& p)
{
	++p.EPos;
}

template<class K, class V, class H>
bool
SCHashMap<K, V, H>::endOfBucket(const iterator& p)
{
	return p.EPos == p.BPos->end();
}

template<class K, class V, class H>
bool
SCHashMap<K, V, H>::beginOfBucket(const iterator& p)
{
	return p.EPos == p.BPos->begin();
}

template<class K, class V, class H>
typename SCHashMap<K, V, H>::iterator
SCHashMap<K, V, H>::finder(const K& key)
{
	size_t index = hash(key, table->size());
	BItor BIndex = table->begin() + index;
	iterator p(*table, BIndex, BIndex->begin());
	while(!endOfBucket(p) && (*p).getKey() != key)
		nextEntry(p);
	return p;
}

template<class K, class V, class H>
typename SCHashMap<K, V, H>::iterator
SCHashMap<K, V, H>::find(const K& key)
{
	iterator p = finder(key);
	if(endOfBucket(p))
		return end();
	return p;
}

template<class K, class V, class H>
typename SCHashMap<K, V, H>::iterator
SCHashMap<K, V, H>::inserter(const iterator& p, const entry& e)
{
	EItor temp = p.BPos->insert(p.EPos, e);
	++entryCount;
	return iterator(*table, p.BPos, temp);
}

template<class K, class V, class H>
typename SCHashMap<K, V, H>::iterator
SCHashMap<K, V, H>::insert(const K& key, const V& value)
{
	iterator p = finder(key);
	return inserter(p, entry(key, value));
}

template<class K, class V, class H>
void
SCHashMap<K, V, H>::eraser(const iterator& p)
{
	EItor temp = p.BPos->erase(p.EPos);
	--entryCount;
}

template<class K, class V, class H>
void
SCHashMap<K, V, H>::erase(const K& key)
{
	iterator p = finder(key);
	if(endOfBucket(p))
		throw std::out_of_range("ERROR: ERASE OF NONEXISTENT");
	eraser(p);
}

template<class K, class V, class H>
void
SCHashMap<K, V, H>::erase(const iterator& p)
{
	if(endOfBucket(p))
		throw std::out_of_range("ERROR: ERASE OF NONEXISTENT");
	eraser(p);
}

template<class K, class V, class H>
void
SCHashMap<K, V, H>::clear()
{
	if(table != nullptr)
	{
		delete table;
		table = new bktArray;
		entryCount = 0;
	}
}

template<class K, class V, class H>
typename SCHashMap<K, V, H>::iterator
SCHashMap<K, V, H>::begin()
{
	if(empty()) return end();
	BItor BPos = table->begin();
	while(BPos->empty()) ++BPos;
	return iterator(*table, BPos, BPos->begin());
}

template<class K, class V, class H>
inline typename SCHashMap<K, V, H>::iterator
SCHashMap<K, V, H>::end()
{
	return iterator(*table, table->end());
}

template<class K, class V, class H>
void
SCHashMap<K, V, H>::printEverything()
{
	for(size_t i = 0; i < table->size(); ++i)
	{
		std::cout << i << ": ";
		if(!table->at(i).empty())
		{
			for(auto j = table->at(i).begin(); j != table->at(i).end(); ++j)
			{
				std::cout << j->getKey() << " ";
			}
		}
		std::cout << '\n';
	}
}

template<class K, class V, class H>
class SCHashMap<K, V, H>::IteratorOfHashMap
{
public:
	friend class SCHashMap<K, V, H>;
public:
	IteratorOfHashMap(const bktArray& BA = bktArray(), const BItor& BI = BItor(), const EItor& EI = EItor()):BArr(new bktArray(BA)), BPos(BI), EPos(EI){}
	const entry& operator*() const;				// get entry
	const entry* operator->() const;
	bool operator==(const iterator&) const;		// are iterators equal?
	bool operator!=(const iterator&) const;
	iterator& operator=(const iterator& p);
	iterator& operator++();
	iterator& operator++(int);
	iterator& operator--();
	iterator& operator--(int);
	
private:
	bktArray* BArr;			// which bucket array
	BItor BPos;				// which bucket
	EItor EPos;				// which entry/pair
};

template<class K, class V, class H>
const
typename SCHashMap<K, V, H>::entry&
SCHashMap<K, V, H>::iterator::operator*() const
{
	return *EPos;
}

template<class K, class V, class H>
const
typename SCHashMap<K, V, H>::entry*
SCHashMap<K, V, H>::iterator::operator->() const
{
	return &(*EPos);
}

template<class K, class V, class H>
bool
SCHashMap<K, V, H>::iterator::operator==(const iterator& p) const
{
	if(BArr != p.BArr || BPos != p.BPos) return false;
	else if(BPos == BArr->end()) return true;
	else return EPos == p.EPos;
}

template<class K, class V, class H>
bool
SCHashMap<K, V, H>::iterator::operator!=(const iterator& p) const
{
	if(BArr == p.BArr || BPos == p.BPos) return false;
	else if(BPos != BArr->end()) return true;
	else return EPos != p.EPos;
}

template<class K, class V, class H>
typename SCHashMap<K, V, H>::iterator&
SCHashMap<K, V, H>::iterator::operator=(const iterator& p)
{
	delete BArr;
	BArr = p.BArr;
	BPos = p.BPos;
	EPos = p.EPos;
	return *this;
}

template<class K, class V, class H>
typename SCHashMap<K, V, H>::iterator&
SCHashMap<K, V, H>::iterator::operator++()
{
	++EPos;
	if(endOfBucket(*this))
	{
		++BPos;
		while(BPos != BArr->end() && BPos->empty())
			++BPos;
		if(BPos == BArr->end())
			return *this;
		EPos = BPos->begin();
	}
	return *this;
}

template<class K, class V, class H>
typename SCHashMap<K, V, H>::iterator&
SCHashMap<K, V, H>::iterator::operator++(int)
{
	iterator temp(*this);
	++EPos;
	if(endOfBucket(*this))
	{
		++BPos;
		while(BPos != BArr->end() && BPos->empty())
			++BPos;
		if(BPos == BArr->end())
			return *this;
		EPos = BPos->begin();
	}
	return temp;
}

template<class K, class V, class H>
typename SCHashMap<K, V, H>::iterator&
SCHashMap<K, V, H>::iterator::operator--()
{
	--EPos;
	if(beginOfBucket(*this))
	{
		--BPos;
		while(BPos != BArr->begin() && BPos->empty())
			--BPos;
		if(BPos == BArr->begin())
			return *this;
		EPos = BPos->begin();
	}
	return *this;
}

template<class K, class V, class H>
typename SCHashMap<K, V, H>::iterator&
SCHashMap<K, V, H>::iterator::operator--(int)
{
	iterator temp(*this);
	--EPos;
	if(beginOfBucket(*this))
	{
		--EPos;
		while(BPos != BArr->begin() && BPos->empty())
			--EPos;
		if(BPos == BArr->begin())
			return *this;
		EPos = BPos->begin();
	}
	return temp;
}

#endif