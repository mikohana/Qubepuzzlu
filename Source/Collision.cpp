#include "Collision.h"
#include <cmath> 

//bool Collision::IntrresectVsSphere(const DirectX::XMFLOAT3& postionA, float radiusA, const DirectX::XMFLOAT3& postionB ,float radiusB, DirectX::XMFLOAT3& outPostionB)
//{
//	//A→Bの単位ベクトルを産出
//	DirectX::XMVECTOR PostionA = DirectX::XMLoadFloat3(&postionA);
//	DirectX::XMVECTOR PostionB = DirectX::XMLoadFloat3(&postionB);
//	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PostionB, PostionA);
//	DirectX::XMVECTOR LengthSq = DirectX::XMVector3Length(Vec);
//	float lengtsSq;
//	DirectX::XMStoreFloat(&lengtsSq, LengthSq);
//
//	//距離判定
//	//float range = (radiusA + radiusB) * (radiusA + radiusB);
//	float combinedRadius = radiusA + radiusB;
//	float range = combinedRadius * combinedRadius;
//
//	//交差していなければ押し出さない場合
//		if (lengtsSq > range)
//		{
//			return false;
//		}
//
//		//交差していればAがBを押し出す
//		if (!lengtsSq >= range)
//		{
//			float distance = sqrt(lengtsSq);//距離を計算
//			float overlap = range - distance;//交差量を計算
//
//			//球体Bを押し出す方向のベクトルを計算
//			DirectX::XMVECTOR direction = DirectX::XMVector3Normalize(Vec);
//			
//			//球体Bを押し出す
//			DirectX::XMVECTOR newPositionB = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&postionB), DirectX::XMVectorScale(direction, overlap));
//			DirectX::XMStoreFloat3(&outPostionB, newPositionB);
//
//			return true;
//		}
//}

//球vs球
bool Collision::IntrresectVsSphere(const DirectX::XMFLOAT3& positionA, float radiusA, const DirectX::XMFLOAT3& positionB, float radiusB, DirectX::XMFLOAT3& outPositionB)
{
	// A→Bの単位ベクトルを計算
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
	DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec); // 距離の二乗を計算
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);

	// 距離判定用変数
	float combinedRadius = radiusA + radiusB;
	float range = combinedRadius * combinedRadius;

	// 交差していなければ押し出さない場合
	if (lengthSq > range)
	{
		return false;
	}

	// 交差していればAがBを押し出す
	float distance = sqrt(lengthSq); // 距離を計算
	float overlap = range - lengthSq; // 交差量を計算

	// Bを押し出す方向のベクトルを計算
	DirectX::XMVECTOR direction = DirectX::XMVector3Normalize(Vec);

	// AがBを押し出す
	DirectX::XMVECTOR newPositionB = DirectX::XMVectorAdd(PositionB, DirectX::XMVectorScale(direction, overlap));
	DirectX::XMStoreFloat3(&outPositionB, newPositionB);

	return true;
}
//円柱vs円柱
bool Collision::IntersecCylinderVsCylinder(const DirectX::XMFLOAT3& positionA, float radiusA, float heightA, const DirectX::XMFLOAT3& positionB, float radiusB, float heightB, DirectX::XMFLOAT3& outPositionB)
{
    // Aの足元がBの頭より上なら当たっていない
    if (positionA.y > positionB.y + heightB)
    {
        return false;
    }
    // Aの頭がBの足元より下なら当たっていない
    if (positionA.y + heightA< positionB.y )
    {
        return false;
    }

    //XZ平面での範囲チェック

    // A→Bのベクトルを計算
    float dx = positionB.x - positionA.x;
    float dz = positionB.z - positionA.z;
    float combinedRadius = radiusA + radiusB; // 距離判定用変数
    float distanceXZ = sqrt(dx * dx + dz * dz); // 距離の計算

  

    // 交差していなければ押し出さない場合
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

//球と円柱の当たり判定処理
bool Collision::InstarsecSphereVsCylinder(const DirectX::XMFLOAT3& spherePosition, float sphereRadius, const DirectX::XMFLOAT3& cylinderPosition, float cylinderRadius, float cylinderHeight, DirectX::XMFLOAT3& outCylinderPosition)
{
    //球の中心から円柱の側面の距離ベクトルを計算
    DirectX::XMVECTOR SpherePosition = DirectX::XMLoadFloat3(&spherePosition);
    DirectX::XMVECTOR CylinderPosition = DirectX::XMLoadFloat3(&cylinderPosition);
    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(SpherePosition, CylinderPosition);
    
    //ベクトルの長さの二乗を計算
    DirectX::XMVECTOR LengtSq = DirectX::XMVector3LengthSq(Vec);
    float lengthSq;
    DirectX::XMStoreFloat(&lengthSq, LengtSq);

    //距離判定変数
    float combineRadiusSq = sphereRadius * sphereRadius + cylinderRadius * cylinderRadius;

    //交差していなければ何もしない
    if (lengthSq > combineRadiusSq)
    {
        return false;
    }

    // 円柱の高さを考慮した判定
    // 円柱の側面に接触しているか、または円柱の上部・下部に衝突しているか判断
    DirectX::XMVECTOR CylinderHeightVec = DirectX::XMVectorSet(0.0f,cylinderHeight,0.0f,0.0f);// 円柱の高さを表すベクトルを設定
    DirectX::XMVECTOR CylinderHalfHeightVec = DirectX::XMVectorScale(CylinderHeightVec, 0.5f);// 上と下の区別をつけるための変数
    DirectX::XMVECTOR CylinderTopVec = DirectX::XMVectorAdd(CylinderPosition, CylinderHalfHeightVec);//+;
    DirectX::XMVECTOR CylinderBottomVec = DirectX::XMVectorSubtract(CylinderPosition, CylinderHalfHeightVec);//-;

    DirectX::XMVECTOR DistanceToTop = DirectX::XMVectorSubtract(SpherePosition, CylinderTopVec);
    DirectX::XMVECTOR DistanceToBottom = DirectX::XMVectorSubtract(SpherePosition, CylinderBottomVec);

    DirectX::XMVECTOR DistanceToTopSq = DirectX::XMVectorSubtract(SpherePosition, CylinderTopVec);
    DirectX::XMVECTOR DistanceToBottomSq = DirectX::XMVectorSubtract(SpherePosition, CylinderHeightVec);

    //2点間距離
    float distanceToTopSq, distanceToBottomSq;
    DirectX::XMStoreFloat(&distanceToTopSq, DistanceToTopSq);
    DirectX::XMStoreFloat(&distanceToBottomSq,DistanceToBottom);

    // 球が円柱の上部または下部に接触しているか、または円柱の側面に接触しているかを判断
    if (distanceToTopSq <= combineRadiusSq || distanceToBottomSq <= combineRadiusSq)
    {
        return true;
    }
    //球は円柱の側面に接触している可能性があるため 
    //円柱の側面に接触しているかをさらに判断するために
    //円柱の側面の距離ベクトルを計算
    DirectX::XMVECTOR CylinderSideVec = DirectX::XMVectorSet(0.0f, 0.0f, cylinderRadius, 0.0f);
    DirectX::XMVECTOR DistanceToSide = DirectX::XMVectorSubtract(SpherePosition, CylinderSideVec);
    DirectX::XMVECTOR DistanceToSizeSq = DirectX::XMVector3Length(DistanceToSide);
    
    float distanceToSizeSq;
    DirectX::XMStoreFloat(&distanceToSizeSq, DistanceToSizeSq);
    // 球が円柱の側面に接触しているかを判断
    if (distanceToSizeSq <= combineRadiusSq)
    {
       //円柱の側面に対する球のベクトルを計算
        DirectX::XMVECTOR DistanceToSideVec = DirectX::XMVectorSubtract(SpherePosition, CylinderSideVec);
        DirectX::XMVECTOR DistanceToSideVecNormalized = DirectX::XMVector3Normalize(DistanceToSideVec);

       //球が円柱の側面に接触している場合の移動量を計算
        float overlap = sqrt(combineRadiusSq) - sqrt(distanceToSizeSq);

        //球を円柱から話すための移動量を計算
        DirectX::XMVECTOR MoveVec = DirectX::XMVectorScale(DistanceToSideVecNormalized, overlap);

        //円柱の新しい位置を計算
        DirectX::XMVECTOR NewCylinderPosition = DirectX::XMVectorAdd(CylinderPosition,MoveVec);
        
        //新しい位置をoutCylinderPositionに格納
        DirectX::XMStoreFloat3(&outCylinderPosition, NewCylinderPosition);

        return true;
    }

    return false;
}

//レイとモデルの交差判定 O
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

    //ワールド空間のレイの長さをHitResultに格納
    DirectX::XMStoreFloat(&result.distance, WorldRayLength);

    bool hit = false;
    const ModelResource* resource = model->GetResource();
    for (const ModelResource::Mesh& mesh : resource->GetMeshes())
    {
        //メッシュノード取得
        const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);

        //レイをワールド空間からローカル空間へ変換
        DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&node.worldTransform);
        DirectX::XMMATRIX InverseWorldTranceform = DirectX::XMMatrixInverse(nullptr, WorldTransform);

        DirectX::XMVECTOR S = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTranceform);
        DirectX::XMVECTOR E = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTranceform);
        DirectX::XMVECTOR SE = DirectX::XMVectorSubtract(E, S);
        DirectX::XMVECTOR V = DirectX::XMVector3Normalize(SE);
        DirectX::XMVECTOR Length = DirectX::XMVector3Length(SE);

        //レイの長さ
        float neart;
        DirectX::XMStoreFloat(&neart, Length);

        //三角形(面)との交差判定
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

                //三角形の頂点を抽出
                const ModelResource::Vertex& a = vertices.at(indices.at(index));
                const ModelResource::Vertex& b = vertices.at(indices.at(index + 1));
                const ModelResource::Vertex& c = vertices.at(indices.at(index + 2));

                DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
                DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
                DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);

                //三角形の三辺のベクトルを算出
                DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
                DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
                DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);

                //三角形の法線ベクトルを算出
                DirectX::XMVECTOR N = DirectX::XMVector3Cross(AB, BC);

                //内積の結果がプラスであれば表向き
                DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(V, N);
                float d;
                DirectX::XMStoreFloat(&d, Dot);
                if (d >= 0)continue;

                //レイと平面の交点を算出
                DirectX::XMVECTOR SA = DirectX::XMVectorSubtract(A, S);
                DirectX::XMVECTOR X = DirectX::XMVectorDivide(DirectX::XMVector3Dot(N, SA), Dot);
                float x;
                DirectX::XMStoreFloat(&x, X);
                if (x < .0f || x > neart)continue; //交点までの距離が今までに計算した最近距離よりも大きい場合はスキップ

                DirectX::XMVECTOR P = DirectX::XMVectorAdd(DirectX::XMVectorMultiply(V, X), S);

                //交点が三角形の内側にあるか判定
                //1つめ
                DirectX::XMVECTOR PA = DirectX::XMVectorSubtract(A, P);
                DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(PA, AB);
                DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(Cross1, N);
                float dot1;
                DirectX::XMStoreFloat(&dot1, Dot1);
                if (dot1 < 0.0f)continue;

                //2つめ
                DirectX::XMVECTOR PB = DirectX::XMVectorSubtract(B, P);
                DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(PB, BC);
                DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(Cross2, N);
                float dot2;
                DirectX::XMStoreFloat(&dot2, Dot2);
                if (dot2 < 0.0f)continue;

                //3つめ
                DirectX::XMVECTOR PC = DirectX::XMVectorSubtract(C, P);
                DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(PC, CA);
                DirectX::XMVECTOR Dot3 = DirectX::XMVector3Dot(Cross3, N);
                float dot3;
                DirectX::XMStoreFloat(&dot3, Dot3);
                if (dot3 < 0.0f)continue;

                //最近距離を更新
                neart = x;

                //交点と法線を更新
                HitPosition = P;
                HitNormal = N;
                materialIndex = subset.materialIndex;
            }
        }
        if (materialIndex >= 0)
        {
            //ローカル空間からワールド空間へ変換
            DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition, WorldTransform);
            DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
            DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
            float distance;
            DirectX::XMStoreFloat(&distance, WorldCrossLength);

            //ヒット情報
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
    // 立方体の各面の最小座標と最大座標を計算
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

    // 立方体同士が重なっているかどうかを判定
    if (max1.x >= min2.x && min1.x <= max2.x &&
        max1.y >= min2.y && min1.y <= max2.y &&
        max1.z >= min2.z && min1.z <= max2.z) {
        // 重なっている場合、押し出し処理を行う

        // 押し出しベクトルの計算
        float overlapX = min(max1.x, max2.x) - max(min1.x, min2.x);
        float overlapY = min(max1.y, max2.y) - max(min1.y, min2.y);
        float overlapZ = min(max1.z, max2.z) - max(min1.z, min2.z);
        
    

        // X方向の押し出し
        if (std::fabsf(overlapX) < std::fabsf(overlapY) && std::fabsf(overlapX) < std::fabsf(overlapZ)) {
            float moveX = overlapX / 2.0f;
            outCubePosition.x = moveX;
        }
        // Y方向の押し出し
        else if (std::fabsf(overlapY) < std::fabsf(overlapX) && std::fabsf(overlapY) < std::fabsf(overlapZ)) {
            float moveY = overlapY / 2.0f;
            outCubePosition.y = moveY;
        }
        // Z方向の押し出し
        else {
            float moveZ = overlapZ / 2.0f;
            outCubePosition.z = moveZ;
        }

        

        return true;  // 重なっていた
    }

    return false;  // 重なっていない
}




////レイとモデルの交差判定
//bool Collision::IntersecRayVsModel(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const Model* model, HitResult& result)
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
//        DirectX::XMMATRIX InverssWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);
//
//        DirectX::XMVECTOR S = DirectX::XMVector3TransformCoord(WorldStart, InverssWorldTransform);
//        DirectX::XMVECTOR E = DirectX::XMVector3TransformCoord(WorldEnd, InverssWorldTransform);
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
//        DirectX::XMVECTOR HitPNormal;
//        for (const ModelResource::Subset& subset : mesh.subsets )
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
//                DirectX::XMVECTOR N = DirectX::XMVector3Cross(AB,BC);//三角形の法線ベクトル
//                DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(V, N); //レイの方向ベクトルと法線ベクトル
//                float d;    
//                DirectX::XMStoreFloat(&d, Dot);
//                if (d >= 0)continue;//表向きの場合はスキップ
//
//                //レイ(光線)と平面の交点を算出
//                DirectX::XMVECTOR SA = DirectX::XMVectorSubtract(A,S);
//                DirectX::XMVECTOR X = DirectX::XMVectorDivide(DirectX::XMVector3Dot(N,SA),Dot);
//                float x;
//                DirectX::XMStoreFloat(&x, X);
//                if (x< .0f || x > neart)continue;//交点までの距離が今までに計算した最近距離より大きい場合はスキップ
//
//                DirectX::XMVECTOR P = DirectX::XMVectorAdd(DirectX::XMVectorMultiply(V, X), S);
//                //交点が三角形の内側にあるか判定
//                //1つめ A～B
//                DirectX::XMVECTOR PA = DirectX::XMVectorSubtract(A,P);
//                DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(PA,AB);
//                DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(Cross1, N);
//                float dot1;
//                DirectX::XMStoreFloat(&dot1, Dot);
//                //内積値が同じ方向を向いていたら三角形の中に入って居る。
//                // 向いて居なかったらcontinue
//                if (dot1 < 0.0f)continue;//内積の値が負の場合、交点は三角形の外側
//                
//                //2つめ B～C
//                DirectX::XMVECTOR PB = DirectX::XMVectorSubtract(B,P);
//                DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(PB,BC);
//                DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(Cross2, N);
//                float dot2;
//                DirectX::XMStoreFloat(&dot2, Dot2);
//                if (dot2 < 0.0f)continue;//内積の値が負の場合、交点は三角形の外側
//
//                //3つめ C～A
//                DirectX::XMVECTOR PC = DirectX::XMVectorSubtract(C,P);
//                DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(PC,CA);
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
//                HitPNormal = N;
//                materialIndex = subset.materialIndex;
//              
//            } 
//
//        }
//        if (materialIndex >= 0)
//        {
//            //ローカル空間からワールド空間へ
//            DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition, WorldTransform);
//            
//            DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
//            DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
//            float distance;
//            DirectX::XMStoreFloat(&distance, WorldCrossLength);
//
//            //ヒット情報保存
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
////レイとモデルの交差判定 自作
//bool Collision::IntersecRayVsModel(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const Model* model, HitResult& result)
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
//        DirectX::XMMATRIX InverssWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);
//
//        DirectX::XMVECTOR S = DirectX::XMVector3TransformCoord(WorldStart, InverssWorldTransform);
//        DirectX::XMVECTOR E = DirectX::XMVector3TransformCoord(WorldEnd, InverssWorldTransform);
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
//        DirectX::XMVECTOR HitPNormal;
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
//                float dotvalue = DirectX::XMVectorGetX(Dot);         //内積の値を取得
//                if (dotvalue >= 0.0f)continue;//表向きの場合はスキップ
//
//                //レイ(光線)と平面の交点を算出
//                DirectX::XMVECTOR SA = DirectX::XMVectorSubtract(A, S);
//                //浮動小数点数に変換してから除算
//                DirectX::XMVECTOR dotSA = DirectX::XMVector3Dot(N, SA);
//                DirectX::XMVECTOR P = DirectX::XMVectorDivide(dotSA, Dot);
//                float x;
//                DirectX::XMStoreFloat(&x, P);
//
//                if (x< 0.0f || x > neart)continue;//交点までの距離が今までに計算した最近距離より大きい場合はスキップ
//
//                //交点が三角形の内側にあるか判定
//                //1つめ A～B
//                DirectX::XMVECTOR PA = DirectX::XMVectorSubtract(A, P);
//                DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(PA, AB);
//                DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(Cross1, N);
//                float dot1value = DirectX::XMVectorGetX(Dot1);
//                //内積値が同じ方向を向いていたら三角形の中に入って居る。
//                // 向いて居なかったらcontinue
//                if (dotvalue >= 0.0f)continue;//内積の値が負の場合、交点は三角形の外側
//
//                //2つめ B～C
//                DirectX::XMVECTOR PB = DirectX::XMVectorSubtract(B, P);
//                DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(PB, BC);
//                DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(Cross2, N);
//                float dot2value = DirectX::XMVectorGetX(Dot2);
//                if (dot2value >= 0.0f)continue;//内積の値が負の場合、交点は三角形の外側
//
//                //3つめ C～A
//                DirectX::XMVECTOR PC = DirectX::XMVectorSubtract(C, P);
//                DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(PC, CA);
//                DirectX::XMVECTOR Dot3 = DirectX::XMVector3Dot(Cross3, N);
//                float dot3value = DirectX::XMVectorGetX(Dot3);
//                if (dot3value >= 0.0f)continue;//内積の値が負の場合、交点は三角形の外側
//
//
//                //最近処理を更新
//                neart = x;
//
//                //交点と法線を更新
//                HitPosition = P;
//                HitPNormal = N;
//                materialIndex = subset.materialIndex;
//
//            }
//
//        }
//        if (materialIndex >= 0)
//        {
//            //ローカル空間からワールド空間へ
//            DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition, WorldTransform);
//
//            DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
//            DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
//            float distance;
//            DirectX::XMStoreFloat(&distance, WorldCrossLength);
//
//            //ヒット情報保存
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


////レイとモデルの交差判定
//bool Collision::IntersecRayVsModel(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const Model* model, HitResult& result)
//{
//    //以前の処理が正しく動くように仮の実装
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

//////球と円柱の当たり判定処理
//bool Collision::InstarsecSphereVsCylinder(const DirectX::XMFLOAT3& spherePosition, float sphereRadius, const DirectX::XMFLOAT3& cylinderPosition, float cylinderRadius, float cylinderHeight, DirectX::XMFLOAT3& outCylinderPosition)
//{
//    //球の中心から円柱の側面の距離ベクトルを計算
//    DirectX::XMVECTOR SpherePosition = DirectX::XMLoadFloat3(&spherePosition);
//    DirectX::XMVECTOR CylinderPosition = DirectX::XMLoadFloat3(&cylinderPosition);
//    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(SpherePosition, CylinderPosition);
//
//    //ベクトルの長さの二乗を計算
//    DirectX::XMVECTOR LengtSq = DirectX::XMVector3Length(Vec);
//    float lengthSq;
//    DirectX::XMStoreFloat(&lengthSq, LengtSq);
//
//    //距離判定変数
//    float combineRadiusSq = sphereRadius * sphereRadius + cylinderRadius * cylinderRadius;
//
//    //交差していなければ何もしない
//    if (lengthSq > combineRadiusSq)
//    {
//        return false;
//    }
//    return true;
//}


//bool Collision::IntersecCylinderVsCylinder(const DirectX::XMFLOAT3& positionA, float radiusA, float heightA, const DirectX::XMFLOAT3& positionB, float radiusB, float heightB, DirectX::XMFLOAT3& outPositionB)
//{
//    // Aの足元がBの頭より上なら当たっていない
//    if (positionA.y + heightA / 2 < positionB.y - heightB / 2)
//    {
//        return false;
//    }
//    // Aの頭がBの足元より下なら当たっていない
//    if (positionA.y - heightA / 2 > positionB.y + heightB / 2)
//    {
//        return false;
//    }
//    // XZ平面での範囲チェック
//    float distanceXZ = sqrt(pow(positionA.x - positionB.x, 2) + pow(positionA.z - positionB.z, 2));
//    if (distanceXZ > radiusA + radiusB)
//    {
//        return false;//重なっていない場合にfalse
//    }
//    // AがBを押し出す
//    // Y軸での押し出しの計算
//    float overlapY = (positionA.y - heightA / 2) - (positionB.y + heightB / 2);
//    if (overlapY > 0) // AがBを押し出す場合
//    {
//        // XZ平面での押し出しの計算
//        float overlapXZ = (radiusA + radiusB) - distanceXZ;
//    if (overlapXZ > 0) // XZ平面で押し出しがある場合
//        {
//            // 押し出し方向の計算
//            float directionX = (positionA.x - positionB.x) / distanceXZ;
//            float directionZ = (positionA.z - positionB.z) / distanceXZ;
//
//            // Bの位置を更新
//            outPositionB.x = positionB.x + (directionX * overlapXZ / 2);
//            outPositionB.y = positionA.y - (heightA / 2 - heightB / 2); // Y座標を更新
//            outPositionB.z = positionB.z + (directionZ * overlapXZ / 2);
//            return true;
//        }
//    }
//    return false;
//}