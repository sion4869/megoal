#pragma once
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<wrl/client.h>
#include	<string>
#include	"dx11mathutil.h"
#include	"Shader.h"
#include	"shaderhashmap.h"
#include	"DX11Settransform.h"
#include	"quad.h"

// comptr
using Microsoft::WRL::ComPtr;

//// ���_�f�[�^
//struct Vertex {
//	DirectX::XMFLOAT3 pos;		// ���W		
//	DirectX::XMFLOAT4 color;	// �J���[
//	DirectX::XMFLOAT2 tex;		// �e�N�X�`��
//};

class Tex {
public:
	// ��`�̏�����
	bool Init(DirectX::XMFLOAT2* localpos,DirectX::XMFLOAT4 color, DirectX::XMFLOAT2* uv,float z=0.0f);

	// �`��
	void Draw();

	// �e�N�X�`���Ȃ��ŕ`��
	void DrawNoTex();

	void UnInit();

	// �g��A�k��
	void SetScale(float sx, float sy, float sz);

	// �ʒu���Z�b�g
	void SetPosition(float x, float y, float z);

	// Z����]
	void SetRotation(float angle);

	// ��`�̐i�s�������擾�i���摜��������ɍ���Ă��邱�Ƃ�O��j
	DirectX::XMFLOAT3 GetForward() {
		DirectX::XMFLOAT3 forward;
		forward.x = m_worldmtx._21;
		forward.y = m_worldmtx._22;
		forward.z = m_worldmtx._23;
		return forward;
	}

	// ��`�̉��������擾
	DirectX::XMFLOAT3 GetSide() {
		DirectX::XMFLOAT3 side;
		side.x = m_worldmtx._11;
		side.y = m_worldmtx._12;
		side.z = m_worldmtx._13;
		return side;
	}

	// ���[���h�ϊ��s����Z�b�g����
	void SetMtx(DirectX::XMFLOAT4X4 mtx) {
		m_worldmtx = mtx;
	}

	// ���_�f�[�^�X�V
	void updateVertex(DirectX::XMFLOAT4 color, DirectX::XMFLOAT2* uv, float z = 0.0f);

	// ���_�o�b�t�@�X�V
	void updateVbuffer();
	//���[�J�����_�ړ�
	void MoveVertex(XMFLOAT2 fix);

private:
	DirectX::XMFLOAT4X4		m_worldmtx;				// ���[���h�ϊ��s��
	ComPtr<ID3D11Buffer>	m_vertexbuffer;			// ���_�o�b�t�@
	ComPtr<ID3D11Buffer>	m_indexbuffer;			// �C���f�b�N�X�o�b�t�@
	Vertex		        	m_vertex[4];			// ��`�S���_
	XMFLOAT2				m_localpos[4];			//���[�J���ȂS���_�ʒu
};