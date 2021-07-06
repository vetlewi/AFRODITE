//
// Created by Vetle Wegner Ingeberg on 05/07/2021.
//

#include <doctest/doctest.h>

#include <user/DetectorSetupMessenger.hh>

#include <G4UImanager.hh>
#include <G4SystemOfUnits.hh>
#include <G4UIcommandStatus.hh>

TEST_CASE("Test of UI class")
{
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    SUBCASE("Test apply type"){
        DetectorSetupMessenger *messenger = new DetectorSetupMessenger;

        auto std = messenger->GetStdSlot();
        REQUIRE_EQ(std[3].type, Detector::Type::clover);

        REQUIRE_EQ(UImanager->ApplyCommand("/Detector/gamma/std/type 3 ocl_labr"), 0);
        std = messenger->GetStdSlot();
        REQUIRE_EQ(std[3].type, Detector::Type::ocl_labr);

        auto smll = messenger->GetSmallSlot();
        REQUIRE_EQ(smll[0].type, Detector::Type::fta_labr);

        REQUIRE_EQ(UImanager->ApplyCommand("/Detector/gamma/small/type 0 none"), 0);
        smll = messenger->GetSmallSlot();
        REQUIRE_EQ(smll[0].type, Detector::Type::none);

        REQUIRE_EQ(UImanager->ApplyCommand("/Detector/particle/present 1 false"), 0);
        auto prt = messenger->GetParticleArray();
        REQUIRE_EQ(prt[1].type, Detector::Type::none);

        REQUIRE_EQ(UImanager->ApplyCommand("/Detector/particle/present 1 true"), 0);
        prt = messenger->GetParticleArray();
        REQUIRE_EQ(prt[1].type, Detector::Type::s2_silicon);

        REQUIRE_EQ(UImanager->ApplyCommand("/Detector/particle/thickness 1 1001.5 mm"), 0);
        prt = messenger->GetParticleArray();
        REQUIRE_EQ(prt[1].thickness, 1001.5*mm);

        REQUIRE_GE(UImanager->ApplyCommand("/Detector/gamma/std/type 33 ocl_labr"), 300);
        REQUIRE_GE(UImanager->ApplyCommand("/Detector/gamma/std/type 3 ldsl"), 300);

        REQUIRE_GE(UImanager->ApplyCommand("/Detector/gamma/small/type 33 none"), 300);
        REQUIRE_GE(UImanager->ApplyCommand("/Detector/gamma/small/type 3 ldsl"), 300);

        delete messenger;

    }

    SUBCASE("Apply distance"){
        DetectorSetupMessenger *messenger = new DetectorSetupMessenger;

        auto std = messenger->GetStdSlot();
        auto dflt_std_value = std[3].distance;

        auto small = messenger->GetSmallSlot();
        auto dflt_small_value = small[3].distance;

        auto particle = messenger->GetParticleArray();
        auto dflt_part_value = particle[1].distance;

        REQUIRE_EQ(UImanager->ApplyCommand("/Detector/gamma/std/distance 3 24 cm"), 0);
        std = messenger->GetStdSlot();
        REQUIRE_EQ(std[3].distance, 24.*cm);
        REQUIRE_NE(std[3].distance, dflt_std_value);

        REQUIRE_EQ(UImanager->ApplyCommand("/Detector/gamma/small/distance 3 24 cm"), 0);
        small = messenger->GetSmallSlot();
        REQUIRE_EQ(small[3].distance, 24.*cm);
        REQUIRE_NE(small[3].distance, dflt_small_value);

        REQUIRE_EQ(UImanager->ApplyCommand("/Detector/particle/distance 1 55.5 cm"), 0);
        particle = messenger->GetParticleArray();
        REQUIRE_EQ(particle[1].distance, 55.5*cm);
        REQUIRE_NE(particle[1].distance, dflt_part_value);

        REQUIRE_EQ(UImanager->ApplyCommand("/Detector/particle/distance 1 -55.5 cm"), 0);
        particle = messenger->GetParticleArray();
        REQUIRE_EQ(particle[1].distance, -55.5*cm);
        REQUIRE_NE(particle[1].distance, dflt_part_value);

        REQUIRE_GE(UImanager->ApplyCommand("/Detector/gamma/std/distance 3 -24 cm"), 300);
        REQUIRE_GE(UImanager->ApplyCommand("/Detector/gamma/std/distance 32 24 cm"), 300);
        REQUIRE_GE(UImanager->ApplyCommand("/Detector/gamma/std/distance 3 24 GeV"), 300);
        REQUIRE_GE(UImanager->ApplyCommand("/Detector/gamma/small/distance 3 -24 cm"), 300);
        REQUIRE_GE(UImanager->ApplyCommand("/Detector/gamma/small/distance 32 24 cm"), 300);
        REQUIRE_GE(UImanager->ApplyCommand("/Detector/gamma/small/distance 3 24 GeV"), 300);
        REQUIRE_GE(UImanager->ApplyCommand("/Detector/particle/distance 3 24 cm"), 300);
        REQUIRE_GE(UImanager->ApplyCommand("/Detector/particle/distance 0 24 GeV"), 300);

        delete messenger;

    }

    SUBCASE("Apply thickness"){
        DetectorSetupMessenger *messenger = new DetectorSetupMessenger;

        auto prt = messenger->GetParticleArray();
        auto prt_dflt_value = prt[1].thickness;

        REQUIRE_EQ(UImanager->ApplyCommand("/Detector/particle/thickness 1 1001.5 mm"), 0);
        prt = messenger->GetParticleArray();
        REQUIRE_EQ(prt[1].thickness, 1001.5*mm);
        REQUIRE_NE(prt[1].thickness, prt_dflt_value);

        REQUIRE_GE(UImanager->ApplyCommand("/Detector/particle/thickness 1 -1001.5 mm"), 300);
        REQUIRE_GE(UImanager->ApplyCommand("/Detector/particle/thickness 10 1001.5 mm"), 300);
        REQUIRE_GE(UImanager->ApplyCommand("/Detector/particle/thickness 1 1001.5 GeV"), 300);

        delete messenger;
    }

    SUBCASE("Disable and enable target"){
        DetectorSetupMessenger *messenger = new DetectorSetupMessenger;

        REQUIRE(messenger->TargetPresent());
        REQUIRE_EQ(UImanager->ApplyCommand("/Detector/target false"), 0);
        REQUIRE_FALSE(messenger->TargetPresent());
        REQUIRE_EQ(UImanager->ApplyCommand("/Detector/target true"), 0);
        REQUIRE(messenger->TargetPresent());

        delete messenger;

    }

    SUBCASE("Disable and enable frame"){
        DetectorSetupMessenger *messenger = new DetectorSetupMessenger;

        REQUIRE(messenger->FramePresent());
        REQUIRE_EQ(UImanager->ApplyCommand("/Detector/frame false"), 0);
        REQUIRE_FALSE(messenger->FramePresent());
        REQUIRE_EQ(UImanager->ApplyCommand("/Detector/frame true"), 0);
        REQUIRE(messenger->FramePresent());

        delete messenger;
    }

    SUBCASE("Disable and enable target chamber"){
        DetectorSetupMessenger *messenger = new DetectorSetupMessenger;

        REQUIRE(messenger->TargetChamberPresent());
        REQUIRE_EQ(UImanager->ApplyCommand("/Detector/targetchamber false"), 0);
        REQUIRE_FALSE(messenger->TargetChamberPresent());
        REQUIRE_EQ(UImanager->ApplyCommand("/Detector/targetchamber true"), 0);
        REQUIRE(messenger->TargetChamberPresent());

        delete messenger;
    }

}