#ifndef DOCUMENTS_MAP_IMPL_H
#define DOCUMENTS_MAP_IMPL_H

#include <string>
#include <memory>
#include <functional>
#include <vector>
#include "data_structures/map/maybe.h"

namespace data_structures {
namespace map {

template<typename KeyType, typename ValueType>
class MapImpl {
public:
    typedef std::function<bool(const KeyType&, const KeyType&)> KeyComparerFn;
    typedef std::function<uint32_t(const KeyType&)> HashCalculator;
private:
    const KeyComparerFn key_comparer_;
    const HashCalculator hash_calculator_;
    const uint32_t capacity_;
    //entries is the map - vector that will hold pairs that hold
    //the key,value relation
    std::vector<std::pair<KeyType, ValueType>> *entries;
public:

    MapImpl(const KeyComparerFn key_comparer, const HashCalculator hash_calculator, const uint32_t capacity)
        : key_comparer_(key_comparer), hash_calculator_(hash_calculator), capacity_(capacity)
    {
        //important to use capacity_ for GetIndex to work properly
        *entries = new std::vector<std::pair<KeyType, ValueType>> [capacity_];
    }

    int Size() const
    {
        return entries.size();
    }

    void Put(const KeyType& key, const ValueType& value)
    {
        int index = GetIndex(key);
        if(entries[index]== NULL)
        {   //Put a pair into the vector "entries" if there is nothing there
            entries[index] = new std::pair<KeyType, ValueType>;
        }
        entries[index].emplace_back(key, value);

    }

    bool Remove(const KeyType& key)
    {
        int index = GetIndex(key);
        //can't remove nonexistant entry
        if(entries[index] == NULL)
        {
            return false;
        }
        //out of bounds check
        if (index < 0)
        {
            return false;
        }
        //erase pair at index
        entries[index].erase();
        return true;
    }

    Maybe<ValueType> Get(const KeyType& key) const
    {
        int index = GetIndex(key);
        if(entries[index] == NULL)
        {
            return Maybe<ValueType>();
        }
        if(index<0)
        {
            return Maybe<ValueType>();
        }
        //return the value part (.second()) of the pair contained at index in entries
        return Maybe<ValueType>(entries[index].second());
    }
private:
    //similar to findEntriesIndex in Java version
    uint32_t GetIndex(const KeyType& key) const {
        uint32_t hash = hash_calculator_(key);
        uint32_t index = hash % capacity_;
        return index;
    }
};

}  // namespace map
}  // namespace data_structures

#endif //DOCUMENTS_MAP_IMPL_H
