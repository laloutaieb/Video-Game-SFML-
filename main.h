// Includes
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <utility>
#include <stdlib.h> 
#include "input.h"
#include "map.h"
#include "portals.h"

// Constantes du programme
const int WIN_WIDTH = 800; // 25 tiles de 32 px
const int WIN_HEIGHT = 576; // 18 tiles de 32 px
const int SPRITE_SIZE = 32; // On a des sprites de 32x32 pixels
const int WALK_SPEED = 2; // Vitesse de déplacement du perso
const int COL_COUNT = 25; // Nombre de cases en largeur (col)
const int ROW_COUNT = 18; // Nombre de cases en lignes (row)
const int BULLET_SPEED = 4; // Vitesse du projectile
float MOB_SPEED = 0.3f; // Vitesse du mob

// Namespaces
using namespace sf;
using namespace std;

// Prototypes
void CheckBtn();
void AnimPlayer();
vector<string> explode(string const& s, char delim);
void HandleBullet();
void HandleMob();
void Debug();
void Start();
void Update();