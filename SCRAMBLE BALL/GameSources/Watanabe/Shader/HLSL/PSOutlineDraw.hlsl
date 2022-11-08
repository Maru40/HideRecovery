#include "IncDraw.hlsli"

float4 main(PSOutlineInput input) : SV_TARGET
{
    return OutlineColor;
}