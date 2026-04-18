#include "passgraph.hpp"
#include "pass.hpp"
#include <cstdio>

Pass& PassGraph::AddPass(std::string name) {
  auto itr = pass_to_index_.find(name);
  if (itr != pass_to_index_.end()) {
    return passes_[itr->second];
  }
  pass_to_index_.emplace(std::move(name), static_cast<uint32_t>(passes_.size()));
  return passes_.emplace_back();
}

int PassGraph::Compile() const {
  for (const auto& pass : passes_) {
    for (const auto& resource : pass.GetResources()) {
      std::puts(resource.first.c_str());
    }
  }
  return 0;
}
