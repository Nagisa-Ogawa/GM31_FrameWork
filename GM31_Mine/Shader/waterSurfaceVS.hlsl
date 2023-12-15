
#include "common.hlsl"


void main(in VS_IN In, out PS_IN Out)
{

    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    
    // ローカル座標のx,zから波の高さを計算
    float time = Param.time;
    float gravity = 9.8f;
    float waveRate = Param.waveRate;
    float waveLength = Param.waveLength;
    float twoPiPerLength = 2.0f * 3.14159f / waveLength;
    float2 waveDirection = Param.waveDirection;
    float2 dropPos = Param.dropPos;
    waveDirection = normalize(waveDirection);
    // float d = dot(waveDirection, In.Position.xz);
    float dropd = sqrt((In.Position.x - dropPos.x) * (In.Position.x - dropPos.x) + (In.Position.z - dropPos.y) * (In.Position.z - dropPos.y));
    float h = waveRate * waveLength / 14.0f * sin(twoPiPerLength * dropd - sqrt(twoPiPerLength * gravity) * time);
    In.Position.y = h;
    
    float4 worldNormal, normal;
    // normal = float4(In.Normal.xyz, 0.0);
    // 法線を算出
    float na = waveRate * waveLength / 14.0f;
    float nc = 2.0f * 3.14159f / waveLength;
    float nd = sqrt(nc * gravity) * time;
    // float cosV = cos(nc * dot(waveDirection, In.Position.xz) - nd);
    float cosV = cos(nc * dropd - nd);
    float dx = na * nc * waveDirection.x * cosV;
    float dz = na * nc * waveDirection.y * cosV;
    normal = normalize(float4(-dx, 1.0f, -dz, 0.0f));
    
    worldNormal = mul(normal, World);
    worldNormal = normalize(worldNormal);
    
    float light = dot(Light.Direction.xyz, worldNormal.xyz);
    light = saturate(light);
    
    Out.Diffuse = In.Diffuse * Material.Diffuse * light * Light.Diffuse;
    Out.Diffuse += In.Diffuse * Material.Ambient * Light.Ambient;
    Out.Diffuse += Material.Emission;
    Out.Diffuse.a = In.Diffuse.a * Material.Diffuse.a;

    Out.Position = mul(In.Position, wvp);
    Out.TexCoord = In.TexCoord;

}

