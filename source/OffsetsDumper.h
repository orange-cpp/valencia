//
// Created by vlad on 5/9/24.
//

#pragma once
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
#include <asio.hpp>



namespace valencia
{
    class OffsetsDumper final {
    public:
        explicit OffsetsDumper(std::vector<uint8_t> segment);

        [[nodiscard]]
        std::unordered_map<std::string, std::optional<uintptr_t>> GetOffsetsOfGlobalObjects();

        [[nodiscard]]
        std::unordered_map<std::string, std::optional<uintptr_t>> GetOffsetsOfBaseEntityFileds();

        [[nodiscard]]
        std::unordered_map<std::string, std::optional<uintptr_t>> GetOffsetsOfWeaponFileds();
    private:

        [[nodiscard]]
        std::optional<uintptr_t> GetLocalPlayerOffset() const;

        [[nodiscard]]
        std::optional<uintptr_t> GetEntityListOffset() const;

        [[nodiscard]]
        std::optional<uintptr_t> GetViewMatrix() const;

        [[nodiscard]]
        std::optional<uintptr_t> GetTeamNumberOffset() const;

        [[nodiscard]]
        std::optional<uintptr_t> GetHealthOffset() const;

        [[nodiscard]]
        std::optional<uintptr_t> GetShieldOffset() const;

        [[nodiscard]]
        std::optional<uintptr_t> GetAbsVelocityOffset() const;

        [[nodiscard]]
        std::optional<uintptr_t> GetLastVisibleTimeOffset() const;

        [[nodiscard]]
        std::optional<uintptr_t> GetOriginOffset() const;

        [[nodiscard]]
        std::optional<uintptr_t> GetBoneMatrixOffset() const;

        [[nodiscard]]
        std::optional<uintptr_t> GetCameraPositionOffset() const;

        [[nodiscard]]
        std::optional<uintptr_t> GetViewAnglesOffset() const;

        [[nodiscard]]
        std::optional<uintptr_t> GetBleedOutStateOffset() const;

        [[nodiscard]]
        std::optional<uintptr_t> GetLifeStateOffset() const;

        [[nodiscard]]
        std::optional<uintptr_t> GetLastPrimaryWeaponOffset() const;

        [[nodiscard]]
        std::optional<uintptr_t> GetBulletScaleOffset() const;

        [[nodiscard]]
        std::optional<uintptr_t> GetBulletSpeedOffset() const;

        [[nodiscard]]
        std::optional<uintptr_t> GetMaxHealthOffset() const;

        [[nodiscard]]
        std::optional<uintptr_t> GetNameOffset() const;

        [[nodiscard]]
        std::optional<uintptr_t> GetNameListIndexOffset() const;

        [[nodiscard]]
        std::optional<uintptr_t> GetNameListOffset() const;

        [[nodiscard]]
        std::optional<uintptr_t> GetClientStateOffset() const;

        [[nodiscard]]
        std::optional<uintptr_t> GetNetworkChannelOffset() const;

        [[nodiscard]]
        std::optional<uintptr_t> GetLastesOffHandWeapons() const;


        [[nodiscard]]
        static std::vector<uint8_t> GetSignatureBytes(const std::string& str);

        [[nodiscard]]
        std::optional<size_t> PatternScan(const std::string &pattern) const;

        std::vector<uint8_t> m_codeSegment;
        asio::thread_pool m_threadPool = {std::thread::hardware_concurrency()};
        asio::executor_work_guard<asio::thread_pool::executor_type> m_guard = asio::make_work_guard(m_threadPool.get_executor());
        [[nodiscard]]
        std::unordered_map<std::string, std::optional<uintptr_t>>
        AsyncFindOffsets(const std::vector<std::pair<std::string, std::function<std::optional<uintptr_t>()>>>& arr);

    };
}
