#pragma once

#include <DirectXMath.h>
#include "Graphics/Model.h"

//hit����
struct  HitResult
{
    DirectX::XMFLOAT3 position = { 0,0,0 };//���C�ƃ|���S���̌�_
    DirectX::XMFLOAT3 normal =   { 0,0,0 };//�Փ˂����|���S���̖@���x�N�g��
    DirectX::XMFLOAT3 rotation = { 0,0,0 };//��]��
    float             distance = 0.0f;     //���C�̎n�_�����_�܂ł̋���
    int               materialIndex = -1;  //�Փ˂����|���S���̃}�e���A���ԍ�
};

class Collision
{
public:
    // �ʂƋʂ̌�������
    static bool IntrresectVsSphere(
        const DirectX::XMFLOAT3& positionA, 
        float radiusA,
        const DirectX::XMFLOAT3& positionB, 
        float radiusB,
        DirectX::XMFLOAT3& outPositionB
    );

    //�~���Ɖ~���̌�������
    static bool IntersecCylinderVsCylinder(
        const DirectX::XMFLOAT3& postionA,
        float radiusA,
        float heightA,
        const DirectX::XMFLOAT3& postionB,
        float radiusB,
        float heightB,
        DirectX::XMFLOAT3& outPositionB)
        ;

    //���Ɖ~���̌�������
    static bool InstarsecSphereVsCylinder(
        const DirectX::XMFLOAT3& spherePosition,
        float sphereRadius,
        const DirectX::XMFLOAT3& cylinderPosition,
        float cylinderRadius,
        float cylinderHeight,
        DirectX::XMFLOAT3& outCylinderPosition
    );

    //���C�ƃ��f���̌�������
   static bool IntersecRayVsModel(
        const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end,
        const Model* model,
        HitResult& result);
  

   //�l�p�`�Ǝl�p�`�̌�������
   static bool InstarsecCubeVsCube(
       const DirectX::XMFLOAT3& cubeA_Position,
       float widthA,
       float heightA,
       const DirectX::XMFLOAT3& cubeB_Position,
       float widthB,
       float heightB,
       DirectX::XMFLOAT3& outCubePositionA, DirectX::XMFLOAT3& outCubePositionB
   );

   //�Ζʏ�̋�`�Ƌ�`�̓����蔻��
   static bool InstarsecCubeVsCubeWithSlope(const DirectX::XMFLOAT3& cubeA_Position, float widthA, float heightA,
       const DirectX::XMFLOAT3& cubeB_Position, float widthB, float heightB,
       const DirectX::XMFLOAT3& blockNormal, DirectX::XMFLOAT3& outCubePosition,
       DirectX::XMFLOAT4 colorA, DirectX::XMFLOAT4 colorB);
   
};

  


