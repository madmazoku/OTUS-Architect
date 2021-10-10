#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include <filesystem>
#include <stdexcept>
#include <list>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

inline float sqr(float x) { return x * x; }

struct Shell {
	sf::Vector2f pos;
	sf::Vector2f move;
	float moveLen;
	float angle;
	float totalMove;
};

const std::string g_projectName(PROJECT_NAME);

int main()
{
	std::cout << "Project: " << g_projectName << std::endl;
	sf::VideoMode vmDesktop = sf::VideoMode::getDesktopMode();
#ifdef _DEBUG
	sf::VideoMode vmWindow(vmDesktop.width >> 1, vmDesktop.height >> 1);
	sf::RenderWindow window(vmWindow, "SFML works!");
#else
	sf::VideoMode vmWindow(vmDesktop.width, vmDesktop.height);
	sf::RenderWindow window(vmWindow, "SFML works!", sf::Style::None);
#endif // _DEBUG

	std::cout << "Window size: " << "{ " << vmWindow.width << ", " << vmWindow.height << " }" << std::endl;

	unsigned int joyConnect = 0;
	for (int idx = 0; idx < sf::Joystick::Count; ++idx)
		if (sf::Joystick::isConnected(idx))
			std::cout << "Joystick " << idx << ": " << sf::Joystick::getIdentification(idx).name.toAnsiString() << std::endl;
		else
			std::cout << "Joystick " << idx << ": Disconnected" << std::endl;

	if (sf::Joystick::isConnected(0))
		std::cout << "Joystick selected: " << sf::Joystick::getIdentification(0).name.toAnsiString() << std::endl;
	else {
		std::cout << "Joystick required" << std::endl;
		return -1;
	}

	sf::Shader shaderPixelate;
	//shaderPixelate.loadFromFile(g_projectName + "/shader.vert", sf::Shader::Vertex);
	shaderPixelate.loadFromFile(g_projectName + "/pixelate.frag", sf::Shader::Fragment);

	shaderPixelate.setUniform("textureIn", sf::Shader::CurrentTexture);
	shaderPixelate.setUniform("textureSize", sf::Vector2f(float(vmWindow.width), float(vmWindow.height)));

	sf::RenderTexture textureRender;
	if (!textureRender.create(vmWindow.width, vmWindow.height))
		throw std::runtime_error("can't create render texture");

	sf::Sprite spriteRender(textureRender.getTexture());

	sf::Texture textureTank;
	if (!textureTank.loadFromFile(g_projectName + "/tank.png"))
		throw std::runtime_error("can't load tank texture");

	sf::Vector2f tankPos(vmWindow.width * 0.5f, vmWindow.height * 0.5f);
	sf::Sprite spriteTank(textureTank, sf::IntRect(1, 1, 32, 32));
	spriteTank.setOrigin(16, 16);
	spriteTank.setScale(4.0f, 4.0f);
	spriteTank.setPosition(tankPos);
	float totalMove = 0;

	sf::Texture textureShell;
	if (!textureShell.loadFromFile(g_projectName + "/shell.png"))
		throw std::runtime_error("can't load shell texture");

	sf::Sprite spriteShell(textureShell, sf::IntRect(1, 1, 8, 8));
	spriteShell.setOrigin(4, 4);
	spriteShell.setScale(4.0f, 4.0f);
	spriteShell.setPosition(tankPos);

	std::list<Shell> shellList;

	sf::Clock clock;
	sf::Time lastTime = clock.getElapsedTime();
	sf::Time prevTime = clock.getElapsedTime();
	sf::Time lastShootTime = clock.getElapsedTime();
	long frameCount = 0;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		textureRender.clear();
		textureRender.draw(spriteTank);
		for (Shell& s : shellList) {
			spriteShell.setPosition(s.pos);
			spriteShell.setRotation(s.angle);
			spriteShell.setTextureRect(sf::IntRect(10 * static_cast<int>(3.0f - s.totalMove) + 1, 1, 8, 8));
			textureRender.draw(spriteShell);
		}
		textureRender.display();

		window.clear();
		window.draw(spriteRender);
		window.draw(spriteRender, &shaderPixelate);
		window.display();

		sf::Time currentTime = clock.getElapsedTime();
		float timeTickSecond = currentTime.asSeconds() - prevTime.asSeconds();
		if (timeTickSecond > 0.01f)
			timeTickSecond = 0.01f;
		prevTime = currentTime;

		sf::Vector2f moveDirection(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X), sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y));
		float moveLen = std::min(std::sqrtf(sqr(moveDirection.x) + sqr(moveDirection.y)), 100.0f);
		float moveAngle = 0;
		float stepAngle = 0;
		if (moveLen > 10) {
			moveAngle = (float(M_PI) - std::atan2f(moveDirection.x, moveDirection.y)) * float(180.0 / M_PI);
			float stepAngle = (moveAngle - spriteTank.getRotation());
			if (std::fabsf(stepAngle) > 180.0f)
				stepAngle = std::copysignf(180.0f, stepAngle) - stepAngle;
			float minStepAngel = std::copysignf(90.0f * timeTickSecond, stepAngle);
			if (std::fabsf(stepAngle) > std::fabs(minStepAngel))
				stepAngle = minStepAngel;
			spriteTank.rotate(stepAngle);

			float stepMove = moveLen * timeTickSecond;
			sf::Vector2f move(
				stepMove * std::sinf(spriteTank.getRotation() * float(M_PI / 180.0)),
				-stepMove * std::cosf(spriteTank.getRotation() * float(M_PI / 180.0))
			);
			totalMove += stepMove;
			while (totalMove >= 3.0f)
				totalMove -= 3.0f;
			spriteTank.setTextureRect(sf::IntRect(34 * static_cast<int>(3.0f - totalMove) + 1, 1, 32, 32));
			spriteTank.move(move);
		}

		for (Shell& s : shellList) {
			s.pos.x += s.move.x * timeTickSecond;
			s.pos.y += s.move.y * timeTickSecond;
			s.totalMove += s.moveLen * timeTickSecond;
			while (s.totalMove >= 3.0f)
				s.totalMove -= 3.0f;
		}
		shellList.remove_if([&vmWindow](const Shell& s) {
			return s.pos.x < -8.0f || s.pos.y < -8.0f || s.pos.x > vmWindow.width + 8.0f || s.pos.y > vmWindow.height + 8.0f;
			});

		float shootDiffSecond = currentTime.asSeconds() - lastShootTime.asSeconds();
		if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::V) > 0.0f && shootDiffSecond > 0.2f) {
			float angle = spriteTank.getRotation();
			sf::Vector2f direction(
				std::sinf(angle * float(M_PI / 180.0)),
				-std::cosf(angle * float(M_PI / 180.0))
			);
			sf::Vector2f pos(spriteTank.getPosition().x + direction.x * 32.0f, spriteTank.getPosition().y + direction.y * 32.0f);
			float moveLen = 320.0f;
			sf::Vector2 move(direction.x * moveLen, direction.y * moveLen);

			lastShootTime = currentTime;
			shellList.push_back({ pos,move,moveLen,angle, 0 });
		}

		++frameCount;
		float timeDiffSecond = currentTime.asSeconds() - lastTime.asSeconds();
		if (timeDiffSecond >= 1.0f && frameCount >= 10) {
			float fps = float(frameCount) / timeDiffSecond;
			std::cout << "FPS: " << floor(fps) << std::endl;
			lastTime = currentTime;
			frameCount = 0;

			//std::cout << "PovX       : " << sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovX) << std::endl;
			//std::cout << "PovY       : " << sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY) << std::endl;
			//std::cout << "R (R vert) : " << sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::R) << std::endl;
			//std::cout << "Z (R horiz): " << sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Z) << std::endl;
			//std::cout << "U (LT)     : " << sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::U) << std::endl;
			//std::cout << "V (RT)     : " << sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::V) << std::endl;
			//std::cout << "X (L horiz): " << sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) << std::endl;
			//std::cout << "Y (L vert) : " << sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) << std::endl;
			//std::cout << "move len: " << moveLen << "; move angle: " << moveAngle << " / " << moveAngle / M_PI * 180 << std::endl;
		}
	}
}