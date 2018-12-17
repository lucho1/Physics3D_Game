#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "ModuleAudio.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL), vehicle2(NULL)
{
	turn = acceleration = brake = turn2 = acceleration2 = brake2 = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car = SetDefaultCar(); //Default car
	
	vehicle = App->physics->AddVehicle(car);
	vehicle2 = App->physics->AddVehicle(car);
	
	vehicle->SetPos(IposP1.x, IposP1.y, IposP1.z);
	vehicle2->SetPos(IposP2.x, IposP2.y, IposP2.z);
	vehicle2->SetRotation(0.0f, 1.0f, 0.0f, 180.0f);

	IOrientation_vector = vehicle->vehicle->getForwardVector();
	IOrientation_vector2 = vehicle2->vehicle->getForwardVector();

	RestartCar();

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = turn2 = acceleration2 = brake2 = 0.0f;

	if (App->input->GetKey(SDL_SCANCODE_Y) == KEY_DOWN) 
		vehicle->SetPos(IposP1.x, IposP1.y, IposP1.z);
		
	if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN) 
		RestartCar();

	

	HandleInput_P1();
	HandleInput_P2();

	//Vehicle 1 move
	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	vehicle->info.color = Red;
	vehicle->Render();

	//Vehicle 2 move
	vehicle2->ApplyEngineForce(acceleration2);
	vehicle2->Turn(turn2);
	vehicle2->Brake(brake2);

	vehicle2->info.color = Blue;
	vehicle2->Render();

	char title[80];
	sprintf_s(title, "P1v: %.1f Km/h   P2v: %.1f Km/h", vehicle->GetKmh(), vehicle2->GetKmh());
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}


void ModulePlayer::HandleInput_P1() {

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		acceleration = MAX_ACCELERATION;


	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {

		if (vehicle->GetKmh() >= 0)
			brake = BRAKE_POWER;
		else
			acceleration = -MAX_ACCELERATION;

	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if (turn < TURN_DEGREES)
			turn += TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if (turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}
}


void ModulePlayer::HandleInput_P2() {

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		acceleration2 = MAX_ACCELERATION;


	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {

		if (vehicle2->GetKmh() >= 0)
			brake2 = BRAKE_POWER;
		else
			acceleration2 = -MAX_ACCELERATION;
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		if (turn2 < TURN_DEGREES)
			turn2 += TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		if (turn2 > -TURN_DEGREES)
			turn2 -= TURN_DEGREES;
	}
}

VehicleInfo ModulePlayer::SetDefaultCar() {

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(4, 2, 8);
	car.chassis_offset.Set(0, 1.5, 0);
	car.mass = 100.0f;
	car.suspensionStiffness = 40.0f;
	car.suspensionCompression = 2.4f;
	car.suspensionDamping = 2.3f;
	car.maxSuspensionTravelCm = 20.0f;
	car.frictionSlip = 2.0f;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.0f;
	float wheel_radius = 0.6f;
	float wheel_width = 1.0f;
	float suspensionRestLength = 0.2f;

	SetConstCarProperties(&car, connection_height, wheel_radius, wheel_width, suspensionRestLength);
	return car;
}


void ModulePlayer::SetConstCarProperties(VehicleInfo* car, float connection_height, float wheel_radius, float wheel_width, float suspensionRestLength) {

	// Don't change anything below this line ------------------
	float half_width = car->chassis_size.x*0.5f;
	float half_length = car->chassis_size.z*0.5f;

	vec3 direction(0, -1, 0);
	vec3 axis(-1, 0, 0);

	car->num_wheels = 4;
	car->wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car->wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car->wheels[0].direction = direction;
	car->wheels[0].axis = axis;
	car->wheels[0].suspensionRestLength = suspensionRestLength;
	car->wheels[0].radius = wheel_radius;
	car->wheels[0].width = wheel_width;
	car->wheels[0].front = true;
	car->wheels[0].drive = true;
	car->wheels[0].brake = false;
	car->wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car->wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car->wheels[1].direction = direction;
	car->wheels[1].axis = axis;
	car->wheels[1].suspensionRestLength = suspensionRestLength;
	car->wheels[1].radius = wheel_radius;
	car->wheels[1].width = wheel_width;
	car->wheels[1].front = true;
	car->wheels[1].drive = true;
	car->wheels[1].brake = false;
	car->wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car->wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car->wheels[2].direction = direction;
	car->wheels[2].axis = axis;
	car->wheels[2].suspensionRestLength = suspensionRestLength;
	car->wheels[2].radius = wheel_radius;
	car->wheels[2].width = wheel_width;
	car->wheels[2].front = false;
	car->wheels[2].drive = false;
	car->wheels[2].brake = true;
	car->wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car->wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car->wheels[3].direction = direction;
	car->wheels[3].axis = axis;
	car->wheels[3].suspensionRestLength = suspensionRestLength;
	car->wheels[3].radius = wheel_radius;
	car->wheels[3].width = wheel_width;
	car->wheels[3].front = false;
	car->wheels[3].drive = false;
	car->wheels[3].brake = true;
	car->wheels[3].steering = false;

}


void ModulePlayer::RestartCar() {

	float fAngle = vehicle2->vehicle->getForwardVector().angle(IOrientation_vector2);

	if (fAngle != 0)
		vehicle2->SetRotation(0.0f, 1.0f, 0.0f, -fAngle, false);

	if (vehicle2->GetKmh() > 0)
		vehicle2->SetLinearVelocity(vec3(0.0f, 0.0f, 0.0f));


	vehicle2->SetPos(IposP2.x, IposP2.y, IposP2.z);

}