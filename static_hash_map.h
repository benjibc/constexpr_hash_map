//  Copyright [2012] <Yufei (Benny) Chen>
//  This file is part of BMVC.

//  BMVC is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.

//  BMVC is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.

//  You should have received a copy of the GNU General Public License
//  along with BMVC.  If not, see <http://www.gnu.org/licenses/>.

// OK end of the legal message with GPL.
//
// I have separated different modules of BMVC in several different but useful
// libraries that the user may use at free will.
//
// This is a static unordered_map created
// Inspired by Marc.Glisse and the code for std::tuple

#ifndef BMVC_STATIC_HASH_MAP_H
#define BMVC_STATIC_HASH_MAP_H

#include <array>
#include <iostream>
#include <utility>

///////////////////////////////////////////////////////////////////////////
// list of integer from 0 to N begin
///////////////////////////////////////////////////////////////////////////
// mimicing the definition of a tuple
template<int...> struct Indices {};

// default
template<class>
struct Next_increasing_indices;

template<int...I>
struct Next_increasing_indices<Indices<I...> > {
    typedef Indices<I...,sizeof...(I)> type;
};

template<int N>
struct N_increasing_indices
  : Next_increasing_indices<
        typename N_increasing_indices<N-1>::type
    > {} ;

template<> struct N_increasing_indices<0> {
    typedef Indices<> type;
};
///////////////////////////////////////////////////////////////////////////
// list of integer from 0 to N end 
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// useful function begin
///////////////////////////////////////////////////////////////////////////
constexpr bool compare(const char*x, const char*y){
	return (*x==0)?(*y!=0):(*y==0)?false:(*x==*y)?compare(x+1,y+1):(*x<*y);
}


constexpr bool equal(const char* x, const char * y){
    return (*x==0 && *y==0)? true : (*x==*y) ? equal(x+1,y+1) : false;
}

// Smallberg taught this in class, fairly easy to implement, also it does 
// not overflow with constexpr unlike other hashing functions 
// hopefully also has a fairly low collision rate
// making the assumption that text is not null
constexpr unsigned int hash_fnv1_recur(const char * text, unsigned int hash, int pos = 0)
{
    // if text[pos] == NULL
    //   return hash;
    // else
    //   return (text, (hash ^ text[pos]) * FNV_prime, pos + 1);
    // FNV prime is 16777619
    return (text[pos] == 0) ? hash : hash_fnv1_recur(text, 
            (hash * 16777619) ^ text[pos], pos +1); 
}

template<class Arr>
constexpr unsigned int hash_fnv1(const Arr arr, const unsigned int bucketSize, 
            const unsigned int i) 
{
    // 32bit FNV offset basis is 2166136261
    return hash_fnv1_recur(arr[i], 2166136261, 0)%bucketSize;
}

constexpr unsigned int hash_fnv1(const char * key,unsigned int bucketSize)
{
    return hash_fnv1_recur(key, 2166136261, 0)%bucketSize;
}

// used to arrange elements in hash value order
// so that when the element next to it has the same hash value
// it will need to resolve the collision
constexpr bool compareHash(const char*x, const char*y, 
                            const unsigned int bucketSize){
	return (hash_fnv1_recur(x, 2166136261, 0)%bucketSize)<
            (hash_fnv1_recur(y, 2166136261, 0)%bucketSize);
}

template<class Arr>
constexpr unsigned int checkCollisionLevel(const Arr arr,unsigned int i,
    unsigned int bucketSize)
{
    // if curIndex == 0
    //   return 0;
    // else if(curIndex == arr.size())
    //   throw logic_error;
    // else if(hash(arr[i]) == hash(arr[i-1])
    //   return 1;
    // else
    //   return 0;
    return (i== 0) ?  0 : (i==arr.size()) ? 
        throw std::logic_error("Out of bound") : 
        (hash_fnv1(arr, bucketSize, i) == hash_fnv1(arr, bucketSize, i-1))?
            1:0;
}


// To decide the address, has to loop through the who array every single time
// this means for the construction of this map, the timing is O(Bucket*N)
// you have to make the array sorted in the index order before making 
// curMapLevel is used to show which hash map level it is on; if it is 2nd,
// it will only allow collided element to go into
// right now, the check collision function will only check for the previous
// element, which means that the collision level will only be 1 
template<class Arr>
constexpr const char* getHash( const Arr arr, unsigned int bucketSize,
           const unsigned int hashVal, int i = 0, 
           unsigned short curMapLevel = 0)
{
    //if(i = arr.size())
    //  return NULL;
    //  else
    // if hash(arr[i] == hashVal)
    //   if(checkCollisionLevel(arr, i) == curMapLevel)
    //     return arr[i] 
    //   else
    //     return getHash(arr, hashVal, curIndex+1, curMap); 
    return (i==arr.size()) ? NULL: (hash_fnv1(arr, bucketSize, i) == hashVal && 
            checkCollisionLevel(arr, i,bucketSize) == curMapLevel)?
            arr[i] : getHash(arr, bucketSize,hashVal, i+1, curMapLevel);
}
///////////////////////////////////////////////////////////////////////////
// useful function end 
///////////////////////////////////////////////////////////////////////////

// assume that size of arr > 2, and 2 arrays are allowed to resolve collision
// cur will start at 1, and check with every element before this
template<class Arr>
constexpr bool check_collision(const Arr arr, const unsigned int bucketSize,
const unsigned int allowedNumColl = 2, int cur = 0, int numCollision = 0)
{
   // if( cur == arr.size())
   //   return true;
   // else if numCollision > 2
   //   throw std::logic_error
   // else if(hash(arr[cur]) == hash(arr[cur]))
   //   return check_collision(arr, cur +1, numCollision +1)
   // else
   //   return chcek_collision(arr, cur+1, 0);
    return (cur == arr.size()) ? true :
        cur == 0? 
            check_collision(arr, bucketSize, allowedNumColl,cur+1, 0):
         (numCollision == allowedNumColl) ?
            (throw std::logic_error("More than 2 collision")) : 
        (hash_fnv1_pair(arr, bucketSize, cur) == 
            hash_fnv1_pair(arr, bucketSize,cur-1)) ? 
            check_collision(arr, bucketSize, allowedNumColl, 
                cur+1, numCollision +1) :
            check_collision(arr, bucketSize, allowedNumColl, cur+1, 0);
}

///////////////////////////////////////////////////////////////////
// No more stuffing around, making functional hash map
// try out the constexpr union

//cannot use std::pair because (as far as I can understand from the error
//message and the standard documentation) converstion of std::pair is not
//implicit, therefore the assignment operator cannot be used in a constexpr
//expression

template<int D, class T2, class T, int...I>
constexpr std::array<T2, D>
GArr( T t, Indices<I...> )
{
    return {{ std::get<I>(t)... }};
}

///////////////////////////////////////////////////////////////////////////
// functions used to replace tuple_cat from D.Abraham's code
// concatenating an element into an array is much faster than concat a tuple
// because there is a lot less work to do in terms of type inference
///////////////////////////////////////////////////////////////////////////

template<int D, class Arr, class T,int...I>
constexpr std::array<T, D>
concat_array_impl( const Arr arr, T t, Indices<I...>)
{
    return {{arr[I]..., t}}; 
}

template<int D, class Arr, class T,int...I>
constexpr std::array<T, D>
concat_array( const Arr arr, T t)
{
    return concat_array_impl<D>(arr,t,
        typename N_increasing_indices<D-1>::type()); 
}

template<int D,class T2, class T>
constexpr std::array<T2,D>
HWithArr(const T t)
{
    return t;
}

template<int D,class T2, class T>
constexpr std::array<T2,D>
HWithArr(int N, const unsigned int bucket, const T t)
{
    return t; 
}

template<int D, class T2,class T>
constexpr std::array<T2, D>
HWithArr(int N, const unsigned int bucket, T t, T2 mini)
{
    return HWithArr<D,T2>(
        0,bucket, concat_array<D>(t,mini));
}

// it is actually selection sort, not bubble sort
// this is the improved version of D.Abrahams code
// tuple is not used because type inference for tuple takes a long time
// simply concatenating array using selection sort is very fast for small array
// Will probably not improve on this using merge sort because:
// 1.This static hash map will have to be typed up manually, so even with
//  Vim's recording ability, programmer will not type in millions of entry 
template<int D, class T2,class T,class...U>
constexpr std::array<T2,D>
HWithArr(int N,const unsigned int bucket, const T t,const T2 mini,
        const T2 u0, U...u)
{
    return (N==sizeof...(U)+1)
        ? HWithArr<D,T2>(
            0,bucket, concat_array<t.size() + 1>(t, mini)
            ,u0 , u...)
        : compareHash(mini.first,u0.first, bucket)
          ? HWithArr<D,T2>(N+1,bucket,  t, mini, u..., u0  )
          : HWithArr<D,T2>(N+1,bucket,  t, u0,   u..., mini);
}


// bubble sort is slowing down the process dramatically
// will make the const version of mergeSort

//assume the index will not be out of bound
template<class T1, class A1, class A2>
constexpr T1 getEle(A1 a1, A2 a2, int index)
{
    // if(index < a1.size())
    //  return a1[index];
    // else
    //  return a2[index-a1.size]; 
    return (index < a1.size()) ? a1[index] : a2[index-a1.size()];
}

template<int D, class T1, class Arr, int...I>
constexpr std::array<T1, D>
getArr(const Arr a1, const Arr a2, Indices<I...> )
{
    return {{ getEle(a1, a2, I)... }};
}

template<int D, class T,  class L, class R>
constexpr std::array<T, D>
concatArr(const L l, const R r) 
{
    return GArr<D, T>(l, r, typename N_increasing_indices<D>::type());
}

template<class Arr>
constexpr unsigned int hash_fnv1_pair(const Arr arr, 
    const unsigned int bucketSize, const unsigned int i) 
{
    // 32bit FNV offset basis is 2166136261
    return hash_fnv1_recur(arr[i].first, 2166136261, 0)%bucketSize;
}
template<class Arr>
constexpr unsigned int hashCollisionLevel(const Arr arr,unsigned int i,
    unsigned int bucketSize)
{
    // if curIndex == 0
    //   return 0;
    // else if(curIndex == arr.size())
    //   throw logic_error;
    // else if(hash(arr[i]) == hash(arr[i-1])
    //   return 1;
    // else
    //   return 0;
    return (i== 0) ?  0 : (i==arr.size()) ? 
        throw std::logic_error("Out of bound") : 
        (hash_fnv1_pair(arr, bucketSize, i) == 
            hash_fnv1_pair(arr, bucketSize, i-1))?
            1:0;
}

//improvement on this function:
//Assume uniform distribution, get a rough indication of the array index
//by hash/bucketSize, and wind back until arr[i] => hashVal
template<class Pair, class Arr>
constexpr auto getHashPair( const Arr arr, unsigned int bucketSize,
           const unsigned int hashVal, int i = 0, 
           unsigned short curMapLevel = 0)->decltype(typename Arr::value_type())
{
    //if(i = arr.size())
    //  return NULL;
    //  else
    // if hash(arr[i] == hashVal)
    //   if(checkCollisionLevel(arr, i) == curMapLevel)
    //     return arr[i] 
    //   else
    //   return getHash(arr, hashVal, curIndex+1, curMap); 
    // else if(hash(arr[i]) > hashVal)
    //   if(i ==0)
    //      return pair();
    //   else
    //   return getHash(arr, hashVal, curIndex-1, curMap); 
    // else
    //   return getHash(arr, hashVal, curIndex+1, curMap); 
    return (i==arr.size()) ? Pair():
        ((hash_fnv1_pair(arr, bucketSize, i) == hashVal) && 
        hashCollisionLevel(arr, i,bucketSize) == curMapLevel)?
         arr[i] :
           // ((hash_fnv1_pair(arr, bucketSize, i)) > hashVal && 
           // hashCollisionLevel(arr, i,bucketSize) != curMapLevel)?
           //     getHashPair<Pair>(arr, bucketSize, hashVal, i-1, curMapLevel):
          getHashPair<Pair>(arr, bucketSize,hashVal, i+1, curMapLevel);
}


///////////////////////////////////////////////////////////////////////////
// current working hashMap for the time being
// right now, this map will only work for const char*; later on, I will
// generalize the map so that it can also work for integer types
// but const char * cases would suffice for my current project
// TODO: improve the speed of map construction by guessing the index of the
// element in the hash sorted array
///////////////////////////////////////////////////////////////////////////

template<int bucketSize,class TKey, class T2>
struct StaticHashMap
{
    // Pair for 
    typedef TKey key_type;
    typedef T2 value_type;
    struct _Pair 
    {
        TKey first;
        T2 second;
        bool empty;
        constexpr _Pair() : first(TKey()), second(T2()), empty(true){}
        constexpr _Pair(TKey val) : first(val), second(T2()), empty(false){}
        constexpr _Pair(T2 val) : first(TKey()), second(val), empty(false){}
        constexpr _Pair(TKey val1, T2 val2) : 
            first(val1), second(val2), empty(false){}
    };
    typedef _Pair pair_value_type; 

    // constructor
    template<class...U>
    constexpr StaticHashMap(U...u) : 
        m_size(sizeof...(U)),
        m_fitAll(check_collision(Hash_sorted_array<_Pair>(bucketSize, u...),
            bucketSize,2, 0, 0)),
        m_hash_lvl1(HashMapCreate<bucketSize>(
            Hash_sorted_array<_Pair>(bucketSize, u...), 0)), 
        m_hash_lvl2(HashMapCreate<bucketSize>(
            Hash_sorted_array<_Pair>(bucketSize, u...), 1)) {} 

    //Hash Map functions
    //making them static decreased the compilation time by a lot
    //maybe because one less pointer to de-reference?
    template<int D, class Pair, class Arr, int...I>
    constexpr static std::array<Pair,D>
    hashPair(const Arr ori , int level, Indices<I...> )
    {
        return {{ getHashPair<Pair>(ori,D, I,0,level)... }};
    }

    template<int D, class Arr>
    constexpr static std::array<_Pair, D>
    HashMapCreate(const Arr pairs, int level)
    {
        return hashPair<D,_Pair>
            (pairs,level,typename N_increasing_indices<D>::type());
    }

    template<class T3, class...U>
    constexpr static std::array<T3,sizeof...(U)> 
    //hash_sorted_array(unsigned int bucket,  U...u)
    Hash_sorted_array(unsigned int bucket, U...u)
    {
        //return HArr<sizeof...(U), T3>(0,bucket, std::make_tuple(),u...);
        return HWithArr<sizeof...(U), T3>(0,bucket, std::array<T3, 0>(),u...);
    }
    constexpr T2 get(TKey key)
    {
        //if m_hash_lvl1[hash(key)].first == key
        //  return  m_hash_lvl1[hash(key)].second;
        //else if m_hash_lvl2[hash(key)].first == key
        //  return m_hash_lvl2[hash(key)].second;
        //else
        //  return T2();
        return (equal(m_hash_lvl1[hash_fnv1(key, bucketSize)].first, key))?
            (m_hash_lvl1[hash_fnv1(key,bucketSize)].second):
               (equal(m_hash_lvl2[hash_fnv1(key, bucketSize)].first, key))?
            (m_hash_lvl2[hash_fnv1(key,bucketSize)].second):
               T2();
    }
    
    constexpr unsigned int size() const
    {
        return m_size;
    }


    //////////////////////////////////////////////////////////////////
    //member variable
    const unsigned int m_size; 
    std::array<_Pair, bucketSize> m_hash_lvl1;
    std::array<_Pair, bucketSize> m_hash_lvl2;
    // if both hash map can fit the array, this member variable will be 
    // initialized successfully
    bool m_fitAll;
};

#endif
