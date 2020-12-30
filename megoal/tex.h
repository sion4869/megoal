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

//// 頂点データ
//struct Vertex {
//	DirectX::XMFLOAT3 pos;		// 座標		
//	DirectX::XMFLOAT4 color;	// カラー
//	DirectX::XMFLOAT2 tex;		// テクスチャ
//};

class Tex {
public:
	// 矩形の初期化
	bool Init(DirectX::XMFLOAT2* localpos,DirectX::XMFLOAT4 color, DirectX::XMFLOAT2* uv,float z=0.0f);

	// 描画
	void Draw();

	// テクスチャなしで描画
	void DrawNoTex();

	void UnInit();

	// 拡大、縮小
	void SetScale(float sx, float sy, float sz);

	// 位置をセット
	void SetPosition(float x, float y, float z);

	// Z軸回転
	void SetRotation(float angle);

	// 矩形の進行方向を取得（元画像が上向きに作られていることを前提）
	DirectX::XMFLOAT3 GetForward() {
		DirectX::XMFLOAT3 forward;
		forward.x = m_worldmtx._21;
		forward.y = m_worldmtx._22;
		forward.z = m_worldmtx._23;
		return forward;
	}

	// 矩形の横方向を取得
	DirectX::XMFLOAT3 GetSide() {
		DirectX::XMFLOAT3 side;
		side.x = m_worldmtx._11;
		side.y = m_worldmtx._12;
		side.z = m_worldmtx._13;
		return side;
	}

	// ワールド変換行列をセットする
	void SetMtx(DirectX::XMFLOAT4X4 mtx) {
		m_worldmtx = mtx;
	}

	// 頂点データ更新
	void updateVertex(DirectX::XMFLOAT4 color, DirectX::XMFLOAT2* uv, float z = 0.0f);

	// 頂点バッファ更新
	void updateVbuffer();
	//ローカル頂点移動
	void MoveVertex(XMFLOAT2 fix);

private:
	DirectX::XMFLOAT4X4		m_worldmtx;				// ワールド変換行列
	ComPtr<ID3D11Buffer>	m_vertexbuffer;			// 頂点バッファ
	ComPtr<ID3D11Buffer>	m_indexbuffer;			// インデックスバッファ
	Vertex		        	m_vertex[4];			// 矩形４頂点
	XMFLOAT2				m_localpos[4];			//ローカルな４頂点位置
};