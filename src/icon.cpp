#include "icon.h"

Icon::Icon(String filePath, string name, IntRect rect, int mineral, int gas) {
	this->mineral = mineral;
	this->gas = gas;
	this->name = name;
	image.loadFromFile("../image/" + filePath);
	image.createMaskFromColor(Color::Black);
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	sprite.setTextureRect(rect);
	sprite.setScale(2, 2);
}

void Icon::draw(RenderWindow &window,const Vector2f &position) {
	sprite.setPosition(position);
	window.draw(sprite);
}

void Icon::checkEnable(Resource &mineral, Resource &gas) {
	if (this->mineral > mineral.getNumber() || this->gas > gas.getNumber()) {
		sprite.setColor(Color::White);
		enable = false;
	}else {
		sprite.setColor(Color::Yellow);
		enable = true;
	}
}

void Icon::drawPrice(RenderWindow &window, Font &font) {
	Text text("", font, 14);
	text.setColor(Color::White);
	text.setString(name + " :\nCost:\n" + to_string(mineral) + " minerals\n" + to_string(gas) + " gas");
	text.setPosition(sprite.getGlobalBounds().left - 25, sprite.getGlobalBounds().top - 72);
	window.draw(text);
}

FloatRect Icon::getRect() {
	return sprite.getGlobalBounds();
}

bool Icon::isEnable() {
	return enable;
}

int Icon::getMineralNumber() {
	return mineral;
}

int Icon::getGasNumber() {
	return gas;
}

void Icon::subtractPrice(Resource &mineral, Resource &gas) {
	mineral.setNumber(mineral.getNumber() - this->mineral);
	gas.setNumber(gas.getNumber() - this->gas);
	checkEnable(mineral,gas);
}