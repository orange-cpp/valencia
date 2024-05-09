//
// Created by vlad on 5/8/24.
//

#pragma once
#include "Signatures.h"
#include "Signatures.h"


inline std::vector<uint8_t> GetSignatureBytes(const std::string& str)
{
    std::vector<uint8_t> bytes;
    for (size_t i = 0; i < str.size();)
    {
        if (str[i] == ' ')
        {
            i += 1;
            continue;
        }
        if (str[i] == '?')
        {
            bytes.push_back('\?');
            i+1 < str.size() and str[i+1] == '?' ? i += 2 : i++;
            continue;
        }
        bytes.push_back(std::stoi(str.substr(i,2), nullptr,16));
        i += 2;
    }
    return bytes;
}

inline std::optional<size_t> PatternScan(const std::vector<uint8_t>& segment, const std::string &pattern)
{
    const auto patternBytes = GetSignatureBytes(pattern);

    for (size_t i = 0; i < segment.size() - patternBytes.size(); i++)
    {
        bool found = true;

        for (size_t j = 0; j < patternBytes.size(); j++)
        {
            found = patternBytes[j] == '\?' or patternBytes[j] == segment.at(i+j);
            if (not found) break;
        }
        if (found)
            return i;
    }
    return std::nullopt;
}

[[nodiscard]]
inline std::optional<uintptr_t> GetLocalPlayerOffset(const std::vector<uint8_t>& segment)
{
    const auto index = PatternScan(segment, signatures::LocalPlayer);

    if (!index)
        return std::nullopt;

    const auto localOffset = *reinterpret_cast<const uint32_t*>(&segment.at(index.value()+3));


    return 0x1000+localOffset+index.value()+7+8;
}

[[nodiscard]]
inline std::optional<uintptr_t> GetEntityListOffset(const std::vector<uint8_t>& segment)
{
    const auto index = PatternScan(segment, signatures::entityList);

    if (!index)
        return std::nullopt;

    const auto localOffset = *reinterpret_cast<const uint32_t*>(&segment.at(index.value()+3));

    return 0x1000+localOffset+index.value()+7;
}

[[nodiscard]]
inline std::optional<uintptr_t> GetViewMatrix(const std::vector<uint8_t>& segment)
{
    const auto index = PatternScan(segment, signatures::viewProjectionMatrix);

    if (!index)
        return std::nullopt;

    const auto localOffset = *reinterpret_cast<const uint32_t*>(&segment.at(index.value()+3));

    return 0x1000+localOffset+index.value()+7+0x10;
}

[[nodiscard]]
inline std::optional<uintptr_t> GetTeamNumberOffset(const std::vector<uint8_t>& segment)
{
    const auto index = PatternScan(segment, signatures::m_iTeamNumber);

    if (!index)
        return std::nullopt;

    const auto localOffset = *reinterpret_cast<const uint32_t*>(&segment.at(index.value()+2));

    return localOffset;
}

[[nodiscard]]
inline std::optional<uintptr_t> GetHealthOffset(const std::vector<uint8_t>& segment)
{
    const auto index = PatternScan(segment, signatures::m_iHealth);

    if (!index)
        return std::nullopt;

    const auto localOffset = *reinterpret_cast<const uint32_t*>(&segment.at(index.value()+3));

    return localOffset;
}
[[nodiscard]]
inline std::optional<uintptr_t> GetShieldOffset(const std::vector<uint8_t>& segment)
{
    const auto index = PatternScan(segment, signatures::m_iShield);

    if (!index)
        return std::nullopt;

    const auto localOffset = *reinterpret_cast<const uint32_t*>(&segment.at(index.value()+2));

    return localOffset;
}

[[nodiscard]]
inline std::optional<uintptr_t> GetAbsVelocityOffset(const std::vector<uint8_t>& segment)
{
    const auto index = PatternScan(segment, signatures::m_vecAbsVelocity);

    if (!index)
        return std::nullopt;

    const auto localOffset = *reinterpret_cast<const uint32_t*>(&segment.at(index.value()+4));

    return localOffset;
}

[[nodiscard]]
inline std::optional<uintptr_t> GetLastVisibleTimeOffset(const std::vector<uint8_t>& segment)
{
    const auto index = PatternScan(segment, signatures::m_lastVisibleTime);

    if (!index)
        return std::nullopt;

    const auto localOffset = *reinterpret_cast<const uint32_t*>(&segment.at(index.value()+6));

    return localOffset;
}

[[nodiscard]]
inline std::optional<uintptr_t> GetOriginOffset(const std::vector<uint8_t>& segment)
{
    const auto index = PatternScan(segment, signatures::m_vecOrigin);

    if (!index)
        return std::nullopt;

    const auto localOffset = *reinterpret_cast<const uint32_t*>(&segment.at(index.value()+4));

    return localOffset;
}

[[nodiscard]]
inline std::optional<uintptr_t> GetBoneMatrixOffset(const std::vector<uint8_t>& segment)
{
    const auto index = PatternScan(segment, signatures::m_boneMatrix);

    if (!index)
        return std::nullopt;

    const auto localOffset = *reinterpret_cast<const uint32_t*>(&segment.at(index.value()+2));

    return localOffset+0x44;
}

[[nodiscard]]
inline std::optional<uintptr_t> GetCameraPositionOffset(const std::vector<uint8_t>& segment)
{
    const auto index = PatternScan(segment, signatures::m_cameraPos);

    if (!index)
        return std::nullopt;

    const auto localOffset = *reinterpret_cast<const uint32_t*>(&segment.at(index.value()+3));

    return localOffset;
}
[[nodiscard]]
inline std::optional<uintptr_t> GetViewAnglesOffset(const std::vector<uint8_t>& segment)
{
    const auto index = PatternScan(segment, signatures::m_viewAngles);

    if (!index)
        return std::nullopt;

    const auto localOffset = *reinterpret_cast<const uint32_t*>(&segment.at(index.value()+4));

    return localOffset;
}

[[nodiscard]]
inline std::optional<uintptr_t> GetBleedOutStateOffset(const std::vector<uint8_t>& segment)
{
    const auto index = PatternScan(segment, signatures::m_bleedOutState);

    if (!index)
        return std::nullopt;

    const auto localOffset = *reinterpret_cast<const uint32_t*>(&segment.at(index.value()+2));

    return localOffset;
}

[[nodiscard]]
inline std::optional<uintptr_t> GetLifeStateOffset(const std::vector<uint8_t>& segment)
{
    const auto index = PatternScan(segment, signatures::m_lifeState);

    if (!index)
        return std::nullopt;

    const auto localOffset = *reinterpret_cast<const uint32_t*>(&segment.at(index.value()+2));

    return localOffset;
}

[[nodiscard]]
inline std::optional<uintptr_t> GetLastPrimaryWeaponOffset(const std::vector<uint8_t>& segment)
{
    const auto index = PatternScan(segment, signatures::m_latestPrimaryWeapons);

    if (!index)
        return std::nullopt;

    const auto localOffset = *reinterpret_cast<const uint32_t*>(&segment.at(index.value()+3));

    return localOffset;
}

[[nodiscard]]
inline std::optional<uintptr_t> GetBulletScaleOffset(const std::vector<uint8_t>& segment)
{
    const auto index = PatternScan(segment, signatures::m_fProjectileBulletScale);

    if (!index)
        return std::nullopt;

    const auto localOffset = *reinterpret_cast<const uint32_t*>(&segment.at(index.value()+4));

    return localOffset;
}
[[nodiscard]]
inline std::optional<uintptr_t> GetBulletSpeedOffset(const std::vector<uint8_t>& segment)
{
    const auto index = PatternScan(segment, signatures::m_projectileBulletSpeed);

    if (!index)
        return std::nullopt;

    const auto localOffset = *reinterpret_cast<const uint32_t*>(&segment.at(index.value()+4));

    return localOffset;
}

[[nodiscard]]
inline std::optional<uintptr_t> GetMaxHealthOffset(const std::vector<uint8_t>& segment)
{
    const auto index = PatternScan(segment, signatures::m_maxHealth);

    if (!index)
        return std::nullopt;

    const auto localOffset = *reinterpret_cast<const uint32_t*>(&segment.at(index.value()+3));

    return localOffset;
}
[[nodiscard]]
inline std::optional<uintptr_t> GetNameOffset(const std::vector<uint8_t>& segment)
{
    const auto index = PatternScan(segment, signatures::m_nameOffset);

    if (!index)
        return std::nullopt;

    const auto localOffset = *reinterpret_cast<const uint32_t*>(&segment.at(index.value()+3));

    return localOffset;
}
[[nodiscard]]
inline std::optional<uintptr_t> GetNameListIndexOffset(const std::vector<uint8_t>& segment)
{
    const auto index = PatternScan(segment, signatures::m_indexInNameList);

    if (!index)
        return std::nullopt;

    const auto localOffset = *reinterpret_cast<const uint32_t*>(&segment.at(index.value()+3));

    return localOffset;
}
[[nodiscard]]
inline std::optional<uintptr_t> GetNameListOffset(const std::vector<uint8_t>& segment)
{
    const auto index = PatternScan(segment, signatures::nameList);

    if (!index)
        return std::nullopt;

    const auto localOffset = *reinterpret_cast<const uint32_t*>(&segment.at(index.value()+3));

    return localOffset+0x1000+7+index.value();
}
[[nodiscard]]
inline std::optional<uintptr_t> GetClientStateOffset(const std::vector<uint8_t>& segment)
{
    const auto index = PatternScan(segment, signatures::clientState);

    if (!index)
        return std::nullopt;

    const auto localOffset = *reinterpret_cast<const uint32_t*>(&segment.at(index.value()+3));

    return localOffset+0x1000+7+index.value();
}
[[nodiscard]]
inline std::optional<uintptr_t> GetNetworkChannelOffset(const std::vector<uint8_t>& segment)
{
    const auto index = PatternScan(segment, signatures::networkChannel);

    if (!index)
        return std::nullopt;

    const auto localOffset = *reinterpret_cast<const uint32_t*>(&segment.at(index.value()+3));

    return localOffset+0x1000+7+index.value();
}