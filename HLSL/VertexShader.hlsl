#include "Header.hlsli"

cbuffer cbSpace : register(b0)
{
	matrix Projection;
};

cbuffer cbSpace : register(b1)
{
	matrix World;
};

cbuffer cbCameraShake : register(b2)
{
	float CameraOffsetX;
	float CameraOffsetY;
	float pad[2];
}

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;

	input.pos.x += CameraOffsetX;
	input.pos.y += CameraOffsetY;

	output.pos = mul(input.pos, World);
	output.pos = mul(output.pos, Projection);
	output.color = input.color;
	output.uv = input.uv;

	return output;
}