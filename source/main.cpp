#include <fstream>
#include <cstdint>
#include <expected>
#include <iomanip>
#include <iostream>
#include <optional>
#include <vector>
#include "PortableExecutableHeaders.h"
#include "OffsetsFinder.h"
#include <unordered_map>
#include <asio.hpp>


[[nodiscard]]
std::expected<std::vector<uint8_t>, std::string> GetCodeSegment(const std::string& filePath)
{
    std::ifstream file(filePath, std::ios::binary);

    if (!file.is_open()) [[unlikely]]
        return std::unexpected("Cannot open file, try to check if path is valid");

    IMAGE_DOS_HEADER dosHeader{};
    file.read(reinterpret_cast<char*>(&dosHeader), sizeof(dosHeader));

    if (dosHeader.e_magic != 0x5A4D) [[unlikely]]
        return std::unexpected("File must be in PE32 format (.exe)");

    file.seekg(dosHeader.e_lfanew, std::ios::beg);

    IMAGE_NT_HEADERS32 ntHeaders{};
    file.read(reinterpret_cast<char*>(&ntHeaders), sizeof(ntHeaders));

    if (ntHeaders.Signature != 0x00004550) [[unlikely]]
        return std::unexpected("File must be in PE32 format (.exe)");

    constexpr size_t sizeOfSignature = 4;
    const auto offsetToSegmentTable = dosHeader.e_lfanew + ntHeaders.FileHeader.SizeOfOptionalHeader
                                      + sizeof(IMAGE_FILE_HEADER) + sizeOfSignature;

    file.seekg(static_cast<std::ifstream::off_type>(offsetToSegmentTable), std::ios::beg);

    for (size_t i = 0; i < ntHeaders.FileHeader.NumberOfSections; i++)
    {
        IMAGE_SECTION_HEADER currentSection{};
        file.read(reinterpret_cast<char*>(&currentSection), sizeof(IMAGE_SECTION_HEADER));

        if (std::string_view(currentSection.Name) != ".text")
            continue;

        std::vector<uint8_t> sectionData(currentSection.SizeOfRawData, 0);

        file.seekg(currentSection.PointerToRawData, std::ios::beg);
        file.read(reinterpret_cast<char*>(sectionData.data()), static_cast<std::streamsize>(sectionData.size()));

        return sectionData;
    }
    return std::unexpected("Executable does not contain .text segment!");
}

std::vector<std::pair<std::string, std::function<std::optional<uintptr_t>(const std::vector<uint8_t>&)>>> payloads
{
    {"LocalPlayer", GetLocalPlayerOffset},
    {"EnityList", GetEntityListOffset},
    {"ViewMatrix", GetViewMatrix},
    {"entity->m_teamNumber", GetTeamNumberOffset},
    {"entity->m_health", GetHealthOffset},
    {"entity->m_shield", GetShieldOffset},
    {"entity->m_absVelocity", GetAbsVelocityOffset},
    {"entity->m_lastVisibleTime", GetLastVisibleTimeOffset},
    {"entity->m_origin", GetOriginOffset},
    {"entity->m_boneMatrix", GetBoneMatrixOffset},
    {"entity->m_camerPosition", GetCameraPositionOffset},
    {"entity->m_viewAngles", GetViewAnglesOffset},
    {"entity->m_lifeState", GetLifeStateOffset},
    {"entity->m_bleedOutState", GetBleedOutStateOffset},
    {"entity->m_lastPrimaryWeapons", GetLastPrimaryWeaponOffset},
    {"entity->m_healthMax", GetMaxHealthOffset},

    {"weapon->m_bulletSpeedOffset", GetBulletSpeedOffset},
    {"weapon->m_bulletGravityScaleOffset", GetBulletScaleOffset},
};

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " /path/to/r5apex.exe\n";
        return 1;
    }
    std::cout << " ██▒   █▓ ▄▄▄       ██▓    ▓█████  ███▄    █  ▄████▄   ██▓ ▄▄▄\n";
    std::cout << "▓██░   █▒▒████▄    ▓██▒    ▓█   ▀  ██ ▀█   █ ▒██▀ ▀█  ▓██▒▒████▄\n";
    std::cout << " ▓██  █▒░▒██  ▀█▄  ▒██░    ▒███   ▓██  ▀█ ██▒▒▓█    ▄ ▒██▒▒██  ▀█▄\n";
    std::cout << "  ▒██ █░░░██▄▄▄▄██ ▒██░    ▒▓█  ▄ ▓██▒  ▐▌██▒▒▓▓▄ ▄██▒░██░░██▄▄▄▄██\n";
    std::cout << "   ▒▀█░   ▓█   ▓██▒░██████▒░▒████▒▒██░   ▓██░▒ ▓███▀ ░░██░ ▓█   ▓██▒\n";
    std::cout << "   ░ ▐░   ▒▒   ▓▒█░░ ▒░▓  ░░░ ▒░ ░░ ▒░   ▒ ▒ ░ ░▒ ▒  ░░▓   ▒▒   ▓▒█░\n";
    std::cout << "   ░ ░░    ▒   ▒▒ ░░ ░ ▒  ░ ░ ░  ░░ ░░   ░ ▒░  ░  ▒    ▒ ░  ▒   ▒▒ ░\n";
    std::cout << "     ░░    ░   ▒     ░ ░      ░      ░   ░ ░ ░         ▒ ░  ░   ▒\n";
    std::cout << "      ░        ░  ░    ░  ░   ░  ░         ░ ░ ░       ░        ░  ░\n";
    std::cout << "     ░                                       ░\n";
    std::cout << "                                                      Made by Orange\n";
    std::cout << "<=========================DUMPED OFFSETS===========================>\n";
    const auto segment = GetCodeSegment(argv[1]);

    if (!segment.has_value())
    {
        std::cerr << segment.error() << '\n';
        return -1;
    }
    std::unordered_map<std::string, std::optional<uintptr_t>> offsets;
    asio::thread_pool threadPool(std::thread::hardware_concurrency());
    std::mutex mtx;


    for (const auto& [offsetName, payload] : payloads)
        asio::post(threadPool,
            [&offsets, &mtx, &segment, &offsetName, &payload]
            {
                const auto offset = payload(segment.value());

                std::scoped_lock lock(mtx);
                offsets.emplace(offsetName, offset);
            }
            );

    threadPool.wait();
    for (const auto& [name, offset] : offsets)
    {
        std::stringstream stream;

        if (offset.has_value())
            stream << std::hex << offset.value();
        else
            stream << "N/A";

        std::cout << name << " = 0x" << stream.str() << '\n';
    }
}
