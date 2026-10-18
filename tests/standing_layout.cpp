#include "HaloCEVR/Immersion/HolsterMarker.h"
#include "HaloCEVR/UI/SettingsPresentation.h"
#include <climits>
#include <iostream>
#include <cstdlib>
using namespace Immersion;
static unsigned checks=0;
#define CHECK(x) do { ++checks; if(!(x)) { std::cerr<<__LINE__<<": " #x "\n";std::exit(1); } } while(false)
bool same(Point a,Point b){return (a-b).LengthSquared()<1.e-10f;}
int main(){
    CHECK(!Layout{}.standing);CHECK(Layout{}.heightCm==180);
    CHECK(Layout::ClampHeight(INT_MIN)==120);CHECK(Layout::ClampHeight(INT_MAX)==220);
    for(bool left:{false,true}) for(int cm:{-100,120,129,150,180,200,220,500}) for(float yaw:{-3.f,-1.f,0.f,1.f,3.f}){
        const Point head{1,2,3};const Layout base{left},sitting{left,false,cm},standing{left,true,cm};
        for(auto zone:{Zone::Shoulder,Zone::WeaponHip,Zone::SupplyHip,Zone::PlasmaChest,Zone::FragChest})
            CHECK(same(base.Anchor(zone,head,yaw),sitting.Anchor(zone,head,yaw)));
        for(auto zone:{Zone::Shoulder,Zone::PlasmaChest,Zone::FragChest})
            CHECK(same(base.Anchor(zone,head,yaw),standing.Anchor(zone,head,yaw)));
        const float drop=standing.BeltDepth()-.43f;CHECK(drop>=0);
        for(auto zone:{Zone::WeaponHip,Zone::SupplyHip})
            CHECK(same(standing.Anchor(zone,head,yaw),base.Anchor(zone,head,yaw)+Point{0,0,-drop}));
        for(unsigned home:{0u,1u})
            CHECK(same(HolsterMarkerAnchor(home,standing,head,yaw),HolsterMarkerAnchor(home,base,head,yaw)+Point{0,0,-drop}));
        CHECK(same(HolsterMarkerAnchor(1,standing,head,yaw),standing.Anchor(Zone::WeaponHip,head,yaw)));
    }
    const Layout calibrated{false,true,180};
    CHECK(std::fabs(calibrated.Offset(Zone::WeaponHip).z+.60f)<1.e-6f);
    CHECK(std::fabs(calibrated.Offset(Zone::SupplyHip).z+.60f)<1.e-6f);
    CHECK(SettingsPresentation::Label("StylishStandingMode")=="Play posture");
    CHECK(SettingsPresentation::Label("StylishPlayerHeightCm")=="Player height (cm)");
    std::cout<<checks<<" posture calibration, fixed shoulder, mirrored zones and display-anchor checks passed\n";
}
