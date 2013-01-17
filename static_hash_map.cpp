//  Copyright [2012] <Yufei (Benny) Chen>
//  This file is part of BHMVC.

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
// This is the unit testing program for static_hash_map.hpp

#include "./static_hash_map.h"
#include <iostream>

int main()
{
    typedef StaticHashMap<0,const char *, int>::pair_value_type KV2;

    constexpr int b_size = findMinBSize<KV2>(
        B_STARTER, KV2("AS", 0), 
        KV2("CE", 1), KV2("DK", 2), KV2("BX", 3), KV2("BD", 3),
        KV2("DE", 1), KV2("EK", 2), KV2("CX", 3), KV2("CD", 3),
        KV2("EE", 1), KV2("FK", 2), KV2("DX", 3), KV2("DD", 3),
        KV2("FE", 1), KV2("GK", 2), KV2("EX", 3), KV2("ED", 3),
        KV2("GE", 1), KV2("HK", 2), KV2("FX", 3), KV2("FD", 3),
        KV2("HE", 1), KV2("IK", 2), KV2("GX", 3), KV2("GD", 3),
        KV2("IE", 1), KV2("JK", 2), KV2("HX", 3), KV2("HD", 3),
        KV2("JE", 1), KV2("KK", 2), KV2("IX", 3), KV2("ID", 3),
        KV2("KE", 1), KV2("LK", 2), KV2("JX", 3), KV2("JD", 3),
        KV2("LE", 1), KV2("MK", 2), KV2("KX", 3), KV2("KD", 3),
        KV2("ME", 1), KV2("NK", 2), KV2("LX", 3), KV2("LD", 3),
        KV2("NE", 1), KV2("OK", 2), KV2("MX", 3), KV2("MD", 3),
        KV2("OE", 1), KV2("PK", 2), KV2("NX", 3), KV2("ND", 3),
        KV2("PE", 1), KV2("QK", 2), KV2("OX", 3), KV2("OD", 3));
    std::cout << b_size << std::endl;

    typedef StaticHashMap<b_size,const char *, int>::pair_value_type KV;
    constexpr StaticHashMap<b_size,const char *, int> curHashMap(KV("AS", 0), 
        KV("CE", 1), KV("DK", 2), KV("BX", 3), KV("BD", 3),
        KV("DE", 1), KV("EK", 2), KV("CX", 3), KV("CD", 3),
        KV("EE", 1), KV("FK", 2), KV("DX", 3), KV("DD", 3),
        KV("FE", 1), KV("GK", 2), KV("EX", 3), KV("ED", 3),
        KV("GE", 1), KV("HK", 2), KV("FX", 3), KV("FD", 3),
        KV("HE", 1), KV("IK", 2), KV("GX", 3), KV("GD", 3),
        KV("IE", 1), KV("JK", 2), KV("HX", 3), KV("HD", 3),
        KV("JE", 1), KV("KK", 2), KV("IX", 3), KV("ID", 3),
        KV("KE", 1), KV("LK", 2), KV("JX", 3), KV("JD", 3),
        KV("LE", 1), KV("MK", 2), KV("KX", 3), KV("KD", 3),
        KV("ME", 1), KV("NK", 2), KV("LX", 5), KV("LD", 3),
        KV("NE", 1), KV("OK", 2), KV("MX", 3), KV("MD", 3),
        KV("OE", 1), KV("PK", 2), KV("NX", 3), KV("ND", 3),
        KV("PE", 1), KV("QK", 2), KV("OX", 3), KV("OD", 3));
    static_assert(curHashMap.get("CE") == 1, "CE is not 1");
    static_assert(curHashMap.get("AS") == 0, "AS is not 0");
    static_assert(curHashMap.get("LX") == 5, "LX is not 3");

    constexpr auto curHashArr =
        StaticHashMap<b_size,const char* ,int>::Hash_sorted_array<KV>(
        b_size, KV("AS", 0), 
        KV("CE", 1), KV("DK", 2), KV("BX", 3), KV("BD", 3),
        KV("DE", 1), KV("EK", 2), KV("CX", 3), KV("CD", 3),
        KV("EE", 1), KV("FK", 2), KV("DX", 3), KV("DD", 3),
        KV("FE", 1), KV("GK", 2), KV("EX", 3), KV("ED", 3),
        KV("GE", 1), KV("HK", 2), KV("FX", 3), KV("FD", 3),
        KV("HE", 1), KV("IK", 2), KV("GX", 3), KV("GD", 3),
        KV("IE", 1), KV("JK", 2), KV("HX", 3), KV("HD", 3),
        KV("JE", 1), KV("KK", 2), KV("IX", 3), KV("ID", 3),
        KV("KE", 1), KV("LK", 2), KV("JX", 3), KV("JD", 3),
        KV("LE", 1), KV("MK", 2), KV("KX", 3), KV("KD", 3),
        KV("ME", 1), KV("NK", 2), KV("LX", 3), KV("LD", 3),
        KV("NE", 1), KV("OK", 2), KV("MX", 3), KV("MD", 3),
        KV("OE", 1), KV("PK", 2), KV("NX", 3), KV("ND", 3),
        KV("PE", 1), KV("QK", 2), KV("OX", 3), KV("OD", 3));


    for (int i = 0 ; i < curHashArr.size(); i++) {
        std::cout << curHashArr[i].first << ' ';
        std::cout << hash_fnv1_pair(curHashArr, b_size, i) << '\t';
    }
    std::cout << '\n' << "size of hash map" << std::endl;
    std::cout << curHashMap.size() << std::endl;
    static_assert(curHashMap.size() == 57,
        "wrong size for the static hash map");
    std::cout << std::endl;
}

