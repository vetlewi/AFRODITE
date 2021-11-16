//
// Created by Vetle Wegner Ingeberg on 28/06/2021.
//

#include "detector/DetectorFactories.hh"

#include "detector/CloverFactory.hh"
#include "detector/FTALaBr3.hh"
#include "detector/OCLLaBr3.hh"
#include "detector/S2Factory.hh"

#include <G4Exception.hh>
#include <magic_enum.hpp>

using namespace Detector;

DetectorFactories::DetectorFactories()
    : factoryMapping{
        {Type::none, nullptr},
        {Type::clover, new CloverFactory},
        {Type::fta_labr, new FTALaBr3},
        {Type::ocl_labr, new OCLLaBr3},
        {Type::alba_labr, nullptr},
        {Type::s2_silicon, new S2Factory}}
{
}

DetectorFactories::~DetectorFactories()
{
    for (auto & factory : factoryMapping){
        delete factory.second;
    }
}

DetectorFactory *DetectorFactories::GetFactory(const Type &type) const
{
    auto it = factoryMapping.find(type);
    if ( it == factoryMapping.end() ){
        G4Exception("DetectorFactories::GetFactory", "Not found",
                    G4ExceptionSeverity::FatalErrorInArgument,
                    "Could not find detector");
    }
    return it->second;
}

DetectorFactory *DetectorFactories::GetFactory(const char *type) const
{
    return GetFactory(magic_enum::enum_cast<Detector::Type>(type).value());
}