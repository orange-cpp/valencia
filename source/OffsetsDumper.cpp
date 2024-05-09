//
// Created by vlad on 5/9/24.
//

#include "OffsetsDumper.h"
#include "Signatures.h"


namespace valencia
{
    OffsetsDumper::OffsetsDumper(std::vector<uint8_t> segment)
    {
        m_codeSegment = std::move(segment);
    }

    std::unordered_map<std::string, std::optional<uintptr_t>> OffsetsDumper::GetOffsetsOfGlobalObjects()
    {
        std::vector<std::pair<std::string, std::function<std::optional<uintptr_t>()>>> payloads
        {
            {"r5apex.exe->LocalPlayer", [this] {return GetLocalPlayerOffset();}},
            {"r5apex.exe->EnityList", [this] {return GetEntityListOffset();}},
            {"r5apex.exe->ViewMatrix", [this] {return GetViewMatrix();}},
            {"r5apex.exe->NameList", [this] {return GetNameListOffset();}},
            {"r5apex.exe->clientState", [this] {return GetClientStateOffset();}},
            {"r5apex.exe->networkChannel", [this] {return GetNetworkChannelOffset();}},
        };

        return AsyncFindOffsets(payloads);
    }

    std::unordered_map<std::string, std::optional<uintptr_t>> OffsetsDumper::GetOffsetsOfBaseEntityFileds()
    {
        std::vector<std::pair<std::string, std::function<std::optional<uintptr_t>()>>> payloads
        {
            {"entity->m_iTeamNumber", [this] {return GetTeamNumberOffset();}},
            {"entity->m_iHealth", [this] {return GetHealthOffset();}},
            {"entity->m_iShield", [this] {return GetShieldOffset();}},
            {"entity->m_vecAbsVelocity", [this] {return GetAbsVelocityOffset();}},
            {"entity->m_fLastVisibleTime", [this] {return GetLastVisibleTimeOffset();}},
            {"entity->m_vecOrigin", [this] {return GetOriginOffset();}},
            {"entity->m_pBoneMatrix", [this] {return GetBoneMatrixOffset();}},
            {"entity->m_vecCamerPosition", [this] {return GetCameraPositionOffset();}},
            {"entity->m_vecViewAngles", [this] {return GetViewAnglesOffset();}},
            {"entity->m_bLifeState", [this] {return GetLifeStateOffset();}},
            {"entity->m_bBleedOutState", [this] {return GetBleedOutStateOffset();}},
            {"entity->m_iLastPrimaryWeapons", [this] {return GetLastPrimaryWeaponOffset();}},
            {"entity->m_iLastOffHandWeapon", [this] {return GetLastesOffHandWeapons();}},
            {"entity->m_iHealthMax", [this] {return GetMaxHealthOffset();}},
            {"entity->m_sName", [this] {return GetNameOffset();}},
            {"entity->m_iIndexInNameList", [this] {return GetNameListIndexOffset();}},
        };
        return AsyncFindOffsets(payloads);
    }

    std::unordered_map<std::string, std::optional<uintptr_t>> OffsetsDumper::GetOffsetsOfWeaponFileds()
    {
        std::vector<std::pair<std::string, std::function<std::optional<uintptr_t>()>>> payloads
        {
                {"weapon->m_fBulletSpeed", [this] {return GetBulletSpeedOffset();}},
                {"weapon->m_fBulletGravity", [this] {return GetBulletScaleOffset();}},
            };
        return AsyncFindOffsets(payloads);
    }

    std::optional<uintptr_t> OffsetsDumper::GetLocalPlayerOffset() const
    {
        const auto index = PatternScan(signatures::LocalPlayer);

        if (!index) [[unlikely]]
            return std::nullopt;

        const auto localOffset = *reinterpret_cast<const uint32_t*>(&m_codeSegment.at(index.value()+3));


        return 0x1000+localOffset+index.value()+7+8;
    }

    std::optional<uintptr_t> OffsetsDumper::GetEntityListOffset() const
    {
        const auto index = PatternScan(signatures::entityList);

        if (!index) [[unlikely]]
            return std::nullopt;

        const auto localOffset = *reinterpret_cast<const uint32_t*>(&m_codeSegment.at(index.value()+3));

        return 0x1000+localOffset+index.value()+7;
    }

    std::optional<uintptr_t> OffsetsDumper::GetViewMatrix() const
    {
        const auto index = PatternScan(signatures::viewProjectionMatrix);

        if (!index) [[unlikely]]
            return std::nullopt;

        const auto localOffset = *reinterpret_cast<const uint32_t*>(&m_codeSegment.at(index.value()+3));

        return 0x1000+localOffset+index.value()+7+0x10;
    }

    std::optional<uintptr_t> OffsetsDumper::GetTeamNumberOffset() const
    {
        const auto index = PatternScan(signatures::m_iTeamNumber);

        if (!index) [[unlikely]]
            return std::nullopt;

        const auto localOffset = *reinterpret_cast<const uint32_t*>(&m_codeSegment.at(index.value()+2));

        return localOffset;
    }

    std::optional<uintptr_t> OffsetsDumper::GetHealthOffset() const
    {
        const auto index = PatternScan(signatures::m_iHealth);

        if (!index) [[unlikely]]
            return std::nullopt;

        const auto localOffset = *reinterpret_cast<const uint32_t*>(&m_codeSegment.at(index.value()+3));

        return localOffset;
    }

    std::optional<uintptr_t> OffsetsDumper::GetShieldOffset() const
    {
        const auto index = PatternScan(signatures::m_iShield);

        if (!index) [[unlikely]]
            return std::nullopt;

        const auto localOffset = *reinterpret_cast<const uint32_t*>(&m_codeSegment.at(index.value()+2));

        return localOffset;
    }

    std::optional<uintptr_t> OffsetsDumper::GetAbsVelocityOffset() const
    {
        const auto index = PatternScan(signatures::m_vecAbsVelocity);

        if (!index) [[unlikely]]
            return std::nullopt;

        const auto localOffset = *reinterpret_cast<const uint32_t*>(&m_codeSegment.at(index.value()+4));

        return localOffset;
    }

    std::optional<uintptr_t> OffsetsDumper::GetLastVisibleTimeOffset() const
    {
        const auto index = PatternScan(signatures::m_lastVisibleTime);

        if (!index) [[unlikely]]
            return std::nullopt;

        const auto localOffset = *reinterpret_cast<const uint32_t*>(&m_codeSegment.at(index.value()+6));

        return localOffset;
    }

    std::optional<uintptr_t> OffsetsDumper::GetOriginOffset() const
    {
        const auto index = PatternScan(signatures::m_vecOrigin);

        if (!index) [[unlikely]]
            return std::nullopt;

        const auto localOffset = *reinterpret_cast<const uint32_t*>(&m_codeSegment.at(index.value()+4));

        return localOffset;
    }

    std::optional<uintptr_t> OffsetsDumper::GetBoneMatrixOffset() const
    {
        const auto index = PatternScan(signatures::m_boneMatrix);

        if (!index) [[unlikely]]
            return std::nullopt;

        const auto localOffset = *reinterpret_cast<const uint32_t*>(&m_codeSegment.at(index.value()+2));

        return localOffset+0x44;
    }

    std::optional<uintptr_t> OffsetsDumper::GetCameraPositionOffset() const
    {
        const auto index = PatternScan(signatures::m_cameraPos);

        if (!index) [[unlikely]]
            return std::nullopt;

        const auto localOffset = *reinterpret_cast<const uint32_t*>(&m_codeSegment.at(index.value()+3));

        return localOffset;
    }

    std::optional<uintptr_t> OffsetsDumper::GetViewAnglesOffset() const
    {
        const auto index = PatternScan(signatures::m_viewAngles);

        if (!index) [[unlikely]]
            return std::nullopt;

        const auto localOffset = *reinterpret_cast<const uint32_t*>(&m_codeSegment.at(index.value()+4));

        return localOffset;
    }

    std::optional<uintptr_t> OffsetsDumper::GetBleedOutStateOffset() const
    {
        const auto index = PatternScan(signatures::m_bleedOutState);

        if (!index) [[unlikely]]
            return std::nullopt;

        const auto localOffset = *reinterpret_cast<const uint32_t*>(&m_codeSegment.at(index.value()+2));

        return localOffset;
    }

    std::optional<uintptr_t> OffsetsDumper::GetLifeStateOffset() const
    {
        const auto index = PatternScan(signatures::m_lifeState);

        if (!index) [[unlikely]]
            return std::nullopt;

        const auto localOffset = *reinterpret_cast<const uint32_t*>(&m_codeSegment.at(index.value()+2));

        return localOffset;
    }

    std::optional<uintptr_t> OffsetsDumper::GetLastPrimaryWeaponOffset() const
    {
        const auto index = PatternScan(signatures::m_latestPrimaryWeapons);

        if (!index) [[unlikely]]
            return std::nullopt;

        const auto localOffset = *reinterpret_cast<const uint32_t*>(&m_codeSegment.at(index.value()+3));

        return localOffset;
    }

    std::optional<uintptr_t> OffsetsDumper::GetBulletScaleOffset() const
    {
        const auto index = PatternScan(signatures::m_fProjectileBulletScale);

        if (!index) [[unlikely]]
            return std::nullopt;

        const auto localOffset = *reinterpret_cast<const uint32_t*>(&m_codeSegment.at(index.value()+4));

        return localOffset;
    }

    std::optional<uintptr_t> OffsetsDumper::GetBulletSpeedOffset() const
    {
        const auto index = PatternScan(signatures::m_projectileBulletSpeed);

        if (!index) [[unlikely]]
            return std::nullopt;

        const auto localOffset = *reinterpret_cast<const uint32_t*>(&m_codeSegment.at(index.value()+4));

        return localOffset;
    }

    std::optional<uintptr_t> OffsetsDumper::GetMaxHealthOffset() const
    {
        const auto index = PatternScan(signatures::m_maxHealth);

        if (!index) [[unlikely]]
            return std::nullopt;

        const auto localOffset = *reinterpret_cast<const uint32_t*>(&m_codeSegment.at(index.value()+3));

        return localOffset;
    }

    std::optional<uintptr_t> OffsetsDumper::GetNameOffset() const
    {
        const auto index = PatternScan(signatures::m_nameOffset);

        if (!index) [[unlikely]]
            return std::nullopt;

        const auto localOffset = *reinterpret_cast<const uint32_t*>(&m_codeSegment.at(index.value()+3));

        return localOffset;
    }

    std::optional<uintptr_t> OffsetsDumper::GetNameListIndexOffset() const
    {
        const auto index = PatternScan(signatures::m_indexInNameList);

        if (!index) [[unlikely]]
            return std::nullopt;

        const auto localOffset = *reinterpret_cast<const uint32_t*>(&m_codeSegment.at(index.value()+3));

        return localOffset;
    }

    std::optional<uintptr_t> OffsetsDumper::GetNameListOffset() const
    {
        const auto index = PatternScan(signatures::nameList);

        if (!index) [[unlikely]]
            return std::nullopt;

        const auto localOffset = *reinterpret_cast<const uint32_t*>(&m_codeSegment.at(index.value()+3));

        return localOffset+0x1000+7+index.value();
    }

    std::optional<uintptr_t> OffsetsDumper::GetClientStateOffset() const
    {
        const auto index = PatternScan(signatures::clientState);

        if (!index) [[unlikely]]
            return std::nullopt;

        const auto localOffset = *reinterpret_cast<const uint32_t*>(&m_codeSegment.at(index.value()+3));

        return localOffset+0x1000+7+index.value();
    }

    std::optional<uintptr_t> OffsetsDumper::GetNetworkChannelOffset() const
    {
        const auto index = PatternScan(signatures::networkChannel);

        if (!index) [[unlikely]]
            return std::nullopt;

        const auto localOffset = *reinterpret_cast<const uint32_t*>(&m_codeSegment.at(index.value()+3));

        return localOffset+0x1000+7+index.value();
    }

    std::optional<uintptr_t> OffsetsDumper::GetLastesOffHandWeapons() const
    {
        const auto index = PatternScan(signatures::latestOffHandWeapons);

        if (!index) [[unlikely]]
            return std::nullopt;

        const auto localOffset = *reinterpret_cast<const uint32_t*>(&m_codeSegment.at(index.value()+5));

        return localOffset;
    }

    std::vector<uint8_t> OffsetsDumper::GetSignatureBytes(const std::string &str)
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

    std::optional<size_t> OffsetsDumper::PatternScan(const std::string &pattern) const
    {
        const auto patternBytes = GetSignatureBytes(pattern);

        for (size_t i = 0; i < m_codeSegment.size() - patternBytes.size(); i++)
        {
            bool found = true;

            for (size_t j = 0; j < patternBytes.size(); j++)
            {
                found = patternBytes[j] == '\?' or patternBytes[j] == m_codeSegment.at(i+j);
                if (not found) break;
            }
            if (found)
                return i;
        }
        return std::nullopt;
    }
    std::unordered_map<std::string, std::optional<uintptr_t>>
    OffsetsDumper::AsyncFindOffsets(
        const std::vector<std::pair<std::string, std::function<std::optional<uintptr_t>()>>>& arr)
    {
        std::unordered_map<std::string, std::optional<uintptr_t>> offsets;

        std::mutex mapMutex;
        std::mutex finishedMutex;

        std::condition_variable cv;
        std::atomic<size_t> scannedOffsets = 0;

        for (const auto& [offsetName, payload] : arr)
        {
            asio::post(m_threadPool,
            [&offsets, &mapMutex, &offsetName,
            &payload, &cv, &scannedOffsets, offCount = arr.size()]
            {
                const auto offset = payload();

                std::scoped_lock slock(mapMutex);
                offsets.emplace(offsetName, offset);

                if (++scannedOffsets != offCount)
                    return;

                cv.notify_one();
            });
        }
        std::unique_lock lock(finishedMutex);
        cv.wait(lock);
        return offsets;
    }
}
