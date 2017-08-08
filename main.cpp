#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <level.h>
#include <list>
#include <player.h>

using namespace sf;
using namespace std;

class Entity {
public:
	vector<Object> obj;
	float x, y, dx, dy, speed, moveTimer;
	int width, height, health;
	bool life, isMove, onGround;
	Texture texture;
	Sprite sprite;
	String name;
	Entity (Image &image, String &name, float &x, float &y, int &width, int &height) {
		this->x = x; this->y = y;
		this->width = width; this->height = height; 	this->name = name;
		dx = 0; dy = 0;
		moveTimer = 0;speed = 0; health = 100;
		life = true; onGround = false; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(width / 2, height / 2);
	}

	virtual void update(float time) = 0;

	FloatRect getRect() { return FloatRect(x, y, width, height); }
};

class Enemy : public Entity {
public:
	Enemy(Image &image, String name, Level &level, float x, float y, int width, int height) : Entity(image, name, x, y, width, height) {
		obj = level.GetObjects("solid");
		if (name == "EasyEnemy") {
			sprite.setTextureRect(IntRect(0, 0, width, height));
			dx = 0.1;
		}
	}

	void checkCollisionWithMap(float dx, float dy) {
		for (int i = 0; i < obj.size(); ++i) {
			if (getRect().intersects(obj[i].rect)) {
				if (dy > 0) y = obj[i].rect.top - height;
				if (dy < 0) y = obj[i].rect.top + obj[i].rect.height;
				if (dx > 0) { x = obj[i].rect.left - width; this->dx = -0.1; sprite.scale(-1, 1); }
				if (dx < 0) {
					x = obj[i].rect.left+ obj[i].rect.width; this->dx = 0.1; sprite.scale(-1, 1); }
			}
		}
	}

	void update(float time) {
		if (name == "EasyEnemy") {
			checkCollisionWithMap(dx, 0);
			x += dx * time;
			sprite.setPosition(x + width / 2, y + height / 2);
			if (health <= 0) life = false;
		}
	}

};

class Bullet : public Entity {
public:
	int direction;

	Bullet (Image &image, String name, Level &lvl, float x, float y, int width, int height, int dir) : Entity(image, name, x, y, width,height) {
		obj = lvl.GetObjects("solid");
		direction = dir;
		speed = 0.8;
	}

	void update(float time) {
		switch (direction) {
			case 0: dx = -speed; dy = 0; break;
			case 1: dx = speed; dy = 0; break;
			case 2: dx = 0; dy = -speed; break;
			case 3: dx = 0; dy = -speed; break;
			case 4: dx = 0; dy = -speed; break;
			case 5: dx = 0; dy = -speed; break;
			case 6: dx = speed; dy = -speed; break;
		}

		x += dx * time;
		y += dy * time;

		if (x <= 0) x = 1;
		if (y <= 0) y = 1;

		for (auto &i : obj)
			if (getRect().intersects(i.rect)) life = false;

		sprite.setPosition(x + width / 2, y + height / 2);
	}
};



int main() {
	RenderWindow window(VideoMode(1366, 768, 32), "StarCraft2",Style::Fullscreen);
	View view;
	view.setSize(window.getSize().x, window.getSize().y);
	view.setCenter(window.getSize().x / 2, window.getSize().y / 2);
	Font font;
	font.loadFromFile("../font/Roboto-Italic.ttf");

	Level lvl("../image/map.tmx");
	Clock clock;
	Player player;

	while (window.isOpen()) {
		double time = clock.getElapsedTime().asMicroseconds();

		clock.restart();
		time /= 500;

		Event event = {};
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) window.close();

		}

		controlView(view, window, time);
		window.setView(view);
		lvl.Draw(window);
		player.drawResources(window, font);
		window.display();
	}

	return EXIT_SUCCESS;
}
#pragma clang diagnostic pop