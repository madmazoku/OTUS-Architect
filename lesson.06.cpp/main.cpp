#include <random>
#include <filesystem>
#include <iostream>
#include <thread>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

const std::string g_projectName(PROJECT_NAME);

void fillRandomBoard(sf::Texture& texture) {
	sf::Vector2u size = texture.getSize();
	unsigned long boardSize = size.x * size.y;
	sf::Uint8* pBoard = new sf::Uint8[boardSize * 4];

	std::random_device rd;
	std::default_random_engine re(rd());
	std::uniform_int_distribution<int> uniform_dist(0, 10);
	sf::Uint32* pBoardPos = reinterpret_cast<sf::Uint32*>(pBoard);
	sf::Uint32* pBoardEnd = pBoardPos + boardSize;
	while (pBoardPos < pBoardEnd) {
		sf::Uint32 cell = uniform_dist(re) == 0 ? 0xff : 0x00;
		*(pBoardPos++) = 0xff000000 | (cell) | (cell << 8) | (cell << 16);
	}
	texture.update(pBoard);

	delete[] pBoard;
}

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

	window.setVerticalSyncEnabled(false);

	sf::Texture texture;
	if (!texture.create(vmWindow.width, vmWindow.height))
		return -1;
	sf::Sprite sprite(texture);

	fillRandomBoard(texture);

	std::shared_ptr<sf::RenderTexture> pTextureIn = std::make_shared< sf::RenderTexture>();
	if (!pTextureIn->create(vmWindow.width, vmWindow.height))
		return -1;
	std::shared_ptr<sf::Sprite> pSpriteIn = std::make_shared< sf::Sprite>(pTextureIn->getTexture());
	pTextureIn->setRepeated(true);

	std::shared_ptr<sf::RenderTexture> pTextureOut = std::make_shared< sf::RenderTexture>();
	if (!pTextureOut->create(vmWindow.width, vmWindow.height))
		return -1;
	std::shared_ptr<sf::Sprite> pSpriteOut = std::make_shared< sf::Sprite>(pTextureOut->getTexture());
	pTextureOut->setRepeated(true);

	sf::Shader shader;
	shader.loadFromFile(g_projectName + "/shader.vert", sf::Shader::Vertex);
	shader.loadFromFile(g_projectName + "/shader.frag", sf::Shader::Fragment);

	shader.setUniform("textureIn", sf::Shader::CurrentTexture);
	shader.setUniform("textureSize", sf::Vector2f(float(vmWindow.width), float(vmWindow.height)));

	pTextureIn->clear();
	pTextureIn->draw(sprite);
	pTextureIn->display();

	sf::Clock clock;
	sf::Time lastTime = clock.getElapsedTime();
	long frameCount = 0;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		pTextureOut->clear();
		pTextureOut->draw(*pSpriteIn, &shader);
		pTextureOut->display();

		window.clear();
		window.draw(*pSpriteOut);
		window.display();

		//std::this_thread::sleep_for(std::chrono::milliseconds(1000));

		//std::this_thread::sleep_for(std::chrono::milliseconds(200));

		std::swap(pTextureIn, pTextureOut);
		std::swap(pSpriteIn, pSpriteOut);

		++frameCount;
		sf::Time currentTime = clock.getElapsedTime();
		float timeDiffSecond = currentTime.asSeconds() - lastTime.asSeconds();
		if (timeDiffSecond >= 1.0f && frameCount >= 10) {
			float fps = float(frameCount) / timeDiffSecond;
			std::cout << "FPS: " << floor(fps) << std::endl;
			lastTime = currentTime;
			frameCount = 0;

			sf::Image imageIn = pTextureIn->getTexture().copyToImage();
			sf::Image imageOut = pTextureOut->getTexture().copyToImage();
			const sf::Uint32* pBoardInPos = reinterpret_cast<const sf::Uint32*>(imageIn.getPixelsPtr());
			const sf::Uint32* pBoardOutPos = reinterpret_cast<const sf::Uint32*>(imageOut.getPixelsPtr());
			const sf::Uint32* pBoardInEnd = pBoardInPos + vmWindow.width * vmWindow.height;
			long diffCells = 0;
			long emptyCells = 0;
			while (pBoardInPos < pBoardInEnd)
				if (((*(pBoardInPos++) & 0xff) > 0x7f) != ((*(pBoardOutPos++) & 0xff) > 0x7f))
					diffCells++;
			float diffCellsPrc = diffCells * 100.0f / (vmWindow.width * vmWindow.height);
			std::cout << "different cells: " << diffCells << " [ " << diffCellsPrc << "% ]" << std::endl;
			if (diffCellsPrc < 0.9) {
				fillRandomBoard(texture);
				pTextureIn->clear();
				pTextureIn->draw(sprite);
				pTextureIn->display();
			}
		}
	}
}