#ifndef INPUT_H
#define INPUT_H

#include <SFML/Graphics.hpp>

using namespace sf;

class Input
{
	struct Button { bool left, right, up, down, attack, magie, escape, space; };

public:
	// Proto du constructeur
	Input();
	// Proto GetBtn
	Button GetButton(void) const;
	// Proto
	void InputHandler(Event event, RenderWindow& window);


private:
	Button button;
};

#endif