//
// Created by Vetle Wegner Ingeberg on 28/06/2021.
//

#include <doctest/doctest.h>
#include <detector/DetectorFactories.hh>

TEST_CASE("Detector")
{
    Detector::DetectorFactories factories;

    SUBCASE("none type"){
        REQUIRE_EQ(factories.GetFactory(Detector::Type::none), nullptr);
        REQUIRE_EQ(factories.GetFactory("none"), nullptr);
        REQUIRE_EQ(factories.GetFactory(Detector::Type::none), factories.GetFactory("none"));
    }

    SUBCASE("clover type"){
        REQUIRE_NE(factories.GetFactory(Detector::Type::clover), nullptr);
        REQUIRE_NE(factories.GetFactory("clover"), nullptr);
        REQUIRE_EQ(factories.GetFactory(Detector::Type::clover), factories.GetFactory("clover"));
    }

    SUBCASE("fta_labr type"){
        REQUIRE_NE(factories.GetFactory(Detector::Type::fta_labr), nullptr);
        REQUIRE_NE(factories.GetFactory("fta_labr"), nullptr);
        REQUIRE_EQ(factories.GetFactory(Detector::Type::fta_labr), factories.GetFactory("fta_labr"));
    }

    SUBCASE("ocl_labr type"){
        REQUIRE_NE(factories.GetFactory(Detector::Type::ocl_labr), nullptr);
        REQUIRE_NE(factories.GetFactory("ocl_labr"), nullptr);
        REQUIRE_EQ(factories.GetFactory(Detector::Type::ocl_labr), factories.GetFactory("ocl_labr"));
    }

    SUBCASE("alba_labr type"){
        REQUIRE_EQ(factories.GetFactory(Detector::Type::alba_labr), nullptr);
        REQUIRE_EQ(factories.GetFactory("alba_labr"), nullptr);
        REQUIRE_EQ(factories.GetFactory(Detector::Type::alba_labr), factories.GetFactory("alba_labr"));
    }

    SUBCASE("s2_silicon type"){
        REQUIRE_NE(factories.GetFactory(Detector::Type::s2_silicon), nullptr);
        REQUIRE_NE(factories.GetFactory("s2_silicon"), nullptr);
        REQUIRE_EQ(factories.GetFactory(Detector::Type::s2_silicon), factories.GetFactory("s2_silicon"));
    }
}