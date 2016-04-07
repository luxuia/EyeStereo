/////////////// Input Vector4f, Vector4f


matrix World;
matrix View;
matrix Projection;
float PointSize= 0.02;

struct VS_OUTPUT {
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
};


// Vertex Shader
VS_OUTPUT VS( float4 vPos	: POSITION,
		  float4 vcolor	: COLOR0 )
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	
	output.Pos = mul( vPos, World );
	output.Pos = mul( output.Pos, View );
	output.Pos = mul( output.Pos, Projection );

	output.Color = vcolor;
	return output;
}

[maxvertexcount(6)]
void GS( point VS_OUTPUT input[1], inout TriangleStream<VS_OUTPUT> triangleStream) {
	VS_OUTPUT output;
	output = input[0];
	float4 oldPos = input[0].Pos;
	//// first Triangle
	output.Pos.x = oldPos.x - PointSize;
	output.Pos.y = oldPos.y - PointSize;
	triangleStream.Append(output);

	output.Pos.x = oldPos.x - PointSize;
	output.Pos.y = oldPos.y + PointSize;
	triangleStream.Append(output);

	output.Pos.x = oldPos.x + PointSize;
	output.Pos.y = oldPos.y - PointSize;
	triangleStream.Append(output);

	triangleStream.RestartStrip();

	//// Second Triangle
	output.Pos.x = oldPos.x + PointSize;
	output.Pos.y = oldPos.y + PointSize;
	triangleStream.Append(output);

	output.Pos.x = oldPos.x - PointSize;
	output.Pos.y = oldPos.y + PointSize;
	triangleStream.Append(output);

	output.Pos.x = oldPos.x + PointSize;
	output.Pos.y = oldPos.y - PointSize;
	triangleStream.Append(output);
	triangleStream.RestartStrip();
}



// Pixel Shader

float4 PS( VS_OUTPUT input) : SV_Target
{
	return input.Color;
}

technique10 RenderScene
{
	pass p0
	{
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( CompileShader(gs_4_0, GS()) );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
	}
}
