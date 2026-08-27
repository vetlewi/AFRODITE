//
// Created by Vetle Wegner Ingeberg on 04/05/2021.
//

#include <Frame/DetectorFrame.hh>

#include <cmath>

#include <G4SystemOfUnits.hh>

namespace {

//! Convert an FTA-native direction (polar angle measured from the +y axis, as
//! the FTA placement code historically expressed it) into the standard physical
//! convention used by Slot (polar angle from +z, azimuth from +x). Used only to
//! seed the small-slot directions from the values that previously lived in
//! DetectorConstruction; placement re-derives the FTA orientation from these.
void ftaNativeToPhysical(double theta_n, double phi_n, double &theta, double &phi)
{
    const double x = -std::sin(theta_n) * std::cos(phi_n);
    const double y = std::cos(theta_n);
    const double z = std::sin(theta_n) * std::sin(phi_n);
    theta = std::acos(z);
    phi = std::atan2(y, x);
}

} // namespace

DetectorFrame::DetectorFrame()
{
    // ---------------------------------------------------------------------
    // Fixed frame geometry: the direction of every slot. These are the
    // physical directions (theta from +z beam axis, phi from +x). The values
    // below are the positions recovered from the historical hardcoded setup;
    // the four marked TODO are not yet known. Update these once the real frame
    // positions are confirmed. All slots default to empty (Type::none) -- use
    // the /AFRODITE/ UI commands to populate them.
    //
    // Large slots (0..15): clover / ocl_labr / fta_labr.
    const double large_angles[num_large][2] = {
        // theta,        phi          (previous occupant)
        {  90. * deg,    0. * deg },  // 0  clover
        {  90. * deg,   45. * deg },  // 1  clover
        {  90. * deg,  135. * deg },  // 2  clover
        {  90. * deg,  180. * deg },  // 3  clover
        {  90. * deg,  315. * deg },  // 4  clover
        { 135. * deg,    0. * deg },  // 5  clover
        { 135. * deg,   90. * deg },  // 6  clover
        { 135. * deg,  180. * deg },  // 7  clover
        {  45. * deg,    0. * deg },  // 8  ocl_labr
        {  45. * deg,   90. * deg },  // 9  ocl_labr
        {  45. * deg,  180. * deg },  // 10 ocl_labr
        {  45. * deg,  270. * deg },  // 11 ocl_labr
        {  90. * deg,  225. * deg },  // 12 ocl_labr
        { 135. * deg,  270. * deg },  // 13 ocl_labr
        {   0. * deg,    0. * deg },  // 14 TODO: confirm direction
        {   0. * deg,    0. * deg },  // 15 TODO: confirm direction
    };
    for (int i = 0; i < num_large; ++i) {
        large_slots[i].theta = large_angles[i][0];
        large_slots[i].phi = large_angles[i][1];
    }

    // Small slots (0..7): fta_labr only. Directions 0..5 are the previous FTA
    // positions (given here in the FTA-native convention and converted), 6..7
    // are not yet known.
    const double small_native[num_small][2] = {
        // theta_n,      phi_n        (previous FTA slot)
        {  55. * deg,   45. * deg },  // 0
        {  55. * deg,  135. * deg },  // 1
        {  55. * deg,  225. * deg },  // 2
        { 125. * deg,  225. * deg },  // 3
        {  55. * deg,  315. * deg },  // 4
        { 125. * deg,  315. * deg },  // 5
        {   0. * deg,    0. * deg },  // 6  TODO: confirm direction
        {   0. * deg,    0. * deg },  // 7  TODO: confirm direction
    };
    for (int i = 0; i < num_small; ++i) {
        ftaNativeToPhysical(small_native[i][0], small_native[i][1],
                            small_slots[i].theta, small_slots[i].phi);
    }
}

bool DetectorFrame::largeAllows(Detector::Type type)
{
    switch (type) {
        case Detector::Type::none:
        case Detector::Type::clover:
        case Detector::Type::ocl_labr:
        case Detector::Type::fta_labr:
            return true;
        default:
            return false;
    }
}

bool DetectorFrame::smallAllows(Detector::Type type)
{
    return type == Detector::Type::none || type == Detector::Type::fta_labr;
}
