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
   /* {
        result.position.x = end.x;
        result.position.y = 0.0f;
        result.position.z = end.z;
        result.normal.x = 0.0f;
        result.normal.y = 1.0f;
        result.normal.z = 0.0f;
        return true;
   }*/

   //四角形と四角形の交差判定
   static bool InstarsecCubeVsCube(
       const DirectX::XMFLOAT3& cubeA_Position,
       float widthA,
       float heightA,
       const DirectX::XMFLOAT3& cubeB_Position,
       float widthB,
       float heightB,
       DirectX::XMFLOAT3& outCubePosition
   );

   static bool InstarsecCubeVsCubeWithSlope(const DirectX::XMFLOAT3& cubeA_Position, float widthA, float heightA,
       const DirectX::XMFLOAT3& cubeB_Position, float widthB, float heightB,
       const DirectX::XMFLOAT3& blockNormal, DirectX::XMFLOAT3& outCubePosition,
       DirectX::XMFLOAT4 colorA, DirectX::XMFLOAT4 colorB);
   
};

  




////レイとモデルの交差判定 X
//bool Collision::IntersecRayVsModel(
//    const DirectX::XMFLOAT3& start,
//    const DirectX::XMFLOAT3& end,
//    const Model* model,
//    HitResult& result)
//{
//    DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);
//    DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);
//    DirectX::XMVECTOR WorldRayVec = DirectX::XMVectorSubtract(WorldEnd, WorldStart);
//    DirectX::XMVECTOR WorldRayLength = DirectX::XMVector3Length(WorldRayVec);
//
//    //ワールド空間のレイの長さをHitResultに格納
//    DirectX::XMStoreFloat(&result.distance, WorldRayLength);
//
//    bool hit = false;
//    const ModelResource* resource = model->GetResource();
//    for (const ModelResource::Mesh& mesh : resource->GetMeshes())
//    {
//        //メッシュノード取得
//        const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);
//
//        //レイをワールド空間からローカル空間へ変換
//        DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&node.worldTransform);
//        DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);
//
//        DirectX::XMVECTOR S = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
//        DirectX::XMVECTOR E = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
//        DirectX::XMVECTOR SE = DirectX::XMVectorSubtract(E, S);
//        DirectX::XMVECTOR V = DirectX::XMVector3Normalize(SE);
//        DirectX::XMVECTOR Length = DirectX::XMVector3Length(SE);
//
//        //レイの長さ
//        float neart;
//        DirectX::XMStoreFloat(&neart, Length);
//
//        //三角形との交差判定
//        const std::vector<ModelResource::Vertex>& vertices = mesh.vertices;
//        const std::vector<UINT> indices = mesh.indices;
//
//        int materialIndex = -1;
//        DirectX::XMVECTOR HitPosition;
//        DirectX::XMVECTOR HitNormal;
//        for (const ModelResource::Subset& subset : mesh.subsets)
//        {
//            for (UINT i = 0; i < subset.indexCount; i += 3)
//            {
//                UINT index = subset.startIndex + i;
//                //三角形の頂点を抽出
//                const ModelResource::Vertex& a = vertices.at(indices.at(index));
//                const ModelResource::Vertex& b = vertices.at(indices.at(index + 1));
//                const ModelResource::Vertex& c = vertices.at(indices.at(index + 2));
//
//                DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
//                DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
//                DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);
//
//                //三角形の三辺ベクトルを算出
//                DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
//                DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
//                DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);
//
//                //交点が三角形の裏面(内部)にある場合の処理
//                DirectX::XMVECTOR N = DirectX::XMVector3Cross(AB, BC);//三角形の法線ベクトル
//                DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(V, N); //レイの方向ベクトルと法線ベクトル
//                float d;
//                DirectX::XMStoreFloat(&d, Dot);
//                if (d >= 0)continue;//表向きの場合はスキップ
//
//                //レイ(光線)と平面の交点を算出
//                DirectX::XMVECTOR SA = DirectX::XMVectorSubtract(A, S);
//                DirectX::XMVECTOR X = DirectX::XMVectorDivide(DirectX::XMVector3Dot(N, SA), Dot);
//                float x;
//                DirectX::XMStoreFloat(&x, X);
//                if (x< .0f || x > neart)continue;//交点までの距離が今までに計算した最近距離より大きい場合はスキップ
//
//                DirectX::XMVECTOR P = DirectX::XMVectorAdd(DirectX::XMVectorMultiply(V, X), S);
//                //交点が三角形の内側にあるか判定
//                //1つめ A～B
//                DirectX::XMVECTOR PA = DirectX::XMVectorSubtract(A, P);
//                DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(PA, AB);
//                DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(Cross1, N);
//                float dot1;
//                DirectX::XMStoreFloat(&dot1, Dot);
//                //内積値が同じ方向を向いていたら三角形の中に入って居る。
//                // 向いて居なかったらcontinue
//                if (dot1 < 0.0f)continue;//内積の値が負の場合、交点は三角形の外側
//
//                //2つめ B～C
//                DirectX::XMVECTOR PB = DirectX::XMVectorSubtract(B, P);
//                DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(PB, BC);
//                DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(Cross2, N);
//                float dot2;
//                DirectX::XMStoreFloat(&dot2, Dot2);
//                if (dot2 < 0.0f)continue;//内積の値が負の場合、交点は三角形の外側
//
//                //3つめ C～A
//                DirectX::XMVECTOR PC = DirectX::XMVectorSubtract(C, P);
//                DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(PC, CA);
//                DirectX::XMVECTOR Dot3 = DirectX::XMVector3Dot(Cross3, N);
//                float dot3;
//                DirectX::XMStoreFloat(&dot3, Dot3);
//                if (dot3 < 0.0f)continue;//内積の値が負の場合、交点は三角形の外側
//
//
//                //最近処理を更新
//                neart = x;
//
//                //交点と法線を更新
//                HitPosition = P;
//                HitNormal = N;
//                materialIndex = subset.materialIndex;
//
//            }
//
//        }
//        if (materialIndex >= 0)
//        {
//            //ローカル空間からワールド空間へ
//            DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition, WorldTransform);
//            DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
//            DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
//            float distance;
//            DirectX::XMStoreFloat(&distance, WorldCrossLength);
//
//            //ヒット情報保存
//            if (result.distance > distance)
//            {
//                DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(HitNormal, WorldTransform);
//
//
//                result.distance = distance;
//                result.materialIndex = materialIndex;
//                DirectX::XMStoreFloat3(&result.position, WorldPosition);
//                DirectX::XMStoreFloat3(&result.normal, DirectX::XMVector3Normalize(WorldNormal));
//                hit = true;
//            }
//        }
//    }
//    return hit;
//}
//
