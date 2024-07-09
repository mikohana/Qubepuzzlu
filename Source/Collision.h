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
   /* {
        result.position.x = end.x;
        result.position.y = 0.0f;
        result.position.z = end.z;
        result.normal.x = 0.0f;
        result.normal.y = 1.0f;
        result.normal.z = 0.0f;
        return true;
   }*/

   //�l�p�`�Ǝl�p�`�̌�������
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

  




////���C�ƃ��f���̌������� X
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
//    //���[���h��Ԃ̃��C�̒�����HitResult�Ɋi�[
//    DirectX::XMStoreFloat(&result.distance, WorldRayLength);
//
//    bool hit = false;
//    const ModelResource* resource = model->GetResource();
//    for (const ModelResource::Mesh& mesh : resource->GetMeshes())
//    {
//        //���b�V���m�[�h�擾
//        const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);
//
//        //���C�����[���h��Ԃ��烍�[�J����Ԃ֕ϊ�
//        DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&node.worldTransform);
//        DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);
//
//        DirectX::XMVECTOR S = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
//        DirectX::XMVECTOR E = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
//        DirectX::XMVECTOR SE = DirectX::XMVectorSubtract(E, S);
//        DirectX::XMVECTOR V = DirectX::XMVector3Normalize(SE);
//        DirectX::XMVECTOR Length = DirectX::XMVector3Length(SE);
//
//        //���C�̒���
//        float neart;
//        DirectX::XMStoreFloat(&neart, Length);
//
//        //�O�p�`�Ƃ̌�������
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
//                //�O�p�`�̒��_�𒊏o
//                const ModelResource::Vertex& a = vertices.at(indices.at(index));
//                const ModelResource::Vertex& b = vertices.at(indices.at(index + 1));
//                const ModelResource::Vertex& c = vertices.at(indices.at(index + 2));
//
//                DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
//                DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
//                DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);
//
//                //�O�p�`�̎O�Ӄx�N�g�����Z�o
//                DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
//                DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
//                DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);
//
//                //��_���O�p�`�̗���(����)�ɂ���ꍇ�̏���
//                DirectX::XMVECTOR N = DirectX::XMVector3Cross(AB, BC);//�O�p�`�̖@���x�N�g��
//                DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(V, N); //���C�̕����x�N�g���Ɩ@���x�N�g��
//                float d;
//                DirectX::XMStoreFloat(&d, Dot);
//                if (d >= 0)continue;//�\�����̏ꍇ�̓X�L�b�v
//
//                //���C(����)�ƕ��ʂ̌�_���Z�o
//                DirectX::XMVECTOR SA = DirectX::XMVectorSubtract(A, S);
//                DirectX::XMVECTOR X = DirectX::XMVectorDivide(DirectX::XMVector3Dot(N, SA), Dot);
//                float x;
//                DirectX::XMStoreFloat(&x, X);
//                if (x< .0f || x > neart)continue;//��_�܂ł̋��������܂łɌv�Z�����ŋߋ������傫���ꍇ�̓X�L�b�v
//
//                DirectX::XMVECTOR P = DirectX::XMVectorAdd(DirectX::XMVectorMultiply(V, X), S);
//                //��_���O�p�`�̓����ɂ��邩����
//                //1�� A�`B
//                DirectX::XMVECTOR PA = DirectX::XMVectorSubtract(A, P);
//                DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(PA, AB);
//                DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(Cross1, N);
//                float dot1;
//                DirectX::XMStoreFloat(&dot1, Dot);
//                //���ϒl�����������������Ă�����O�p�`�̒��ɓ����ċ���B
//                // �����ċ��Ȃ�������continue
//                if (dot1 < 0.0f)continue;//���ς̒l�����̏ꍇ�A��_�͎O�p�`�̊O��
//
//                //2�� B�`C
//                DirectX::XMVECTOR PB = DirectX::XMVectorSubtract(B, P);
//                DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(PB, BC);
//                DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(Cross2, N);
//                float dot2;
//                DirectX::XMStoreFloat(&dot2, Dot2);
//                if (dot2 < 0.0f)continue;//���ς̒l�����̏ꍇ�A��_�͎O�p�`�̊O��
//
//                //3�� C�`A
//                DirectX::XMVECTOR PC = DirectX::XMVectorSubtract(C, P);
//                DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(PC, CA);
//                DirectX::XMVECTOR Dot3 = DirectX::XMVector3Dot(Cross3, N);
//                float dot3;
//                DirectX::XMStoreFloat(&dot3, Dot3);
//                if (dot3 < 0.0f)continue;//���ς̒l�����̏ꍇ�A��_�͎O�p�`�̊O��
//
//
//                //�ŋߏ������X�V
//                neart = x;
//
//                //��_�Ɩ@�����X�V
//                HitPosition = P;
//                HitNormal = N;
//                materialIndex = subset.materialIndex;
//
//            }
//
//        }
//        if (materialIndex >= 0)
//        {
//            //���[�J����Ԃ��烏�[���h��Ԃ�
//            DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition, WorldTransform);
//            DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
//            DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
//            float distance;
//            DirectX::XMStoreFloat(&distance, WorldCrossLength);
//
//            //�q�b�g���ۑ�
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
