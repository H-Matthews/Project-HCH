#pragma once

#include <map>
#include <memory>
#include <string>
#include <string_view>

/**
 * Owns resources (textures, fonts, ...) keyed by an identifier. With std::string identifiers,
 * lookups also accept std::string_view.
 *
 * load() throws std::runtime_error if the file cannot be loaded or the id is already used.
 * get() throws std::out_of_range for an unknown id.
 */
template <typename Resource, typename Identifier> class ResourceHolder {
  public:
    void load(const Identifier& id, const std::string& fileName);

    bool contains(std::string_view id) const;

    Resource& get(std::string_view id);
    const Resource& get(std::string_view id) const;

  private:
    void insertResource(const Identifier& id, std::unique_ptr<Resource> resource);

    std::map<Identifier, std::unique_ptr<Resource>, std::less<>> mResourceMap;
};

#include "ResourceHolder.inl"
