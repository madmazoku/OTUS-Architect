#include <iostream>
#include <random>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

template<typename T>
inline T sqr(T x) { return x * x; }

template<typename T>
inline T powN(T x, size_t N) {
	if (N == 0)
		return 1;
	if (N == 1)
		return x;
	if (N == 2)
		return x * x;

	T half = powN(x, N >> 1);
	T r = half * half;
	if (N & 0x1)
		r *= x;

	return r;
}

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

	std::random_device rd;
	std::default_random_engine re(rd());
	std::uniform_real_distribution<float> uniform_dist_pos_x(2.0f, vmWindow.width - 2.0f);
	std::uniform_real_distribution<float> uniform_dist_pos_y(2.0f, vmWindow.height - 2.0f);
	std::uniform_real_distribution<float> uniform_dist_vel(-10, 10);

	std::size_t count = 100;
	sf::VertexArray va(sf::PrimitiveType::Lines, count << 1);

	sf::Vector2f* pPos = new sf::Vector2f[count];
	sf::Vector2f* pVel = new sf::Vector2f[count];
	sf::Vector2f* pAcc = new sf::Vector2f[count];
	for (int idx = 0; idx < count; ++idx) {
		pAcc[idx] = sf::Vector2f(0.0f, 0.0f);
		pVel[idx] = sf::Vector2f(uniform_dist_vel(re), uniform_dist_vel(re));
		pPos[idx] = sf::Vector2f(uniform_dist_pos_x(re), uniform_dist_pos_y(re));
		va[idx << 1].position = pPos[idx];
		va[idx << 1].color = sf::Color::White;
		va[(idx << 1) + 1].position = pPos[idx] + pVel[idx];
		va[(idx << 1) + 1].color = sf::Color::White;
	}

	sf::Clock clock;
	sf::Time lastTime = clock.getElapsedTime();
	sf::Time prevTime = clock.getElapsedTime();
	long frameCount = 0;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(va);
		window.display();

		sf::Time currentTime = clock.getElapsedTime();
		float timeTickSecond = currentTime.asSeconds() - prevTime.asSeconds();
		if (timeTickSecond > 0.01f)
			timeTickSecond = 0.01f;
		prevTime = currentTime;

		for (int idx = 0; idx < count; ++idx) {
			va[idx << 1].position = pPos[idx];
			va[(idx << 1) + 1].position = pPos[idx] + pVel[idx];
		}

		for (int idx = 0; idx < count - 1; ++idx)
			for (int sidx = idx + 1; sidx < count; ++sidx) {
				sf::Vector2f vDiff = pPos[idx] - pPos[sidx];
				float length = std::sqrt(sqr(vDiff.x) + sqr(vDiff.y));
				sf::Vector2f vDiffNorm = vDiff / length;
				float force = length > 1.0f ? 1.0f / sqr(0.01f * length) : 0.0f;
				sf::Vector2f acc = vDiffNorm * force;
				pAcc[idx] -= acc;
				pAcc[sidx] += acc;
			}

		for (int idx = 0; idx < count; ++idx) {
			pPos[idx] += pVel[idx] * timeTickSecond;
			pVel[idx] += pAcc[idx] * timeTickSecond;
			pAcc[idx] = sf::Vector2f(0.0f, 0.0f);
		}

		++frameCount;
		float timeDiffSecond = currentTime.asSeconds() - lastTime.asSeconds();
		if (timeDiffSecond >= 1.0f && frameCount >= 10) {
			float fps = float(frameCount) / timeDiffSecond;
			std::cout << "FPS: " << floor(fps) << std::endl;
			lastTime = currentTime;
			frameCount = 0;
		}
	}
}