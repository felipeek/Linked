#pragma once
class Creature
{
public:
	Creature();
	~Creature();
	virtual unsigned int getHp() = 0;
	virtual void setHp(unsigned int hp) = 0;
	virtual unsigned int getTotalMaximumHp() = 0;
	virtual void doDamage(unsigned int damage) = 0;
	virtual unsigned int getTotalAttack() = 0;
	virtual unsigned int getTotalDefense() = 0;
	virtual unsigned int getTotalSpeed() = 0;
	virtual unsigned int getTotalAttackSpeed() = 0;
	virtual unsigned int getTotalMagicalPower() = 0;
	virtual bool isAlive() = 0;
};

