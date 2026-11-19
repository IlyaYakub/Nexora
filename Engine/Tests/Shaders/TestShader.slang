[shader("vertex")]
float4 vertexMain(uint vertexId : SV_VertexID) : SV_Position
{
    return float4(float(vertexId), 0.0, 0.0, 1.0);
}
