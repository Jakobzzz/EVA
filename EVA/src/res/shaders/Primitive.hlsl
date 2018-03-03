/*
    Basic VS/PS shaders for a primitive
*/

struct VS_IN
{
    float4 position : POSITION;
};

struct VS_OUT
{
    float4 position : SV_Position;
};

cbuffer cbData : register(b0)
{
    float4x4 WVP;
}

VS_OUT VS_MAIN(VS_IN input)
{
    VS_OUT output;

    output.position = mul(input.position, WVP);
    return output;
}

float4 PS_MAIN(VS_OUT input) : SV_Target
{
    return float4(1.f, 0.f, 0.f, 1.f);
}