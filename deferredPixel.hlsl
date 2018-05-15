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

cbuffer CAMERA_BUFFERT : register(b4)
{
	float4 cameraPosition;
    float4 cameraDirection;
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
	//normal = float3((normal.x * 2) + 1, (normal.y * 2) + 1, (normal.z * 2) + 1);
    float3 diffuse = gTexDiffuse.Sample(sampAni, input.Tex).rgb;
	float3 shadow = gTexShadow.Sample(sampAni, input.Tex).rgb;
	float3 position = gTexPosition.Sample(sampAni, input.Tex).rgb;
	normal = normalize(normal);
	//float3 view = normalize(float3(1,1,1) - position);

    //return float4(position, 1);

    //diffuse = diffuse + float3(0.2f, 0.2f, 0.2f);

    //return float4(normal.rgb,1);

	//float r = reflect(light.dir, normal);

	//pOut = calcPhongLighting(mat, lightCol, normal, -light.dir, view,r)

	//return float4(position, 1.0f);
    //float3 lightDir = normalize(float3(500, 500, 0) - position);

    //float3 finalColor;

    //finalColor = diffuse * light.ambient.xyz * 0.1f;
    //finalColor += saturate(dot(lightDir, normal) * light.diffuse.xyz * 0.1f  * diffuse);

    
    
    float3 posToCamera = normalize(cameraPosition.xyz - position); // Vector from position to camera
    float3 posToLight = normalize(float3(10,2,0) - position); // The light dir from position to light
                                        //LIGHT
    float3 diffusee = diffuse * max(dot(normal, posToLight), 0.5f); //calculate the diffuse factor
		
		
    float3 halfwayDir = normalize(posToLight + posToCamera); // Create a vector to use to get the specular level

		
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0); // Calculate the "size" of the shineness-reflection of the pixel
		
		// final specular
    float3 specular = spec * 0.8;
    //spec *= -1;
    return float4((specular + diffusee) * float3(1.0f, 1.0f, 1.0f), 1.0);








    /*
    if (length(normal) > 0.0f )
    {
       


    //float3 sunLightToObject = normalize(-sunDir.xyz);

    //float3 viewer = normalize(cameraPosition.xyz - wordPos);

    float3 halfWayDir;
	/*WWWHOOOOOOOOO WE'RE...*/
   // halfWayDir = normalize(sunLightToObject + viewer);

    //return float4(finalColor, 1.0f);
}