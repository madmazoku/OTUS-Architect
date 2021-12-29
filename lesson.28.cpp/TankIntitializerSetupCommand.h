#pragma once

#include <string>
#include <vector>

#include "../lesson.02.cpp/Vector.h"
#include "../lesson.03.cpp/UObject.h"
#include "../lesson.03.cpp/IExecuteable.h"
#include "../lesson.03.cpp/MoveableAdapter.h"
#include "../lesson.13.cpp/DirectionRotateableAdapter.h"
#include "../lesson.13.cpp/FuelableAdapter.h"

#include "Ioc.h"
#include "SetPlayerByRange.h"
#include "SetPositionByOffset.h"
#include "LambdaCommand.h"
#include "IdentifyableAdapter.h"
#include "OwnableAdapter.h"

typedef std::vector<UObject::Ptr> UObjects;
typedef std::shared_ptr<UObjects> UObjectsPtr;

class TankIntitializerSetupCommand : public IExecuteable
{
public:

	virtual void Execute() override {
		// create new set player strategy (by range), register it
		UObjectsPtr pUObjects = std::make_shared<UObjects>();
		Fabric::Register(
			IoC,
			"Globals.Tanks",
			[pUObjects](Fabric::Args args) {
				return pUObjects;
			}
		);

		// register default set player strategy: by range
		Fabric::Register(
			IoC,
			"TankInitializer.Select.SetPlayer.ByRange",
			[](Fabric::Args args) {
				size_t range = std::stoull(Fabric::GetRequiredArg<std::string>(args, 0, "invalid set player by range strategy range"));
				IExecuteable::Ptr pCommand = std::make_shared<LambdaCommand>([range] {
					ISetPlayer::Ptr pSetPlayerByRange = std::make_shared<SetPlayerByRange>(range);
					Fabric::Register(
						IoC,
						"Globals.SetPlayer",
						[pSetPlayerByRange](Fabric::Args args) { return pSetPlayerByRange; }
					);
					});
				return pCommand;
			}
		);

		// register default set position strategy: by range
		Fabric::Register(
			IoC,
			"TankInitializer.Select.SetPosition.ByOffset",
			[](Fabric::Args args) {
				double x1 = std::stod(Fabric::GetRequiredArg<std::string>(args, 0, "invalid set position by offset strategy x1"));
				double y1 = std::stod(Fabric::GetRequiredArg<std::string>(args, 1, "invalid set position by offset strategy y1"));
				double x2 = std::stod(Fabric::GetRequiredArg<std::string>(args, 2, "invalid set position by offset strategy x2"));
				double y2 = std::stod(Fabric::GetRequiredArg<std::string>(args, 3, "invalid set position by offset strategy y2"));
				double ox = std::stod(Fabric::GetRequiredArg<std::string>(args, 4, "invalid set position by offset strategy ox"));
				double oy = std::stod(Fabric::GetRequiredArg<std::string>(args, 5, "invalid set position by offset strategy oy"));
				IExecuteable::Ptr pCommand = std::make_shared<LambdaCommand>([x1, y1, x2, y2, ox, oy] {
					ISetPosition::Ptr pSetPositionByOffset = std::make_shared<SetPositionByOffset>(Vector{ x1, y1 }, Vector{ x2,y2 }, Vector{ ox,oy });
					Fabric::Register(
						IoC,
						"Globals.SetPosition",
						[pSetPositionByOffset](Fabric::Args args) { return pSetPositionByOffset; }
					);
					});
				return pCommand;
			}
		);

		// create new tank object, register it
		Fabric::Register(
			IoC,
			"TankInitializer.Create.Tank",
			[](Fabric::Args args) {
				UObjectsPtr pTanks = IoC->Resolve<UObjectsPtr>("Globals.Tanks");
				size_t idx = pTanks->size();
				UObject::Ptr pUObject = std::make_shared<UObject>();
				IIdentifyable::Ptr pIdentifyable = std::make_shared< IdentifyableAdapter>(pUObject);
				pIdentifyable->SetIndex(idx);
				IoC->Resolve<ISetPlayer::Ptr>(std::string("Globals.SetPlayer"))->SetPlayer(pUObject);
				IoC->Resolve<ISetPosition::Ptr>(std::string("Globals.SetPosition"))->SetPosition(pUObject);
				pTanks->emplace_back(pUObject);
				return pUObject;
			}
		);

		// get tank's player
		Fabric::Register(
			IoC,
			"Tank.GetIndex",
			[](Fabric::Args args) {
				UObject::Ptr pUObject = Fabric::GetRequiredArg<UObject::Ptr>(args, 0, "invalid object");
				IIdentifyable::Ptr pIdentifyable = std::make_shared< IdentifyableAdapter>(pUObject);
				size_t idx = pIdentifyable->GetIndex();
				return idx;
			}
		);

		// set tank's player
		Fabric::Register(
			IoC,
			"Tank.SetPlayer",
			[](Fabric::Args args) {
				UObject::Ptr pUObject = Fabric::GetRequiredArg<UObject::Ptr>(args, 0, "invalid object");
				size_t ownerIdx = std::stoull(Fabric::GetRequiredArg<std::string>(args, 1, "invalid player"));
				IOwnable::Ptr pOwnable = std::make_shared<OwnableAdapter>(pUObject);
				IExecuteable::Ptr pCommand = std::make_shared<LambdaCommand>([pOwnable, ownerIdx] {pOwnable->SetOwnerIndex(ownerIdx); });
				return pCommand;
			}
		);

		// get tank's player
		Fabric::Register(
			IoC,
			"Tank.GetPlayer",
			[](Fabric::Args args) {
				UObject::Ptr pUObject = Fabric::GetRequiredArg<UObject::Ptr>(args, 0, "invalid object");
				IOwnable::Ptr pOwnable = std::make_shared<OwnableAdapter>(pUObject);
				size_t ownerIdx = pOwnable->GetOwnerIndex();
				return ownerIdx;
			}
		);

		// set tank's position
		Fabric::Register(
			IoC,
			"Tank.SetPosition",
			[](Fabric::Args args) {
				UObject::Ptr pUObject = Fabric::GetRequiredArg<UObject::Ptr>(args, 0, "invalid object");
				double x = std::stod(Fabric::GetRequiredArg<std::string>(args, 1, "invalid position x"));
				double y = std::stod(Fabric::GetRequiredArg<std::string>(args, 2, "invalid position y"));
				IMoveable::Ptr pMoveable = std::make_shared < MoveableAdapter>(pUObject);
				IExecuteable::Ptr pCommand = std::make_shared<LambdaCommand>([x, y, pMoveable] { pMoveable->SetPosition({ x,y }); });
				return pCommand;
			}
		);

		// get tank's position
		Fabric::Register(
			IoC,
			"Tank.GetPosition",
			[](Fabric::Args args) {
				UObject::Ptr pUObject = Fabric::GetRequiredArg<UObject::Ptr>(args, 0, "invalid object");
				IMoveable::Ptr pMoveable = std::make_shared<MoveableAdapter>(pUObject);
				Vector pos = pMoveable->GetPosition();
				return pos;
			}
		);

		// set tank's direction
		Fabric::Register(
			IoC,
			"Tank.SetDirection",
			[](Fabric::Args args) {
				UObject::Ptr pUObject = Fabric::GetRequiredArg<UObject::Ptr>(args, 0, "invalid object");
				double x = std::stod(Fabric::GetRequiredArg<std::string>(args, 1, "invalid direction x"));
				double y = std::stod(Fabric::GetRequiredArg<std::string>(args, 2, "invalid direction y"));
				IDirectionRotateable::Ptr pDirectionRotateable = std::make_shared<DirectionRotateableAdapter>(pUObject);
				IExecuteable::Ptr pCommand = std::make_shared<LambdaCommand>([x, y, pDirectionRotateable] { pDirectionRotateable->SetDirection({ x,y }); });
				return pCommand;
			}
		);

		// get tank's direction
		Fabric::Register(
			IoC,
			"Tank.GetDirection",
			[](Fabric::Args args) {
				UObject::Ptr pUObject = Fabric::GetRequiredArg<UObject::Ptr>(args, 0, "invalid object");
				IMoveable::Ptr pMoveable = std::make_shared<MoveableAdapter>(pUObject);
				Vector pos = pMoveable->GetPosition();
				return pos;
			}
		);

		// set tank's fuel level
		Fabric::Register(
			IoC,
			"Tank.SetFuelLevel",
			[](Fabric::Args args) {
				UObject::Ptr pUObject = Fabric::GetRequiredArg<UObject::Ptr>(args, 0, "invalid object");
				size_t fuelLevel = std::stoull(Fabric::GetRequiredArg<std::string>(args, 1, "invalid fuel"));
				IFuelable::Ptr pFuelable = std::make_shared < FuelableAdapter>(pUObject);
				IExecuteable::Ptr pCommand = std::make_shared<LambdaCommand>([fuelLevel, pFuelable] { pFuelable->SetFuelLevel(static_cast<unsigned long>(fuelLevel)); });
				return pCommand;
			}
		);

		// get tank's fuel level
		Fabric::Register(
			IoC,
			"Tank.GetFuelLevel",
			[](Fabric::Args args) {
				UObject::Ptr pUObject = Fabric::GetRequiredArg<UObject::Ptr>(args, 0, "invalid object");
				IFuelable::Ptr pFuelable = std::make_shared < FuelableAdapter>(pUObject);
				size_t fuelLevel = pFuelable->GetFuelLevel();
				return fuelLevel;
			}
		);
	}
};
