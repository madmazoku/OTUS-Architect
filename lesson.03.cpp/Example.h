/*
#pragma once

#include <map>
#include <any>
#include <variant>
#include <string>
#include <memory>
#include <exception>
#include <stdexcept>

class UObject {
private:
	typedef std::map<std::string, std::any> str2any;
	str2any m_properties;

public:

	const std::any& GetProperty(const std::string& key) const {
		str2any::const_iterator it = m_properties.find(key);
		if (it == m_properties.end())
			throw std::invalid_argument(key);
		return *it;
	}
	void SetProperty(const std::string& key, const std::any& value) {
		m_properties[key] = value;
	}
	bool HasProperty(const std::string& key) const {
		return m_properties.find(key) != m_properties.end();
	}
};

class Vector
{
public:
	double m_x;
	double m_y;

	Vector() : m_x(0), m_y(0) {}
	Vector(double x, double y) : m_x(x), m_y(y) {}

	Vector operator+(const Vector& v) const { return Vector(m_x + v.m_x, m_y + v.m_y); }
	Vector& operator+=(const Vector& v) { m_x += v.m_x; m_y += v.m_y; return *this; }

	Vector operator*(double d) const { return Vector(m_x * d, m_y * d); }

	double operator!() const { return sqrt(m_x * m_x + m_y * m_y); }

	Vector operator/(double d) const { return Vector(m_x / d, m_y / d); }
};

typedef std::variant<Vector, double> Property;

std::any a = Vector();
Vector v = std::any_cast<Vector>(a);

Property var = Vector();

class IMovable {
public:
	virtual const Vector& GetPosition() = 0;
	virtual void SetPosition(const Vector&) = 0;
	virtual const Vector& GetVelocity() = 0;
};

class MovableAdapter : public IMovable {
public:
	UObject* m_pUObject;

	MovableAdapter(UObject* pUObject): m_pUObject(pUObject) {}

	virtual const Vector& GetPosition() override {
		return std::any_cast<Vector>(m_pUObject->GetProperty("position"));
	}
	virtual void SetPosition(const Vector& v) override {
		m_pUObject->SetProperty("position", v);
	}
	virtual const Vector& GetVelocity() override {
		return std::any_cast<Vector>(m_pUObject->GetProperty("position"));
	}
};

Move* move = new Move(new MovableAdapter(std::make_shared<UObject>()));

Command move = IoC.resolve<Command>("move", obj);

class ICommand {
public:
	virtual void Execute() = 0;
};

class ICheckFuelable {
public:
	virtual double GetFuel() = 0;
	virtual double GetFuelVelocity() = 0;
};

class CheckFuel : ICommand {
public:
	ICheckFuelable* m_pCheckFuelable;

	CheckFuel(ICheckFuelable* pCheckFuelable) : m_pCheckFuelable(pCheckFuelable) {}

	virtual void Execute() override {
		if (m_pCheckFuelable->GetFuel() - m_pCheckFuelable->GetFuelVelocity() < 0)
			throw CommandException();
	}
};

class BurnFuel : ICommand {
public:
	IBurnFuelable* m_pBurnFuelable;

	BurnFuel(IBurnFuelable* pBurnFuelable) : m_pBurnFuelable(pBurnFuelable) {}

	virtual void Execute() override {
		m_pBurnFuelable->SetFuel(m_pBurnFuelable->GetFuel() - m_pBurnFuelable->GetFuelVelocity())
	}
};

class Move : public ICommand {
public:
	IMovable* m_pMovable;

	Move(IMovable* pMovable) : m_pMovable(pMovable) {};

	virtual void Execute() override {
		m_pMovable->SetPosition(m_pMovable->GetPosition() + m_pMovable->GetVelocity();
	}
};

class CheckFuelAdapter : public ICheckFuelable {
public:
	UObject* m_pUObject;

	CheckFuelAdapter(UObject* pUObject) : m_pUObject(pUObject) {}

	virtual double GetFuel() {
		return IoC.resolve<double>("CheckFuel_fuel", obj);

		return std::any_cast<double>(m_pUObject->GetProperty("fuel"));
	}
	virtual double GetFuelVelocity() {
		return std::any_cast<double>(m_pUObject->GetProperty("fuel_velocity"));
	}
};

ICommand* move = new MacroCommand({
		new CheckFuel(new CheckFuelAdapter(obj)),
		new Move(new MoveAdapter(obj)),
		new BurnFuel(new BurnFuelAdapter(obj)),
	});

"public $(T) get$(name)() { return $(T)obj.getProperty(\"$(name)\") }"

*/