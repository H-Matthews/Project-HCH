#pragma once

#include "assert.h"
#include <memory>
#include <string>
#include <map>

template <typename Resource, typename Identifier> class ResourceHolder {
  public:
    void load(Identifier id, const std::string& fileName);

    Resource& get(Identifier id);
    const Resource& get(Identifier id) const;

  private:
    void insertResource(Identifier id, std::unique_ptr<Resource> resource);

    std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};

#include "ResourceHolder.inl"