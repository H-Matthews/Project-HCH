#include <stdexcept>

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(const Identifier& id, const std::string& fileName) {
    auto resource = std::make_unique<Resource>();

    // SFML 3 fonts are opened (they keep the file open); everything else is loaded
    bool success = false;
    if constexpr (requires { resource->loadFromFile(fileName); }) {
        success = resource->loadFromFile(fileName);
    } else {
        success = resource->openFromFile(fileName);
    }

    if (!success) {
        throw std::runtime_error("ResourceHolder::load - Failed to load " + fileName);
    }

    // If Successful...
    insertResource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
bool ResourceHolder<Resource, Identifier>::contains(std::string_view id) const {
    return mResourceMap.find(id) != mResourceMap.end();
}

template <typename Resource, typename Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(std::string_view id) {
    auto found = mResourceMap.find(id);
    if (found == mResourceMap.end()) {
        throw std::out_of_range("ResourceHolder::get - unknown resource id \"" + std::string(id) +
                                "\"");
    }

    return *found->second;
}

template <typename Resource, typename Identifier>
const Resource& ResourceHolder<Resource, Identifier>::get(std::string_view id) const {
    auto found = mResourceMap.find(id);
    if (found == mResourceMap.end()) {
        throw std::out_of_range("ResourceHolder::get - unknown resource id \"" + std::string(id) +
                                "\"");
    }

    return *found->second;
}

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::insertResource(const Identifier& id,
                                                          std::unique_ptr<Resource> resource) {
    // Insert and check success
    auto inserted = mResourceMap.emplace(id, std::move(resource));
    if (!inserted.second) {
        throw std::runtime_error("ResourceHolder::load - resource id already in use");
    }
}
