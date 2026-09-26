#include "Network/Server/SpotSnapshotStore.h"

namespace Network::Server
{
namespace
{
constexpr std::size_t HeaderLength = 13;
constexpr std::size_t RecordLength = 12;
constexpr std::uint8_t PacketCode = 0xFA;

std::uint16_t Read16(std::span<const std::uint8_t> bytes, std::size_t offset)
{
    return static_cast<std::uint16_t>((bytes[offset] << 8) | bytes[offset + 1]);
}

std::uint32_t Read32(std::span<const std::uint8_t> bytes, std::size_t offset)
{
    return (static_cast<std::uint32_t>(bytes[offset]) << 24)
        | (static_cast<std::uint32_t>(bytes[offset + 1]) << 16)
        | (static_cast<std::uint32_t>(bytes[offset + 2]) << 8)
        | bytes[offset + 3];
}
}

SpotSnapshotStore& SpotSnapshotStore::Instance()
{
    static SpotSnapshotStore store;
    return store;
}

void SpotSnapshotStore::Clear()
{
    _mapNumber = 0;
    _expectedMapNumber = 0;
    _hasExpectedMap = false;
    _expectedPage = 0;
    _pageCount = 0;
    _spots.clear();
}

void SpotSnapshotStore::ExpectMap(std::uint16_t mapNumber)
{
    Clear();
    _expectedMapNumber = mapNumber;
    _hasExpectedMap = true;
}

bool SpotSnapshotStore::ApplyPacket(std::span<const std::uint8_t> packet)
{
    if (packet.size() < HeaderLength || packet[0] != 0xC2 || Read16(packet, 1) != packet.size()
        || packet[3] != PacketCode || packet[4] != 2)
    {
        return false;
    }

    const auto mapNumber = Read16(packet, 5);
    const auto page = Read16(packet, 7);
    const auto pageCount = Read16(packet, 9);
    const auto recordCount = Read16(packet, 11);
    if (pageCount == 0 || page >= pageCount || packet.size() != HeaderLength + (recordCount * RecordLength))
    {
        return false;
    }
    if (!_hasExpectedMap || mapNumber != _expectedMapNumber)
    {
        return false;
    }

    if (page == 0)
    {
        _spots.clear();
        _expectedPage = 0;
        _mapNumber = mapNumber;
        _pageCount = pageCount;
    }
    if (page != _expectedPage || pageCount != _pageCount || mapNumber != _mapNumber)
    {
        return false;
    }

    for (std::size_t i = 0; i < recordCount; ++i)
    {
        const auto offset = HeaderLength + (i * RecordLength);
        _spots.push_back({packet[offset], packet[offset + 1], packet[offset + 2], packet[offset + 3],
                          Read16(packet, offset + 4), Read16(packet, offset + 6), Read32(packet, offset + 8)});
    }
    ++_expectedPage;
    return true;
}
}
