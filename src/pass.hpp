#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>

class Pass {
  public:
    void AddResource(std::string name) {
      resources_.emplace(std::move(name), 0);
    }

    [[nodiscard]] const std::unordered_map<std::string, uint32_t>& GetResources() const {
      return resources_;
    }

  private:
    std::unordered_map<std::string, uint32_t> resources_;
};
