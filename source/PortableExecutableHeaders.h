//
// Created by vlad on 5/8/24.
//

#pragma once
#include <cstdint>

#pragma pack(push, 1)

// Structure definitions for PE headers without using windows.h
struct IMAGE_DOS_HEADER {      // DOS .EXE header
    uint16_t e_magic;          // Magic number
    uint16_t e_cblp;           // Bytes on last page of file
    uint16_t e_cp;             // Pages in file
    uint16_t e_crlc;           // Relocations
    uint16_t e_cparhdr;        // Size of header in paragraphs
    uint16_t e_minalloc;       // Minimum extra paragraphs needed
    uint16_t e_maxalloc;       // Maximum extra paragraphs needed
    uint16_t e_ss;             // Initial (relative) SS value
    uint16_t e_sp;             // Initial SP value
    uint16_t e_csum;           // Checksum
    uint16_t e_ip;             // Initial IP value
    uint16_t e_cs;             // Initial (relative) CS value
    uint16_t e_lfarlc;         // File address of relocation table
    uint16_t e_ovno;           // Overlay number
    uint16_t e_res[4];         // Reserved words
    uint16_t e_oemid;          // OEM identifier (for e_oeminfo)
    uint16_t e_oeminfo;        // OEM information; e_oemid specific
    uint16_t e_res2[10];       // Reserved words
    int32_t  e_lfanew;         // File address of new exe header
};

struct IMAGE_FILE_HEADER {
    uint16_t Machine;
    uint16_t NumberOfSections;
    uint32_t TimeDateStamp;
    uint32_t PointerToSymbolTable;
    uint32_t NumberOfSymbols;
    uint16_t SizeOfOptionalHeader;
    uint16_t Characteristics;
};

struct IMAGE_DATA_DIRECTORY {
    uint32_t VirtualAddress;
    uint32_t Size;
};

struct IMAGE_OPTIONAL_HEADER32 {
    uint16_t Magic;
    uint8_t MajorLinkerVersion;
    uint8_t MinorLinkerVersion;
    uint32_t SizeOfCode;
    uint32_t SizeOfInitializedData;
    uint32_t SizeOfUninitializedData;
    uint32_t AddressOfEntryPoint;
    uint32_t BaseOfCode;
    uint32_t BaseOfData;
    uint32_t ImageBase;
    uint32_t SectionAlignment;
    uint32_t FileAlignment;
    uint16_t MajorOperatingSystemVersion;
    uint16_t MinorOperatingSystemVersion;
    uint16_t MajorImageVersion;
    uint16_t MinorImageVersion;
    uint16_t MajorSubsystemVersion;
    uint16_t MinorSubsystemVersion;
    uint32_t Win32VersionValue;
    uint32_t SizeOfImage;
    uint32_t SizeOfHeaders;
    uint32_t CheckSum;
    uint16_t Subsystem;
    uint16_t DllCharacteristics;
    uint32_t SizeOfStackReserve;
    uint32_t SizeOfStackCommit;
    uint32_t SizeOfHeapReserve;
    uint32_t SizeOfHeapCommit;
    uint32_t LoaderFlags;
    uint32_t NumberOfRvaAndSizes;
    IMAGE_DATA_DIRECTORY DataDirectory[16];
};

struct IMAGE_NT_HEADERS32 {
    uint32_t Signature;
    IMAGE_FILE_HEADER FileHeader;
    IMAGE_OPTIONAL_HEADER32 OptionalHeader;
};

struct IMAGE_SECTION_HEADER {
    char Name[8];
    uint32_t PhysicalAddress;
    uint32_t VirtualAddress;
    uint32_t SizeOfRawData;
    uint32_t PointerToRawData;
    uint32_t PointerToRelocations;
    uint32_t PointerToLinenumbers;
    uint16_t NumberOfRelocations;
    uint16_t NumberOfLinenumbers;
    uint32_t Characteristics;
};

#pragma pack(pop)