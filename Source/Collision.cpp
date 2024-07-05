#include "Collision.h"
#include <cmath> 

//bool Collision::IntrresectVsSphere(const DirectX::XMFLOAT3& postionA, float radiusA, const DirectX::XMFLOAT3& postionB ,float radiusB, DirectX::XMFLOAT3& outPostionB)
//{
//	//A��B�̒P�ʃx�N�g�����Y�o
//	DirectX::XMVECTOR PostionA = DirectX::XMLoadFloat3(&postionA);
//	DirectX::XMVECTOR PostionB = DirectX::XMLoadFloat3(&postionB);
//	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PostionB, PostionA);
//	DirectX::XMVECTOR LengthSq = DirectX::XMVector3Length(Vec);
//	float lengtsSq;
//	DirectX::XMStoreFloat(&lengtsSq, LengthSq);
//
//	//��������
//	//float range = (radiusA + radiusB) * (radiusA + radiusB);
//	float combinedRadius = radiusA + radiusB;
//	float range = combinedRadius * combinedRadius;
//
//	//�������Ă��Ȃ���Ή����o���Ȃ��ꍇ
//		if (lengtsSq > range)
//		{
//			return false;
//		}
//
//		//�������Ă����A��B�������o��
//		if (!lengtsSq >= range)
//		{
//			float distance = sqrt(lengtsSq);//�������v�Z
//			float overlap = range - distance;//�����ʂ��v�Z
//
//			//����B�������o�������̃x�N�g�����v�Z
//			DirectX::XMVECTOR direction = DirectX::XMVector3Normalize(Vec);
//			
//			//����B�������o��
//			DirectX::XMVECTOR newPositionB = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&postionB), DirectX::XMVectorScale(direction, overlap));
//			DirectX::XMStoreFloat3(&outPostionB, newPositionB);
//
//			return true;
//		}
//}

//��vs��
bool Collision::IntrresectVsSphere(const DirectX::XMFLOAT3& positionA, float radiusA, const DirectX::XMFLOAT3& positionB, float radiusB, DirectX::XMFLOAT3& outPositionB)
{
	// A��B�̒P�ʃx�N�g�����v�Z
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
	DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec); // �����̓����v�Z
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);

	// ��������p�ϐ�
	float combinedRadius = radiusA + radiusB;
	float range = combinedRadius * combinedRadius;

	// �������Ă��Ȃ���Ή����o���Ȃ��ꍇ
	if (lengthSq > range)
	{
		return false;
	}

	// �������Ă����A��B�������o��
	float distance = sqrt(lengthSq); // �������v�Z
	float overlap = range - lengthSq; // �����ʂ��v�Z

	// B�������o�������̃x�N�g�����v�Z
	DirectX::XMVECTOR direction = DirectX::XMVector3Normalize(Vec);

	// A��B�������o��
	DirectX::XMVECTOR newPositionB = DirectX::XMVectorAdd(PositionB, DirectX::XMVectorScale(direction, overlap));
	DirectX::XMStoreFloat3(&outPositionB, newPositionB);

	return true;
}
//�~��vs�~��
bool Collision::IntersecCylinderVsCylinder(const DirectX::XMFLOAT3& positionA, float radiusA, float heightA, const DirectX::XMFLOAT3& positionB, float radiusB, float heightB, DirectX::XMFLOAT3& outPositionB)
{
    // A�̑�����B�̓�����Ȃ瓖�����Ă��Ȃ�
    if (positionA.y > positionB.y + heightB)
    {
        return false;
    }
    // A�̓���B�̑�����艺�Ȃ瓖�����Ă��Ȃ�
    if (positionA.y + heightA< positionB.y )
    {
        return false;
    }

    //XZ���ʂł͈̔̓`�F�b�N

    // A��B�̃x�N�g�����v�Z
    float dx = positionB.x - positionA.x;
    float dz = positionB.z - positionA.z;
    float combinedRadius = radiusA + radiusB; // ��������p�ϐ�
    float distanceXZ = sqrt(dx * dx + dz * dz); // �����̌v�Z

  

    // �������Ă��Ȃ���Ή����o���Ȃ��ꍇ
    if (distanceXZ > combinedRadius)
    {
        return false;
    }

    dx /= combinedRadius;
    dz /= combinedRadius;

    outPositionB.x = positionA.x + (dx * combinedRadius);
    outPositionB.y = positionA.y;
    outPositionB.z = positionA.z + (dz * combinedRadius);
    return true;
}

//���Ɖ~���̓����蔻�菈��
bool Collision::InstarsecSphereVsCylinder(const DirectX::XMFLOAT3& spherePosition, float sphereRadius, const DirectX::XMFLOAT3& cylinderPosition, float cylinderRadius, float cylinderHeight, DirectX::XMFLOAT3& outCylinderPosition)
{
    //���̒��S����~���̑��ʂ̋����x�N�g�����v�Z
    DirectX::XMVECTOR SpherePosition = DirectX::XMLoadFloat3(&spherePosition);
    DirectX::XMVECTOR CylinderPosition = DirectX::XMLoadFloat3(&cylinderPosition);
    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(SpherePosition, CylinderPosition);
    
    //�x�N�g���̒����̓����v�Z
    DirectX::XMVECTOR LengtSq = DirectX::XMVector3LengthSq(Vec);
    float lengthSq;
    DirectX::XMStoreFloat(&lengthSq, LengtSq);

    //��������ϐ�
    float combineRadiusSq = sphereRadius * sphereRadius + cylinderRadius * cylinderRadius;

    //�������Ă��Ȃ���Ή������Ȃ�
    if (lengthSq > combineRadiusSq)
    {
        return false;
    }

    // �~���̍������l����������
    // �~���̑��ʂɐڐG���Ă��邩�A�܂��͉~���̏㕔�E�����ɏՓ˂��Ă��邩���f
    DirectX::XMVECTOR CylinderHeightVec = DirectX::XMVectorSet(0.0f,cylinderHeight,0.0f,0.0f);// �~���̍�����\���x�N�g����ݒ�
    DirectX::XMVECTOR CylinderHalfHeightVec = DirectX::XMVectorScale(CylinderHeightVec, 0.5f);// ��Ɖ��̋�ʂ����邽�߂̕ϐ�
    DirectX::XMVECTOR CylinderTopVec = DirectX::XMVectorAdd(CylinderPosition, CylinderHalfHeightVec);//+;
    DirectX::XMVECTOR CylinderBottomVec = DirectX::XMVectorSubtract(CylinderPosition, CylinderHalfHeightVec);//-;

    DirectX::XMVECTOR DistanceToTop = DirectX::XMVectorSubtract(SpherePosition, CylinderTopVec);
    DirectX::XMVECTOR DistanceToBottom = DirectX::XMVectorSubtract(SpherePosition, CylinderBottomVec);

    DirectX::XMVECTOR DistanceToTopSq = DirectX::XMVectorSubtract(SpherePosition, CylinderTopVec);
    DirectX::XMVECTOR DistanceToBottomSq = DirectX::XMVectorSubtract(SpherePosition, CylinderHeightVec);

    //2�_�ԋ���
    float distanceToTopSq, distanceToBottomSq;
    DirectX::XMStoreFloat(&distanceToTopSq, DistanceToTopSq);
    DirectX::XMStoreFloat(&distanceToBottomSq,DistanceToBottom);

    // �����~���̏㕔�܂��͉����ɐڐG���Ă��邩�A�܂��͉~���̑��ʂɐڐG���Ă��邩�𔻒f
    if (distanceToTopSq <= combineRadiusSq || distanceToBottomSq <= combineRadiusSq)
    {
        return true;
    }
    //���͉~���̑��ʂɐڐG���Ă���\�������邽�� 
    //�~���̑��ʂɐڐG���Ă��邩������ɔ��f���邽�߂�
    //�~���̑��ʂ̋����x�N�g�����v�Z
    DirectX::XMVECTOR CylinderSideVec = DirectX::XMVectorSet(0.0f, 0.0f, cylinderRadius, 0.0f);
    DirectX::XMVECTOR DistanceToSide = DirectX::XMVectorSubtract(SpherePosition, CylinderSideVec);
    DirectX::XMVECTOR DistanceToSizeSq = DirectX::XMVector3Length(DistanceToSide);
    
    float distanceToSizeSq;
    DirectX::XMStoreFloat(&distanceToSizeSq, DistanceToSizeSq);
    // �����~���̑��ʂɐڐG���Ă��邩�𔻒f
    if (distanceToSizeSq <= combineRadiusSq)
    {
       //�~���̑��ʂɑ΂��鋅�̃x�N�g�����v�Z
        DirectX::XMVECTOR DistanceToSideVec = DirectX::XMVectorSubtract(SpherePosition, CylinderSideVec);
        DirectX::XMVECTOR DistanceToSideVecNormalized = DirectX::XMVector3Normalize(DistanceToSideVec);

       //�����~���̑��ʂɐڐG���Ă���ꍇ�̈ړ��ʂ��v�Z
        float overlap = sqrt(combineRadiusSq) - sqrt(distanceToSizeSq);

        //�����~������b�����߂̈ړ��ʂ��v�Z
        DirectX::XMVECTOR MoveVec = DirectX::XMVectorScale(DistanceToSideVecNormalized, overlap);

        //�~���̐V�����ʒu���v�Z
        DirectX::XMVECTOR NewCylinderPosition = DirectX::XMVectorAdd(CylinderPosition,MoveVec);
        
        //�V�����ʒu��outCylinderPosition�Ɋi�[
        DirectX::XMStoreFloat3(&outCylinderPosition, NewCylinderPosition);

        return true;
    }

    return false;
}

//���C�ƃ��f���̌������� O
bool Collision::IntersecRayVsModel(
    const DirectX::XMFLOAT3& start,
    const DirectX::XMFLOAT3& end,
    const Model* model,
    HitResult& result)
{
    DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);
    DirectX::XMVECTOR WorldRayVec = DirectX::XMVectorSubtract(WorldEnd, WorldStart);
    DirectX::XMVECTOR WorldRayLength = DirectX::XMVector3Length(WorldRayVec);

    //���[���h��Ԃ̃��C�̒�����HitResult�Ɋi�[
    DirectX::XMStoreFloat(&result.distance, WorldRayLength);

    bool hit = false;
    const ModelResource* resource = model->GetResource();
    for (const ModelResource::Mesh& mesh : resource->GetMeshes())
    {
        //���b�V���m�[�h�擾
        const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);

        //���C�����[���h��Ԃ��烍�[�J����Ԃ֕ϊ�
        DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&node.worldTransform);
        DirectX::XMMATRIX InverseWorldTranceform = DirectX::XMMatrixInverse(nullptr, WorldTransform);

        DirectX::XMVECTOR S = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTranceform);
        DirectX::XMVECTOR E = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTranceform);
        DirectX::XMVECTOR SE = DirectX::XMVectorSubtract(E, S);
        DirectX::XMVECTOR V = DirectX::XMVector3Normalize(SE);
        DirectX::XMVECTOR Length = DirectX::XMVector3Length(SE);

        //���C�̒���
        float neart;
        DirectX::XMStoreFloat(&neart, Length);

        //�O�p�`(��)�Ƃ̌�������
        const std::vector<ModelResource::Vertex>& vertices = mesh.vertices;
        const std::vector<UINT>indices = mesh.indices;

        int materialIndex = -1;
        DirectX::XMVECTOR HitPosition;
        DirectX::XMVECTOR HitNormal;
        for (const ModelResource::Subset& subset : mesh.subsets)
        {
            for (UINT i = 0; i < subset.indexCount; i += 3)
            {
                UINT index = subset.startIndex + i;

                //�O�p�`�̒��_�𒊏o
                const ModelResource::Vertex& a = vertices.at(indices.at(index));
                const ModelResource::Vertex& b = vertices.at(indices.at(index + 1));
                const ModelResource::Vertex& c = vertices.at(indices.at(index + 2));

                DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
                DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
                DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);

                //�O�p�`�̎O�ӂ̃x�N�g�����Z�o
                DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
                DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
                DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);

                //�O�p�`�̖@���x�N�g�����Z�o
                DirectX::XMVECTOR N = DirectX::XMVector3Cross(AB, BC);

                //���ς̌��ʂ��v���X�ł���Ε\����
                DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(V, N);
                float d;
                DirectX::XMStoreFloat(&d, Dot);
                if (d >= 0)continue;

                //���C�ƕ��ʂ̌�_���Z�o
                DirectX::XMVECTOR SA = DirectX::XMVectorSubtract(A, S);
                DirectX::XMVECTOR X = DirectX::XMVectorDivide(DirectX::XMVector3Dot(N, SA), Dot);
                float x;
                DirectX::XMStoreFloat(&x, X);
                if (x < .0f || x > neart)continue; //��_�܂ł̋��������܂łɌv�Z�����ŋߋ��������傫���ꍇ�̓X�L�b�v

                DirectX::XMVECTOR P = DirectX::XMVectorAdd(DirectX::XMVectorMultiply(V, X), S);

                //��_���O�p�`�̓����ɂ��邩����
                //1��
                DirectX::XMVECTOR PA = DirectX::XMVectorSubtract(A, P);
                DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(PA, AB);
                DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(Cross1, N);
                float dot1;
                DirectX::XMStoreFloat(&dot1, Dot1);
                if (dot1 < 0.0f)continue;

                //2��
                DirectX::XMVECTOR PB = DirectX::XMVectorSubtract(B, P);
                DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(PB, BC);
                DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(Cross2, N);
                float dot2;
                DirectX::XMStoreFloat(&dot2, Dot2);
                if (dot2 < 0.0f)continue;

                //3��
                DirectX::XMVECTOR PC = DirectX::XMVectorSubtract(C, P);
                DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(PC, CA);
                DirectX::XMVECTOR Dot3 = DirectX::XMVector3Dot(Cross3, N);
                float dot3;
                DirectX::XMStoreFloat(&dot3, Dot3);
                if (dot3 < 0.0f)continue;

                //�ŋߋ������X�V
                neart = x;

                //��_�Ɩ@�����X�V
                HitPosition = P;
                HitNormal = N;
                materialIndex = subset.materialIndex;
            }
        }
        if (materialIndex >= 0)
        {
            //���[�J����Ԃ��烏�[���h��Ԃ֕ϊ�
            DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition, WorldTransform);
            DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
            DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
            float distance;
            DirectX::XMStoreFloat(&distance, WorldCrossLength);

            //�q�b�g���
            if (result.distance > distance)
            {
                DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(HitNormal, WorldTransform);

                result.distance = distance;
                result.materialIndex = materialIndex;
                DirectX::XMStoreFloat3(&result.position, WorldPosition);
                DirectX::XMStoreFloat3(&result.normal, DirectX::XMVector3Normalize(WorldNormal));
                hit = true;
            }
        }
    }
    return hit;
}

bool Collision::InstarsecCubeVsCube(const DirectX::XMFLOAT3& cubeA_Position, float widthA, float heightA, const DirectX::XMFLOAT3& cubeB_Position, float widthB, float heightB, DirectX::XMFLOAT3& outCubePosition)
{
    // �����̂̊e�ʂ̍ŏ����W�ƍő���W���v�Z
    DirectX::XMFLOAT3 min1 = {
        
        cubeA_Position.x,
        cubeA_Position.y,
        cubeA_Position.z
    };
    DirectX::XMFLOAT3 max1 = {
        
        cubeA_Position.x + widthA,
        cubeA_Position.y + heightA,
        cubeA_Position.z + widthA

    };

    DirectX::XMFLOAT3 min2 = {
       
         cubeB_Position.x,
         cubeB_Position.y,
         cubeB_Position.z
    };
    DirectX::XMFLOAT3 max2 = {
       
        cubeB_Position.x + widthB,
        cubeB_Position.y + heightB,
        cubeB_Position.z + widthB
    };

    // �����̓��m���d�Ȃ��Ă��邩�ǂ����𔻒�
    if (max1.x >= min2.x && min1.x <= max2.x &&
        max1.y >= min2.y && min1.y <= max2.y &&
        max1.z >= min2.z && min1.z <= max2.z) {
        // �d�Ȃ��Ă���ꍇ�A�����o���������s��

        // �����o���x�N�g���̌v�Z
        float overlapX = min(max1.x, max2.x) - max(min1.x, min2.x);
        float overlapY = min(max1.y, max2.y) - max(min1.y, min2.y);
        float overlapZ = min(max1.z, max2.z) - max(min1.z, min2.z);
        
    

        // X�����̉����o��
        if (std::fabsf(overlapX) < std::fabsf(overlapY) && std::fabsf(overlapX) < std::fabsf(overlapZ)) {
            float moveX = overlapX / 2.0f;
            outCubePosition.x = moveX;
        }
        // Y�����̉����o��
        else if (std::fabsf(overlapY) < std::fabsf(overlapX) && std::fabsf(overlapY) < std::fabsf(overlapZ)) {
            float moveY = overlapY / 2.0f;
            outCubePosition.y = moveY;
        }
        // Z�����̉����o��
        else {
            float moveZ = overlapZ / 2.0f;
            outCubePosition.z = moveZ;
        }

        

        return true;  // �d�Ȃ��Ă���
    }

    return false;  // �d�Ȃ��Ă��Ȃ�
}




////���C�ƃ��f���̌�������
//bool Collision::IntersecRayVsModel(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const Model* model, HitResult& result)
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
//        DirectX::XMMATRIX InverssWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);
//
//        DirectX::XMVECTOR S = DirectX::XMVector3TransformCoord(WorldStart, InverssWorldTransform);
//        DirectX::XMVECTOR E = DirectX::XMVector3TransformCoord(WorldEnd, InverssWorldTransform);
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
//        DirectX::XMVECTOR HitPNormal;
//        for (const ModelResource::Subset& subset : mesh.subsets )
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
//                DirectX::XMVECTOR N = DirectX::XMVector3Cross(AB,BC);//�O�p�`�̖@���x�N�g��
//                DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(V, N); //���C�̕����x�N�g���Ɩ@���x�N�g��
//                float d;    
//                DirectX::XMStoreFloat(&d, Dot);
//                if (d >= 0)continue;//�\�����̏ꍇ�̓X�L�b�v
//
//                //���C(����)�ƕ��ʂ̌�_���Z�o
//                DirectX::XMVECTOR SA = DirectX::XMVectorSubtract(A,S);
//                DirectX::XMVECTOR X = DirectX::XMVectorDivide(DirectX::XMVector3Dot(N,SA),Dot);
//                float x;
//                DirectX::XMStoreFloat(&x, X);
//                if (x< .0f || x > neart)continue;//��_�܂ł̋��������܂łɌv�Z�����ŋߋ������傫���ꍇ�̓X�L�b�v
//
//                DirectX::XMVECTOR P = DirectX::XMVectorAdd(DirectX::XMVectorMultiply(V, X), S);
//                //��_���O�p�`�̓����ɂ��邩����
//                //1�� A�`B
//                DirectX::XMVECTOR PA = DirectX::XMVectorSubtract(A,P);
//                DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(PA,AB);
//                DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(Cross1, N);
//                float dot1;
//                DirectX::XMStoreFloat(&dot1, Dot);
//                //���ϒl�����������������Ă�����O�p�`�̒��ɓ����ċ���B
//                // �����ċ��Ȃ�������continue
//                if (dot1 < 0.0f)continue;//���ς̒l�����̏ꍇ�A��_�͎O�p�`�̊O��
//                
//                //2�� B�`C
//                DirectX::XMVECTOR PB = DirectX::XMVectorSubtract(B,P);
//                DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(PB,BC);
//                DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(Cross2, N);
//                float dot2;
//                DirectX::XMStoreFloat(&dot2, Dot2);
//                if (dot2 < 0.0f)continue;//���ς̒l�����̏ꍇ�A��_�͎O�p�`�̊O��
//
//                //3�� C�`A
//                DirectX::XMVECTOR PC = DirectX::XMVectorSubtract(C,P);
//                DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(PC,CA);
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
//                HitPNormal = N;
//                materialIndex = subset.materialIndex;
//              
//            } 
//
//        }
//        if (materialIndex >= 0)
//        {
//            //���[�J����Ԃ��烏�[���h��Ԃ�
//            DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition, WorldTransform);
//            
//            DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
//            DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
//            float distance;
//            DirectX::XMStoreFloat(&distance, WorldCrossLength);
//
//            //�q�b�g���ۑ�
//            if (result.distance > distance)
//            {
//                DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(HitPNormal, WorldTransform);
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
////���C�ƃ��f���̌������� ����
//bool Collision::IntersecRayVsModel(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const Model* model, HitResult& result)
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
//        DirectX::XMMATRIX InverssWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);
//
//        DirectX::XMVECTOR S = DirectX::XMVector3TransformCoord(WorldStart, InverssWorldTransform);
//        DirectX::XMVECTOR E = DirectX::XMVector3TransformCoord(WorldEnd, InverssWorldTransform);
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
//        DirectX::XMVECTOR HitPNormal;
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
//                float dotvalue = DirectX::XMVectorGetX(Dot);         //���ς̒l���擾
//                if (dotvalue >= 0.0f)continue;//�\�����̏ꍇ�̓X�L�b�v
//
//                //���C(����)�ƕ��ʂ̌�_���Z�o
//                DirectX::XMVECTOR SA = DirectX::XMVectorSubtract(A, S);
//                //���������_���ɕϊ����Ă��珜�Z
//                DirectX::XMVECTOR dotSA = DirectX::XMVector3Dot(N, SA);
//                DirectX::XMVECTOR P = DirectX::XMVectorDivide(dotSA, Dot);
//                float x;
//                DirectX::XMStoreFloat(&x, P);
//
//                if (x< 0.0f || x > neart)continue;//��_�܂ł̋��������܂łɌv�Z�����ŋߋ������傫���ꍇ�̓X�L�b�v
//
//                //��_���O�p�`�̓����ɂ��邩����
//                //1�� A�`B
//                DirectX::XMVECTOR PA = DirectX::XMVectorSubtract(A, P);
//                DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(PA, AB);
//                DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(Cross1, N);
//                float dot1value = DirectX::XMVectorGetX(Dot1);
//                //���ϒl�����������������Ă�����O�p�`�̒��ɓ����ċ���B
//                // �����ċ��Ȃ�������continue
//                if (dotvalue >= 0.0f)continue;//���ς̒l�����̏ꍇ�A��_�͎O�p�`�̊O��
//
//                //2�� B�`C
//                DirectX::XMVECTOR PB = DirectX::XMVectorSubtract(B, P);
//                DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(PB, BC);
//                DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(Cross2, N);
//                float dot2value = DirectX::XMVectorGetX(Dot2);
//                if (dot2value >= 0.0f)continue;//���ς̒l�����̏ꍇ�A��_�͎O�p�`�̊O��
//
//                //3�� C�`A
//                DirectX::XMVECTOR PC = DirectX::XMVectorSubtract(C, P);
//                DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(PC, CA);
//                DirectX::XMVECTOR Dot3 = DirectX::XMVector3Dot(Cross3, N);
//                float dot3value = DirectX::XMVectorGetX(Dot3);
//                if (dot3value >= 0.0f)continue;//���ς̒l�����̏ꍇ�A��_�͎O�p�`�̊O��
//
//
//                //�ŋߏ������X�V
//                neart = x;
//
//                //��_�Ɩ@�����X�V
//                HitPosition = P;
//                HitPNormal = N;
//                materialIndex = subset.materialIndex;
//
//            }
//
//        }
//        if (materialIndex >= 0)
//        {
//            //���[�J����Ԃ��烏�[���h��Ԃ�
//            DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition, WorldTransform);
//
//            DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
//            DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
//            float distance;
//            DirectX::XMStoreFloat(&distance, WorldCrossLength);
//
//            //�q�b�g���ۑ�
//            //if (result.distance > distance)
//            {
//                DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(HitPNormal, WorldTransform);
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


////���C�ƃ��f���̌�������
//bool Collision::IntersecRayVsModel(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const Model* model, HitResult& result)
//{
//    //�ȑO�̏����������������悤�ɉ��̎���
//    if (end.y < 0.0f)
//    {
//        result.position.x = end.x;
//        result.position.y = 0.0f;
//        result.position.z = end.z;
//        result.normal.x = 0.0f;
//        result.normal.y = 1.0f;
//        result.normal.z = 0.0f;
//        return true;
//    }
//
//    return false;
//}

//////���Ɖ~���̓����蔻�菈��
//bool Collision::InstarsecSphereVsCylinder(const DirectX::XMFLOAT3& spherePosition, float sphereRadius, const DirectX::XMFLOAT3& cylinderPosition, float cylinderRadius, float cylinderHeight, DirectX::XMFLOAT3& outCylinderPosition)
//{
//    //���̒��S����~���̑��ʂ̋����x�N�g�����v�Z
//    DirectX::XMVECTOR SpherePosition = DirectX::XMLoadFloat3(&spherePosition);
//    DirectX::XMVECTOR CylinderPosition = DirectX::XMLoadFloat3(&cylinderPosition);
//    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(SpherePosition, CylinderPosition);
//
//    //�x�N�g���̒����̓����v�Z
//    DirectX::XMVECTOR LengtSq = DirectX::XMVector3Length(Vec);
//    float lengthSq;
//    DirectX::XMStoreFloat(&lengthSq, LengtSq);
//
//    //��������ϐ�
//    float combineRadiusSq = sphereRadius * sphereRadius + cylinderRadius * cylinderRadius;
//
//    //�������Ă��Ȃ���Ή������Ȃ�
//    if (lengthSq > combineRadiusSq)
//    {
//        return false;
//    }
//    return true;
//}


//bool Collision::IntersecCylinderVsCylinder(const DirectX::XMFLOAT3& positionA, float radiusA, float heightA, const DirectX::XMFLOAT3& positionB, float radiusB, float heightB, DirectX::XMFLOAT3& outPositionB)
//{
//    // A�̑�����B�̓�����Ȃ瓖�����Ă��Ȃ�
//    if (positionA.y + heightA / 2 < positionB.y - heightB / 2)
//    {
//        return false;
//    }
//    // A�̓���B�̑�����艺�Ȃ瓖�����Ă��Ȃ�
//    if (positionA.y - heightA / 2 > positionB.y + heightB / 2)
//    {
//        return false;
//    }
//    // XZ���ʂł͈̔̓`�F�b�N
//    float distanceXZ = sqrt(pow(positionA.x - positionB.x, 2) + pow(positionA.z - positionB.z, 2));
//    if (distanceXZ > radiusA + radiusB)
//    {
//        return false;//�d�Ȃ��Ă��Ȃ��ꍇ��false
//    }
//    // A��B�������o��
//    // Y���ł̉����o���̌v�Z
//    float overlapY = (positionA.y - heightA / 2) - (positionB.y + heightB / 2);
//    if (overlapY > 0) // A��B�������o���ꍇ
//    {
//        // XZ���ʂł̉����o���̌v�Z
//        float overlapXZ = (radiusA + radiusB) - distanceXZ;
//    if (overlapXZ > 0) // XZ���ʂŉ����o��������ꍇ
//        {
//            // �����o�������̌v�Z
//            float directionX = (positionA.x - positionB.x) / distanceXZ;
//            float directionZ = (positionA.z - positionB.z) / distanceXZ;
//
//            // B�̈ʒu���X�V
//            outPositionB.x = positionB.x + (directionX * overlapXZ / 2);
//            outPositionB.y = positionA.y - (heightA / 2 - heightB / 2); // Y���W���X�V
//            outPositionB.z = positionB.z + (directionZ * overlapXZ / 2);
//            return true;
//        }
//    }
//    return false;
//}