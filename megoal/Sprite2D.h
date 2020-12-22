#pragma once

#include <d3d11.h>

//2D�̈ꖇ�l�p�`�I�u�W�F�N�g��\��

class Sprite2D
{
public:
	~Sprite2D();
	void Update(void);
	void Draw2(void);    //����2D�ꖇ�|���S����`�悷��

	HRESULT Initialize(void);   //���̃N���X�̃I�u�W�F�N�g���ƂɕK�v�ȏ��������s��
	HRESULT LoadTexture(const char* const pTexFileName);   //���̃|���S���Ɏg���e�N�X�`����ǂݍ���


	static HRESULT CreateShader(void);   //2D�\���p�̃V�F�[�_�[���쐬����
	static void ReleaseShader(void);     //2D�V�F�[�_�[���J������

	static ID3D11InputLayout* spIL;
	static ID3D11VertexShader* spVS;
	static ID3D11PixelShader* spPS;

	struct VECTOR2D {
		float x, y;
	} mPos, mScale;  //���̃|���S���̍�����W�A�|���S���̊g��k��

	struct COLOR {
		float r, g, b, a;
	} mColor;       //�|���S���̒��_�J���[

	struct UV {
		float u, v;
	} mUV0, mUV3;   //�|���S����UV���W  mUV0 = ����  mUV3 = �E��

private:
	ID3D11Buffer* mpVetexBuffer;

	ID3D11Resource* mpTexRes;
	ID3D11ShaderResourceView* mpTexSRV;

	ID3D11Buffer* mpConstantBuffer;

	struct VERTEX2D {
		float x, y, z;
	};

	struct CB_DATA {
		VECTOR2D pos, scale;

		float r, g, b, a;
		float u0, v0, u3, v3;
	};

};

