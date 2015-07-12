#include "display.h"
#include <GL\glew.h>
#include <iostream>
#include <assert.h>

#define TRAPPED_MOUSE 1

namespace gemini { namespace util {

	Display::Display(unsigned int width, unsigned int height, const char* title)
	{

		SDL_Init(SDL_INIT_EVERYTHING);

		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

		m_context = SDL_GL_CreateContext(m_window);

		GLenum status = glewInit();
		if (status != GLEW_OK) {
			std::cerr << "GLEW failed to initialize!" << std::endl;
		}

		m_width = width;
		m_height = height;

		m_isClosing = false;
		m_hasResized = false;

		for (unsigned int i = 0; i < 256; i++){
			m_keystate[i] = false;
		}
		
		m_mouse_x = m_width / 2;
		m_mouse_y = m_height / 2;
		m_mouse_old_x = m_width / 2;
		m_mouse_old_y = m_height / 2;

		//SDL_SetWindowFullscreen(m_window, SDL_TRUE);

		//SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);

#if TRAPPED_MOUSE
		SDL_ShowCursor(SDL_DISABLE);
#endif

	}


	Display::~Display()
	{

		SDL_GL_DeleteContext(m_context);
		SDL_DestroyWindow(m_window);
		SDL_Quit();

	}

	bool firstframe = true;
	void Display::update() 
	{
		m_hasResized = false;
		SDL_GL_SwapWindow(m_window);
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			switch (e.type){
			case SDL_QUIT:
				m_isClosing = true;
				break;
			case SDL_KEYDOWN: {
				unsigned int keycode = SDL_GetScancodeFromKey(e.key.keysym.sym);

				if (keycode >= 256)
					std::cerr << "ERROR: Keycode '" << keycode << "' to high. Max is 256." << std::endl;
				else
					m_keystate[keycode] = true;
				break; }
			case SDL_KEYUP: {
				unsigned int keycode = SDL_GetScancodeFromKey(e.key.keysym.sym);
				if (keycode >= 256)
					std::cerr << "ERROR: Keycode '" << keycode << "' to high. Max is 256." << std::endl;
				else
					m_keystate[keycode] = false;
				break; }
			case SDL_MOUSEMOTION:
				m_mouse_x = e.motion.x;
				m_mouse_y = e.motion.y;
				break;
			}
		}
		SDL_WarpMouseInWindow(m_window, m_width / 2, m_height / 2);
		int oldw = m_width, oldh = m_height;
		SDL_GetWindowSize(m_window, &m_width, &m_height);
		if (oldw != m_width || oldh != m_height){
			m_mouse_x = m_width / 2;
			m_mouse_y = m_height / 2;
			m_mouse_old_x = m_width / 2;
			m_mouse_old_y = m_height / 2;
			m_hasResized = true;
		}
	}

	void Display::getMousePosition(int &x, int &y)
	{
		x = m_mouse_x;
		y = m_mouse_y;
	}
	
	void Display::getMouseSpeed(int &dx, int &dy)
	{
		dx = m_mouse_x - m_mouse_old_x;
		dy = m_mouse_y - m_mouse_old_y;
	}


	bool Display::isKeyHold(SDL_Keycode key)
	{
		unsigned int keycode = SDL_GetScancodeFromKey(key);
		if (m_keystate[keycode]) return true;
		return false;
	}

	bool Display::isKeyDown(SDL_Keycode key)
	{
		unsigned int keycode = SDL_GetScancodeFromKey(key);
		if (m_keystate[keycode]) return true;
		return false;
	}

	void Display::maximizeWindow(){
		SDL_MaximizeWindow(m_window);

	}

} }

