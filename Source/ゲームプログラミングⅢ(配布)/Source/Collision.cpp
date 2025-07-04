#include"Collision.h"

//���Ƌ��̌�������

bool Collision::IntersectSphereVsSpgere(
	const DirectX::XMFLOAT3& positionA,
	float radiusA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	DirectX::XMFLOAT3& outPositionB)
{

	//A->B�̒P�ʃx�N�g�����Z�o
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB,PositionA);
	DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);

//��������
	float range = radiusA + radiusB;
	if(lengthSq > range)
	{
		return false;
	}

//A��B�������o��

	Vec = DirectX::XMVector3Normalize(Vec);//�����o�������̒P�ʃx�N�g��
	Vec = DirectX::XMVectorScale(Vec, range);
	PositionB = DirectX::XMVectorAdd(PositionA, Vec);


	DirectX::XMStoreFloat3(&outPositionB, PositionB);

	return true;

}

//�~���Ɖ~���̌�������
bool Collision::IntersectCylinderVsCylinder(
	const DirectX::XMFLOAT3& positionA,
	float radiusA,
	float heightA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	float heightB,
	DirectX::XMFLOAT3& outPositionB)
{
	//A�̑�����B�̓�����Ȃ瓖�����ĂȂ�
	if (positionA.y + positionB.y + heightB)
	{
		return false;
	}
	//A�̓���B�̑�����艺�Ȃ瓖�����ĂȂ�
	if (positionA.y + heightA < positionB.y)
	{
		return false;
	}
	//XZ���ʂł͈̔͂��`�F�b�N
	float vx = positionB.x - positionA.x;
	float vz = positionB.z - positionA.z;
	float range = radiusA + radiusB;
	float distXZ = sqrtf(vx * vx + vz * vz);
	if (distXZ > range)
	{ 
		return false;
	}
	//�P�ʃx�N�g����
	vx /= distXZ;
	vz /= distXZ;

	//�����~���������o��
	outPositionB.x = positionA.x + (vx * range);
	outPositionB.y = positionB.y;
	outPositionB.z = positionA.z + (vz * range);

	return true;


}

//���Ɖ~���̌�������
bool Collision::IntersectSphereVsCylinder(
		const DirectX::XMFLOAT3 & spherePosition,
		float sphereRadius,
		const DirectX::XMFLOAT3 & cylinderPosition,
		float cylinderRadius,
		float cylinderHeight,
		DirectX::XMFLOAT3 & outCylinderPosition)
{

	//	A�̑�����B�̓�����Ȃ瓖�����ĂȂ�
	if(spherePosition.y - sphereRadius> cylinderPosition.y + cylinderHeight)
	{
		return false;
	}
	//A�̓���B�̑�����艺�Ȃ瓖�����Ă��Ȃ�
	if (spherePosition.y  + cylinderRadius < cylinderPosition.y)
	{
		return false;
	}
	//XZ���ʏo�͈̔̓`�F�b�N
	float vx = cylinderPosition.x - spherePosition.x;//X�����������Z
	float vz = cylinderPosition.z - spherePosition.z;//�������������Z

	//���a�̍��v�������Z
	float range = sphereRadius + cylinderRadius;

	//�`����a�ւ̃x�N�g�������߂�i���������߂�j
	float distXZ = sqrtf(vx * vx + vz * vz);

	//���������a���傫�������瓖�����ĂȂ�
	if (distXZ > range)
	{
		return false;
	}
	//�P�ʃx�N�g����
	vx /= distXZ;
	vz /= distXZ;

	//�����~���������o��
	outCylinderPosition.x = spherePosition.x+(vx * range);
	outCylinderPosition.y = cylinderPosition.y;
	outCylinderPosition.z = spherePosition.z +(vz * range);

	return true;
}
