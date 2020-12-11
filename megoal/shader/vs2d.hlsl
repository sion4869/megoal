struct VSOutput
{
	float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

cbuffer _cb0 : register(b0)
{
    float2 pos;
    float2 scale;
    float r, g, b, a;
    float u0, v0, u3, v3;
};

VSOutput vs_main(
	float4 inPos : SV_POSITION,
    uint index : SV_VertexID
)
{
    VSOutput output;
    
    output.pos.x = inPos.x * scale.x + pos.x;
    output.pos.y = inPos.y * scale.y + pos.y;
    output.pos.z = 0.0f;
    output.pos.w = 1.0f;
    
    switch (index)
    {
        case 0:
            output.uv.x = u0;
            output.uv.y = v0;
            break;
        case 1:
            output.uv.x = u3;
            output.uv.y = v0;
            break;
        case 2:
            output.uv.x = u0;
            output.uv.y = v3;
            break;
        case 3:
            output.uv.x = u3;
            output.uv.y = v3;
            break;
    }
    
    return output;
}