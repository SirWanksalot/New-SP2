#ifndef DRIVESCENE_H
#define DRIVESCENE_H

#include "Scene.h"
#include <MatrixStack.h>
#include "Camera2.h"
#include "Mesh.h"
#include "Light.h"


class DriveScene : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_CHAR,
		GEO_DICE,
		GEO_LIGHTSPHERE,
		GEO_TEXT,
		GEO_NPC,
		GEO_SHOP,
		GEO_SHOP_UI,
		GEO_SHOP_NPC,
		//	GEO_CAR_BOOST,
		GEO_CAR1BODY,
		GEO_CAR2BODY,
		GEO_CAR3BODY,
		GEO_CAR4BODY,
		GEO_CAR1WHEEL,
		GEO_CAR2WHEEL,
		GEO_CAR3WHEEL,
		GEO_CAR4WHEEL,
		GEO_PLATFORM,
		GEO_SLOT_BODY,
		GEO_SLOT_ARM,
		GEO_SLOT_SCREEN_IMAGE,
		GEO_DOOR,
		GEO_DOORSCREEN,
		GEO_HOLO0,
		GEO_HOLO1,
		GEO_HOLO2,
		GEO_HOLO3,
		GEO_TRACK,
		NUM_GEOMETRY,
	};

	enum Scene5_UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHTENABLED,
		//add these enum in UNIFORM_TYPE before U_TOTAL
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		// light 1
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_TYPE,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,
		U_LIGHT1ENABLED,
		U_NUMLIGHTS,
		// add these enum for texture
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		//text
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};

private:
	unsigned m_vertexArrayID;
	unsigned m_programID;
	/*unsigned m_indexBuffer;*/
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_parameters[U_TOTAL];

	MS modelStack, viewStack, projectionStack;
	Light light[2];

	Camera2 camera;
	Camera2 firstpersoncamera;

	int fps;
	float framespersecond;
	float lastTime;
	float currentTime;

	float carVelocity;
	float carTurningSpeed;
	float carAcceleration;
	float friction;

	const float playerMovementSpeed = 10.f;
	const float playerTurningSpeed = 135.f;

	TRS track;

	//characters
	TRS Aplayer;
	TRS ANPC;
	//cars
	TRS ACarBody;
	TRS ACarWheel[4];
	//showroom UI
	std::string UIText[3];
	//UIText[0] - Movement text
	//UIText[1] - Camera movement text
	//UIText[2] - Camera Toggle (between 1st and 3rd)

	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderSkybox();
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderObj(Mesh* mesh, TRS& trs, bool end, bool enableLight);

	//player movement
	void PlayerMoveUp(double dt);
	void PlayerMoveDown(double dt);
	void PlayerMoveRight(double dt);
	void PlayerMoveLeft(double dt);

public:
	DriveScene();
	~DriveScene();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

#endif