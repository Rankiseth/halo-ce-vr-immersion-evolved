// Included inside namespace Immersion by RetailWeaponEngine.cpp. Uses the same
// checked object/tag readers and verified native rendering/position routines.
namespace {
bool LocalBiped(Object& unit,std::array<unsigned char,0x328>& body,std::uintptr_t& address){
    std::uint32_t mode=1,table=0,players=0;std::array<unsigned char,0x38> header{},player{};
    if(!ReadAt(0x6f1d20,mode) || mode!=0 || !ReadAt(0x87a480,table) || !ReadAt(table,header) ||
        Field<std::uint16_t>(header,0x22)!=0x200 || !Field<std::uint16_t>(header,0x30) ||
        !Field<std::uint16_t>(header,0x20) || Field<std::uint16_t>(header,0x20)>16)return false;
    players=Field<std::uint32_t>(header,0x34);
    if(!ReadAt(players,player) || !Field<std::uint16_t>(player,0) || Field<std::uint16_t>(player,2)!=0)return false;
    unit=Field<Object>(player,0x34);
    return UnitBytes(unit,body,address) && std::isfinite(Field<float>(body,0xe0)) && !(Field<std::uint16_t>(body,0x106)&4u) &&
        Field<Object>(body,0x218)==(static_cast<Object>(Field<std::uint16_t>(player,0))<<16);
}
bool TagPath(Object tag,char (&path)[256]){
    std::uint32_t table=0,text=0;Object check=None;
    if(tag==None || !ReadAt(0x87bc14,table) || !ReadAt(table+(tag&0xffffu)*0x20+0xc,check) || check!=tag ||
        !ReadAt(table+(tag&0xffffu)*0x20+0x10,text))return false;
    if(Copy(text,path,sizeof(path)))return std::memchr(path,0,sizeof(path))!=nullptr;
    for(unsigned i=0;i<255;++i){if(!ReadAt(text+i,path[i]))return false;if(!path[i])return true;}
    return false;
}
bool GrenadeModel(unsigned type,Object& model){
    std::uint32_t globals=0,count=0,entries=0,data=0;Object projectile=None;
    return type<2 && ReadAt(0x746fa0,globals) && ReadAt(globals+0x128,count) && count>=2 && count<=4 &&
        ReadAt(globals+0x12c,entries) && ReadAt(entries+type*0x44+0x40,projectile) &&
        TagData(projectile,0x70726f6au,data) && ReadAt(data+0x34,model) && model!=None;
}
bool LiveObjectNodes(Object unit,const std::array<unsigned char,0x328>& body,std::uintptr_t address,
    std::array<ModelNode,64>& nodes,std::array<Transform,64>& transforms,unsigned& count,std::uint32_t group){
    std::uint32_t data=0;Object model=None;
    if(!TagData(Field<Object>(body,0),group,data) || !ReadAt(data+0x34,model) || !ModelNodes(model,nodes,count))return false;
    const auto offset=Field<std::int16_t>(body,0x1f2);
    std::uint32_t table=0,entries=0;std::array<unsigned char,12> entry{};
    if(offset<0x1f4 || !ReadAt(0x8603b0,table) || !ReadAt(table+0x34,entries) ||
        !ReadAt(entries+(unit&0xffffu)*12,entry) || Field<std::uint16_t>(entry,0)!=(unit>>16) ||
        Field<std::uint32_t>(entry,8)!=address || unsigned(offset)+count*sizeof(Transform)>Field<std::uint16_t>(entry,6) ||
        !Copy(address+offset,transforms.data(),count*sizeof(Transform)))return false;
    for(unsigned i=0;i<count;++i){const auto m=ModelMatrix(transforms[i]);
        for(unsigned j=0;j<16;++j)if(!std::isfinite(m[j]))return false;
        if(std::fabs(m.getDeterminant())<.00001f)return false;
    }
    return true;
}
bool LiveBipedNodes(Object unit,const std::array<unsigned char,0x328>& body,std::uintptr_t address,
    std::array<ModelNode,64>& nodes,std::array<Transform,64>& transforms,unsigned& count){
    return LiveObjectNodes(unit,body,address,nodes,transforms,count,0x62697064u);
}
bool RenderedRoot(const ModelNode* nodes,const Transform* transforms,unsigned count,Matrix4& result){
    for(unsigned i=0;i<count;++i)if(nodes[i].parent<0){
        result=ModelMatrix(transforms[i])*ModelMatrix(nodes[i].inverseBind);
        // Cancel the root's bind offset before using it as the chassis frame.
        // Refuse degenerate/scaled layouts rather than distort the seat.
        for(unsigned axis=0;axis<3;++axis){
            const Vector3 v(result[axis*4],result[axis*4+1],result[axis*4+2]);
            if(v.lengthSqr()<.5f || v.lengthSqr()>1.5f)return false;
        }
        return true;
    }
    return false;
}
// Retail mod2 compiled marker layout (Invader model.json). Positions are
// NODE-local: never apply the root or inverse bind a second time. Keep reads
// bounded and match the live object's region permutations before averaging
// paired emitters such as the Banshee's plasma guns.
struct VehicleMarker {Point origin{},forward{};bool valid=false;};
bool ReadVehicleMarker(Object model,const std::array<unsigned char,0x328>& body,
    const Transform* transforms,unsigned nodeCount,const char* wanted,VehicleMarker& out){
    out={};std::uint32_t data=0,count=0,markers=0;
    if(!TagData(model,0x6d6f6432u,data) || !ReadAt(data+0xac,count) || count>256 ||
        !ReadAt(data+0xb0,markers))return false;
    for(unsigned i=0;i<count;++i){
        std::array<unsigned char,0x40> marker{};
        if(!ReadAt(static_cast<std::uintptr_t>(markers)+i*0x40,marker))return false;
        char name[33]{};std::memcpy(name,marker.data(),32);
        for(char& c:name)c=static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        if(std::strcmp(name,wanted))continue;
        const auto instances=Field<std::uint32_t>(marker,0x34),address=Field<std::uint32_t>(marker,0x38);
        if(!instances || instances>32)return false;
        Vector3 position{},direction{},firstDirection{};unsigned matched=0;
        for(unsigned j=0;j<instances;++j){
            std::array<unsigned char,0x20> instance{};
            if(!ReadAt(static_cast<std::uintptr_t>(address)+j*0x20,instance))return false;
            const unsigned region=instance[0],permutation=instance[1],node=instance[2];
            if(region!=255 && (region>=8 || body[0x180+region]!=permutation))continue;
            if(node>=nodeCount)return false;
            const auto p=Field<Point>(instance,4);const auto q=Field<Vector4>(instance,0x10);
            const float length=q.x*q.x+q.y*q.y+q.z*q.z+q.w*q.w;
            if(!p.Finite() || p.LengthSquared()>400.f || !std::isfinite(length) || length<.5f || length>1.5f)return false;
            Transform local{};Helpers::MakeTransformFromQuat(&q,&local);local.translation={p.x,p.y,p.z};
            const Matrix4 world=ModelMatrix(transforms[node])*ModelMatrix(local);
            const Vector3 muzzle=world*Vector3(),forward=world.getLeftAxis().normalize();
            if(!std::isfinite(muzzle.x) || !std::isfinite(muzzle.y) || !std::isfinite(muzzle.z) ||
                !std::isfinite(forward.x) || !std::isfinite(forward.y) || !std::isfinite(forward.z))return false;
            if(matched && forward.dot(firstDirection)<.9f)return false;
            if(!matched)firstDirection=forward;
            position+=muzzle;direction+=forward;++matched;
        }
        if(!matched || direction.lengthSqr()<.01f)return false;
        position*=1.f/matched;direction.normalize();
        out.origin={position.x,position.y,position.z};out.forward={direction.x,direction.y,direction.z};out.valid=true;
        return true;
    }
    return false;
}
void ReadVehicleSights(Object parent,const std::array<unsigned char,0x328>& car,
    const Transform* transforms,unsigned nodeCount,VehicleState& out){
    Object model=None;std::uint32_t carData=0;
    if(!IsArmedDriver(out.kind) || !TagData(Field<Object>(car,0),0x76656869u,carData) ||
        !ReadAt(carData+0x34,model))return;
    std::array<VehicleMarker,2> markers{};
    ReadVehicleMarker(model,car,transforms,nodeCount,"primary trigger",markers[0]);
    ReadVehicleMarker(model,car,transforms,nodeCount,"secondary trigger",markers[1]);
    const auto centre=VehicleBasis(out)*Vector3();
    for(auto& marker:markers)if(marker.valid && !Within(marker.origin,{centre.x,centre.y,centre.z},20.f))marker={};
    if(markers[0].valid){out.barrelFacing=markers[0].forward;out.barrelValid=true;}
    const int slot=Field<std::int16_t>(car,0x2f2);
    if(slot<0 || slot>=4)return;
    const Object weapon=Field<Object>(car,0x2f8+slot*4);
    std::array<unsigned char,0x2cc> gun{};std::uintptr_t gunAddress=0;std::uint32_t data=0,count=0,triggers=0;
    if(!WeaponBytes(weapon,gun,gunAddress) || Field<Object>(gun,0x11c)!=parent ||
        !TagData(Field<Object>(gun,0),0x77656170u,data) || !ReadAt(data+0x4fc,count) || !count || count>2 ||
        !ReadAt(data+0x500,triggers))return;
    // The vehicle is the weapon owner. The seated biped's look direction is
    // a control request and may differ while the turret/airframe catches up.
    Point nativeAim=Field<Point>(car,0x23c);
    if(!nativeAim.Finite() || nativeAim.LengthSquared()<.5f || nativeAim.LengthSquared()>1.5f)nativeAim=out.aim;
    for(unsigned t=0;t<count;++t){
        std::uint32_t flags=0;if(!markers[t].valid || !ReadAt(static_cast<std::uintptr_t>(triggers)+t*0x114,flags))continue;
        // Camera-origin custom weapons need a different native origin reader.
        // Do not present a guessed muzzle as their accurate firing point.
        if(!(flags&(0x20u|0x800u)))continue;
        Vector3 direction(markers[t].forward.x,markers[t].forward.y,markers[t].forward.z);
        if(!(flags&0x800u)){
            // Native unit_adjust_projectile_ray starts with owner aim. The
            // weapon deviation cone limits subsequent player aim assistance;
            // it does NOT clamp owner aim against the rendered barrel marker.
            // Do not invent that extra clamp (R1.34), especially while turning.
            direction=Vector3(nativeAim.x,nativeAim.y,nativeAim.z).normalize();
        }
        out.muzzle[t]=markers[t].origin;out.shotDirection[t]={direction.x,direction.y,direction.z};out.shotValid[t]=true;
    }
}
}
bool RetailWeaponEngine::ReadGrenades(GrenadeInventory& out) const{
    out={};auto fail=[&](const char* why){grenadeReadStatus=why;return false;};
    if(!interactionAvailable)return fail("grenade profile unavailable");
    std::array<unsigned char,0x328> body{};std::uintptr_t address=0;Object unit=None;
    if(!LocalBiped(unit,body,address))return fail("no live local campaign biped");
    if(Field<Object>(body,0x11c)!=None)return fail("seated; chest grenades suspended");
    out.unit=unit;out.storage=static_cast<std::uint32_t>(address);out.age=Field<std::uint32_t>(body,0xc);
    out.count={body[0x31e],body[0x31f]};out.state=body[0x28d];out.selected=static_cast<std::int8_t>(body[0x31c]);
    if(out.count[0]>127 || out.count[1]>127 || out.state>4 || out.selected < -1 || out.selected>=2)
        return fail("invalid grenade count/state/type");
    grenadeReadStatus="chest holsters active";return true;
}
bool RetailWeaponEngine::SelectGrenade(Object unit,unsigned type){
    GrenadeInventory state;
    if(type>=2 || !ReadGrenades(state) || state.unit!=unit || !state.count[type] || state.state==1 || state.state==2)return false;
    // player_control_update retains a desired type at control+0x32 and copies
    // it back on each simulation tick. Update that command with the unit bytes,
    // or the old grenade type can win between input and the attach animation.
    std::uint32_t control=0;std::array<unsigned char,0x50> command{};
    if(!ReadAt(0x6b145c,control) || !ReadAt(control,command) || Field<Object>(command,0x10)!=unit)return false;
    const auto previous=Field<std::int16_t>(command,0x32);
    if(previous < -1 || previous>1)return false;
    const auto desired=static_cast<std::int16_t>(type);
    const std::uint16_t value=static_cast<std::uint16_t>(type|(type<<8));
    if(!Store(control+0x32,&desired,sizeof(desired)))return false;
    if(!Store(state.storage+0x31c,&value,sizeof(value))){Store(control+0x32,&previous,sizeof(previous));return false;}
    return true;
}
Object RetailWeaponEngine::AttachedGrenade(Object unit)const{
    GrenadeInventory state;Object projectile=None;
    if(!ReadGrenades(state) || state.unit!=unit || state.state!=2 || !ReadAt(state.storage+0x294,projectile))return None;
    std::array<unsigned char,0x1f4> object{};std::uintptr_t address=0;
    return ObjectBytes(projectile,object,address,5) && Field<Object>(object,0x11c)==unit?projectile:None;
}
bool RetailWeaponEngine::FinishGrenade(Object projectile,const TrackedGrenadeRelease& release,float strength,float maxWorldSpeed){
    GrenadeInventory state;std::array<unsigned char,0x328> body{};std::uintptr_t unitAddress=0,address=0;
    std::array<unsigned char,0x1f4> object{};
    if(!ReadGrenades(state) || state.unit!=release.unit || !UnitBytes(state.unit,body,unitAddress) ||
        !ObjectBytes(projectile,object,address,5) || Field<Object>(object,0x11c)!=None ||
        !release.pose.position.Finite() || !release.pose.forward.Finite() || !release.pose.up.Finite() ||
        !release.pose.velocity.Finite())return false;
    const Point previous=Field<Point>(object,0x5c);
    // Bound relocation independently of an editable throw-strength setting.
    if(!Within(previous,release.pose.position,1.5f))return false;
    PositionObject(projectile,&release.pose.position,&release.pose.forward,&release.pose.up);
    if(!ValidatePosition(projectile,&previous))PositionObject(projectile,&previous,nullptr,nullptr);
    const Point inherited=Field<Point>(body,0x68);
    if(!inherited.Finite())return false;
    const Point velocity=GrenadeVelocity(release.pose.velocity,inherited,strength,maxWorldSpeed);
    return ObjectBytes(projectile,object,address,5) && Field<Object>(object,0x11c)==None && Store(address+0x68,&velocity,sizeof(velocity));
}
bool RetailWeaponEngine::IsArmedVehicleDriver()const{
    // Trigger mapping also works when first-person vehicle cameras are disabled.
    if(!vehicleAvailable)return false;
    std::array<unsigned char,0x328> body{},car{};std::uintptr_t address=0,carAddress=0;Object unit=None;
    if(!LocalBiped(unit,body,address))return false;
    const Object parent=Field<Object>(body,0x11c);const auto seat=Field<std::int16_t>(body,0x2f0);
    char path[256]{},label[33]{};std::uint32_t data=0,count=0,seats=0;
    if(seat<0 || seat>=16 || parent==None || !ObjectBytes(parent,car,carAddress,1) ||
        !TagPath(Field<Object>(car,0),path) || !TagData(Field<Object>(car,0),0x76656869u,data) ||
        !ReadAt(data+0x2e4,count) || count>16 || unsigned(seat)>=count || !ReadAt(data+0x2e8,seats) ||
        !Copy(seats+seat*0x11c+4,label,32))return false;
    for(char& c:path)c=static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    for(char& c:label)c=static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return IsArmedDriver(ClassifyVehicleSeat(path,label,count,seat));
}
bool RetailWeaponEngine::ReadVehicle(VehicleState& out)const{
    out={};auto fail=[&](const char* why){vehicleReadStatus=why;return false;};
    if(!vehicleAvailable)return fail("vehicle profile unavailable");
    std::array<unsigned char,0x328> body{},car{};std::uintptr_t address=0,carAddress=0;Object unit=None;
    if(!LocalBiped(unit,body,address))return fail("no live local campaign biped");
    const Object parent=Field<Object>(body,0x11c);const int seat=Field<std::int16_t>(body,0x2f0);
    if(parent==None || seat<0 || seat>=16 || !ObjectBytes(parent,car,carAddress,1))return fail("not in a valid vehicle seat");
    char path[256]{};std::uint32_t data=0,seats=0,count=0;
    const Object tag=Field<Object>(car,0);
    if(!TagPath(tag,path) || !TagData(tag,0x76656869u,data) || !ReadAt(data+0x2e4,count) ||
        count>16 || unsigned(seat)>=count || !ReadAt(data+0x2e8,seats))return fail("vehicle tag/seat block unavailable");
    char label[33]{};if(!Copy(seats+seat*0x11c+4,label,32))return fail("seat label unavailable");
    for(char& c:label)c=static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    for(char& c:path)c=static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    const auto kind=ClassifyVehicleSeat(path,label,count,seat);
    if(kind==VehicleSeat::None)return fail("passenger or unsupported seat"); // Passenger/other vehicles keep their native behavior.
    std::array<ModelNode,64> nodes{};std::array<Transform,64> transforms{};unsigned n=0;
    if(!LiveBipedNodes(unit,body,address,nodes,transforms,n))return fail("live seated skeleton unavailable");
    int head=-1;int hand[2]={-1,-1};
    for(unsigned i=0;i<n;++i){char name[33]{};std::memcpy(name,nodes[i].name,32);
        if(std::strstr(name,"head"))head=i;
        if(std::strstr(name,"l hand") || std::strstr(name,"l wrist"))hand[0]=i;
        if(std::strstr(name,"r hand") || std::strstr(name,"r wrist"))hand[1]=i;
    }
    if(head<0)return fail("head node missing");
    const auto eye=transforms[head].translation;
    if(IsTurretSeat(kind)){
        const Matrix4 headFrame=ModelMatrix(transforms[head])*ModelMatrix(nodes[head].inverseBind);
        const auto facing=headFrame.getLeftAxis();
        if(std::isfinite(facing.x) && std::isfinite(facing.y) && std::isfinite(facing.z) &&
            facing.x*facing.x+facing.y*facing.y>.01f){
            out.renderedHeadFacing={facing.x,facing.y,facing.z};out.renderedHeadValid=true;
        }
    }
    out.unit=unit;out.vehicle=parent;out.seat=seat;out.kind=kind;out.eye={eye.x,eye.y,eye.z};
    out.forward=Field<Point>(car,0x74);out.up=Field<Point>(car,0x80);out.aim=Field<Point>(body,0x23c);
    out.position=Field<Point>(car,0x5c);out.velocity=Field<Point>(car,0x68);
    if(!out.eye.Finite() || !out.forward.Finite() || !out.aim.Finite() || !out.up.Finite() ||
        !out.position.Finite() || !out.velocity.Finite() || out.forward.LengthSquared()<.5f || out.aim.LengthSquared()<.5f ||
        Vector3(out.forward.x,out.forward.y,out.forward.z).cross({out.up.x,out.up.y,out.up.z}).lengthSqr()<.25f)return fail("invalid seat transform");
    for(unsigned h=0;h<2;++h)if(hand[h]>=0){out.handles[h]=ModelMatrix(transforms[hand[h]]);out.handleValid[h]=true;}
    Matrix4 seatedRoot;
    const bool bodyRootValid=RenderedRoot(nodes.data(),transforms.data(),n,seatedRoot);
    std::array<ModelNode,64> carNodes{};std::array<Transform,64> carTransforms{};unsigned carCount=0;
    if(LiveObjectNodes(parent,car,carAddress,carNodes,carTransforms,carCount,0x76656869u) &&
        RenderedRoot(carNodes.data(),carTransforms.data(),carCount,out.renderedBasis)){
        out.renderedPoseValid=true;
        const auto facing=((IsTurretSeat(kind) || kind==VehicleSeat::PelicanPassenger) && bodyRootValid?seatedRoot:out.renderedBasis).getLeftAxis();
        out.renderedFacing={facing.x,facing.y,facing.z};
        ReadVehicleSights(parent,car,carTransforms.data(),carCount,out);
        if(kind==VehicleSeat::ScorpionDriver){
            int best=-1,score=0;
            for(unsigned i=0;i<carCount;++i){
                char name[33]{};std::memcpy(name,carNodes[i].name,32);
                for(char& c:name)c=static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
                const int candidate=std::strstr(name,"turret")?3:std::strstr(name,"cannon")?2:std::strstr(name,"gun")?1:0;
                if(candidate>score){best=int(i);score=candidate;}
            }
            if(best>=0){
                const auto frame=ModelMatrix(carTransforms[best])*ModelMatrix(carNodes[best].inverseBind);
                const auto center=carTransforms[best].translation,forward=frame.getLeftAxis();
                out.cameraAnchor={center.x,center.y,center.z};out.cameraFacing={forward.x,forward.y,forward.z};
                out.cameraAnchorValid=out.cameraAnchor.Finite() && out.cameraFacing.Finite() &&
                    out.cameraFacing.LengthSquared()>.5f && (center-VehicleBasis(out)*Vector3()).length()<10.f;
            }
        }
    }
    vehicleReadStatus="first-person seat active";return true;
}
const Transform* RetailWeaponEngine::VehicleBodyNodes(Object model,const Transform* submitted,Object owner)const{
    // Preserve the actual interpolated native render pose/materials. A separate
    // copy of the simulation skeleton can flicker against the world body.
    VehicleState current;Object unit=None,bodyModel=None;std::uintptr_t address=0;
    std::array<unsigned char,0x328> body{};std::uint32_t data=0;std::int16_t player=-1;
    if(!submitted || !visualGuardsInstalled || !ReadVehicle(current) || current.unit!=owner ||
        !ReadAt(0x7c3108,player) || player!=0 || !LocalBiped(unit,body,address) || unit!=owner ||
        !TagData(Field<Object>(body,0),0x62697064u,data) || !ReadAt(data+0x34,bodyModel) || model!=bodyModel)return submitted;
    std::array<ModelNode,64> nodes{};unsigned count=0;
    // Native draw flushes synchronously before returning; each rendering thread
    // keeps its own copy. Nothing is written back to the submitted node buffer.
    thread_local std::array<Transform,64> copy{};
    if(!ModelNodes(model,nodes,count) || !Copy(reinterpret_cast<std::uintptr_t>(submitted),copy.data(),count*sizeof(Transform)))return submitted;
    for(unsigned i=0;i<count;++i){const auto m=ModelMatrix(copy[i]);
        for(unsigned j=0;j<16;++j)if(!std::isfinite(m[j]))return submitted;}
    if(!HideSeatedHead(nodes.data(),count,copy.data()))return submitted;
    thread_local bool reported=false;
    if(!reported){Logger::log<<"[PhysicalVehicles] native seated head masked model="<<model<<" unit="<<owner<<std::endl;reported=true;}
    return copy.data();
}
bool RetailWeaponEngine::DrawOnFootBody()const{
    std::array<unsigned char,0x328> body{};std::uintptr_t address=0;
    Object unit=None,model=None;std::uint32_t data=0;std::int16_t renderPlayer=-1;
    if(!interactionAvailable || !visualGuardsInstalled || !LocalBiped(unit,body,address) ||
        Field<float>(body,0xe0)<=0 || Field<Object>(body,0x11c)!=None ||
        !ReadAt(0x7c3108,renderPlayer) || renderPlayer!=0 || !ModelDrawReady() ||
        !MatchFingerprint(RetailObjectLighting) || !TagData(Field<Object>(body,0),0x62697064u,data) ||
        !ReadAt(data+0x34,model))return false;
    std::array<ModelNode,64> nodes{};std::array<Transform,64> live{},posed{};unsigned count=0;
    if(!LiveBipedNodes(unit,body,address,nodes,live,count) ||
        !PlaceOnFootBody(nodes.data(),count,live.data(),posed.data()))return false;
    void* lighting=ObjectLighting(unit);if(!lighting)return false;
    DrawNativeModel(model,posed.data(),unit,address,lighting,true);
    return true;
}
bool RetailWeaponEngine::DrawGrenade(unsigned type,const Matrix4& pose,bool grey)const{
    GrenadeInventory state;std::int16_t renderPlayer=-1;Object model=None;
    if(!visualGuardsInstalled || !ReadGrenades(state) || !ReadAt(0x7c3108,renderPlayer) || renderPlayer!=0 ||
        !GrenadeModel(type,model) || !ModelDrawReady() || !MatchFingerprint(RetailObjectLighting))return false;
    std::array<ModelNode,64> nodes{};std::array<Transform,64> transforms{};unsigned count=0;
    for(unsigned i=0;i<16;++i)if(!std::isfinite(pose[i]))return false;
    if(!ModelNodes(model,nodes,count))return false;
    unsigned root=0;for(unsigned i=0;i<count;++i)if(nodes[i].parent<0){root=i;break;}
    const Vector3 pivot=ModelMatrix(nodes[root].inverseBind).invertAffine()*Vector3();
    const Matrix4 centered=pose*Matrix4().translate(-pivot);
    if(!PlaceModel(nodes.data(),count,centered,transforms.data()))return false;
    void* lighting=ObjectLighting(state.unit);if(!lighting)return false;
    DrawNativeModel(model,transforms.data(),state.unit,state.storage,lighting,true,grey);return true;
}
bool RetailWeaponEngine::DrawInteractionHands(const Matrix4* poses,unsigned mask,unsigned closedMask,Matrix4* palms)const{
    std::int16_t renderPlayer=-1;std::array<unsigned char,0x328> body{};std::uintptr_t address=0;Object unit=None,model=None;
    std::uint32_t globals=0,entries=0,firstPerson=0;
    if(!interactionAvailable || !visualGuardsInstalled || !poses || !mask || !LocalBiped(unit,body,address) ||
        !ReadAt(0x7c3108,renderPlayer) || renderPlayer!=0 || !ModelDrawReady() || !MatchFingerprint(RetailObjectLighting) ||
        !ReadAt(0x746fa0,globals) || !ReadAt(globals+0x17c,entries) || entries<1 || entries>16 ||
        !ReadAt(globals+0x180,firstPerson) || !ReadAt(firstPerson+0xc,model))return false;
    std::array<ModelNode,64> nodes{};std::array<Transform,64> transforms{};unsigned count=0;
    if(!ModelNodes(model,nodes,count) || !PlaceChiefHands(nodes.data(),count,poses,transforms.data()))return false;
    struct GripBone{char BoneName[32];int Parent;};std::array<GripBone,64> bones{};int wrists[2]={-1,-1};
    for(unsigned i=0;i<count;++i){std::memcpy(bones[i].BoneName,nodes[i].name,32);bones[i].Parent=nodes[i].parent;
        const auto key=HandBoneKey(nodes[i].name);
        if(std::strstr(key.data(),"lwrist"))wrists[0]=i;
        if(std::strstr(key.data(),"rwrist"))wrists[1]=i;
    }
    for(unsigned h=0;h<2;++h)if(wrists[h]>=0){
        if(palms)LeftPalmFrame(bones.data(),count,transforms.data(),wrists[h],palms[h]);
        if(closedMask&(1u<<h))CloseGripHand(bones.data(),count,transforms.data(),wrists[h],-1,nodes.data(),count);
        if(!(mask&(1u<<h)))for(unsigned i=0;i<count;++i)
            if(HandDescendant(bones.data(),count,i,wrists[h])){transforms[i].scale=0;transforms[i].translation=poses[h]*Vector3();}
    }
    void* lighting=ObjectLighting(unit);if(!lighting)return false;
    DrawNativeModel(model,transforms.data(),unit,address,lighting);return true;
}
