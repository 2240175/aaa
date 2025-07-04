#pragma once

#include<DirectXMath.h>

//ƒRƒŠƒWƒ‡ƒ“

class Collision
{
public:
	//‹…‚Æ‹…‚ÌŒğ·”»’è

	static bool IntersectSphereVsSpgere(
		const DirectX::XMFLOAT3& positionA,
		float radiusA,
		const DirectX::XMFLOAT3& positionB,
		float radiusB,
		DirectX::XMFLOAT3& outPositionB);
	
	//<summary>
	//‰~’Œ‚Æ‰~’Œ‚ÌŒğ·”»’è
	//‰~’Œ‚ÌA‚ÌˆÊ’u
	//‰~’ŒA‚Ì”¼Œa
	//

	//‰~’Œ‚Æ‰~’Œ‚ÌŒğ·”»’è
	static bool IntersectCylinderVsCylinder(
		const DirectX::XMFLOAT3& positionA,
		float radiusA,
		float heightA,
		const DirectX::XMFLOAT3& positionB,
		float radiusB,
		float heightB,
		DirectX::XMFLOAT3& outPositionB);

	//‹…‚Æ‰~’Œ‚ÌŒğ·”»’è
	static bool IntersectSphereVsCylinder(
		const DirectX::XMFLOAT3& spherePosition,
		float sphereRadius,
		const DirectX::XMFLOAT3& cylinderPosition,
		float cylinderRadius,
		float cylinderHeight,
		DirectX::XMFLOAT3& outCylinderPosition);


};
