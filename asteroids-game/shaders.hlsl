struct VOut
{
	float4 position : SV_POSITION;
	float4 colour : COLOR;
};

cbuffer uniforms
{
	float4x4 MVM;
	float4 globalColour;

};

VOut VShader(float4 position : POSITION, float4 colour : COLOR)
{
	VOut output;

	float4 transformedPos = mul(position, MVM);

	output.position = transformedPos;
	output.colour = colour;

	return output;
}


float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
	return color * globalColour;
}