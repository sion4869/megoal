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



// ��`�̏�����
bool Quad::Init(uint32_t width, uint32_t height, DirectX::XMFLOAT4 color, DirectX::XMFLOAT2* uv,float z) {
	DX11MtxIdentity(m_worldmtx);	// �����p��

	// �s�N�Z���V�F�[�_�[�𐶐�
	bool sts = ShaderHashmap::GetInstance()->SetPixelShader(psfilename[0]);
	if (!sts) {
		MessageBox(nullptr, "SetPixelShader error(basicps.hlsl)", "error", MB_OK);
		return false;
	}

	// �s�N�Z���V�F�[�_�[�𐶐�
	sts = ShaderHashmap::GetInstance()->SetPixelShader(psfilename[1]);
	if (!sts) {
		MessageBox(nullptr, "SetPixelShader error(basicnotexps.hlsl)", "error", MB_OK);
		return false;
	}

	// ���_�f�[�^�̒�`
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// �G�������g��
	unsigned int numElements = ARRAYSIZE(layout);

	// ���_�V�F�[�_�[�𐶐�
	sts = ShaderHashmap::GetInstance()->SetVertexShader(
		vsfilename[0],			// ���_�V�F�[�_�[�t�@�C����
		layout,					// ���_���C�A�E�g
		numElements);			// �G�������g��

	// ���_������
	updateVertex(width, height, color, uv, z);

	// �f�o�C�X���擾
	ID3D11Device* device;
	device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// ���_�o�b�t�@�𐶐�
	sts = CreateVertexBufferWrite(
		device,						// �f�o�C�X�I�u�W�F�N�g
		sizeof(Vertex),		// �P���_������o�C�g��
		4,							// ���_��
		m_vertex,					// ���_�f�[�^�i�[�������擪�A�h���X
		&m_vertexbuffer				// ���_�o�b�t�@
	);
	if (!sts) {
		MessageBox(nullptr, "CreateVertexBuffer error", "error", MB_OK);
		return false;
	}

	// �C���f�b�N�X�o�b�t�@�𐶐�
	uint32_t index[4] = { 0,1,2,3 };

	sts = CreateIndexBuffer(
		device,						// �f�o�C�X�I�u�W�F�N�g
		4,							// �C���f�b�N�X��
		index,						// �C���f�b�N�X�i�[�������擪�A�h���X
		&m_indexbuffer				// �C���f�b�N�X�o�b�t�@
	);
	if (!sts) {
		MessageBox(nullptr, "CreateIndexBuffer error", "error", MB_OK);
		return false;
	}

	return true;
}

// �`��
void Quad::Draw() {
	// �f�o�C�X�R���e�L�X�g���擾
	ID3D11DeviceContext* devcontext;
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// ���W�ϊ��p�̍s����Z�b�g
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, m_worldmtx);

	unsigned int stride = sizeof(Vertex);	// �X�g���C�h���Z�b�g�i�P���_������̃o�C�g���j
	unsigned  offset = 0;						// �I�t�Z�b�g�l���Z�b�g

	// ���_�o�b�t�@���f�o�C�X�R���e�L�X�g�փZ�b�g
	devcontext->IASetVertexBuffers(
		0,									// �X�^�[�g�X���b�g
		1,									// ���_�o�b�t�@��
		m_vertexbuffer.GetAddressOf(),		// ���_�o�b�t�@�̐擪�A�h���X
		&stride,							// �X�g���C�h
		&offset);							// �I�t�Z�b�g

	// �C���f�b�N�X�o�b�t�@���f�o�C�X�R���e�L�X�g�փZ�b�g
	devcontext->IASetIndexBuffer(
		m_indexbuffer.Get(),				// �C���f�b�N�X�o�b�t�@
		DXGI_FORMAT_R32_UINT,				// �t�H�[�}�b�g
		0);									// �I�t�Z�b�g

	// �g�|���W�[���Z�b�g
	devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ���_�V�F�[�_�[�A�s�N�Z���V�F�[�_�[�擾
	ID3D11VertexShader* vs;
	vs = ShaderHashmap::GetInstance()->GetVertexShader(vsfilename[0]);
	ID3D11PixelShader* ps;
	ps = ShaderHashmap::GetInstance()->GetPixelShader(psfilename[0]);

	// ���_���C�A�E�g�擾
	ID3D11InputLayout* layout;
	layout = ShaderHashmap::GetInstance()->GetVertexLayout(vsfilename[0]);

	devcontext->VSSetShader(vs, nullptr, 0);
	devcontext->GSSetShader(nullptr, nullptr, 0);
	devcontext->HSSetShader(nullptr, nullptr, 0);
	devcontext->DSSetShader(nullptr, nullptr, 0);
	devcontext->PSSetShader(ps, nullptr, 0);

	// ���_�t�H�[�}�b�g���Z�b�g
	devcontext->IASetInputLayout(layout);

	// �h���[�R�[�����s
	devcontext->DrawIndexed(
		4,						// �C���f�b�N�X��
		0,						// �J�n�C���f�b�N�X
		0);						// ����_�C���f�b�N�X
}


// �`��
void Quad::DrawNoTex() {
	// �f�o�C�X�R���e�L�X�g���擾
	ID3D11DeviceContext* devcontext;
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// ���W�ϊ��p�̍s����Z�b�g
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, m_worldmtx);

	unsigned int stride = sizeof(Vertex);	// �X�g���C�h���Z�b�g�i�P���_������̃o�C�g���j
	unsigned  offset = 0;						// �I�t�Z�b�g�l���Z�b�g

	// ���_�o�b�t�@���f�o�C�X�R���e�L�X�g�փZ�b�g
	devcontext->IASetVertexBuffers(
		0,									// �X�^�[�g�X���b�g
		1,									// ���_�o�b�t�@��
		m_vertexbuffer.GetAddressOf(),		// ���_�o�b�t�@�̐擪�A�h���X
		&stride,							// �X�g���C�h
		&offset);							// �I�t�Z�b�g

	// �C���f�b�N�X�o�b�t�@���f�o�C�X�R���e�L�X�g�փZ�b�g
	devcontext->IASetIndexBuffer(
		m_indexbuffer.Get(),				// �C���f�b�N�X�o�b�t�@
		DXGI_FORMAT_R32_UINT,				// �t�H�[�}�b�g
		0);									// �I�t�Z�b�g

	// �g�|���W�[���Z�b�g
	devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ���_�V�F�[�_�[�A�s�N�Z���V�F�[�_�[�擾
	ID3D11VertexShader* vs;
	vs = ShaderHashmap::GetInstance()->GetVertexShader(vsfilename[0]);
	ID3D11PixelShader* ps;
	ps = ShaderHashmap::GetInstance()->GetPixelShader(psfilename[1]);

	// ���_���C�A�E�g�擾
	ID3D11InputLayout* layout;
	layout = ShaderHashmap::GetInstance()->GetVertexLayout(vsfilename[0]);

	devcontext->VSSetShader(vs, nullptr, 0);
	devcontext->GSSetShader(nullptr, nullptr, 0);
	devcontext->HSSetShader(nullptr, nullptr, 0);
	devcontext->DSSetShader(nullptr, nullptr, 0);
	devcontext->PSSetShader(ps, nullptr, 0);

	// ���_�t�H�[�}�b�g���Z�b�g
	devcontext->IASetInputLayout(layout);

	// �h���[�R�[�����s
	devcontext->DrawIndexed(
		4,						// �C���f�b�N�X��
		0,						// �J�n�C���f�b�N�X
		0);						// ����_�C���f�b�N�X
}

void Quad::UnInit() {

}

// �g��A�k��
void Quad::SetScale(float sx, float sy, float sz) {
	DX11MtxScale(sx, sy, sz, m_worldmtx);
}

// �ʒu���Z�b�g
void Quad::SetPosition(float x, float y, float z) {
	m_worldmtx._41 = x;
	m_worldmtx._42 = y;
	m_worldmtx._43 = z;
}

// Z����]
void Quad::SetRotation(float angle) {
	DX11MtxRotationZ(angle, m_worldmtx);
}

// ���_�f�[�^�X�V
void Quad::updateVertex(uint32_t width, uint32_t height, DirectX::XMFLOAT4 color, DirectX::XMFLOAT2* uv,float z) {

	m_width = static_cast<float>(width);
	m_height = static_cast<float>(height);

	Vertex	v[4] = {
		// ���W													// �J���[	// UV	
		XMFLOAT3(	-m_width / 2.0f,	-m_height / 2.0f, z),	color,		uv[0],
		XMFLOAT3(	 m_width / 2.0f,	-m_height / 2.0f, z),	color,		uv[1],
		XMFLOAT3(	-m_width / 2.0f,	 m_height / 2.0f, z),	color,		uv[2],
		XMFLOAT3(	 m_width / 2.0f,	 m_height / 2.0f, z),	color,		uv[3]
	};

	for (int i = 0; i < 4; i++) {
		m_vertex[i] = v[i];
	}
}

// ���_�o�b�t�@�X�V
void Quad::updateVbuffer() {
	D3D11_MAPPED_SUBRESOURCE pData;
	ID3D11DeviceContext* devcontext;
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	HRESULT hr = devcontext->Map(m_vertexbuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	if (SUCCEEDED(hr)) {
		memcpy_s(pData.pData, pData.RowPitch, (void*)(m_vertex), sizeof(Vertex) * 4);
		devcontext->Unmap(m_vertexbuffer.Get(), 0);
	}
}