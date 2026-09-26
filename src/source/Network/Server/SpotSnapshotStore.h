#pragma once

#include <cstdint>
#include <span>
#include <vector>

namespace Network::Server
{
struct SpotRecord
{
    std::uint8_t centerX;
    std::uint8_t centerY;
    std::uint8_t spawnRadius;
    std::uint8_t roamingRadius;
    std::uint16_t monsterNumber;
    std::uint16_t quantity;
    std::uint32_t respawnMilliseconds;
};

class SpotSnapshotStore
{
public:
    static SpotSnapshotStore& Instance();
    void Clear();
    void ExpectMap(std::uint16_t mapNumber);
    bool ApplyPacket(std::span<const std::uint8_t> packet);
    [[nodiscard]] std::uint16_t MapNumber() const { return _mapNumber; }
    [[nodiscard]] const std::vector<SpotRecord>& Spots() const { return _spots; }

private:
    std::uint16_t _mapNumber = 0;
    std::uint16_t _expectedMapNumber = 0;
    bool _hasExpectedMap = false;
    std::uint16_t _expectedPage = 0;
    std::uint16_t _pageCount = 0;
    std::vector<SpotRecord> _spots;
};
}
