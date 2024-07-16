#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "Graphics/RenderContext.h"
#include "Graphics/Model.h"
#include <mutex>

class Shader
{
public:
	Shader() {}
	virtual ~Shader() {}

	// �`��J�n
	virtual void Begin(ID3D11DeviceContext* dc, const RenderContext& rc) = 0;

	// �`��
	virtual void Draw(ID3D11DeviceContext* dc, const Model* model) = 0;

	// �`��I��
	virtual void End(ID3D11DeviceContext* context) = 0;
	
    // �F��ݒ肷�郁�\�b�h
	//virtual void SetColor(const DirectX::XMFLOAT4& color) = 0;

private:
    ID3D11Buffer* constantBuffer;  // �萔�o�b�t�@

};
