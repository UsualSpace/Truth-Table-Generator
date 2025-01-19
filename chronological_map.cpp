/*
 * Filename: chronological_map.cpp 
 * Programmer: Abdurrahman Alyajouri
 * Date: 1/14/2025
 * Purpose: To implement the ChronologicalMap class defined in 
 *          chronological_map.h.
 */
 
#include <iterator>
#include "chronological_map.h"
        
ChronologicalMap::ChronologicalMap() {}
        
bool& ChronologicalMap::operator[](char a_key) {
    if(!this->map.count(a_key)) {
        this->list.push_back({a_key, false});
        this->map.insert({a_key, std::prev(this->list.end())});
        return this->map[a_key]->second;
    }
    return this->map[a_key]->second;
}
        
std::list<ChronologicalMap::kv>::iterator ChronologicalMap::begin() {
    return this->list.begin();
}

std::list<ChronologicalMap::kv>::iterator ChronologicalMap::end() {
    return this->list.end(); 
}

size_t ChronologicalMap::size() const {
    return this->map.size();
}
