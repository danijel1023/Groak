/*
* When trying to access (reading or writing) 'text' and 'text color' buffers,
* first you need to lock the m_Buffer mutex by calling Get_Mutex().
* The easies way of doing that is with the unique lock, like this:
* 
*   std::unique_lock<std::mutex> Temp_Lck(m_Text_Box->Get_Mutex());
* 
* 
*/




#include "GBasic_Window.h"
#include "GCharacter.h"
#include <mutex>

class GText_Box : public GBasic_Window {
public:
    GText_Box(GBasic_Window* Parent, const GSize& Window, const GPos& Screen, bool Overlay = false);
    ~GText_Box();

    std::mutex& Get_Mutex();

private:
    std::vector<GString> m_Text;
    std::vector<std::vector<GColor>> m_Text_Color;
    std::mutex m_Buffer_Mutex;

    GFont* m_Console_Font = nullptr;
    float m_Text_Height = 20;

    //Scrolling: Y is in multiples of lines and X is in pixels
    GPos m_Text_Offset;

protected:
    int Callback_Func(const GEvent& Event);

private:
    static int Callback_Func(void* _This, const GEvent& Event) {
        auto This = static_cast<GText_Box*>(_This);
        return This->Callback_Func(Event);
    }
};
