#pragma once

#include <SDL2\SDL.h>

namespace gemini { namespace util {
	
	class Display
	{
	public:
		Display(unsigned int width, unsigned int height, const char* title);
		void update();
		inline int getWidth() const { return m_width; }
		inline int getHeight() const { return m_height; }
		inline bool isCloseRequested() const { return m_isClosing; }
		inline bool hasResized() const { return m_hasResized; }
		inline void close() { m_isClosing = true; }
		void getMousePosition(int &x, int &y);
		void getMouseSpeed(int &dx, int &dy);
		virtual ~Display();
		bool isKeyHold(SDL_Keycode key);
		bool isKeyDown(SDL_Keycode key);

	private:
		bool m_isClosing, m_hasResized;
		SDL_Window* m_window;
		SDL_GLContext m_context;
		int m_width, m_height;
		int m_mouse_x, m_mouse_y, m_mouse_old_x, m_mouse_old_y;
		bool m_keystate[256];
	};

} }

