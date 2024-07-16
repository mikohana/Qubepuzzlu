#include "Cube.h"

Cube::Cube(const char* filename)
{
	model = new Model(filename);
}
Cube::~Cube()
{
	delete model;
}

void  Cube::UpdateTransform()
{
	//スケール行列を作成
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	//回転行列を作成
	//DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(this->angle.x,this->angle.y,this->angle.z);
	// X軸の回転行列
	DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
	// Y軸の回転行列
	DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
	// Z軸の回転行列
	DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);

	//回転行列を作成
	DirectX::XMMATRIX R = Y * X * Z;

	//位置行列を作成
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);//もし引数がtranceform._11,tranceform._22,tranceform._33だとその場で回転になる

	//3つの行列を組み合わせワールド行列を作成
	DirectX::XMMATRIX W = S * R * T;

	//計算したワールド行列を取り出す
	DirectX::XMStoreFloat4x4(&tranceform, W);

	model->UpdateTransform(tranceform);
}

void Cube::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}