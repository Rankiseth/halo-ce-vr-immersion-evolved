// Actual camera input functions: prevent headset/cinematic feedback and wrap yaw.
#include "HaloCEVR/Immersion/PhysicalInteractions.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <limits>
namespace Logger{std::ostringstream log;}
struct UnitDynamicObject{struct {unsigned id=0xffff;} parent;};
#define VR_PROFILE_SCOPE(x)
using InputBindingID=unsigned long long;
struct VRDouble{
    Vector2 stick{},physical{};unsigned polls=0;bool tracked=true;void UpdateInputs(){++polls;}bool IsHMDTracked()const{return tracked;}Matrix4 head;float yaw=0;unsigned recentres=0;void Recentre(){++recentres;head.translate(-(head*Vector3()));}
    Vector2 GetVector2Input(InputBindingID id){return id==1?physical:stick;}
    float GetYawOffset()const{return yaw;}void SetYawOffset(float v){yaw=v;}
    Matrix4 GetHMDTransform()const{return Matrix4(head).rotateZ(-yaw);}
};
using IVR=VRDouble;
struct InputHandler{
    char lastSnapState=0,cinematicSnapState=0;InputBindingID Look=0,PhysicalLook=1;unsigned menuPolls=0;
    bool wasCinematic=false,cinematicResetPending=false;
    bool cinematicLookNeedsNeutral=false,gameplayLookNeedsNeutral=false;
    void ObserveCinematicTransition();void UpdateCinematicTransition();bool AlignViewYaw(float);
    bool CinematicHandoffPending()const{return cinematicResetPending;}
    void UpdateMenuInput(){++menuPolls;}void ApplyViewTurn(Vector2,char&,float);void UpdateCinematicInput();void RecentreOrientation();unsigned vehicleCalls=0;
    void UpdateCamera(float&,float&);void UpdateCameraForVehicles(float& y,float& p){++vehicleCalls;y=.1f;p=.2f;}
};
namespace Helpers{UnitDynamicObject player;bool loading=false,menu=false;bool IsMouseVisible(){return menu;}
    void* GetLocalPlayer(){return &player;}bool IsCampaignLoading(){return loading;}
    struct Scene{bool bInCutscene=false;} scene;bool sceneAvailable=true;Scene* GetCutsceneData(){return sceneAvailable?&scene:nullptr;}
    struct Input{float yaw=0;} input;Input& GetInputData(){return input;}
    struct Camera{Vector3 lookDir{1,0,0};} camera;Camera& GetCamera(){return camera;}}
template<class T>struct Prop{T v;T Value()const{return v;}};
struct Game{
    static Game instance;VRDouble vr;InputHandler inputHandler;Immersion::VehicleExitGuard vehicleExit;
    struct Interactions{unsigned resets=0;void OnRecentre(){++resets;}bool active=false,overrideView=false;bool ViewOverrideActive()const{return overrideView;}bool VehicleActive()const{return active;}} interactions;
    Prop<bool> snap{false},firstPerson{true};Prop<bool>* c_FirstPersonVehicles=&firstPerson;Prop<float> amount{45},smooth{90};
    Prop<bool>* c_SnapTurn=&snap;Prop<float>* c_SnapTurnAmount=&amount,*c_SmoothTurnAmount=&smooth;
    bool bNeedsRecentre=false,bNeedsOrientationRecentre=false,bIgnoreNextRoomScaleMovement=false;
    // INSERT_ROOM_SCALE_RESET
    bool bInVehicle=false,bHasWeapon=true,bDetectedChimera=false;Vector3 LastLookDir{1,0,0};float lastDeltaTime=.01f;
    IVR* GetVR(){return &vr;}void UpdateCamera(float&,float&);
};
Game Game::instance;
// INSERT_CAMERA_INPUT
unsigned checks=0;
#define CHECK(x)do{++checks;if(!(x)){std::cerr<<__LINE__<<": " #x "\n";std::exit(1);}}while(false)
int main(){
    auto& g=Game::instance;float y=0,p=0;
    for(bool chimera:{false,true})for(float sign:{-1.f,1.f}){
        g.bDetectedChimera=chimera;g.vr.head=Matrix4().rotateZ(sign*179);
        Helpers::camera.lookDir=g.LastLookDir=Matrix4().rotateZ(-sign*179).getLeftAxis();
        g.UpdateCamera(y,p);CHECK(std::fabs(y+sign*2.f*3.14159265f/180.f)<.00001f);CHECK(p==0);
    }
    for(bool vehicle:{false,true}){
        g.interactions.active=vehicle;Helpers::scene.bInCutscene=true;g.inputHandler.UpdateCinematicTransition();g.vr.stick={1,1};
        const auto yaw=g.vr.yaw;const auto calls=g.inputHandler.vehicleCalls;y=p=100;g.UpdateCamera(y,p);
        CHECK(y==0 && p==0);CHECK(g.vr.yaw==yaw);CHECK(g.inputHandler.vehicleCalls==calls);
    }
    Helpers::scene.bInCutscene=false;g.inputHandler.UpdateCinematicTransition();g.UpdateCamera(y,p);CHECK(y==.1f && p==.2f);
    g.interactions.active=false;g.bInVehicle=false;g.vr.stick={};
    for(float fps:{60.f,72.f,90.f,120.f,144.f})for(bool chimera:{false,true}){
        g.vehicleExit.Reset();g.bDetectedChimera=chimera;g.vr.head=Matrix4().rotateZ(73).rotateY(25);g.vr.yaw=0;
        Helpers::player.parent.id=1;g.UpdateCamera(y,p); // Establish actual seat ownership.
        Helpers::player.parent.id=0xffff;
        unsigned frames=0;
        while(frames<unsigned(fps)){
            Helpers::camera.lookDir=g.LastLookDir=Matrix4().rotateZ(frames%2?170:-170).rotateY(frames%2?60:-60).getLeftAxis();
            g.UpdateCamera(y,p);CHECK(y==0 && p==0); // Animated reference never gets fed back.
            g.UpdateCamera(y,p);CHECK(y==0 && p==0); // Extra input hooks do not spend the cooldown.
            g.vehicleExit.Advance(1.f/fps);++frames;
        }
        g.vehicleExit.Advance(.01f);g.UpdateCamera(y,p);CHECK(y==0 && p==0);
        CHECK(Helpers::camera.lookDir.dot(g.vr.GetHMDTransform().getLeftAxis())>.9999f);
        CHECK(g.LastLookDir.dot(Helpers::camera.lookDir)>.9999f);
        g.UpdateCamera(y,p);CHECK(std::fabs(y)<.00001f && std::fabs(p)<.00001f);
        Helpers::player.parent.id=1;g.UpdateCamera(y,p);Helpers::player.parent.id=0xffff;g.UpdateCamera(y,p);
        CHECK(g.vehicleExit.Suppressing());Helpers::player.parent.id=1;g.UpdateCamera(y,p);CHECK(!g.vehicleExit.Suppressing());
        Helpers::loading=true;g.UpdateCamera(y,p);CHECK(!g.vehicleExit.Suppressing());Helpers::loading=false;
    }
    Helpers::scene.bInCutscene=true;g.interactions.active=false;g.inputHandler.UpdateCinematicTransition();
    for(float fps:{60.f,72.f,90.f,120.f,144.f}) {
        g.vr.physical={};g.inputHandler.UpdateCinematicInput();
        g.lastDeltaTime=1.f/fps;g.vr.yaw=0;g.vr.physical={1,1};g.vr.stick={-1,0};
        for(unsigned n=0;n<unsigned(fps);++n){g.inputHandler.UpdateCinematicInput();g.UpdateCamera(y,p);CHECK(y==0 && p==0);}
        CHECK(std::fabs(g.vr.yaw-90.f)<.002f); // Physical right stick, exactly once per frame.
    }
    CHECK(g.inputHandler.menuPolls>0); // Y recenter remains polled without gameplay callbacks.
    g.interactions.overrideView=true;const float seatYaw=g.vr.yaw;
    g.inputHandler.UpdateCinematicInput();CHECK(g.vr.yaw==seatYaw); // Pelican already owns seated look.
    g.interactions.overrideView=false;g.snap.v=true;g.vr.yaw=0;
    g.vr.physical={0,0};g.inputHandler.UpdateCinematicInput();g.vr.physical={1,0};
    for(unsigned n=0;n<10;++n){g.inputHandler.UpdateCinematicInput();}
    CHECK(g.vr.yaw==45);
    g.vr.physical={0,0};g.inputHandler.UpdateCinematicInput();g.vr.physical={-1,0};
    g.inputHandler.UpdateCinematicInput();CHECK(g.vr.yaw==0);
    Helpers::menu=true;g.inputHandler.UpdateCinematicInput();CHECK(g.vr.yaw==0);Helpers::menu=false;
    g.vr.head=Matrix4().rotateZ(123);g.vr.yaw=-47;
    g.inputHandler.RecentreOrientation();CHECK(g.vr.GetHMDTransform().getLeftAxis().dot({1,0,0})>.9999f);
    Helpers::scene.bInCutscene=false;Helpers::camera.lookDir={0,1,0};g.vr.yaw=5;
    g.inputHandler.RecentreOrientation();CHECK(g.vr.GetHMDTransform().getLeftAxis().dot({0,1,0})>.9999f);

    // Entry/exit can be observed by input before rendering, or by render first.
    // The mission's native heading intentionally disagrees with its last shot.
    for(bool chimera:{false,true})for(bool inputFirst:{false,true})for(float nativeYaw:{-179.f,-90.f,0.f,90.f,179.f}){
        g.inputHandler={};g.vr={};g.bDetectedChimera=chimera;g.interactions.active=false;
        g.bInVehicle=false;g.vehicleExit.Reset();Helpers::player.parent.id=0xffff;
        g.vr.head=Matrix4().rotateZ(123).translate(.3f,-.2f,1.7f);g.vr.yaw=-47;
        Helpers::scene.bInCutscene=true;Helpers::input.yaw=nativeYaw*.01745329252f;
        Helpers::camera.lookDir=Matrix4().rotateZ(nativeYaw+180).getLeftAxis();
        if(inputFirst){g.UpdateCamera(y,p);CHECK(y==0 && p==0);CHECK(g.inputHandler.CinematicHandoffPending());}
        g.inputHandler.UpdateCinematicTransition();
        CHECK(g.vr.recentres==1);CHECK((g.vr.head*Vector3()).length()<.0001f);
        g.snap.v=true;g.vr.physical={1,0};g.inputHandler.UpdateCinematicInput();
        CHECK(g.vr.GetHMDTransform().getLeftAxis().dot({1,0,0})>.99999f);
        g.vr.physical={};g.inputHandler.UpdateCinematicInput();
        CHECK(g.vr.GetHMDTransform().getLeftAxis().dot({1,0,0})>.99999f);
        g.vr.head.rotateZ(37);g.vr.yaw+=45; // Look/snap away during the movie.
        g.inputHandler.UpdateCinematicTransition();CHECK(g.vr.recentres==1);
        Helpers::scene.bInCutscene=false;
        if(inputFirst){const auto before=g.vr.yaw;g.UpdateCamera(y,p);CHECK(y==0 && p==0);CHECK(g.vr.yaw==before);}
        g.inputHandler.UpdateCinematicTransition();
        CHECK(g.vr.recentres==2);CHECK(!g.inputHandler.CinematicHandoffPending());
        CHECK(g.bIgnoreNextRoomScaleMovement);
        const auto expected=Matrix4().rotateZ(nativeYaw).getLeftAxis();
        CHECK(g.vr.GetHMDTransform().getLeftAxis().dot(expected)>.99999f);
        CHECK(g.LastLookDir.dot(expected)>.99999f);
        for(unsigned frame=0;frame<5;++frame){
            Helpers::camera.lookDir=expected;g.vr.stick=frame<3?Vector2(1,0):Vector2();g.UpdateCamera(y,p);
            CHECK(std::fabs(y)<.00001f && std::fabs(p)<.00001f);
            g.inputHandler.UpdateCinematicTransition();CHECK(g.vr.recentres==2);
        }
    }
    // Defer invalid poses/menus/loading; never spend the reset or leak old yaw.
    g.inputHandler={};g.vr={};Helpers::scene.bInCutscene=true;g.vr.tracked=false;
    g.inputHandler.UpdateCinematicTransition();CHECK(g.inputHandler.CinematicHandoffPending());CHECK(g.vr.recentres==0);
    g.vr.tracked=true;Helpers::sceneAvailable=false;g.inputHandler.UpdateCinematicTransition();CHECK(g.vr.recentres==0);
    Helpers::sceneAvailable=true;Helpers::menu=true;g.inputHandler.UpdateCinematicTransition();CHECK(g.vr.recentres==0);
    Helpers::menu=false;Helpers::loading=true;g.inputHandler.UpdateCinematicTransition();CHECK(g.vr.recentres==0);
    Helpers::loading=false;g.inputHandler.UpdateCinematicTransition();CHECK(g.vr.recentres==1);
    Helpers::scene.bInCutscene=false;Helpers::input.yaw=std::numeric_limits<float>::quiet_NaN();
    g.inputHandler.UpdateCinematicTransition();CHECK(g.inputHandler.CinematicHandoffPending());
    g.UpdateCamera(y,p);CHECK(y==0 && p==0);
    Helpers::input.yaw=1.f;g.inputHandler.UpdateCinematicTransition();CHECK(g.vr.recentres==2);
    g.firstPerson.v=false;g.bInVehicle=g.bHasWeapon=true;g.interactions.active=false;
    const auto vehicleCalls=g.inputHandler.vehicleCalls;g.UpdateCamera(y,p);
    CHECK(g.inputHandler.vehicleCalls==vehicleCalls+1);CHECK(y==.1f && p==.2f);
    std::cout<<checks<<" production cinematic input and wrapped yaw checks passed\n";
}
