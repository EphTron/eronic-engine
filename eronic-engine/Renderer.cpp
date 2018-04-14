#include "Renderer.h"
#include "RenderComponent.h"

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

void Renderer::Draw(RenderComponent* rc, glm::mat4 MVM)
{
	if (rc->ShouldDraw())
	{
		MVM = glm::translate(MVM, glm::vec3(rc->GetPosition().x(), rc->GetPosition().y(), 0));
		MVM = glm::rotate(MVM, -rc->GetAngle(), glm::vec3(0, 0, 1)); // Rotates anti-clockwise
		MVM = glm::scale(MVM, glm::vec3(rc->GetScale(), rc->GetScale(), 1));

		if (rc->GetMesh())
		{
			Draw(rc->GetMesh(), MVM, rc->GetColour());
		}
	}
}

/******************************************************************************************************************/
