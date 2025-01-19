#pragma once

/*
 * Filename: chronological_map.h 
 * Programmer: Abdurrahman Alyajouri
 * Date: 1/14/2025
 * Purpose: To establish a bare bones container that allows key/value mapping
 *          functionality while preserving order of insertion.
 *          Specifically designed for the purposes of the truth table project,
 *          mapping chars (variables can only be 1 char) to booleans 
 *          (the values of each variable).
 */
 
#include <unordered_map>
#include <utility>
#include <cstddef>
#include <list>

class ChronologicalMap {
    public:
        typedef std::pair<char, bool> kv;
        
        ChronologicalMap();
        
        bool& operator[](char a_key);
        
        std::list<kv>::iterator begin();
        std::list<kv>::iterator end();
        
        size_t size() const;
        
    private:
        std::list<kv> list;
        std::unordered_map<char, std::list<kv>::iterator> map;
};

typedef ChronologicalMap chrono_map;
