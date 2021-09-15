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
	for (long oy = -1, oidx = 0; oy <= 1; ++oy)
		for (long ox = -1; ox <= 1; ++ox, ++oidx) {
			for (long y = -1, idx = 0; y <= 1; ++y)
				for (long x = -1; x <= 1; ++x)
					if (x != 0 || y != 0)
					{
						long offset = 0;
						offset += x - width * (x * ox > 0 ? ox : 0);
						offset += width * (y - height * (y * oy > 0 ? oy : 0));
						ppOffsets[oidx][idx] = offset;
						++idx;
					}
		}
}

void fillBoardWithRandom(sf::Uint8* pBoard, long boardSize) {
	std::random_device rd;
	std::default_random_engine re(rd());
	std::uniform_int_distribution<int> uniform_dist(1, 10);
	sf::Uint8* pBoardEnd = pBoard + boardSize;
	while (pBoard < pBoardEnd)
		*(pBoard++) = uniform_dist(re) > 9 ? 0xff : 00;
}

void doLifeStep(long ppOffsets[9][8], sf::Uint8* ppBoards[2], long width, long height) {
	long size = width * height;
	sf::Uint8* pBoardNew = ppBoards[0];
	sf::Uint8* pBoardOld = ppBoards[1];
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
				neighbors += *(pBoardOld + *pOffsets) > 0 ? 1 : 0;
			if (*pBoardOld > 0)
				*pBoardNew = (neighbors == 2 || neighbors == 3) ? (*pBoardOld - 1) : 0x00;
			else
				*pBoardNew = neighbors == 3 ? 0xff : 0x00;
		}
}

inline sf::Uint32 cellToPixel(sf::Uint8 cell) {
	return 0xff000000 | (sf::Uint32(cell)) | (sf::Uint32(cell) << 8) | (sf::Uint32(cell) << 16);
}

void copyBoardToPixels(sf::Uint8* pBoard, sf::Uint8* pPixels, long boardSize) {
	sf::Uint8* pBoardEnd = pBoard + boardSize;
	sf::Uint32* pPixels32 = reinterpret_cast<sf::Uint32*>(pPixels);
	while (pBoard < pBoardEnd)
		*(pPixels32++) = cellToPixel(*(pBoard++));
}

void rotateBoards(sf::Uint8* ppBoards[2]) {
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
	sf::Uint8* ppBoards[2];
	ppBoards[0] = new sf::Uint8[boardSize];
	ppBoards[1] = new sf::Uint8[boardSize];
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