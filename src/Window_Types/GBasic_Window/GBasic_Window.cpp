#include "GBasic_Window.h"
#include "GWindow.h"


GBasic_Window::GBasic_Window(GBasic_Window* Parent, int Window_X, int Window_Y, int Screen_X, int Screen_Y)
	: m_Parent(Parent), m_Window_X(Window_X), m_Window_Y(Window_Y), m_Screen_X(Screen_X), m_Screen_Y(Screen_Y) {

	//If this is child window - the main windows' m_Parent is always nullptr
	if (m_Parent) {
		//Get the direct GWindow pointer
		m_Main_Window = m_Parent->m_Main_Window;
		
		//Add this child window to parents' child window list
		m_Parent->m_Child_Windows.push_back(this);

		//Register the absolute position from (0, 0) from main window
		m_Absolute_Screen_X = m_Parent->m_Absolute_Screen_X + Screen_X;
		m_Absolute_Screen_Y = m_Parent->m_Absolute_Screen_Y + Screen_Y;
	}
}

GBasic_Window::~GBasic_Window() {
	for (auto Ch_Wnd : m_Child_Windows) {
		delete Ch_Wnd;
	}
}



GFrameBuffer* GBasic_Window::Create_Framebuffer() {
	return new GFrameBuffer(m_Window_X, m_Window_Y, this);
}

void GBasic_Window::Add_Quad(GQuad* Quad) {
	m_Quad_List.push_back(Quad);
}


void GBasic_Window::Set_Viewport() {
	auto& Renderer = *(m_Main_Window->m_Renderer);
	Renderer.Set_Window_Screen(m_Absolute_Screen_X - m_Main_Window->m_FB_Ancor.X,
							   m_Absolute_Screen_Y - m_Main_Window->m_FB_Ancor.Y,
							   m_Window_X, m_Window_Y);
}



int GBasic_Window::Dispatcher_Func(void* _This, GEvent* Event) {
	auto This = static_cast<GBasic_Window*>(_This);
	return This->Dispatcher_Func(Event);
}

int GBasic_Window::Callback_Func(void* _This, GEvent* Event) {
	auto This = static_cast<GBasic_Window*>(_This);
	return This->Callback_Func(Event);
}

int GBasic_Window::Dispatcher_Func(GEvent* Event) {
	switch (Event->Type) {
		case GEType::Mouse:
		{
			auto& MP = Event->MP;
			for (auto Ch_Wnd : m_Child_Windows) {
				//If the mouse is left form the child wondow, ignore
				if (MP.X < Ch_Wnd->m_Screen_X) {
					continue;
				}

				//If the mouse is right form the child wondow, ignore
				if (MP.X > Ch_Wnd->m_Screen_X + Ch_Wnd->m_Window_X) {
					continue;
				}


				//If the mouse is below form the child wondow, ignore
				if (MP.Y < Ch_Wnd->m_Screen_Y) {
					continue;
				}

				//If the mouse is above form the child wondow, ignore
				if (MP.Y > Ch_Wnd->m_Screen_Y + Ch_Wnd->m_Window_Y) {
					continue;
				}

				//else - the mouse is inside the child windows' box
				GEvent Child_Event;
				Child_Event = *Event;
				Child_Event.MP.X -= Ch_Wnd->m_Screen_X;
				Child_Event.MP.Y -= Ch_Wnd->m_Screen_Y;
				if (GCall(Ch_Wnd, m_Dispatcher_Ptr, &Child_Event)) return 1;
			}

			//No child window handled the event so this window will now process the event
			switch (Event->Mouse_Message) {
				case GEMouse_Message::Down:
				{
					m_Main_Window->m_Mouse_Focus = this;
					break;
				}
				case GEMouse_Message::Up:
				{
					m_Main_Window->m_Mouse_Focus = nullptr;
					break;
				}
			}


			if ((MP.X >= 0 && MP.Y >= 0) && (MP.X <= m_Window_X && MP.Y <= m_Window_Y)) {
				if (m_Main_Window->m_Wind_Under_Cursor != this) {
					GBasic_Window*& Top_Window = m_Main_Window->m_Wind_Under_Cursor;

					if (Top_Window) {
						GEvent Leave;
						Leave.Type = GEType::Mouse;
						Leave.Mouse_Message = GEMouse_Message::Leave;
						Leave.MP = Event->MP;

						GCall(Top_Window, m_Callback_Ptr, &Leave);
					}

					GEvent Enter;
					Enter.Type = GEType::Mouse;
					Enter.Mouse_Message = GEMouse_Message::Enter;
					Enter.MP = Event->MP;

					GCall(this, m_Callback_Ptr, &Enter);
					Top_Window = this;
				}
			}

			return GCall(this, m_Callback_Ptr, Event);
		}

		case GEType::Window:
		{
			if (Event->Wind_Message == GEWind_Message::Close) {
				for (auto& Ch_Wnd : m_Child_Windows) {
					GCall(Ch_Wnd, m_Dispatcher_Ptr, Event);
				}

				GCall(this, m_Callback_Ptr, Event);
			}

			else {
				GCall(this, m_Callback_Ptr, Event);

				for (auto Ch_Wnd : m_Child_Windows) {
					GCall(Ch_Wnd, m_Dispatcher_Ptr, Event);
				}
			}
		}
	}

	return 0;
}


int GBasic_Window::Callback_Func(GEvent* Event) {
	switch (Event->Type) {
		case GEType::Window:
		{
			switch (Event->Wind_Message) {
				case GEWind_Message::Render:
				{
					auto& Renderer = *(m_Main_Window->m_Renderer);

					Set_Viewport();
					for (auto& Quad : m_Quad_List) if (Quad->m_Active) Renderer.Add_Quad(*Quad);
					Renderer.Flush();

					return 1;
				}

				case GEWind_Message::Move:
				{
					m_Screen_X = Event->WP.X;
					m_Screen_Y = Event->WP.Y;

					m_Absolute_Screen_X = m_Parent->m_Absolute_Screen_X + m_Screen_X;
					m_Absolute_Screen_Y = m_Parent->m_Absolute_Screen_Y + m_Screen_Y;
					break;
				}

				case GEWind_Message::Resize:
				{
					m_Window_X = Event->WS.X;
					m_Window_Y = Event->WS.Y;

					break;
				}
			}

			break;
		}



		//[TODO] This is for Child Window
		//case EType::Mouse:
		//{
		//	switch (*static_cast<EMouse_Message*>(Event->Message)) {
		//		case EMouse_Message::LMB_Down:
		//		{
		//			m_Main_hwnd->m_Focus = this;
		//			break;
		//		}
		//	}
		//
		//	break;
		//}
	}

	return 0;
}
