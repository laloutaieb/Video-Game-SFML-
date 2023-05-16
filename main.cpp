// Inclusion des fonctionnalités
#include "main.h"

// Variables globales
#pragma region Variables Globales
RenderWindow window;
Input input;
// Préparation affichage personnage 2D
Texture heroTexture;
// enum des actions / directions
enum Dir { Down, Right, Up, Left, Down_Atk, Right_Atk, Up_Atk, Left_Atk };
// Définir l'animation (la ligne) à jouer
// x = lignes & y = col
Vector2i heroAnim(0, Down);
// Timer / clock
Clock heroAnimClock;
// bool le perso est il à l'arrêt ?
bool heroIdle = true;
// Gestion de l'attaque à l'épée
bool needResetAnim = false;
// Tableau représentant la map (le niveau)
int levelLoaded[450]; // 450 = 25 * 18
// Tableau représentant les collisions de la map
int levelLoadedCollisions[450];
// Pour débug collisions (Tableau de rect de visualisation)
RectangleShape rects[450];
bool canShowCollisionsDebug = false;
// Slime (monstre)
Texture slimeTexture;
Sprite slimeSprite;
Vector2i mobAnim(0, 0);
Clock mobClock;
bool mobDestroyed = false;
// Projectile / flèche
Texture bTexture;
Sprite bullet;
bool bulletActive = false;
int bulletDir;
Clock bulletClock;
#pragma endregion

// Fonction main, point de départ du programme
int main()
{
    // Window, Init, Load res...
    Start();

    // Update
    while (window.isOpen())
    {
        Event event;
        // On boucle sur les événements
        while (window.pollEvent(event))
        {
            // Gestion des inputs / events
            input.InputHandler(event, window);
        }

        // Input, clear, draw, display, collisions, functions...
        Update();
    }

    // Fin du programme
    return 0;
}


void CheckBtn()
{
    int hPosX = round(heroSprite.getPosition().x / SPRITE_SIZE);
    int hPosY = round(heroSprite.getPosition().y / SPRITE_SIZE);
    if (canShowCollisionsDebug)
    {
        cout << levelLoaded[(hPosX + hPosY * COL_COUNT)] << endl;
        cout << hPosX << " , " << hPosY << endl;
    }

    if (!needResetAnim) // Si pas d'ttaque en cours
    {
        if (input.GetButton().left == true)
        {
            heroAnim.y = Left;
            // Vérification si à notre gauche on a une case solide ou pas
            hPosX = round((heroSprite.getPosition().x - (WALK_SPEED * 2)) / SPRITE_SIZE);
            // Si la position future est sur une case non solide (machable)
            if (levelLoadedCollisions[(hPosX + hPosY * COL_COUNT)] == 0 && hPosX > -1)
            {
                // On se déplace vers la gauche
                heroSprite.move(-WALK_SPEED, 0);
            }
            heroIdle = false;
        }
        else if (input.GetButton().right == true)
        {
            heroAnim.y = Right;
            hPosX = round((heroSprite.getPosition().x + (WALK_SPEED * 2)) / SPRITE_SIZE);
            if (levelLoadedCollisions[(hPosX + hPosY * COL_COUNT)] == 0 && hPosX < COL_COUNT)
            {
                heroSprite.move(WALK_SPEED, 0);
            }
            heroIdle = false;
        }
        else if (input.GetButton().up == true)
        {
            heroAnim.y = Up;
            hPosY = round((heroSprite.getPosition().y - (WALK_SPEED * 2)) / SPRITE_SIZE);
            if (levelLoadedCollisions[(hPosX + hPosY * COL_COUNT)] == 0 && hPosY > -1)
            {
                heroSprite.move(0, -WALK_SPEED);
            }
            heroIdle = false;
        }
        else if (input.GetButton().down == true)
        {
            heroAnim.y = Down;
            hPosY = round((heroSprite.getPosition().y + (WALK_SPEED * 4)) / SPRITE_SIZE);
            if (levelLoadedCollisions[(hPosX + hPosY * COL_COUNT)] == 0 && hPosY < ROW_COUNT)
            {
                heroSprite.move(0, WALK_SPEED);
            }
            heroIdle = false;
        }
        else // Perso en IDLE
        {
            heroIdle = true;
        }
        // Condition d'attaque
        if (input.GetButton().attack == true)
        {
            // Attaque à l'épée
            needResetAnim = true; // Après attaque, retourner sur anim walk
            heroAnim.x = 0; // Retourner à la col 0 (Jouer l'anim depuis le début)
            // On passe de la ligne walk à la ligne atk sur la texture
            heroAnim.y += 4; // On descend de 4 lignes sur notre sprite sheet
            // Todo: Envoyer un projectile invisible avec durée de vie très très courte
        }
        if (input.GetButton().magie == true)
        {
            needResetAnim = true;
            heroAnim.x = 0;
            heroAnim.y += 4;
            // Attaque à distance
            if (!bulletActive)
            {
                bulletActive = true;
                bullet.setPosition(heroSprite.getPosition().x + 16, heroSprite.getPosition().y + 16);
                bullet.setScale(0.75f, 0.75f);
                bullet.setOrigin(16, 16);
                bulletDir = heroAnim.y;
                bulletClock.restart();
            }
        }
    }

    if (input.GetButton().escape == true)
    {
        window.close();
    }

    canShowCollisionsDebug = input.GetButton().space;
}

void AnimPlayer()
{
    // Modifie le Sprite à l'écran
    heroSprite.setTextureRect(IntRect(heroAnim.x * SPRITE_SIZE, heroAnim.y * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE));
    // Animer en boucle la col (0 -> 3)
    // Si il s'est écoupé 0.1 seconde (1/10 de seconde)
    if (heroAnimClock.getElapsedTime().asSeconds() > 0.1f)
    { // Anim de 10 FPS
        // On est au bout de la texture (en largeur)
        if (heroAnim.x * SPRITE_SIZE >= heroTexture.getSize().x - SPRITE_SIZE)
        {
            heroAnim.x = 0;
            // Si l'anim d'atk tourne
            if (needResetAnim)
            {
                // On l'arrête & on retourne sur l'anim de marche
                needResetAnim = false;
                heroAnim.y -= 4;
            }
        }
        // On se décale de 1 col
        else
        {
            // Si le personnage n'est pas à l'arrêt = !idle
            if (!heroIdle || needResetAnim)
                heroAnim.x++;
        }
        heroAnimClock.restart();
    }
}

// Fonction explode
vector<string> explode(string const& s, char delim)
{
    vector<string> result;
    istringstream iss(s);

    for (string token; getline(iss, token, delim); )
    {
        result.push_back(move(token));
    }

    return result;
}

void UpdateMap()
{
    // Chargement map depuis un fichier
    ifstream ifs("res/map" + to_string(actualMap) + ".txt");
    string content((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
    cout << "Map chargée : " << endl << content << endl;
    // Découpe de la chaîne en char
    auto exploded = explode(content, ' ');
    // Boucler sur chaque char, le convertir en int, le stocker dans le tableau de int
    for (int i = 0; i < 450; i++)
        levelLoaded[i] = stoi(exploded[i]);

    ifstream ifsCol("res/map" + to_string(actualMap) + "_collisions.txt");
    string contentCol((istreambuf_iterator<char>(ifsCol)), (istreambuf_iterator<char>()));
    cout << contentCol << endl;
    auto explodedCol = explode(contentCol, ' ');
    for (int i = 0; i < 450; i++)
        levelLoadedCollisions[i] = stoi(explodedCol[i]);

    if (!mapInstance.load("res/tileset.png", Vector2u(SPRITE_SIZE, SPRITE_SIZE), levelLoaded, COL_COUNT, ROW_COUNT))
        cout << "err chargement map" << endl;
}

// Fonction de tir + déplacement + collision + ... du projectile / flèche 
void HandleBullet()
{
    if (bulletActive) // Si on a tiré
    {
        // Dans quelle direction ?
        switch (bulletDir)
        {
        case Down_Atk: // Si on tire vers le bas
            // On oriente la flèche vers le bas
            bullet.setRotation(270);
            // Puis on tire le projectile
            bullet.move(0, BULLET_SPEED);
            break;
        case Up_Atk:
            bullet.setRotation(90);
            bullet.move(0, -BULLET_SPEED);
            break;
        case Left_Atk:
            bullet.setRotation(0);
            bullet.move(-BULLET_SPEED, 0);
            break;
        case Right_Atk:
            bullet.setRotation(180);
            bullet.move(BULLET_SPEED, 0);
            break;
        }
        // On affiche la flèche
        window.draw(bullet);

        // Gérer la réinitialisation 
        if (bulletClock.getElapsedTime().asSeconds() > 1.5f) // 0.1f pour l'attaque à l'épée
        {
            bulletActive = false;
        }

        // Gestion de la collision entre la flèche et le monstre
        // Hit box du projectile
        FloatRect bulletHitbox;
        bulletHitbox = bullet.getGlobalBounds();
        // Hit box du mob
        FloatRect slimeHitbox;
        slimeHitbox = slimeSprite.getGlobalBounds();
        // Détection collision
        if (bulletHitbox.intersects(slimeHitbox))
        {
            // On masque la flèche et le monstre !
            mobDestroyed = true;
            bulletActive = false;
            slimeSprite.setPosition(10000, 10000);
        }
    }
}

// Déplacement + Animation monstre
void HandleMob()
{
    if (!mobDestroyed)
    {
        // Déplacement du mob
        slimeSprite.move(MOB_SPEED, 0);
        if (slimeSprite.getPosition().x > 600 || slimeSprite.getPosition().x < 100) MOB_SPEED *= -1;

        // Animation du mob
        slimeSprite.setTextureRect(IntRect(mobAnim.x * SPRITE_SIZE, mobAnim.y * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE));
        if (mobClock.getElapsedTime().asSeconds() > 0.2f)
        {
            if (mobAnim.x * SPRITE_SIZE >= slimeTexture.getSize().x - SPRITE_SIZE)
            {
                mobAnim.x = 0;
            }
            else
            {
                mobAnim.x++;
            }
            mobClock.restart();
        }
    }
}

// Débug visuel avec touche espace
void Debug()
{
    // Pour débug visuellement les collisions de la map
    for (unsigned int j = 0; j < ROW_COUNT; ++j)
    { // On boucle sur chaque ligne et col
        for (unsigned int i = 0; i < COL_COUNT; ++i)
        {
            if (levelLoadedCollisions[(i + j * COL_COUNT)] == 1)
            {
                Vector2f pos = Vector2f(i * SPRITE_SIZE, j * SPRITE_SIZE);
                rects[(i + j * COL_COUNT)].setPosition(pos);
                rects[(i + j * COL_COUNT)].setSize(Vector2f(SPRITE_SIZE, SPRITE_SIZE));
                rects[(i + j * COL_COUNT)].setFillColor(Color(250, 0, 0, 100));
                if (canShowCollisionsDebug)
                    window.draw(rects[(i + j * COL_COUNT)]);
            }
        }
    }
    // Fin débug collisions
}

// Fonction Start
void Start()
{
    // Création d'une fenêtre type RenderWindow
    window.create(VideoMode(WIN_WIDTH, WIN_HEIGHT, 32), "Mon jeu SFML", Style::Default);
    window.setVerticalSyncEnabled(true);

    // Chargement du visuel du heros
    if (!heroTexture.loadFromFile("res/hero_sheet.png"))
        cout << "Erreur chargement texture héros" << endl;
    // On applique la texture à notre Sprite
    heroSprite.setTexture(heroTexture);
    // On découpe pour afficher 1 case de 32x32 px
    heroSprite.setTextureRect(IntRect(heroAnim.x * SPRITE_SIZE, heroAnim.y * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE));
    // Préparation du monstre
    slimeTexture.loadFromFile("res/slime.png");
    slimeSprite.setTexture(slimeTexture);
    slimeSprite.setTextureRect(IntRect(0, 0, SPRITE_SIZE, SPRITE_SIZE));
    slimeSprite.setPosition(5 * SPRITE_SIZE, 5 * SPRITE_SIZE);
    slimeSprite.setScale(0.6f, 0.6f);
    // Bullet
    bTexture.loadFromFile("res/arrow.png");
    bullet.setTexture(bTexture);

    // Chargement map
    UpdateMap();
    // Portail vers maps 1 & 2
    goToMap2.setFillColor(Color(250, 0, 0, 100));
    goToMap2.setPosition(SPRITE_SIZE * 6, 0);
    goToMap1.setFillColor(Color(250, 0, 0, 100));
    goToMap1.setPosition(SPRITE_SIZE * 6, SPRITE_SIZE * 17);
}

// Fonction Update
void Update()
{
    CheckBtn();
    AnimPlayer();
    HandleMob();
    window.clear(Color::Black);
    window.draw(mapInstance);
    window.draw(heroSprite);
    window.draw(goToMap2);
    window.draw(goToMap1);
    window.draw(slimeSprite);
    HandleBullet();
    Debug();
    SimpleCollisions();
    window.display();
} 