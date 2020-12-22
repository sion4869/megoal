#include "Sprite2D.h"
#include "CDirectxGraphics.h"
#include "Shader.h"


ID3D11InputLayout* Sprite2D::spIL;
ID3D11VertexShader* Sprite2D::spVS;
ID3D11PixelShader* Sprite2D::spPS;


#define DX11_SAFE_RELEASE(o)  if(o!=nullptr){o->Release();o=nullptr;}


Sprite2D::~Sprite2D() {
	DX11_SAFE_RELEASE(mpConstantBuffer);
	DX11_SAFE_RELEASE(mpVetexBuffer);
	DX11_SAFE_RELEASE(mpTexRes);
	DX11_SAFE_RELEASE(mpTexSRV);
}


void Sprite2D::Update(void)
{
	// TODO: �����Ɏ����R�[�h��ǉ����܂�.
}

void Sprite2D::Draw2(void)
{
	ID3D11DeviceContext*		 devicecontext;				// �f�o�C�X�R���e�L�X�g

	// �f�o�C�X�R���e�L�X�g���擾����
	devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	unsigned int stride = sizeof(VERTEX2D);					// �X�g���C�h���Z�b�g�i�P���_������̃o�C�g���j
	unsigned  offset = 0;									// �I�t�Z�b�g�l���Z�b�g

	// ���_�o�b�t�@���f�o�C�X�R���e�L�X�g�փZ�b�g
	devicecontext->IASetVertexBuffers(
		0,													// �X�^�[�g�X���b�g
		1,													// ���_�o�b�t�@��
		&mpVetexBuffer,									// ���_�o�b�t�@�̐擪�A�h���X
		&stride,											// �X�g���C�h
		&offset);											// �I�t�Z�b�g

	// �g�|���W�[���Z�b�g
	devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ���_�t�H�[�}�b�g���Z�b�g
	devicecontext->IASetInputLayout(spIL);

	devicecontext->VSSetShader(spVS, nullptr, 0);		// ���_�V�F�[�_�[���Z�b�g
	devicecontext->PSSetShader(spPS, nullptr, 0);		// �s�N�Z���V�F�[�_�[���Z�b�g

	devicecontext->PSSetShaderResources(0, 1, &mpTexSRV);  // �V�F�[�_�[���e�N�X�`���ɃA�N�Z�X�ł���悤�ɃZ�b�g

	CB_DATA cbdata;
	cbdata.pos = mPos;
	cbdata.scale = mScale;

	cbdata.r = mColor.r;
	cbdata.g = mColor.g;
	cbdata.b = mColor.b;
	cbdata.a = mColor.a;

	cbdata.u0 = mUV0.u;
	cbdata.v0 = mUV0.v;
	cbdata.u3 = mUV3.u;
	cbdata.v3 = mUV3.v;

	D3D11_MAPPED_SUBRESOURCE mapRes;
	HRESULT hr = devicecontext->Map(mpConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapRes);

	if (SUCCEEDED(hr)) {
		memcpy_s(mapRes.pData, mapRes.RowPitch, &cbdata, sizeof(cbdata));
		devicecontext->Unmap(mpConstantBuffer, 0);
	}

	devicecontext->VSSetConstantBuffers(0, 1, &mpConstantBuffer);
	devicecontext->PSSetConstantBuffers(0, 1, &mpConstantBuffer);

	// Z�o�b�t�@�𖳌��ɂ���
	CDirectXGraphics::GetInstance()->TurnOffZbuffer();

	// �f�o�C�X�R���e�L�X�g�ɒ��_�f�[�^�𗬂�����
	devicecontext->Draw(
		4,									// ���_��
		0);									// �J�n���_�C���f�b�N�X

	// Z�o�b�t�@��L���ɂ���
	CDirectXGraphics::GetInstance()->TurnOnZBuffer();
}

HRESULT Sprite2D::CreateShader(void)
{
	// �f�o�C�X���擾����
	ID3D11Device* device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// ���_�i���C�A�E�g�j�f�[�^�̒�`
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "SV_POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// �G�������g��
	unsigned int numElements = ARRAYSIZE(layout);

	// ���_�V�F�[�_�[�I�u�W�F�N�g�𐶐��A�����ɒ��_���C�A�E�g������
	bool sts = CreateVertexShader(
		device,								// �f�o�C�X�I�u�W�F�N�g						
		"shader/vs2d.hlsl", // ���_�V�F�[�_�[�\�[�X�t�@�C��					
		"vs_main",								// �G���g���֐�
		"vs_5_0",							// ���_�V�F�[�_�[���f���T�D�O
		layout,								// ���_�f�[�^���C�A�E�g
		numElements,						// ���_�f�[�^�̃G�������g��
		&spVS,					// ���_�V�F�[�_�[�I�u�W�F�N�g
		&spIL);					// ���_�f�[�^���C�A�E�g�I�u�W�F�N�g

	if (!sts) {
		MessageBox(nullptr, "CreateVertexShader error", "error", MB_OK);
		return E_FAIL;
	}

	// �s�N�Z���V�F�[�_�[�𐶐�
	sts = CreatePixelShader(			// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g�𐶐�
		device,						// �f�o�C�X�I�u�W�F�N�g
		"shader/ps2d.hlsl",		// �s�N�Z���V�F�[�_�[�\�[�X�t�@�C��
		"ps_main",						// �G���g���֐�
		"ps_5_0",					// �s�N�Z���V�F�[�_�[���f���T�D�O
		&spPS);			// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g

	if (!sts) {
		MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}


void Sprite2D::ReleaseShader(void)
{
	DX11_SAFE_RELEASE(spIL);
	DX11_SAFE_RELEASE(spVS);
	DX11_SAFE_RELEASE(spPS);
}

HRESULT Sprite2D::Initialize(void)
{
	VERTEX2D vx[4] = {
		0, 0, 0,
		1, 0, 0,
		0, -1, 0,
		1, -1, 0
	};

	ID3D11Device* device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// ���_�o�b�t�@�𐶐�
	bool sts = CreateVertexBuffer(
		device,						// �f�o�C�X�I�u�W�F�N�g
		sizeof(VERTEX2D),				// �P���_������o�C�g��
		4,							// ���_��
		vx,							// ���_�f�[�^�i�[�������擪�A�h���X
		&mpVetexBuffer				// ���_�o�b�t�@
	);

	if (!sts) {
		MessageBox(nullptr, "CreateVertexBuffer error", "error", MB_OK);
		return E_FAIL;
	}

	// �萔�o�b�t�@�𐶐�
	sts = CreateConstantBufferWrite(device, sizeof(CB_DATA), &mpConstantBuffer);
	if (!sts) {
		MessageBox(nullptr, "Create Constant Buffer Error", "error", MB_OK);
		return false;
	}

	mPos.x = -1.0f;
	mPos.y = 1.0f;
	mScale.x = 2.0f;
	mScale.y = 2.0f;

	mColor.r = 1.0f;
	mColor.g = 1.0f;
	mColor.b = 1.0f;
	mColor.a = 1.0f;

	mUV0.u = 0.0f;
	mUV0.v = 0.0f;
	mUV3.u = 1.0f;
	mUV3.v = 1.0f;


	return S_OK;
}

HRESULT Sprite2D::LoadTexture(const char * const pTexFileName)
{
	bool sts = CreateSRVfromFile(pTexFileName,
		CDirectXGraphics::GetInstance()->GetDXDevice(),
		CDirectXGraphics::GetInstance()->GetImmediateContext(), &mpTexRes, &mpTexSRV);

	if (!sts) {
		MessageBox(nullptr, "Texture Load Error", "error", MB_OK);
		return E_FAIL;
	}
	return S_OK;
}
