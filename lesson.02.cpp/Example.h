#pragma once

#include "Vector.h"

class Movable {
public:
	Vector m_position;
	Vector m_velocity;

	const Vector& getPosition() const { return m_position; }
	void setPosition(const Vector& position) { m_position = position; }
	const Vector& getVelocity() const { return m_velocity; }
};

class Move {
public:
	Movable& m_movable;

	Move(Movable& movable) : m_movable(movable) {};

	void Execute(double step) { m_movable.setPosition(m_movable.getPosition() + m_movable.getVelocity() * step); }

};

// можно сделать генератор, итеративно обновляющий координаты
