Texture2D		g_Tex : register(t0);					// テクスチャ
SamplerState	g_SamplerLinear : register(s0);	// サンプラー

cbuffer ConstantBufferWorld : register(b0)		// ワールド変換行列
{
	matrix World;
}

cbuffer ConstantBufferViewPort : register(b5)
{
	uint4	ViewportWidth;						// ビューポート幅
	uint4	ViewportHeight;						// ビューポート高さ
};

// 頂点シェーダーの出力データ
struct VS_OUTPUT {
	float4 Position : SV_POSITION;
	float4 Color	: COLOR;
	float2 Tex		: TEXCOORD;
};