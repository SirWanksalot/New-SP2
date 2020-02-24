#include "DriveScene.h"
#include "GL\glew.h"
#include "Application.h"
#include <Mtx44.h>
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <string>

#define ROT_LIMIT 45.f;
#define SCALE_LIMIT 5.f;
#define LSPEED 10.f

DriveScene::DriveScene()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
}

DriveScene::~DriveScene()
{
}

void DriveScene::Init()
{
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);

	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");

	//m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Texture.fragmentshader"); 

	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	//light 1
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1ENABLED] = glGetUniformLocation(m_programID, "light1Enabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");

	//Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");

	glUseProgram(m_programID);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	light[0].type = Light::LIGHT_DIRECTIONAL;
	light[0].position.Set(10, 10, 10);
	light[0].color.Set(0.5f, 0.5f, 0.5f);
	light[0].power = 1;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 10.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);
	//light 1
	light[1].type = Light::LIGHT_POINT;
	light[1].position.Set(0, 28, -30);
	light[1].color.Set(0.5f, 0.5f, 0.5f);
	light[1].power = 1.f;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = cos(Math::DegreeToRadian(45));
	light[1].cosInner = cos(Math::DegreeToRadian(30));
	light[1].exponent = 30.f;
	light[1].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &light[0].spotDirection.x);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);
	//light 1
	glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
	glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &light[1].spotDirection.x);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], light[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], light[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);
	glUniform1i(m_parameters[U_NUMLIGHTS], 2);

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//left.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//right.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//top.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//bottom.tga");

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//front.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//back.tga");

	meshList[GEO_LIGHTSPHERE] = MeshBuilder::GenerateSphere("lightBall", Color(1.f, 1.f, 0.f), 9, 36, 1.f);

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	meshList[GEO_DICE] = MeshBuilder::GenerateOBJ("dice", "OBJ//doorman.obj");
	meshList[GEO_DICE]->textureID = LoadTGA("Image//doorman.tga");
	meshList[GEO_DICE]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_DICE]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_DICE]->material.kSpecular.Set(1.f, 1.f, 1.f);
	meshList[GEO_DICE]->material.kShininess = 1.f;
	Aplayer.translate = Vector3(0, 0, 5);
	Aplayer.RotateY.degree += 180;

	Acarnumber = 0;

	if (Acarnumber == 0)
	{
		meshList[GEO_CARBODY] = MeshBuilder::GenerateOBJ("Car1", "OBJ//GuangThengCarBody.obj");
		meshList[GEO_CARBODY]->textureID = LoadTGA("Image//GuangThengCarTex.tga");
		meshList[GEO_CARBODY]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
		meshList[GEO_CARBODY]->material.kDiffuse.Set(1.f, 1.f, 1.f);
		meshList[GEO_CARBODY]->material.kSpecular.Set(1.f, 1.f, 1.f);
		meshList[GEO_CARBODY]->material.kShininess = 1.f;
		ACarBody.translate = Vector3(-130, 2.05, 140);
		ACarBody.Scale = Vector3(1.55, 1.55, 1.55);

		meshList[GEO_CARWHEEL] = MeshBuilder::GenerateOBJ("Car1Wheel", "OBJ//GuangThengCarWheel.obj");
		meshList[GEO_CARWHEEL]->textureID = LoadTGA("Image//GuangThengCarTex.tga");
		meshList[GEO_CARWHEEL]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
		meshList[GEO_CARWHEEL]->material.kDiffuse.Set(1.f, 1.f, 1.f);
		meshList[GEO_CARWHEEL]->material.kSpecular.Set(1.f, 1.f, 1.f);
		meshList[GEO_CARWHEEL]->material.kShininess = 1.f;
		ACarWheel[0].translate = Vector3(2, -0.4, -2.8);
		ACarWheel[1].translate = Vector3(-2.1, -0.4, -2.8);
		ACarWheel[2].translate = Vector3(2.05, -0.4, 2.8);
		ACarWheel[2].Scale = Vector3(1.18, 1.18, 1.18);
		ACarWheel[3].translate = Vector3(-2.15, -0.4, 2.8);
		ACarWheel[3].Scale = Vector3(1.18, 1.18, 1.18);
	}
	else if (Acarnumber == 1)
	{
		meshList[GEO_CARBODY] = MeshBuilder::GenerateOBJ("Car2", "OBJ//RyanCarBody.obj");
		meshList[GEO_CARBODY]->textureID = LoadTGA("Image//RyanCarBodyTex.tga");
		meshList[GEO_CARBODY]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
		meshList[GEO_CARBODY]->material.kDiffuse.Set(1.f, 1.f, 1.f);
		meshList[GEO_CARBODY]->material.kSpecular.Set(1.f, 1.f, 1.f);
		meshList[GEO_CARBODY]->material.kShininess = 1.f;
		ACarBody.translate = Vector3(0, 3.2, 0);
		ACarBody.Scale = Vector3(2, 2, 2);

		meshList[GEO_CARWHEEL] = MeshBuilder::GenerateOBJ("Car2Wheel", "OBJ//RyanCarWheel.obj");
		meshList[GEO_CARWHEEL]->textureID = LoadTGA("Image//RyanCarWheelTex.tga");
		meshList[GEO_CARWHEEL]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
		meshList[GEO_CARWHEEL]->material.kDiffuse.Set(1.f, 1.f, 1.f);
		meshList[GEO_CARWHEEL]->material.kSpecular.Set(1.f, 1.f, 1.f);
		meshList[GEO_CARWHEEL]->material.kShininess = 1.f;
		ACarWheel[0].translate = Vector3(0.7, -0.95, -1.2);
		ACarWheel[1].translate = Vector3(-1.3, -0.95, -1.2);
		ACarWheel[2].translate = Vector3(0.75, -0.95, 1.7);
		ACarWheel[3].translate = Vector3(-1.3, -0.95, 1.7);
	}
	else if (Acarnumber == 2)
	{
		meshList[GEO_CARBODY] = MeshBuilder::GenerateOBJ("Car3", "OBJ//JCCarBody.obj");
		meshList[GEO_CARBODY]->textureID = LoadTGA("Image//JCCarTex.tga");
		meshList[GEO_CARBODY]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
		meshList[GEO_CARBODY]->material.kDiffuse.Set(1.f, 1.f, 1.f);
		meshList[GEO_CARBODY]->material.kSpecular.Set(1.f, 1.f, 1.f);
		meshList[GEO_CARBODY]->material.kShininess = 1.f;
		ACarBody.translate = Vector3(0, 3, 0);
		ACarBody.Scale = Vector3(2, 2, 2);

		meshList[GEO_CARWHEEL] = MeshBuilder::GenerateOBJ("Car3Wheel", "OBJ//JCCarWheel.obj");
		meshList[GEO_CARWHEEL]->textureID = LoadTGA("Image//JCCarTex.tga");
		meshList[GEO_CARWHEEL]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
		meshList[GEO_CARWHEEL]->material.kDiffuse.Set(1.f, 1.f, 1.f);
		meshList[GEO_CARWHEEL]->material.kSpecular.Set(1.f, 1.f, 1.f);
		meshList[GEO_CARWHEEL]->material.kShininess = 1.f;
		ACarWheel[0].translate = Vector3(1.8, -0.86, -2.1);
		ACarWheel[1].translate = Vector3(-1.8, -0.86, -2.1);
		ACarWheel[2].translate = Vector3(1.8, -0.86, 2.1);
		ACarWheel[3].translate = Vector3(-1.8, -0.86, 2.1);
	}
	else if (Acarnumber == 3)
	{
		meshList[GEO_CARBODY] = MeshBuilder::GenerateOBJ("Car4", "OBJ//JianFengCarBody.obj");
		meshList[GEO_CARBODY]->textureID = LoadTGA("Image//JianFengCarTex.tga");
		meshList[GEO_CARBODY]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
		meshList[GEO_CARBODY]->material.kDiffuse.Set(1.f, 1.f, 1.f);
		meshList[GEO_CARBODY]->material.kSpecular.Set(1.f, 1.f, 1.f);
		meshList[GEO_CARBODY]->material.kShininess = 1.f;
		ACarBody.translate = Vector3(0, 4.7, 0);
		ACarBody.Scale = Vector3(1.7, 1.7, 1.7);

		meshList[GEO_CARWHEEL] = MeshBuilder::GenerateOBJ("Car4Wheel", "OBJ//JianFengCarWheel.obj");
		meshList[GEO_CARWHEEL]->textureID = LoadTGA("Image//JianFengCarTex.tga");
		meshList[GEO_CARWHEEL]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
		meshList[GEO_CARWHEEL]->material.kDiffuse.Set(1.f, 1.f, 1.f);
		meshList[GEO_CARWHEEL]->material.kSpecular.Set(1.f, 1.f, 1.f);
		meshList[GEO_CARWHEEL]->material.kShininess = 1.f;
		ACarWheel[0].translate = Vector3(1.45, -1.5, -2.5);
		ACarWheel[1].translate = Vector3(-1.45, -1.5, -2.5);
		ACarWheel[2].translate = Vector3(1.55, -1.35, 2.5);
		ACarWheel[2].Scale = Vector3(1.2, 1.2, 1.2);
		ACarWheel[3].translate = Vector3(-1.77, -1.35, 2.5);
		ACarWheel[3].Scale = Vector3(1.2, 1.2, 1.2);
	}

	meshList[GEO_BUILDING1] = MeshBuilder::GenerateOBJ("building1", "OBJ//firstbuilding.obj");
	meshList[GEO_BUILDING1]->textureID = LoadTGA("Image//firstbuilding.tga");
	meshList[GEO_BUILDING1]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_BUILDING1]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_BUILDING1]->material.kSpecular.Set(1.f, 1.f, 1.f);
	meshList[GEO_BUILDING1]->material.kShininess = 1.f;
	ABuilding[0].translate = Vector3(0, 5, 0);

	meshList[GEO_BUILDING2] = MeshBuilder::GenerateOBJ("building2", "OBJ//secondbuilding.obj");
	meshList[GEO_BUILDING2]->textureID = LoadTGA("Image//secondbuilding.tga");
	meshList[GEO_BUILDING2]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_BUILDING2]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_BUILDING2]->material.kSpecular.Set(1.f, 1.f, 1.f);
	meshList[GEO_BUILDING2]->material.kShininess = 1.f;
	ABuilding[1].translate = Vector3(10, 5, 0);

	meshList[GEO_BUILDING3] = MeshBuilder::GenerateOBJ("building3", "OBJ//thirdbuilding.obj");
	meshList[GEO_BUILDING3]->textureID = LoadTGA("Image//thirdbuilding.tga");
	meshList[GEO_BUILDING3]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_BUILDING3]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_BUILDING3]->material.kSpecular.Set(1.f, 1.f, 1.f);
	meshList[GEO_BUILDING3]->material.kShininess = 1.f;
	ABuilding[2].translate = Vector3(20, 5, 0);

	meshList[GEO_BUILDING4] = MeshBuilder::GenerateOBJ("building4", "OBJ//fourthbuilding.obj");
	meshList[GEO_BUILDING4]->textureID = LoadTGA("Image//fourthbuilding.tga");
	meshList[GEO_BUILDING4]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_BUILDING4]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_BUILDING4]->material.kSpecular.Set(1.f, 1.f, 1.f);
	meshList[GEO_BUILDING4]->material.kShininess = 1.f;
	ABuilding[3].translate = Vector3(30, 5, 0);

	meshList[GEO_SPIKE] = MeshBuilder::GenerateOBJ("spike", "OBJ//spike.obj");
	meshList[GEO_SPIKE]->textureID = LoadTGA("Image//spike.tga");
	meshList[GEO_SPIKE]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_SPIKE]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_SPIKE]->material.kSpecular.Set(1.f, 1.f, 1.f);
	meshList[GEO_SPIKE]->material.kShininess = 1.f;
	ASpike.translate = Vector3(0, 5, 10);

	meshList[GEO_COIN] = MeshBuilder::GenerateOBJ("coin", "OBJ//coin.obj");
	meshList[GEO_COIN]->textureID = LoadTGA("Image//coin.tga");
	meshList[GEO_COIN]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_COIN]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_COIN]->material.kSpecular.Set(1.f, 1.f, 1.f);
	meshList[GEO_COIN]->material.kShininess = 1.f;
	ACoin.translate = Vector3(10, 5, 10);

	meshList[GEO_BOX] = MeshBuilder::GenerateOBJ("box", "OBJ//box.obj");
	meshList[GEO_BOX]->textureID = LoadTGA("Image//box.tga");
	meshList[GEO_BOX]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_BOX]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_BOX]->material.kSpecular.Set(1.f, 1.f, 1.f);
	meshList[GEO_BOX]->material.kShininess = 1.f;
	ABox[0].translate = Vector3(-115, 5, 125);
	ABox[1].translate = Vector3(-115, 5, 110);
	ABox[2].translate = Vector3(-115, 5, 95);
	ABox[3].translate = Vector3(-115, 5, 70);
	ABox[4].translate = Vector3(-115, 5, 55);
	ABox[5].translate = Vector3(-115, 5, 40);
	ABox[6].translate = Vector3(-115, 5, 25);


	meshList[GEO_BOOSTPAD] = MeshBuilder::GenerateOBJ("boost", "OBJ//boostpad.obj");
	meshList[GEO_BOOSTPAD]->textureID = LoadTGA("Image//boostpad.tga");
	meshList[GEO_BOOSTPAD]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_BOOSTPAD]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_BOOSTPAD]->material.kSpecular.Set(1.f, 1.f, 1.f);
	meshList[GEO_BOOSTPAD]->material.kShininess = 1.f;
	ABoostpad.translate = Vector3(30, 5, 10);

	// Door
	meshList[GEO_DOOR] = MeshBuilder::GenerateOBJ("door", "OBJ//door.obj");
	meshList[GEO_DOOR]->textureID = LoadTGA("Image//door.tga");
	meshList[GEO_DOOR]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_DOOR]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_DOOR]->material.kSpecular.Set(1.f, 1.f, 1.f);
	meshList[GEO_DOOR]->material.kShininess = 1.f;
	Door.translate = Vector3(-147, 4, 130);
	Door.RotateY.degree += 90;
	Door.Scale = Vector3(1.5, 1.5, 1.5);

	meshList[GEO_DOORSCREEN] = MeshBuilder::GenerateQuad("doorscreen", Color(0, 0, 0), 8, 9);
	meshList[GEO_DOORSCREEN]->textureID = LoadTGA("Image//doorscreen.tga");
	meshList[GEO_DOORSCREEN]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_DOORSCREEN]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_DOORSCREEN]->material.kSpecular.Set(1.f, 1.f, 1.f);
	meshList[GEO_DOORSCREEN]->material.kShininess = 1.f;
	DoorScreen.translate = Vector3(0, -0.2, 2);
	DoorScreen.Scale = Vector3(1, 1, 1);

	carVelocity = 0.f;
	carTurningSpeed = 135.f;
	carAcceleration = 10.f;
	friction = 8.f;

	camera.Init(ACarBody.translate + Vector3(0, 5, 30), ACarBody.translate + Vector3(0, 5, 0), Vector3(0, 1, 0));
	//camera.Init(Vector3(0, 20, 10), Vector3(0, 20, 0), Vector3(0, 1, 0));

	//firstpersoncamera.Init(Vector3(Aplayer.translate.x, Aplayer.translate.y + 6.1, Aplayer.translate.z), Vector3(0, Aplayer.translate.y + 6.1, 0), Vector3(0, 1, 0));

}

void DriveScene::Update(double dt)
{
	const float playerMovementSpeed = 10.f;
	const float playerTurningSpeed = 135.f;

	if (Application::IsKeyPressed(0x31))
	{
		glDisable(GL_CULL_FACE);
	}
	else if (Application::IsKeyPressed(0x32))
	{
		glEnable(GL_CULL_FACE);
	}
	else if (Application::IsKeyPressed(0x33))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if (Application::IsKeyPressed(0x34))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	//UI Text (for movement) logic
	if (Application::IsKeyPressed('W')) {
		UIText[0] = "W - Move Forward";
		PlayerMoveUp(dt);
	}
	if (Application::IsKeyPressed('A')) {
		UIText[0] = "A - Move Left";
		PlayerMoveLeft(dt);
	}
	if (Application::IsKeyPressed('S')) {
		UIText[0] = "S - Move Backwards";
		PlayerMoveDown(dt);
	}
	if (Application::IsKeyPressed('D')) {
		UIText[0] = "D - Move Right";
		PlayerMoveRight(dt);
	}
	if (Application::IsKeyPressed(VK_LEFT)/* && CameraToggle*/) {
		UIText[0] = "Q - Rotate Left";
		Aplayer.RotateY.degree += (float)(playerTurningSpeed * dt);
	}
	if (Application::IsKeyPressed(VK_RIGHT)/*&& CameraToggle*/) {
		UIText[0] = "E - Rotate Right";
		Aplayer.RotateY.degree -= (float)(playerTurningSpeed * dt);
	}

	// Increase Car Velocity to move forward
	if (Application::IsKeyPressed('W'))
	{
		if (carVelocity < 0)
		{
			carVelocity += (1.5 * ((carAcceleration * dt) + (friction * dt)));
		}
		else
		{
			if (carVelocity < 60)
			{
				carVelocity += (carAcceleration * dt);
			}
		}
	}
	// Decrease Car Velocity to move backwards
	if (Application::IsKeyPressed('S'))
	{
		if (carVelocity > 0)
		{
			carVelocity -= (1.5 * ((carAcceleration * dt) + (friction * dt)));
		}
		else
		{
			if (carVelocity > -60)
			{
				carVelocity -= (carAcceleration * dt);
			}
		}
	}
	// Turn car to the left
	if (carVelocity != 0.f && Application::IsKeyPressed('A'))
	{
		ACarBody.RotateY.degree += (float)(carTurningSpeed * dt);
	}
	// Turn car to the right
	if (carVelocity != 0.f && Application::IsKeyPressed('D'))
	{
		ACarBody.RotateY.degree -= (float)(carTurningSpeed * dt);
	}

	// If car is moving without key inputs, increase/decrease car velocity to being the car to a stop
	if (!Application::IsKeyPressed('W') && !Application::IsKeyPressed('S'))
	{
		if (carVelocity < 0.f)
		{
			carVelocity += (2 * (friction * dt));
			if (carVelocity > 0.f)
			{
				carVelocity = 0.f;
			}
		}
		else if (carVelocity > 0.f)
		{
			carVelocity -= (2 * (friction * dt));
			if (carVelocity < 0.f)
			{
				carVelocity = 0.f;
			}
		}
	}
	//Car Moving
	ACarBody.translate.z -= cos(Math::DegreeToRadian(ACarBody.RotateY.degree)) * (float)(carVelocity * dt);
	ACarBody.translate.x -= sin(Math::DegreeToRadian(ACarBody.RotateY.degree)) * (float)(carVelocity * dt);


	camera.CarUpdate(dt, ACarBody);
	//firstpersoncamera.Update(dt, Aplayer);

}

void DriveScene::Render()
{
	//Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();
	//viewStack.LookAt(firstpersoncamera.position.x, firstpersoncamera.position.y, firstpersoncamera.position.z, firstpersoncamera.target.x, firstpersoncamera.target.y, firstpersoncamera.target.z, firstpersoncamera.up.x, firstpersoncamera.up.y, firstpersoncamera.up.z);
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();

	// passing the light direction if it is a direction light
	if (light[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	// if it is spot light, pass in position and direction 
	else if (light[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		// default is point light (only position since point light is 360 degrees)
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}
	if (light[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	// if it is spot light, pass in position and direction 
	else if (light[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		// default is point light (only position since point light is 360 degrees)
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}

	modelStack.PushMatrix();
	modelStack.Translate(0, 50, 0);
	RenderSkybox();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	RenderMesh(meshList[GEO_LIGHTSPHERE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(light[1].position.x, light[1].position.y, light[1].position.z);
	RenderMesh(meshList[GEO_LIGHTSPHERE], false);
	modelStack.PopMatrix();

	//RenderObj(meshList[GEO_DICE], Aplayer, true, false);

	RenderObj(meshList[GEO_CARBODY], ACarBody, false, false);
	for (int carnumwheel = 0; carnumwheel < 4; carnumwheel++)
	{
		RenderObj(meshList[GEO_CARWHEEL], ACarWheel[carnumwheel], true, false);
	}
	modelStack.PopMatrix();

	//for (int i = 0; i < 4; i++)
	//{
	//	if (i == 0)
	//	{
	//		RenderObj(meshList[GEO_BUILDING1], ABuilding[i], true, false);
	//	}
	//	else if (i == 1)
	//	{
	//		RenderObj(meshList[GEO_BUILDING2], ABuilding[i], true, false);
	//	}
	//	else if (i == 2)
	//	{
	//		RenderObj(meshList[GEO_BUILDING3], ABuilding[i], true, false);
	//	}
	//	else if (i == 3)
	//	{
	//		RenderObj(meshList[GEO_BUILDING4], ABuilding[i], true, false);
	//	}
	//}

	RenderObj(meshList[GEO_SPIKE], ASpike, true, false);
	RenderObj(meshList[GEO_COIN], ACoin, true, false);
	for (int i = 0; i < 7; i++)
	{
		RenderObj(meshList[GEO_BOX], ABox[i], true, false);
	}
	RenderObj(meshList[GEO_BOOSTPAD], ABoostpad, true, false);

	RenderObj(meshList[GEO_DOOR], Door, false, false);
	RenderObj(meshList[GEO_DOORSCREEN], DoorScreen, true, false);
	modelStack.PopMatrix();

}

void DriveScene::Exit()
{
	// Cleanup here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i] != NULL)
			delete meshList[i];
	}
	// Cleanup VBO here
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

void DriveScene::RenderMesh(Mesh* mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);


	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if (mesh->textureID > 0) {
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else {
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render(); //this line should only be called once in the whole function

	if (mesh->textureID > 0) glBindTexture(GL_TEXTURE_2D, 0);
}

void DriveScene::RenderSkybox()
{
	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(-150.f, 50.f, 0.f);
	modelStack.Scale(303.f, 303.f, 303.f);
	modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(150.f, 50.f, 0.f);
	modelStack.Scale(303.f, 303.f, 303.f);
	modelStack.Rotate(-90.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(0.f, 150.f, 1.f);
	modelStack.Scale(303.f, 303.f, 303.f);
	modelStack.Rotate(90.f, 1.f, 0.f, 0.f);
	modelStack.Rotate(0.f, 0.f, 0.f, 1.f);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(0.f, -50.f, 0.f);
	modelStack.Scale(303.f, 303.f, 303.f);
	modelStack.Rotate(-90.f, 1.f, 0.f, 0.f);
	modelStack.Rotate(90.f, 0.f, 0.f, 1.f);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(0.f, 50.f, -150.f);
	modelStack.Scale(303.f, 303.f, 303.f);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(0.f, 50.f, 150.f);
	modelStack.Scale(303.f, 303.f, 303.f);
	modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();
}

void DriveScene::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) {
		return;
	}
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i) {
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.5f, 0, 0);
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void DriveScene::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) {
		return;
	}
	glDisable(GL_DEPTH_TEST);

	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i) {
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.7f, 0, 0);
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void DriveScene::RenderObj(Mesh* mesh, TRS& trs, bool end, bool enableLight)
{
	modelStack.PushMatrix();
	modelStack.Translate(trs.translate);
	modelStack.Rotate(trs.RotateX);
	modelStack.Rotate(trs.RotateY);
	modelStack.Rotate(trs.RotateZ);
	modelStack.Scale(trs.Scale);
	RenderMesh(mesh, enableLight);
	if (end)
		modelStack.PopMatrix();
}

void DriveScene::PlayerMoveUp(double dt)
{
	Aplayer.translate.z += cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	Aplayer.translate.x += sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	//if (collision_detector(Aplayer, Cplayer, Aslot_body, Cslot_body)) {
	//	Aplayer.translate.z -= cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	//	Aplayer.translate.x -= sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	//}
	//for (int i = 0; i < 4; i++)
	//{
	//	if (collision_detector(Aplayer, Cplayer, Platform[i], CCar[i]))
	//	{
	//		Aplayer.translate.z -= cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	//		Aplayer.translate.x -= sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	//	}
	//}
}

void DriveScene::PlayerMoveDown(double dt)
{
	Aplayer.translate.z -= cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	Aplayer.translate.x -= sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	//if (collision_detector(Aplayer, Cplayer, Aslot_body, Cslot_body)) {
	//	Aplayer.translate.z += cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	//	Aplayer.translate.x += sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	//}
	//for (int i = 0; i < 4; i++)
	//{
	//	if (collision_detector(Aplayer, Cplayer, Platform[i], CCar[i]))
	//	{
	//		Aplayer.translate.z += cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	//		Aplayer.translate.x += sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	//	}
	//}
}

void DriveScene::PlayerMoveRight(double dt)
{

	Aplayer.translate.z += sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	Aplayer.translate.x -= cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	//if (collision_detector(Aplayer, Cplayer, Aslot_body, Cslot_body)) {
	//	Aplayer.translate.z -= sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	//	Aplayer.translate.x += cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	//}
	//for (int i = 0; i < 4; i++)
	//{
	//	if (collision_detector(Aplayer, Cplayer, Platform[i], CCar[i]))
	//	{
	//		Aplayer.translate.z -= sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	//		Aplayer.translate.x += cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	//	}
	//}
}

void DriveScene::PlayerMoveLeft(double dt)
{
	Aplayer.translate.z -= sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	Aplayer.translate.x += cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	//if (collision_detector(Aplayer, Cplayer, Aslot_body, Cslot_body)) {
	//	Aplayer.translate.z += sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	//	Aplayer.translate.x -= cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	//}
	//for (int i = 0; i < 4; i++)
	//{
	//	if (collision_detector(Aplayer, Cplayer, Platform[i], CCar[i]))
	//	{
	//		Aplayer.translate.z += sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	//		Aplayer.translate.x -= cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	//	}
	//}
}