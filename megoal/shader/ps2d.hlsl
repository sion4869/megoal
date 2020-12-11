Texture2D g_Tex : register(t0); // テクスチャ
SamplerState g_SamplerLinear : register(s0); // サンプラー


cbuffer _cb0 : register(b0)
{
    float2 pos;
    float2 scale;
    float r, g, b, a;
    float u0, v0, u3, v3;
};


float4 ps_main(float4 pos : SV_Position, float2 uv : TEXCOORD) : SV_Target
{
    float4 diffuse = float4(r, g, b, a);
    
    return g_Tex.Sample(g_SamplerLinear, uv) * diffuse;
}