#include "input.h"

// Constructeur
Input::Input()
{
	button.left = button.right = button.up = button.down = button.attack = button.magie = button.escape = false;
}

// Fonction qui accède à button (struc) et nous donne l'info private
Input::Button Input::GetButton(void) const
{
	return button;
}

// Fonction de gestion des inputs
void Input::InputHandler(Event event, RenderWindow& window)
{
	// Fermer la fenêtre si on clique sur la croix
	if (event.type == Event::Closed)
	{
		// On ferme la fenêtre
		window.close();
	}

	// Gestion des inputs (clavier / souris) 
	// Détection btn appuyé
	if (event.type == Event::KeyPressed)
	{
		switch (event.key.code) // Code de la touche utilisée
		{
		case Keyboard::Escape:
			button.escape = true;
			break;
		case Keyboard::Left:
			button.left = true;
			break;
		case Keyboard::Right:
			button.right = true;
			break;
		case Keyboard::Down:
			button.down = true;
			break;
		case Keyboard::Up:
			button.up = true;
			break;
		case Keyboard::Space:
			button.space = true;
			break;
		default:
			break;
		}
	}
	// Test des key released
	if (event.type == Event::KeyReleased)
	{
		// Touche relâchée
		switch (event.key.code)
		{
		case Keyboard::Escape:
			button.escape = false;
			break;
		case Keyboard::Left:
			button.left = false;
			break;
		case Keyboard::Right:
			button.right = false;
			break;
		case Keyboard::Down:
			button.down = false;
			break;
		case Keyboard::Up:
			button.up = false;
			break;
		case Keyboard::Space:
			button.space = false;
			break;
		default:
			break;
		}
	}

	// Détection btn souris
	if (event.type == Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == Mouse::Left)
		{
			button.attack = true;
		}
	}
	if (event.type == Event::MouseButtonReleased)
	{
		if (event.mouseButton.button == Mouse::Left)
		{
			button.attack = false;
		}
	}

	if (event.type == Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == Mouse::Right)
		{
			button.magie = true;
		}
	}
	if (event.type == Event::MouseButtonReleased)
	{
		if (event.mouseButton.button == Mouse::Right)
		{
			button.magie = false;
		}
	}
}


