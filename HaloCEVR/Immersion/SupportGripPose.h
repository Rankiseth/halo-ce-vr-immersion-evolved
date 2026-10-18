#pragma once
#include "HandGripPose.h"

namespace Immersion {
// Preserve the actual bracing palm/fingers in gun space when the trigger
// hand lets go. Controller tracking moves the gun; animation must not turn
// its supporting skin back into a free hand or a different weapon grip.
class SupportGripPose {
public:
    void Reset(){valid=false;}
    bool Matches(std::uint64_t id,unsigned n,bool mirrored)const{
        return valid && key==id && count==n && left==mirrored;
    }
    bool AttachedAim(std::uint64_t id,unsigned n,bool mirrored,bool aiming,bool supportOnly)const{
        return aiming || (supportOnly && Matches(id,n,mirrored) && twoHandAim);
    }
    template<class B> void Capture(std::uint64_t id,bool mirrored,bool aiming,const B* bones,
        unsigned n,int wrist,int gun,const Transform* pose){
        Reset();
        if(!bones || !pose || !n || n>64 || wrist<0 || gun<0 || wrist>=int(n) || gun>=int(n) || wrist==gun)return;
        const auto anchor=ModelMatrix(pose[gun]);
        if(!Finite(anchor) || std::fabs(anchor.getDeterminant())<.00001f)return;
        const auto inverse=Matrix4(anchor).invertAffine();
        for(unsigned i=0;i<n;++i){
            parent[i]=bones[i].Parent;
            selected[i]=HandDescendant(bones,n,i,wrist) && !HandDescendant(bones,n,i,gun);
            if(selected[i]){relative[i]=inverse*ModelMatrix(pose[i]);if(!Finite(relative[i]))return;}
        }
        key=id;count=n;left=mirrored;twoHandAim=aiming;valid=true;
    }
    template<class B> bool Apply(std::uint64_t id,bool mirrored,const B* bones,unsigned n,int gun,Transform* pose)const{
        if(!Matches(id,n,mirrored) || !bones || !pose || gun<0 || gun>=int(n))return false;
        const auto anchor=ModelMatrix(pose[gun]);if(!Finite(anchor))return false;
        for(unsigned i=0;i<n;++i)if(bones[i].Parent!=parent[i])return false;
        for(unsigned i=0;i<n;++i)if(selected[i])pose[i]=ModelTransform(anchor*relative[i]);
        return true;
    }
private:
    static bool Finite(const Matrix4& m){for(unsigned i=0;i<16;++i)if(!std::isfinite(m[i]))return false;return true;}
    std::array<Matrix4,64> relative{};
    std::array<int,64> parent{};
    std::array<bool,64> selected{};
    std::uint64_t key=0;unsigned count=0;
    bool valid=false,left=false,twoHandAim=false;
};
}
