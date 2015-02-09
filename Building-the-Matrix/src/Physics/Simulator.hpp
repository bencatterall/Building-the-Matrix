#ifndef SIMULATOR_H
#define SIMULATOR_H

class ObjectManager;

class Simulator
{
public:
	Simulator & getInstance();
	void tick(float);

protected:

private:
	Simulator();
	~Simulator();

	void processCollisions();

	static Simulator instance;
	//ObjectManager & objMan;
	float accumulator;
};

#endif