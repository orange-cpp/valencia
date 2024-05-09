//
// Created by vlad on 5/8/24.
//

#pragma once
#include <string>


namespace signatures
{
    const std::string LocalPlayer = "48 8B 05 ? ? ? ? 48 8D 0D ? ? ? ? 44 88 2D"; // + 0x8
    const std::string entityList = "48 8D 35 ? ? ? ? 0F 84";
    const std::string viewProjectionMatrix = "48 8D 0D ? ? ? ? FF 90 ? ? ? ? 41 8B" ; // + 0x10

    const std::string m_iTeamNumber = "8B 87 ? ? ? ? 89 87 ? ? ? ? 33 C0 8B 15";
    const std::string m_iHealth = "44 8B 83 ? ? ? ? 45 85 C0 44 0F 48 C7 41 8B D0 2B 93 ? ? ? ? 85 D2";
    const std::string m_iShield = "39 87 ? ? ? ? 0F 84 ? ? ? ? 48 8B 07 48 8B CF FF 90 ? ? ? ? 44 8B 97";
    const std::string m_vecAbsVelocity = "F3 0F 10 9F ? ? ? ? F3 0F 10 97 ? ? ? ? F3 0F 59 DB";
    const std::string m_lastVisibleTime = " F3 42 0F 10 84 91 ? ? ? ? 0F 2F C6 77 6A";
    const std::string m_vecOrigin = "F3 0F 7E 97";
    const std::string m_boneMatrix = "89 81 ? ? ? ? C3 CC 48 89 6C 24"; // + 0x44
    const std::string m_cameraPos = "0F 2E 89 ? ? ? ? 7A 58";
    const std::string m_viewAngles = "F2 0F 10 B6 ? ? ? ? F3 41 0F 58 D3";
    const std::string m_lifeState = "80 B9 ? ? ? ? ? 75 25 48 8B 05 ? ? ? ? F3 0F 10 48";
    const std::string m_maxHealth = "41 FF 92 ? ? ? ? 84 C0 0F B6 CB 0F 44 CD 0F B6 C1";
    const std::string m_bleedOutState = "39 B8 ? ? ? ? 0F 45 F1 48 8B 03 48 8B CB FF 90";
    const std::string m_latestPrimaryWeapons = "8B 94 81 ? ? ? ? 83 FA FF 74 1D 0F B7 DA 48 8D 05";
    const std::string m_fProjectileBulletScale = "F3 0F 10 93 ? ? ? ? F3 0F 5E C1 F3 0F 59 50 ? F3 0F 59 15 ? ? ? ? F3 0F 59 D0 F3 0F 5E D1";
    const std::string m_projectileBulletSpeed = "F3 0F 59 B3 ? ? ? ? 0F 29 BC 24";
    const std::string m_nameOffset = "48 8D 8E ? ? ? ? 49 8B D3 E8 ? ? ? ? 85 C0 74 21";
    const std::string m_indexInNameList = "41 03 AE ? ? ? ? FF 92 ? ? ? ? 41 8B 8E ? ? ? ? 48 8B 3D ? ? ? ? 83 C1 02";
    const std::string nameList = "48 8D 05 ? ? ? ? 48 8B 54 D0 ? E8 ? ? ? ? C6 45 4F 00 F3 45 0F 10 86";
    const std::string clientState = "48 8D 0D ? ? ? ? E8 ? ? ? ? F3 0F 58 C1 F3 0F 11 42";
    const std::string networkChannel = "48 8B 0D ? ? ? ? 48 8D 54 24 ? 45 33 C9 C6 84 24 ? ? ? ? ? 45 33 C0";
    const std::string latestOffHandWeapons = "48 0F BE 94 07 ? ? ? ? 80 FA FF";
}