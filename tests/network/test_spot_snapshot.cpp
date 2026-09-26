#include <doctest.h>

#include "Network/Server/SpotSnapshotStore.h"

#include <array>

TEST_CASE("Spot snapshot replaces data and validates packets")
{
    auto& store = Network::Server::SpotSnapshotStore::Instance();
    store.ExpectMap(0);

    const std::array<std::uint8_t, 25> oneSpot = {
        0xC2, 0x00, 0x19, 0xFA, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01,
        100, 101, 1, 5, 0, 20, 0, 3, 0, 0, 0x27, 0x10};
    CHECK(store.ApplyPacket(oneSpot));
    CHECK(store.MapNumber() == 0);
    REQUIRE(store.Spots().size() == 1);
    CHECK(store.Spots()[0].monsterNumber == 20);
    CHECK(store.Spots()[0].respawnMilliseconds == 10'000);

    const std::array<std::uint8_t, 13> emptyMap = {
        0xC2, 0x00, 0x0D, 0xFA, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};
    store.ExpectMap(1);
    CHECK_FALSE(store.ApplyPacket(oneSpot));
    CHECK(store.ApplyPacket(emptyMap));
    CHECK(store.MapNumber() == 1);
    CHECK(store.Spots().empty());

    auto malformed = oneSpot;
    malformed[2] = 0x18;
    CHECK_FALSE(store.ApplyPacket(malformed));
    CHECK(store.Spots().empty());
    store.Clear();
}
