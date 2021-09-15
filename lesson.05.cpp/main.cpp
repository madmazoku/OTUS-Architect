#include <random>
#include <iostream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

enum class OffsetType {
	TopLeft = 0,
	TopMid = 1,
	TopRight = 2,
	MidLeft = 3,
	MidMid = 4,
	MidRight = 5,
	BottomLeft = 6,
	BottomMid = 7,
	BottomRight = 8
};

void fillOffsets(long ppOffsets[9][8], long width, long height) {
	long size = width * height;
	std::cout << "width: " << width << "; height: " << height << "; size: " << size << std::endl;
	for (long oy = -1, oidx = 0; oy <= 1; ++oy)
		for (long ox = -1; ox <= 1; ++ox, ++oidx) {
			std::cout << "ox: " << ox << "; oy: " << oy << "; oidx: " << oidx << std::endl;
			for (long y = -1, idx = 0; y <= 1; ++y)
				for (long x = -1; x <= 1; ++x)
					if (x != 0 || y != 0)
					{
						std::cout << "\t" << "x: " << x << "; y: " << y << "; idx: " << idx;
						long offset = 0;
						offset += x - ox * width * (x * ox > 0 ? 1 : 0);
						offset += y * width - oy * size * (y * oy > 0 ? 1 : 0);
						ppOffsets[oidx][idx] = offset;
						std::cout << "; offset: " << offset << std::endl;
						++idx;
					}
		}
}

void fillBoardWithRandom(bool* pBoard, long boardSize) {
	std::random_device rd;
	std::default_random_engine re(rd());
	std::uniform_int_distribution<int> uniform_dist(1, 10);
	bool* pBoardEnd = pBoard + boardSize;
	while (pBoard < pBoardEnd)
		*(pBoard++) = uniform_dist(re) > 9;
}

void doLifeStep(long ppOffsets[9][8], bool* ppBoards[2], long width, long height) {
	long size = width * height;
	bool* pBoardNew = ppBoards[0];
	bool* pBoardOld = ppBoards[1];
	for (long y = 0; y < height; ++y)
		for (long x = 0; x < width; ++x, ++pBoardOld, ++pBoardNew) {
			long oidx = 0;
			if (x == 0)
				oidx += 0;
			else if (x == width - 1)
				oidx += 2;
			else
				oidx += 1;
			if (y == 0)
				oidx += 0;
			else if (y == height - 1)
				oidx += 6;
			else
				oidx += 3;
			long* pOffsets = ppOffsets[oidx];
			long neighbors = 0;
			for (long idx = 0; idx < 8; ++idx, ++pOffsets)
				neighbors += *(pBoardOld + *pOffsets) ? 1 : 0;
			*pBoardNew = neighbors == 2 ? *pBoardOld : neighbors == 3;
		}
}

inline sf::Uint32 cellToPixel(bool cell) {
	return cell ? 0xffffffff : 0xff00000;
}

void copyBoardToPixels(bool* pBoard, sf::Uint8* pPixels, long boardSize) {
	bool* pBoardEnd = pBoard + boardSize;
	sf::Uint32* pPixels32 = reinterpret_cast<sf::Uint32*>(pPixels);
	while (pBoard < pBoardEnd)
		*(pPixels32++) = cellToPixel(*(pBoard++));
}

void rotateBoards(bool* ppBoards[2]) {
	std::swap(ppBoards[0], ppBoards[1]);
}

int main()
{
	sf::VideoMode vmDesktop = sf::VideoMode::getDesktopMode();
	sf::VideoMode vmWindow(vmDesktop.width, vmDesktop.height);

	sf::RenderWindow window(vmWindow, "SFML works!", sf::Style::None);

	sf::Texture texture;
	if (!texture.create(vmWindow.width, vmWindow.height))
		return -1;
	sf::Sprite sprite(texture);

	sf::Clock clock;
	sf::Time lastTime = clock.getElapsedTime();
	long frameCount = 0;

	long ppOffsets[9][8];
	fillOffsets(ppOffsets, vmWindow.width, vmWindow.height);

	long boardSize = vmWindow.width * vmWindow.height;
	bool* ppBoards[2];
	ppBoards[0] = new bool[boardSize];
	ppBoards[1] = new bool[boardSize];
	sf::Uint8* pPixels = new sf::Uint8[(long long)(boardSize) << 2];

	fillBoardWithRandom(ppBoards[0], boardSize);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		rotateBoards(ppBoards);
		doLifeStep(ppOffsets, ppBoards, vmWindow.width, vmWindow.height);
		copyBoardToPixels(ppBoards[0], pPixels, boardSize);

		texture.update(pPixels);

		window.clear();
		window.draw(sprite);
		window.display();

		++frameCount;
		sf::Time currentTime = clock.getElapsedTime();
		float timeDiffSecond = currentTime.asSeconds() - lastTime.asSeconds();
		if (timeDiffSecond >= 1.0f && frameCount >= 10) {
			float fps = float(frameCount) / timeDiffSecond;
			std::cout << "FPS: " << floor(fps) << std::endl;
			lastTime = currentTime;
			frameCount = 0;
		}
	}

	delete[] pPixels;
	delete[] ppBoards[0];
	delete[] ppBoards[1];
}