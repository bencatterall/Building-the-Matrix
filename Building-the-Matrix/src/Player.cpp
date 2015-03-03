#include "GameObject.hpp"
#include "Cube.hpp"
#include "Chunk.hpp"
#include "Player.hpp"
#include "ObjectManager.hpp"
#include "LocationComponent.hpp"
#include "shader.hpp"
#include "RenderableComponent.hpp"
#include "Texture.hpp"

#include <utility>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

Player::Player(float xPos, float yPos, float zPos) :
GameObject(glm::vec3(xPos, yPos, zPos)) {
	headCube = std::make_shared<Cube>(glm::vec3(xPos, yPos + 1.5*Chunk::getCubeSize(), zPos), (float)Chunk::getCubeSize() / 2.0f);
	vehicleCube = std::make_shared<Cube>(glm::vec3(xPos, yPos, zPos), (float)Chunk::getCubeSize());

	//Add to object manager
	ObjectManager& objManager = ObjectManager::getInstance();
	objManager.addObject(headCube);
	objManager.addObject(vehicleCube);


	//Setup rendering 
	//Init starting renderable data 
	std::shared_ptr<TextureAtlas> redCarTexture = std::make_shared<TextureAtlas>("resources/textures/carAssets/RedCar/car_texture_atlas.png", "resources/textures/carAssets/RedCar/car_name_mappings.txt");
	//std::shared_ptr<TextureAtlas> blueCarTexture = std::make_shared<TextureAtlas>("resources/textures/carAssets/BlueCar/car_texture_atlas.png", "resources/textures/carAssets/RedCar/car_name_mappings.txt");
	std::shared_ptr<TextureAtlas> boyHeadTexture = std::make_shared<TextureAtlas>("resources/textures/FaceAssets/Boy/boy_texture_atlas.png", "resources/textures/FaceAssets/Boy/boy_name_mappings.txt");
	std::shared_ptr<TextureAtlas> girlHeadTexture = std::make_shared<TextureAtlas>("resources/textures/FaceAssets/Girl/girl_texture_atlas.png", "resources/textures/FaceAssets/Girl/firl_name_mappings.txt");

	std::shared_ptr<Shader> shader = std::make_shared<Shader>("resources//shaders//default_shader");
	if (!shader->isLoaded()) {
		std::cerr << "ERROR: In Player::Player, can not load shader." << std::endl;
		return;
	}

	headCube->getRenderableComponent()->setShader(shader);
	headCube->getRenderableComponent()->setTexture(boyHeadTexture);

	vehicleCube->getRenderableComponent()->setShader(shader);
	vehicleCube->getRenderableComponent()->setTexture(redCarTexture);

	//Map of cube types to a string identifer
	std::map<std::string , std::string> headTypeToID;
	headTypeToID.insert(std::make_pair<std::string, std::string>(std::string("headTop"), std::string("BoyHeadTop")));
	headTypeToID.insert(std::make_pair<std::string, std::string>(std::string("headBottom"), std::string("BoyHeadBottom")));
	headTypeToID.insert(std::make_pair<std::string, std::string>(std::string("headFront"), std::string("BoyHeadFront")));
	headTypeToID.insert(std::make_pair<std::string, std::string>(std::string("headBack"), std::string("BoyHeadBack")));
	headTypeToID.insert(std::make_pair<std::string, std::string>(std::string("headLeft"), std::string("BoyHeadSide2")));
	headTypeToID.insert(std::make_pair<std::string, std::string>(std::string("headRight"), std::string("BoyHeadSide1")));

	std::map<std::string, std::string> vehicleTypeToID;
	vehicleTypeToID.insert(std::make_pair<std::string, std::string>(std::string("vehicleTop"), std::string("RedCarTop")));
	vehicleTypeToID.insert(std::make_pair<std::string, std::string>(std::string("vehicleBottom"), std::string("RedCarBottom")));
	vehicleTypeToID.insert(std::make_pair<std::string, std::string>(std::string("vehicleFront"), std::string("RedCarFront")));
	vehicleTypeToID.insert(std::make_pair<std::string, std::string>(std::string("vehicleBack"), std::string("RedCarBack")));
	vehicleTypeToID.insert(std::make_pair<std::string, std::string>(std::string("vehicleLeft"), std::string("RedCarSide1")));
	vehicleTypeToID.insert(std::make_pair<std::string, std::string>(std::string("vehicleRight"), std::string("RedCarSide2")));

	//get texture coords
	std::vector<GLfloat> headTexCoords;
	std::vector<GLfloat> vehicleTexCoords;
	std::vector<GLfloat> headVertData;
	std::vector<GLfloat> vehicleVertData;

	//create texture coords
	//head
	int numHeadVertices = 0;
	setTileTexture(0.0f, 0.0f, 0.0f, &numHeadVertices, headTypeToID.at("headTop"), boyHeadTexture, Chunk::cubeTopFace, Chunk::cubeTopTextureCoords, headVertData, headTexCoords, (float)Chunk::getCubeSize() / 2.0f);
	setTileTexture(0.0f, 0.0f, 0.0f, &numHeadVertices, headTypeToID.at("headBottom"), boyHeadTexture, Chunk::cubeBottomFace, Chunk::cubeBottomTextureCoords, headVertData, headTexCoords, (float)Chunk::getCubeSize() / 2.0f);
	setTileTexture(0.0f, 0.0f, 0.0f, &numHeadVertices, headTypeToID.at("headFront"), boyHeadTexture, Chunk::cubeFrontFace, Chunk::cubeFrontTextureCoords, headVertData, headTexCoords, (float)Chunk::getCubeSize() / 2.0f);
	setTileTexture(0.0f, 0.0f, 0.0f, &numHeadVertices, headTypeToID.at("headBack"), boyHeadTexture, Chunk::cubeBackFace, Chunk::cubeBackTextureCoords, headVertData, headTexCoords, (float)Chunk::getCubeSize() / 2.0f);
	setTileTexture(0.0f, 0.0f, 0.0f, &numHeadVertices, headTypeToID.at("headLeft"), boyHeadTexture, Chunk::cubeLeftFace, Chunk::cubeLeftTextureCoords, headVertData, headTexCoords, (float)Chunk::getCubeSize() / 2.0f);
	setTileTexture(0.0f, 0.0f, 0.0f, &numHeadVertices, headTypeToID.at("headRight"), boyHeadTexture, Chunk::cubeRightFace, Chunk::cubeRightTextureCoords, headVertData, headTexCoords, (float)Chunk::getCubeSize() / 2.0f);
	
	//car
	int numVehicleVertices = 0;
	setTileTexture(0.0f, 0.0f, 0.0f, &numVehicleVertices, vehicleTypeToID.at("vehicleTop"), redCarTexture, Chunk::cubeTopFace, Chunk::cubeTopTextureCoords, vehicleVertData, vehicleTexCoords, (float)Chunk::getCubeSize());
	setTileTexture(0.0f, 0.0f, 0.0f, &numVehicleVertices, vehicleTypeToID.at("vehicleBottom"), redCarTexture, Chunk::cubeBottomFace, Chunk::cubeBottomTextureCoords, vehicleVertData, vehicleTexCoords, (float)Chunk::getCubeSize());
	setTileTexture(0.0f, 0.0f, 0.0f, &numVehicleVertices, vehicleTypeToID.at("vehicleFront"), redCarTexture, Chunk::cubeFrontFace, Chunk::cubeFrontTextureCoords, vehicleVertData, vehicleTexCoords, (float)Chunk::getCubeSize());
	setTileTexture(0.0f, 0.0f, 0.0f, &numVehicleVertices, vehicleTypeToID.at("vehicleBack"), redCarTexture, Chunk::cubeBackFace, Chunk::cubeBackTextureCoords, vehicleVertData, vehicleTexCoords, (float)Chunk::getCubeSize());
	setTileTexture(0.0f, 0.0f, 0.0f, &numVehicleVertices, vehicleTypeToID.at("vehicleLeft"), redCarTexture, Chunk::cubeLeftFace, Chunk::cubeLeftTextureCoords, vehicleVertData, vehicleTexCoords, (float)Chunk::getCubeSize());
	setTileTexture(0.0f, 0.0f, 0.0f, &numVehicleVertices, vehicleTypeToID.at("vehicleRight"), redCarTexture, Chunk::cubeRightFace, Chunk::cubeRightTextureCoords, vehicleVertData, vehicleTexCoords, (float)Chunk::getCubeSize());


	//set coords
	headCube->getRenderableComponent()->setTextureCoordsData(headTexCoords, false);
	headCube->getRenderableComponent()->setVertexData(headVertData, false);
	headCube->getRenderableComponent()->setNumVerticesRender(numHeadVertices);

	vehicleCube->getRenderableComponent()->setTextureCoordsData(vehicleTexCoords, false);
	vehicleCube->getRenderableComponent()->setVertexData(vehicleVertData, false);
	vehicleCube->getRenderableComponent()->setNumVerticesRender(numVehicleVertices);

}
void Player::setTileTexture(float xPos, float yPos, float zPos, int* numVertices, std::string tileName, std::shared_ptr<TextureAtlas> texture, 
	const glm::vec3(&vertexData)[6], const GLfloat(&texData)[6 * 2], std::vector<GLfloat>& vertData, std::vector<GLfloat>& texCoords, float sf)  {

	for (size_t i = 0; i < sizeof(vertexData) / sizeof(glm::vec3); ++i) {
		//div by 2 as cube data is -1 to +1
		glm::vec3 newCubeData = vertexData[i];
		newCubeData *= sf;
		newCubeData.x += xPos;
		newCubeData.y += yPos;
		newCubeData.z += zPos;
		vertData.push_back(newCubeData.x);
		vertData.push_back(newCubeData.y);
		vertData.push_back(newCubeData.z);
		(*numVertices)++;
	}
	std::tuple<float, float, float, float> rect = texture->calculateTextureCoords(tileName);
	//pairs of (x, y)
	//(0.0, 0.0) -> (x,y)
	//(1.0, 0.0) -> (x + width,y)
	//(0.0, 1.0) -> (x, y + height)
	//(1.0, 1.0) -> (x + width, y + height)
	float xOffset = std::get<0>(rect);
	float yOffset = std::get<1>(rect);
	float width = std::get<2>(rect);
	float height = std::get<3>(rect);
	for (size_t i = 0; i < sizeof(texData) / sizeof(GLfloat); i += 2) {
		if (texData[i] == 0.0f && texData[i + 1] == 0.0f) {
			texCoords.push_back(xOffset);
			texCoords.push_back(yOffset);
		}
		else if (texData[i] == 1.0f && texData[i + 1] == 0.0f) {
			texCoords.push_back(xOffset + width);
			texCoords.push_back(yOffset);
		}
		else if (texData[i] == 0.0f && texData[i + 1] == 1.0f) {
			texCoords.push_back(xOffset);
			texCoords.push_back(yOffset + height);
		}
		else if (texData[i] == 1.0f && texData[i + 1] == 1.0f) {
			texCoords.push_back(xOffset + width);
			texCoords.push_back(yOffset + height);
		}
		else {
			assert(false);
			std::cerr << "ERROR: In Player, texture coords calc. Comparisions failed.";
		}
	}
}

//Up is aways  (0.0, 1.0, 0.0)
void Player::setVehicleOrientation(glm::vec3 forwardVec) {
	//glm::mat4 rotationMat = glm::orientation(forwardVec, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotationMat = glm::orientation(forwardVec, glm::vec3(0.0f, 0.0f, -1.0f));
	(this->getLocationComponent())->setRotationMatrix(rotationMat);
	vehicleCube->getLocationComponent()->setRotationMatrix(rotationMat);
}

//NOTE: INVERTED WHEN GOING IN THE +Z DIRECTION - THINK IT ASSUMES FORWARD FACING DIRECTION TO BE CONSTANT
void Player::setHeadOrientation(glm::vec3 PRY) {
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	
	//roll
	rotationMatrix = glm::rotate(rotationMatrix, PRY.x, glm::vec3(0.0f, 0.0f, -1.0f));

	//yaw
	rotationMatrix = glm::rotate(rotationMatrix, PRY.z, glm::vec3(0.0f, 1.0f, 0.0f));

	//pitch
	rotationMatrix = glm::rotate(rotationMatrix, PRY.y, glm::vec3(1.0f, 0.0f, 0.0f));

	headCube->getLocationComponent()->setRotationMatrix(rotationMatrix * ((this->getLocationComponent()->getRotationMatrix())));
}

//places serialized values into object return the size of the buffer read as an int 
int Player::deserialize(unsigned char* buffer) {
	Serializer serializer = Serializer();
	int next = 0;
	this->setGlobalID(serializer.unpackInt(buffer, next));
	(this->physicsObject) = std::make_shared<PhysicsObject>(&buffer[next], next);
	this->setVisible(serializer.unpackBool(&buffer[next], next));
	this->setRenderable(serializer.unpackBool(&buffer[next], next));
	(this->control) = KeyboardControl(serializer, &buffer[next], next);
	(this->pitch) = serializer.unpackFloat(&buffer[next], next);
	(this->roll) = serializer.unpackFloat(&buffer[next], next);
	(this->yaw) = serializer.unpackFloat(&buffer[next], next);
	(this->score) = serializer.unpackInt(&buffer[next], next);
	(this->locationComponent) = (this->physicsObject)->getLocationComponent();

	//update position of head and body
	glm::vec3 newPos = this->getLocationComponent()->getPosition();
	headCube->getLocationComponent()->setPosition(glm::vec3(newPos.x, newPos.y + 1.5*Chunk::getCubeSize(), newPos.z));
	vehicleCube->getLocationComponent()->setPosition(glm::vec3(newPos.x, newPos.y, newPos.z));
	//Update orientation of the player
	setVehicleOrientation(physicsObject->getOrientation());
	setHeadOrientation(glm::vec3(this->roll, this->pitch, this->yaw));


	return next;
}