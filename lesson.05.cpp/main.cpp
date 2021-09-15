#include <random>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cmath>
#include <algorithm>
#include <list>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

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

void fillBoardWithRandom(sf::Uint8* pBoard, long  width, long height) {
	std::random_device rd;
	std::default_random_engine re(rd());
	std::uniform_int_distribution<int> uniform_dist(0, 10);
	sf::Uint8* pBoardEnd = pBoard + width * height;
	while (pBoard < pBoardEnd)
		*(pBoard++) = uniform_dist(re) == 0 ? 0xff : 00;
}

void doLifeStep(long ppOffsets[9][8], sf::Uint8* ppBoards[3], long width, long height, long yFrom, long yTo) {
	long yOffset = yFrom * width;
	sf::Uint8* pBoardNew = ppBoards[0] + yOffset;
	sf::Uint8* pBoardOld = ppBoards[1] + yOffset;
	for (long y = yFrom; y < yTo; ++y)
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

void copyBoardToPixels(sf::Uint8* pBoard, sf::Uint8* pPixels, long width, long height, long yFrom, long yTo) {
	long yFromOffset = yFrom * width;
	long yToOffset = yTo * width;
	sf::Uint8* pBoardFrom = pBoard + yFromOffset;
	sf::Uint8* pBoardTo = pBoard + yToOffset;
	sf::Uint32* pPixels32From = reinterpret_cast<sf::Uint32*>(pPixels) + yFromOffset;
	while (pBoardFrom < pBoardTo)
		*(pPixels32From++) = cellToPixel(*(pBoardFrom++));
}

void rotateBoards(sf::Uint8* ppBoards[3]) {
	std::swap(ppBoards[2], ppBoards[1]);
	std::swap(ppBoards[1], ppBoards[0]);
}

bool compareBoards(sf::Uint8* pBoardSrc, sf::Uint8* pBoardDst, long width, long height) {
	sf::Uint8* pBoardSrcEnd = pBoardSrc + width * height;
	while (pBoardSrc < pBoardSrcEnd)
		if ((*(pBoardSrc++) > 0) != (*(pBoardDst++) > 0))
			return false;
	return true;
}

int main()
{
	sf::VideoMode vmDesktop = sf::VideoMode::getDesktopMode();
#ifdef _DEBUG
	sf::VideoMode vmWindow(vmDesktop.width >> 2, vmDesktop.height >> 2);
	sf::RenderWindow window(vmWindow, "SFML works!");
#else
	sf::VideoMode vmWindow(vmDesktop.width, vmDesktop.height);
	sf::RenderWindow window(vmWindow, "SFML works!", sf::Style::None);
#endif // _DEBUG
	window.setVerticalSyncEnabled(false);

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
	sf::Uint8* ppBoards[3];
	ppBoards[0] = new sf::Uint8[boardSize];
	ppBoards[1] = new sf::Uint8[boardSize];
	ppBoards[2] = new sf::Uint8[boardSize];
	sf::Uint8* pPixels = new sf::Uint8[(long long)(boardSize) << 2];

	fillBoardWithRandom(ppBoards[0], vmWindow.width, vmWindow.height);

	unsigned int steps = std::thread::hardware_concurrency();
	unsigned int step = vmWindow.height / steps;
	if (step * steps < vmWindow.height)
		++steps;

	std::list<std::thread> threads;

	std::mutex lock;
	std::condition_variable cvCheck;

	bool bRun = false;
	bool bRender = false;
	long inRun = 0;
	int cnt = 0;
	for (unsigned int yFrom = 0; yFrom < vmWindow.height; yFrom += step) {
		unsigned int yTo = std::min<long>(yFrom + step, vmWindow.height);
		threads.push_back(std::thread([&](unsigned int yFrom, unsigned int yTo) {
			{
				std::unique_lock<std::mutex> ul(lock);
				cvCheck.wait(ul, [&] { return bRun; });
			}
			while (true) {
				{
					std::unique_lock<std::mutex> ul(lock);
					cvCheck.wait(ul, [&] { return !bRun || bRender; });
					if (!bRun)
						break;
					++inRun;
				}
				doLifeStep(ppOffsets, ppBoards, vmWindow.width, vmWindow.height, yFrom, yTo);
				copyBoardToPixels(ppBoards[0], pPixels, vmWindow.width, vmWindow.height, yFrom, yTo);
				{
					std::lock_guard<std::mutex> lg(lock);
					if (--inRun == 0)
						bRender = false;
				}
				cvCheck.notify_all();
				{
					std::unique_lock<std::mutex> ul(lock);
					cvCheck.wait(ul, [&] {return !bRender; });
				}
			}
			}, yFrom, yTo));
	}

	{
		std::lock_guard<std::mutex> lg(lock);
		bRun = true;
	}
	cvCheck.notify_all();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		rotateBoards(ppBoards);

		{
			std::lock_guard<std::mutex> lg(lock);
			bRender = true;
		}
		cvCheck.notify_all();
		{
			std::unique_lock<std::mutex> ul(lock);
			cvCheck.wait(ul, [&] { return !bRender; });
		}

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

			if (compareBoards(ppBoards[0], ppBoards[2], vmWindow.width, vmWindow.height))
				fillBoardWithRandom(ppBoards[0], vmWindow.width, vmWindow.height);
		}
	}

	{
		std::lock_guard<std::mutex> lg(lock);
		bRun = false;
	}
	cvCheck.notify_all();

	while (!threads.empty()) {
		threads.front().join();
		threads.pop_front();
	}

	delete[] pPixels;
	delete[] ppBoards[0];
	delete[] ppBoards[1];
	delete[] ppBoards[2];

	return 0;
}