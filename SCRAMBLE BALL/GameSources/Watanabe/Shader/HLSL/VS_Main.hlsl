float4 main(uint id : SV_VertexID) : SV_POSITION
{
    // Calculate the UV (0,0) to (2,2) via the ID
    float2 uv = float2(
		(id << 1) & 2, // id % 2 * 2
		id & 2);

	// Adjust the position based on the UV
    float4 position = float4(uv, 0, 1);
    position.x = position.x * 2 - 1;
    position.y = position.y * -2 + 1;

    return position;
}