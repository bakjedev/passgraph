#pragma once
#include <list>
#include <optional>
#include <unordered_map>

namespace fwrk {
  template<typename Key, typename Value, typename Hash = std::hash<Key>>
  class LRUCache {
  public:
    using Node = std::pair<Key, Value>;
    using Iterator = std::list<Node>::iterator;

    explicit LRUCache(const size_t capacity) : capacity_(capacity) {}

    void put(const Key& key, const Value& value)
    {
      auto it = map_.find(key);
      if (it != map_.end()) {
        it->second->second = value;
        lru_.splice(lru_.begin(), lru_, it->second);
        return;
      }

      lru_.emplace_front(key, value);
      map_[key] = lru_.begin();

      if (lru_.size() > capacity_) {
        map_.erase(lru_.back().first);
        lru_.pop_back();
      }
    }

    [[nodiscard]] std::optional<Value> get_copy(const Key& key)
    {
      auto it = map_.find(key);
      if (it == map_.end()) return std::nullopt;
      lru_.splice(lru_.begin(), lru_, it->second);
      return it->second->second;
    }

    Iterator begin() { return lru_.begin(); }
    Iterator end() { return lru_.end(); }
  private:
    std::unordered_map<Key, Iterator, Hash> map_;
    std::list<Node> lru_;
    size_t capacity_;
  };
} // namespace fwrk
