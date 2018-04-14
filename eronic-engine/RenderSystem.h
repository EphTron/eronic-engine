#pragma once
#include "System.h"
#include "Renderer.h"
#include <vector>

class RenderComponent;

// Moves objects around based on PhysicsComponents
class RenderSystem :
	public System
{
	// Data
protected:
	Renderer* _renderer;
	glm::mat4 _MVM;

	// Structors
public:
	RenderSystem();
	virtual ~RenderSystem();

	// Get / sets
public:
	Renderer* GetRenderer()		const	{ return _renderer; }
	void SetRenderer(Renderer* r)		{ _renderer = r; }
	glm::mat4 GetMVM()			const	{ return _MVM; }
	void SetMVM(glm::mat4 m)			{ _MVM = m; }

	// Functions
public:
	virtual void Process(std::vector<GameObject*>& list, double deltaTime);
};

