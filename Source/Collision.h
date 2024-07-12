#pragma once

#include <DirectXMath.h>
#include "Graphics/Model.h"

//hit結果
struct  HitResult
{
    DirectX::XMFLOAT3 position = { 0,0,0 };//レイとポリゴンの交点
    DirectX::XMFLOAT3 normal =   { 0,0,0 };//衝突したポリゴンの法線ベクトル
    DirectX::XMFLOAT3 rotation = { 0,0,0 };//回転量
    float             distance = 0.0f;     //レイの始点から交点までの距離
    int               materialIndex = -1;  //衝突したポリゴンのマテリアル番号
};

class Collision
{
public:
    // 玉と玉の交差判定
    static bool IntrresectVsSphere(
        const DirectX::XMFLOAT3& positionA, 
        float radiusA,
        const DirectX::XMFLOAT3& positionB, 
        float radiusB,
        DirectX::XMFLOAT3& outPositionB
    );

    //円柱と円柱の交差判定
    static bool IntersecCylinderVsCylinder(
        const DirectX::XMFLOAT3& postionA,
        float radiusA,
        float heightA,
        const DirectX::XMFLOAT3& postionB,
        float radiusB,
        float heightB,
        DirectX::XMFLOAT3& outPositionB)
        ;

    //球と円柱の交差判定
    static bool InstarsecSphereVsCylinder(
        const DirectX::XMFLOAT3& spherePosition,
        float sphereRadius,
        const DirectX::XMFLOAT3& cylinderPosition,
        float cylinderRadius,
        float cylinderHeight,
        DirectX::XMFLOAT3& outCylinderPosition
    );

    //レイとモデルの交差判定
   static bool IntersecRayVsModel(
        const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end,
        const Model* model,
        HitResult& result);
  

   //四角形と四角形の交差判定
   static bool InstarsecCubeVsCube(
       const DirectX::XMFLOAT3& cubeA_Position,
       float widthA,
       float heightA,
       const DirectX::XMFLOAT3& cubeB_Position,
       float widthB,
       float heightB,
       DirectX::XMFLOAT3& outCubePositionA, DirectX::XMFLOAT3& outCubePositionB
   );

   //斜面上の矩形と矩形の当たり判定
   static bool InstarsecCubeVsCubeWithSlope(const DirectX::XMFLOAT3& cubeA_Position, float widthA, float heightA,
       const DirectX::XMFLOAT3& cubeB_Position, float widthB, float heightB,
       const DirectX::XMFLOAT3& blockNormal, DirectX::XMFLOAT3& outCubePosition,
       DirectX::XMFLOAT4 colorA, DirectX::XMFLOAT4 colorB);
   
};

  


