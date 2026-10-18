// Actual Guide builder + bundled font metrics; no D3D/headset required.
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <functional>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include "HaloCEVR/UI/PlayerGuide.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "ThirdParty/stb/stb_truetype.h"
using D3DCOLOR=unsigned;
static unsigned checks=0;
#define CHECK(x) do{++checks;if(!(x)){std::cerr<<__LINE__<<": " #x "\n";std::exit(1);}}while(false)
struct Font{float lineHeight;stbtt_bakedchar cdata[96];};
struct UIRenderer{
    float CalculateWrappedString(const Font&,const char*,float,std::string&) const;
    Font large{},medium{},small{};
    Font* GetDescriptionFont(){return &small;}
    Font* GetLargeFont(){return &large;}Font* GetMediumFont(){return &medium;}Font* GetSmallFont(){return &small;}
    float CalculateStringWidth(const Font& font,const char* text) const {float result=0;for(unsigned char c:std::string(text)){CHECK(c>=32&&c<128);result+=font.cdata[c-32].xadvance;}return result;}
    void Init(const char* path){
        std::ifstream file(path,std::ios::binary);std::vector<unsigned char> bytes((std::istreambuf_iterator<char>(file)),{});CHECK(!bytes.empty());
        for(auto* font:{&large,&medium,&small}){
            const float size=font==&large?32:font==&medium?28:18;
            std::vector<unsigned char> bitmap(512*512);
            CHECK(stbtt_BakeFontBitmap(bytes.data(),0,size,bitmap.data(),512,512,32,96,font->cdata)>0);
            font->lineHeight=size;
        }
    }
};
struct Element{float x=0,y=0,w=0,h=0;bool bVisible=true;virtual ~Element()=default;};
struct UIPanel:Element{
    std::vector<Element*> children;
    UIPanel(float X,float Y,float W,float H,D3DCOLOR){x=X;y=Y;w=W;h=H;}
    void AddElement(Element* p){children.push_back(p);}
    ~UIPanel(){for(auto* p:children)delete p;}
};
struct UILabel:Element{
    std::string text;Font* font;
    UILabel(float X,float Y,std::string T,Font* F,D3DCOLOR):text(T),font(F){x=X;y=Y;h=font->lineHeight;}
};
struct UITextButton:Element{
    std::string text;Font* font;std::function<void()> onClick;float textX=0;
    void SetText(const std::string& value);
    UITextButton(float X,float Y,float W,float H,std::string T,Font* F,D3DCOLOR,D3DCOLOR,D3DCOLOR):text(T),font(F){x=X;y=Y;w=W;h=H;}
};

#include "HaloCEVR/Maths/Vectors.h"
#include "HaloCEVR/Immersion/InteractionModel.h"
#include "HaloCEVR/UI/SettingsPresentation.h"
#include <unordered_map>
#include <sstream>
namespace Logger { std::ostringstream log; }
// INSERT_CONFIG_TYPES
struct UICheckbox:Element {bool bIsActive;std::function<void()> onClick;UICheckbox(float,float,float,bool b,D3DCOLOR,D3DCOLOR):bIsActive(b){} };
struct UITextBox:UITextButton {using UITextButton::UITextButton;};
struct UIIntBox:UITextButton {int intValue;UIIntBox(float x,float y,float w,float h,int v,Font* f,D3DCOLOR a,D3DCOLOR b,D3DCOLOR c):UITextButton(x,y,w,h,"",f,a,b,c),intValue(v){text=std::to_string(v);} };
struct UIFloatBox:UITextButton {float floatValue;UIFloatBox(float x,float y,float w,float h,float v,Font* f,D3DCOLOR a,D3DCOLOR b,D3DCOLOR c):UITextButton(x,y,w,h,"",f,a,b,c),floatValue(v){} };
struct Game {static Game instance;Config config;UIRenderer renderer;UIRenderer* uiRenderer=&renderer;
    BoolProperty physical{true,""},flashlight{true,""},threeDOF{false,""};
    BoolProperty* c_PhysicalWeapons=&physical,*c_GripHeadFlashlight=&flashlight,*c_Use3DOFAiming=&threeDOF;
    BoolProperty* c_StandingMode=nullptr;IntProperty* c_PlayerHeightCm=nullptr;
    void Setup(){ // INSERT_COMFORT_REGISTRATION
    }
}; Game Game::instance;
struct SettingsMenu {
    static constexpr D3DCOLOR darkPanelColour=0,mediumPanelColour=1,lightPanelColour=2,textLabelColour=3,textValueColour=4;
    UIPanel* GenerateSettingsPanel(std::string,UIPanel*,float,float,float);
};
// INSERT_WRAP_TEXT
// INSERT_SET_TEXT
// INSERT_SETTINGS_PANEL
int main(int argc,char** argv){
    CHECK(argc==3);auto& g=Game::instance;g.renderer.Init(argv[1]);g.Setup();SettingsMenu menu;
    UIPanel parent(0,0,640,340,0);
    auto* posture=menu.GenerateSettingsPanel("StylishStandingMode",&parent,10,84,10);
    auto* button=dynamic_cast<UITextButton*>(posture->children.back());CHECK(button);
    CHECK(button->text=="Sitting");CHECK(!g.c_StandingMode->Value());
    button->onClick();CHECK(button->text=="Standing");CHECK(g.c_StandingMode->Value());
    CHECK(button->textX>=0);CHECK(button->textX+g.renderer.CalculateStringWidth(*button->font,button->text.c_str())<=button->w);
    auto* height=menu.GenerateSettingsPanel("StylishPlayerHeightCm",&parent,20+posture->h,84,10);
    auto* box=dynamic_cast<UIIntBox*>(height->children.back());CHECK(box&&box->intValue==180);
    CHECK(height->y+height->h<=parent.h);
    CHECK(g.c_PlayerHeightCm->GetDesc().find("ignored when Sitting")!=std::string::npos);
    box->intValue=500;box->onClick();CHECK(g.c_PlayerHeightCm->Value()==220);CHECK(box->text=="220");
    box->intValue=-20;box->onClick();CHECK(g.c_PlayerHeightCm->Value()==120);CHECK(box->text=="120");
    box->intValue=180;box->onClick();CHECK(g.c_PlayerHeightCm->Value()==180);
    CHECK(g.config.SaveToFile(argv[2]));button->onClick();box->intValue=150;box->onClick();
    CHECK(!g.c_StandingMode->Value());CHECK(g.config.LoadFromFile(argv[2]));
    CHECK(g.c_StandingMode->Value()&&g.c_PlayerHeightCm->Value()==180);
    g.c_PlayerHeightCm->SetValue(-999);
    auto* bounded=menu.GenerateSettingsPanel("StylishPlayerHeightCm",&parent,0,84,10);
    CHECK(dynamic_cast<UIIntBox*>(bounded->children.back())->intValue==120);
    CHECK(g.c_PlayerHeightCm->Value()==120);
    std::cout<<checks<<" production Comfort controls, text alignment, height bounds and config persistence checks passed\n";
}
