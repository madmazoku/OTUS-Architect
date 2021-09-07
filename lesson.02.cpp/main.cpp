#include <iostream>
#include <list>

#include "Tank.h"
#include "Bunker.h"

int main()
{
	Tank tank({ 0, 0 }, { 0, 1 }, { 0, 1 }, 0.1, { 0, 1 }, 0.1);
	Bunker bunker({ 1,1 }, { 0,1 }, 0.1);
	std::list<Projectile*> projectiles;

	int count = 0;
	while (++count < 10) {
		tank.Execute(0.01);
		bunker.Execute(0.01);
		for (Projectile* projectile : projectiles)
			projectile->Execute(0.01);
		projectiles.push_back(tank.m_cannon.Shoot());
	}

	while (!projectiles.empty()) {
		Projectile* projectile = projectiles.back();
		projectiles.pop_back();
		delete projectile;
	}
}