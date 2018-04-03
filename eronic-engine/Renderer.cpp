#include "Renderer.h"
#include "GameObject.h"

/******************************************************************************************************************/

Renderer::Renderer()
	: _clearColour(0.0f, 0.0f, 0.0f, 1.0f)
{
}

/******************************************************************************************************************/

Renderer::~Renderer()
{
}

/******************************************************************************************************************/

void Renderer::Draw(GameObject* gob, glm::mat4 MVM)
{
	MVM = glm::translate(MVM, glm::vec3(gob->GetPosition().x(), gob->GetPosition().y(), 0));
	MVM = glm::rotate(MVM, -gob->GetAngle(), glm::vec3(0, 0, 1)); // Rotates anti-clockwise
	MVM = glm::scale(MVM, glm::vec3(gob->GetScale(), gob->GetScale(), 1));

	Draw(gob->GetMesh(), MVM, gob->GetColour());
}

/******************************************************************************************************************/
