#pragma once

#include "Common/this.hpp"
#include <algorithm>
#include <optional>
#include <unordered_map>
#include <vector>

template <typename Key, typename Value> class IdMap {
  public:
    IdMap() : m_map(), m_pool() {}

    auto IdOf(this IdMap const &self, Key const &key) -> std::optional<usize> {
        if (self.m_map.contains(key)) {
            return self.m_map.at(key);
        }
        return std::nullopt;
    }

    auto Insert(this IdMap &self, Key key, Value value) -> usize {
        if (auto id = self.IdOf(key); id.has_value())
            return id.value();
        usize new_id = self.m_pool.size();
        self.m_pool.push_back(std::move(value));
        self.m_map[key] = new_id;
        return new_id;
    }

    auto ReplaceOrInsert(this IdMap &self, Key key, Value value) -> usize {
        if (auto id = self.IdOf(key); id.has_value()) {
            self.m_pool[id.value()] = std::move(value);
            return id.value();
        } else {
            usize new_id = self.m_pool.size();
            self.m_pool.push_back(std::move(value));
            self.m_map[key] = new_id;
            return new_id;
        }
    }
    auto Contains(this IdMap const &self, Key const &key) -> bool { TODO(); }

    auto Get(this IdMap &self, Key const &key) -> Value * {
        if (self.m_map.contains(key)) {
            return &self.m_pool.at(self.m_map.at(key));
        }
        return nullptr;
    }

    auto Get(this IdMap const &self, Key const &key)
        -> Value const * {
        if (self.m_map.contains(key)) {
            return &self.m_pool.at(self.m_map.at(key));
        }
        return nullptr;
    }

    auto GetFromId(this IdMap &self, usize id) -> std::optional<Value &> {
        if (id >= self.m_pool.size())
            return std::nullopt;
        return self.m_pool[id];
    }

    auto GetFromId(this IdMap const &self, usize id)
        -> Value const * {
        if (id >= self.m_pool.size())
            return nullptr;
        return &self.m_pool[id];
    }

    auto GetFromIdUnchecked(this IdMap &self, usize id) -> Value * {
        return &self.m_pool[id];
    }

    auto GetFromIdUnchecked(this IdMap const &self, usize id)
        -> Value const & {
        return self.m_pool[id];
    }

  private:
    std::unordered_map<Key, usize> m_map;
    std::vector<Value> m_pool;
};
