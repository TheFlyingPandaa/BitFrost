Texture2D txDiffuse : register(t0);
SamplerState sampAni;

Texture2D gTexDiffuse : register(t1);
Texture2D gTexNormal : register(t2);
Texture2D gTexPosition : register(t3);
Texture2D gTexShadow : register(t4);

struct Light
{
    float3 dir;
	float pad;
    float4 ambient;
    float4 diffuse;
	float3 position;
};

cbuffer PER_FRAME_BUFF : register(b1)
{
    Light light;
    //float4x4 view;
};

cbuffer CAMERA_BUFFERT : register(b2)
{
	float4 cameraPosition;
};

struct VS_OUT
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD;
    float3 Normal : NORMAL;
	//float4x4 mulMatrix;
};

float4 ambientLight = (1.0f, 1.0f, 1.0f, 1.0f);

struct Material
{
	float Ka, Kd, Ks, A;
};

float4 calcPhongLighting(Material M, float4 LColor, float3 N, float3 L, float3 V, float3 R)
{
	float4 Ia = M.Ka * ambientLight;
	float4 Id = M.Kd * saturate(dot(N, L));
	float4 Is = M.Ks * pow(saturate(dot(R, V)), M.A);

	return Ia + (Id + Is) * LColor;
}

float4 main(VS_OUT input) : SV_Target
{

	
	float4 lightCol = (1.0f,1.0f,1.0f,1.0f);
	Material mat;
	mat.Ka = 0.1f;
	mat.Kd = 0.5f;
	mat.Ks = 0.5f;
	mat.A = 30;

    float4 pOut;
    float3 normal = gTexNormal.Sample(sampAni, input.Tex).rgb;
    float3 diffuse = gTexDiffuse.Sample(sampAni, input.Tex).rgb;
	float3 shadow = gTexShadow.Sample(sampAni, input.Tex).rgb;
	float3 position = gTexPosition.Sample(sampAni, input.Tex).rgb;
	normal = normalize(normal);
	float3 view = normalize(float3(1,1,1) - position);

	float r = reflect(light.dir, normal);

	pOut = calcPhongLighting(mat, lightCol, normal, -light.dir, view,r);

	return float4(position, 1.0f);

  //  
  //      float3 lightDir = normalize(light.dir);
  //  if (length(normal) > 0.0f && false)
  //  {
  //     
  //      float3 position = gTexPosition.Sample(sampAni, input.Tex).rgb; //float3(1, 1, 1); //float3(view._41, view._42, view._43);
  //  
  //      float lambertian = max(dot(lightDir, normal), 0.0f);
  //      float specular = 0.0f;

		//[flatten]
  //      if (lambertian > 0.0f)
  //      {
  //          float3 viewDir = normalize(-position);
  //          float3 halfDir = normalize(lightDir + viewDir);
  //          float specAngle = max(dot(halfDir, normal), 0.0f);
  //          specular = pow(specAngle, 30.0f);
  //      }
  //      float3 colorLinear = lambertian * diffuse + specular * float3(1.0f, 1.0f, 1.0f);
  //      pOut = float4(pow(colorLinear, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f)), 1.0f);
  //      return pOut;
  //  }

	/*float3 finalColor;

    finalColor = diffuse * light.ambient;
    finalColor += saturate(dot(light.dir, normal) * light.diffuse * diffuse);
    
    return float4(finalColor, 1.0f);*/

    /*pOut = float4(normal, 1.0f);
    return pOut;*/
}