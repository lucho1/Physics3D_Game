#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
	void CreateSlope();

public:

	PhysBody3D *pb_limit1 = nullptr;
	Cube limit1;

	PhysBody3D *pb_limit2 = nullptr;
	Cube limit2;

	PhysBody3D *pb_limit3 = nullptr;
	Cube limit3;

	PhysBody3D *pb_limit4 = nullptr;
	Cube limit4;

	PhysBody3D *pb_slope1 = nullptr;
	Cube slope1;

};
