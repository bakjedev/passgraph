#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

class Pass;

class PassGraph {
  public:
    [[nodiscard]] Pass& AddPass(std::string name);

    int Compile() const;
  private:
    std::vector<Pass> passes_; 
    std::unordered_map<std::string, uint32_t> pass_to_index_;
};
