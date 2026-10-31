#include <gtest/gtest.h>

#include "core/GameAssetContainer/ResourceHolder.hpp"

#include <stdexcept>
#include <string>

namespace {
// Stand-in resource so the test needs no graphics context or real files
struct FakeResource {
    bool loadFromFile(const std::string& fileName) {
        if (fileName == "missing")
            return false;
        loadedFrom = fileName;
        return true;
    }
    std::string loadedFrom;
};

using FakeHolder = ResourceHolder<FakeResource, std::string>;
} // namespace

TEST(Core_ResourceHolder_Test, loadThenGetByString) {
    FakeHolder holder;
    holder.load("hero", "hero.png");

    EXPECT_EQ(holder.get(std::string("hero")).loadedFrom, "hero.png");
}

TEST(Core_ResourceHolder_Test, getAcceptsStringView) {
    FakeHolder holder;
    holder.load("hero", "hero.png");

    constexpr std::string_view key = "hero";
    EXPECT_EQ(holder.get(key).loadedFrom, "hero.png");
    EXPECT_TRUE(holder.contains(key));
}

TEST(Core_ResourceHolder_Test, getUnknownIdThrowsAndNamesTheId) {
    FakeHolder holder;

    try {
        holder.get("nope");
        FAIL() << "expected std::out_of_range";
    } catch (const std::out_of_range& e) {
        EXPECT_NE(std::string(e.what()).find("nope"), std::string::npos);
    }
}

TEST(Core_ResourceHolder_Test, failedLoadThrowsAndStoresNothing) {
    FakeHolder holder;

    EXPECT_THROW(holder.load("broken", "missing"), std::runtime_error);
    EXPECT_FALSE(holder.contains("broken"));
}

TEST(Core_ResourceHolder_Test, duplicateIdThrowsAndKeepsOriginal) {
    FakeHolder holder;
    holder.load("hero", "first.png");

    EXPECT_THROW(holder.load("hero", "second.png"), std::runtime_error);
    EXPECT_EQ(holder.get("hero").loadedFrom, "first.png");
}

TEST(Core_ResourceHolder_Test, constGetWorks) {
    FakeHolder holder;
    holder.load("hero", "hero.png");

    const FakeHolder& constHolder = holder;
    EXPECT_EQ(constHolder.get("hero").loadedFrom, "hero.png");
}
