#pragma once

enum EquipmentClass
{
	ARMOR,
	WEAPON,
	SHIELD
};

class Equipment
{
public:
	Equipment();
	~Equipment();
	EquipmentClass getEquipmentClass();
private:
	EquipmentClass equipmentClass;
};

