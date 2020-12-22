#pragma once

#include <d3d11.h>

//2Dの一枚四角形オブジェクトを表す

class Sprite2D
{
public:
	~Sprite2D();
	void Update(void);
	void Draw2(void);    //この2D一枚ポリゴンを描画する

	HRESULT Initialize(void);   //このクラスのオブジェクトごとに必要な初期化を行う
	HRESULT LoadTexture(const char* const pTexFileName);   //このポリゴンに使うテクスチャを読み込む


	static HRESULT CreateShader(void);   //2D表示用のシェーダーを作成する
	static void ReleaseShader(void);     //2Dシェーダーを開放する

	static ID3D11InputLayout* spIL;
	static ID3D11VertexShader* spVS;
	static ID3D11PixelShader* spPS;

	struct VECTOR2D {
		float x, y;
	} mPos, mScale;  //このポリゴンの左上座標、ポリゴンの拡大縮小

	struct COLOR {
		float r, g, b, a;
	} mColor;       //ポリゴンの頂点カラー

	struct UV {
		float u, v;
	} mUV0, mUV3;   //ポリゴンのUV座標  mUV0 = 左上  mUV3 = 右下

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

