

matrix World;
matrix View;
matrix Projection;


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
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
	}
}