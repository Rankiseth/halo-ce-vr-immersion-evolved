// Compile the complete Game HUD placement method. Native ray extraction and
// world/metre conversion are exercised separately by the adapter fixtures.
#include "HaloCEVR/Helpers/Camera.h"
#include "HaloCEVR/Maths/Matrices.h"
#include <cstdlib>
#include <iostream>
#define VR_PROFILE_SCOPE(x)
template<class T>struct Property{T value;T Value()const{return value;}};
namespace Helpers{
    Camera camera{};InputData input{};
    Camera& GetCamera(){return camera;}InputData& GetInputData(){return input;}
}
struct Game{
    struct Interactions{
        bool active=false,hidden=false,valid=false;Vector3 target{4,-1,-.5f};
        bool HideDrivingReticle()const{return hidden;}
        bool VehicleActive()const{return active;}
        Vector3 VehicleAim()const{return {1,0,0};}
        bool VehicleReticleTarget(float,Vector3& out)const{if(!active || !valid)return false;out=target;return true;}
    } interactions;
    struct Weapon{
        bool GetLocalWeaponAim(Vector3& p,Vector3& d,Vector3& u){p={1,2,3};d={0,1,0};u={0,0,1};return true;}
        bool GetLocalWeaponScope(Vector3&,Vector3&,Vector3&){return false;}
    } weaponHandler;
    struct VR{
        Matrix4 head;Matrix4 sight;unsigned draws=0;
        Matrix4 GetHMDTransform(bool)const{return head;}
        void SetCrosshairTransform(Matrix4& m){sight=m;++draws;}
    } instance,*vr=&instance;
    Property<float> distance{15};Property<bool> show{true},firstPerson{true};
    Property<bool>* c_FirstPersonVehicles=&firstPerson;
    Property<float>* c_CrosshairDistance=&distance;Property<bool>* c_ShowCrosshair=&show;
    bool bUse3DOFAiming=false,bInVehicle=false,bHasWeapon=true;
    bool ShouldUseOriginalScope(){return false;}
    void SetScopeTransform(Matrix4&,bool){}
    void UpdateCrosshairAndScope();
};
// INSERT_PRODUCTION_CROSSHAIR
int main(){
    unsigned checks=0;
    auto check=[&](bool pass){++checks;if(!pass){std::cerr<<"HUD placement check "<<checks<<" failed\n";std::exit(1);}};
    Game g;Helpers::input.zoomLevel=-1;g.interactions.active=g.interactions.valid=true;
    for(float lean:{0.f,.2f,-.3f}){
        g.instance.head=Matrix4().translate(lean,.5f,1.f);g.UpdateCrosshairAndScope();
        check((g.instance.sight*Vector3()-g.interactions.target).length()<.0001f);
    }
    g.interactions.valid=false;g.UpdateCrosshairAndScope();
    check((g.instance.sight*Vector3()-Vector3(14.7f,.5f,1.f)).length()<.0001f);
    g.interactions.active=false;g.UpdateCrosshairAndScope();
    check((g.instance.sight*Vector3()-Vector3(1,17,3)).length()<.0001f);
    g.bUse3DOFAiming=true;g.UpdateCrosshairAndScope();
    check((g.instance.sight*Vector3()-Vector3(-.3f,15.5f,1)).length()<.0001f);
    g.bUse3DOFAiming=false;g.bInVehicle=g.bHasWeapon=true;g.firstPerson.value=false;
    Helpers::camera.lookDir={0,1,0};g.UpdateCrosshairAndScope();
    check((g.instance.sight*Vector3()-Vector3(-.3f,15.5f,1)).length()<.0001f);
    const auto draws=g.instance.draws;g.interactions.hidden=true;g.UpdateCrosshairAndScope();check(g.instance.draws==draws);
    g.interactions.hidden=false;g.show.value=false;g.UpdateCrosshairAndScope();check(g.instance.draws==draws);
    std::cout<<checks<<" production vehicle HUD placement/fallback checks passed\n";
}
