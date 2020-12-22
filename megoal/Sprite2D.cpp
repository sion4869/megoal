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
	// TODO: ここに実装コードを追加します.
}

void Sprite2D::Draw2(void)
{
	ID3D11DeviceContext*		 devicecontext;				// デバイスコンテキスト

	// デバイスコンテキストを取得する
	devicecontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	unsigned int stride = sizeof(VERTEX2D);					// ストライドをセット（１頂点当たりのバイト数）
	unsigned  offset = 0;									// オフセット値をセット

	// 頂点バッファをデバイスコンテキストへセット
	devicecontext->IASetVertexBuffers(
		0,													// スタートスロット
		1,													// 頂点バッファ個数
		&mpVetexBuffer,									// 頂点バッファの先頭アドレス
		&stride,											// ストライド
		&offset);											// オフセット

	// トポロジーをセット
	devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 頂点フォーマットをセット
	devicecontext->IASetInputLayout(spIL);

	devicecontext->VSSetShader(spVS, nullptr, 0);		// 頂点シェーダーをセット
	devicecontext->PSSetShader(spPS, nullptr, 0);		// ピクセルシェーダーをセット

	devicecontext->PSSetShaderResources(0, 1, &mpTexSRV);  // シェーダーがテクスチャにアクセスできるようにセット

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

	// Zバッファを無効にする
	CDirectXGraphics::GetInstance()->TurnOffZbuffer();

	// デバイスコンテキストに頂点データを流し込む
	devicecontext->Draw(
		4,									// 頂点数
		0);									// 開始頂点インデックス

	// Zバッファを有効にする
	CDirectXGraphics::GetInstance()->TurnOnZBuffer();
}

HRESULT Sprite2D::CreateShader(void)
{
	// デバイスを取得する
	ID3D11Device* device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// 頂点（レイアウト）データの定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "SV_POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// エレメント数
	unsigned int numElements = ARRAYSIZE(layout);

	// 頂点シェーダーオブジェクトを生成、同時に頂点レイアウトも生成
	bool sts = CreateVertexShader(
		device,								// デバイスオブジェクト						
		"shader/vs2d.hlsl", // 頂点シェーダーソースファイル					
		"vs_main",								// エントリ関数
		"vs_5_0",							// 頂点シェーダーモデル５．０
		layout,								// 頂点データレイアウト
		numElements,						// 頂点データのエレメント数
		&spVS,					// 頂点シェーダーオブジェクト
		&spIL);					// 頂点データレイアウトオブジェクト

	if (!sts) {
		MessageBox(nullptr, "CreateVertexShader error", "error", MB_OK);
		return E_FAIL;
	}

	// ピクセルシェーダーを生成
	sts = CreatePixelShader(			// ピクセルシェーダーオブジェクトを生成
		device,						// デバイスオブジェクト
		"shader/ps2d.hlsl",		// ピクセルシェーダーソースファイル
		"ps_main",						// エントリ関数
		"ps_5_0",					// ピクセルシェーダーモデル５．０
		&spPS);			// ピクセルシェーダーオブジェクト

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

	// 頂点バッファを生成
	bool sts = CreateVertexBuffer(
		device,						// デバイスオブジェクト
		sizeof(VERTEX2D),				// １頂点当たりバイト数
		4,							// 頂点数
		vx,							// 頂点データ格納メモリ先頭アドレス
		&mpVetexBuffer				// 頂点バッファ
	);

	if (!sts) {
		MessageBox(nullptr, "CreateVertexBuffer error", "error", MB_OK);
		return E_FAIL;
	}

	// 定数バッファを生成
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
