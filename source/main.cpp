#include <fstream>
#include <cstdint>
#include <expected>
#include <iomanip>
#include <iostream>
#include <vector>
#include "PortableExecutableHeaders.h"
#include "OffsetsDumper.h"

namespace valencia
{
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

    void PrintLogo()
    {
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
        std::cout << "                                                      Made by Orange";
    }

    void PrintOffsets(const std::string& category,
                      const std::unordered_map<std::string, std::optional<uintptr_t>>& offsets)
    {

        std::cout << "\n<===" << category << "===>\n";

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
}
int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " /path/to/r5apex.exe\n";
        return 1;
    }

    valencia::PrintLogo();

    auto segment = valencia::GetCodeSegment(argv[1]);

    if (!segment.has_value())
    {
        std::cerr << segment.error() << '\n';
        return -1;
    }

    valencia::OffsetsDumper dumper(std::move(segment.value()));

    valencia::PrintOffsets("GLOBAL", dumper.GetOffsetsOfGlobalObjects());
    valencia::PrintOffsets("BASE_ENTITY", dumper.GetOffsetsOfBaseEntityFileds());
    valencia::PrintOffsets("WEAPON", dumper.GetOffsetsOfWeaponFileds());
}
