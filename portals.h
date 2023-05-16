#ifndef PORTALS_H
#define PORTALS_H

#include <SFML/Graphics.hpp>
#include <iostream>

void UpdateMap();

Sprite heroSprite;
// Transition map
Map mapInstance;
int actualMap = 1;
FloatRect heroHitbox;
RectangleShape goToMap2(Vector2f(32, 32));
FloatRect goToMap2Hitbox;
RectangleShape goToMap1(Vector2f(32, 32));
FloatRect goToMap1Hitbox;

void SimpleCollisions()
{
    goToMap2Hitbox = goToMap2.getGlobalBounds();
    goToMap1Hitbox = goToMap1.getGlobalBounds();
    heroHitbox = heroSprite.getGlobalBounds();
    if (heroHitbox.intersects(goToMap2Hitbox) && actualMap == 1)
    {
        actualMap = 2;
        UpdateMap();
        heroSprite.setPosition(heroSprite.getPosition().x, heroSprite.getPosition().y + (15 * 32));
    }
    if (heroHitbox.intersects(goToMap1Hitbox) && actualMap == 2)
    {
        actualMap = 1;
        UpdateMap();
        heroSprite.setPosition(heroSprite.getPosition().x, heroSprite.getPosition().y - (15 * 32));
    }
}
#endif