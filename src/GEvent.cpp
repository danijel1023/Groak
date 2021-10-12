#include "GEvent.h"

void GEvent::Resolve_Event(std::ostream* Stream_Ptr, const GString& Prefix, bool New_Line, bool Print_Active) const {
    if (!Print_Active && Type == GEType::Mouse && Mouse_Message == GEMouse_Message::Active) return;

    std::ostream& Stream = *Stream_Ptr;
    Stream << Prefix;


    switch (Type) {
        case GEType::Core:
        {
            Stream << "Core: ";

            switch (Core_Message) {
                case GECore_Message::Register:
                    Stream << "Register(" << Data_Ptr << ")";
                    break;
                case GECore_Message::Run_Lambda:
                    Stream << "Run_Lambda";
                    break;
                case GECore_Message::Move:
                    Stream << "Move(" << WP << ")";
                    break;
                case GECore_Message::Resize:
                    Stream << "Resize(" << WS << ")";
                    break;
                case GECore_Message::Iconify:
                    Stream << "Iconify(" << Data_Ptr << ")";
                    break;
                case GECore_Message::Maximise:
                    Stream << "Maximise(" << Data_Ptr << ")";
                    break;
                case GECore_Message::Restore:
                    Stream << "Restore(" << Data_Ptr << ")";
                    break;
                case GECore_Message::Show:
                    Stream << "Show(" << Data_Ptr << ")";
                    break;
                case GECore_Message::Hide:
                    Stream << "Hide(" << Data_Ptr << ")";
                    break;
                case GECore_Message::Close:
                    Stream << "Close(" << Data_Ptr << ")";
                    break;
                case GECore_Message::Terminate:
                    Stream << "Terminate";
                    break;
                case GECore_Message::Create_Cursor:
                    Stream << "Create_Cursor";
                    break;
                case GECore_Message::Set_Cursor:
                    Stream << "Set_Cursor";
                    break;
                case GECore_Message::Destroy_Cursor:
                    Stream << "Destroy_Cursor";
                    break;

                default:
                    Stream << "Core -- Unknown";
            }

            break;
        }

        case GEType::Window:
        {
            Stream << "Window: ";

            switch (Wind_Message) {
                case GEWind_Message::Resize:
                    Stream << "Resize(" << WS << ")";
                    break;

                case GEWind_Message::Lose_Focus:
                    Stream << "Lose focus";
                    break;

                case GEWind_Message::Gain_Focus:
                    Stream << "Gain focus";
                    break;

                case GEWind_Message::Run:
                    Stream << "Run";
                    break;

                case GEWind_Message::Move:
                    Stream << "Move(" << WP << ")";
                    break;

                case GEWind_Message::Render:
                    Stream << "Render";
                    break;

                case GEWind_Message::Show:
                    Stream << "Show";
                    break;

                case GEWind_Message::Hide:
                    Stream << "Hide";
                    break;

                case GEWind_Message::Close:
                    Stream << "Close";
                    break;

                case GEWind_Message::Iconify:
                    Stream << "Iconify";
                    break;

                case GEWind_Message::Should_Iconify:
                    Stream << "Should_Iconify";
                    break;

                case GEWind_Message::Maximise:
                    Stream << "Maximise";
                    break;

                case GEWind_Message::Should_Maximise:
                    Stream << "Should_Maximise";
                    break;

                case GEWind_Message::Restore:
                    Stream << "Restore";
                    break;

                case GEWind_Message::Should_Restore:
                    Stream << "Should_Restore";
                    break;

                case GEWind_Message::Terminate_Thread:
                    Stream << "Terminating thread";
                    break;

                default:
                    Stream << "Window -- Unknown";
            }

            break;
        }

        case GEType::Mouse:
        {
            Stream << "Mouse: ";

            switch (Mouse_Message) {
                case GEMouse_Message::Down:
                    Stream << "Down(" << MP << ") - ";
                    switch (Mouse_Button) {
                        case GEMouse_Button::LMB: Stream << "LMB"; break;
                        case GEMouse_Button::RMB: Stream << "RMB"; break;
                        case GEMouse_Button::MMB: Stream << "MMB"; break;
                    }

                    if (Modifier_Shift) Stream << " (Shift)";
                    if (Modifier_Alt) Stream << " (Alt)";
                    if (Modifier_Ctrl) Stream << " (Ctrl)";

                    break;

                case GEMouse_Message::Up:
                    Stream << "Up(" << MP << ") - ";
                    switch (Mouse_Button) {
                        case GEMouse_Button::LMB: Stream << "LMB"; break;
                        case GEMouse_Button::RMB: Stream << "RMB"; break;
                        case GEMouse_Button::MMB: Stream << "MMB"; break;
                    }

                    if (Modifier_Shift) Stream << " (Shift)";
                    if (Modifier_Alt) Stream << " (Alt)";
                    if (Modifier_Ctrl) Stream << " (Ctrl)";

                    break;

                case GEMouse_Message::Scroll_Up:
                    Stream << "Scroll_Up(" << MP << ")";
                    break;

                case GEMouse_Message::Scroll_Down:
                    Stream << "Scroll_Down(" << MP << ")";
                    break;

                case GEMouse_Message::Scroll_Left:
                    Stream << "Scroll_Left(" << MP << ")";
                    break;

                case GEMouse_Message::Scroll_Right:
                    Stream << "Scroll_Right(" << MP << ")";
                    break;

                case GEMouse_Message::Move:
                    Stream << "Move(" << MP << ")";
                    break;

                case GEMouse_Message::Enter:
                    Stream << "Enter(" << MP << ")";
                    break;

                case GEMouse_Message::Leave:
                    Stream << "Leave(" << MP << ")";
                    break;

                case GEMouse_Message::Active:
                    Stream << "Is active?";
                    break;

                case GEMouse_Message::Gain_Focus:
                    Stream << "Gain mouse focus (send mouse msgs to that window)";
                    break;

                case GEMouse_Message::Lose_Focus:
                    Stream << "Lose mouse focus";
                    break;

                default:
                    Stream << "Mouse -- Unknown";
            }

            break;
        }

        case GEType::Keyboard:
        {
            Stream << "Keyboard: ";

            switch (Keyboard_Message) {
                case GEKeyboard_Message::Key:
                    Stream << "Key(key:" << Key << ", scan_code:" << Scancode << ") - ";
                    switch (Key_Action) {
                        case GEKey_Action::Up:     Stream << "Up"; break;
                        case GEKey_Action::Down:   Stream << "Down"; break;
                        case GEKey_Action::Repeat: Stream << "Repeat"; break;
                    }

                    if (Modifier_Shift) Stream << " (Shift)";
                    if (Modifier_Alt)   Stream << " (Alt)";
                    if (Modifier_Ctrl)  Stream << " (Ctrl)";

                    break;

                case GEKeyboard_Message::Text:
                    Stream << "Text(num:" << Code_Point << ", ch: " << GString().push_back(Code_Point) << ")";
                    break;

                default:
                    Stream << "Keyboard -- Unknown";
            }

            break;
        }

        case GEType::Renderer:
        {
            Stream << "Renderer: ";

            switch (Renderer_Message) {
                case GERenderer_Message::Terminate_Thread:
                    Stream << "Terminate_Thread";
                    break;

                case GERenderer_Message::Render:
                    Stream << "Render";
                    break;

                case GERenderer_Message::Send_Event:
                    Stream << "Send_Event";
                    break;

                case GERenderer_Message::Load_Texture:
                    Stream << "Load_Texture";
                    break;

                default:
                    Stream << "Renderer -- Unknown";
                    break;
            }

            break;
        }

        case GEType::Console:
        {
            Stream << "Console: ";

            switch (Console_Message) {
                case GEConsole_Message::Sync:
                    Stream << "Sync";
                    break;

                default:
                    Stream << "Renderer -- Unknown";
                    break;
            }

            break;
        }

        case GEType::Custom:
        {
            Stream << "Custom event";
            break;
        }
    }

    if (New_Line) Stream << std::endl;
}
