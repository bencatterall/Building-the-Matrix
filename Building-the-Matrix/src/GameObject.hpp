#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Common.hpp"
#include "RenderableComponent.hpp"
#include <memory>

class GameObject {
	bool isRenderable = false;
	std::shared_ptr<RenderableComponent> renderableComponent;
public:
	GameObject();
	~GameObject();

};


#endif