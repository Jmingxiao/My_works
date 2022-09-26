$input o_viewDir,o_uv, o_normal,o_tangent, o_bitangent 

#include "../common/common.sh"
#define PI 3.1415926

SAMPLER2D(texColor,  0);
SAMPLER2D(texNormal, 1);
SAMPLER2D(texaorm, 2);

SAMPLERCUBE(skyboxIrr, 3);
SAMPLERCUBE(skybox, 4);
SAMPLER2D(Lut, 5);

uniform vec4 light;

float NDF(float nh,float roughness){
    float a2 = roughness*roughness;
    float ud = pow(nh*nh*(a2-1.0)+1.0,2.0);
    return a2/(ud*PI);
}
vec3 fresnelTerm(vec3 F0, float cosa){
    float t = pow(1.0-cosa,5.0);
    return F0+(1.0-F0)*t;
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    float oneminusr = 1.0 - roughness;
    return F0 + (max(vec3(oneminusr,oneminusr,oneminusr), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}  

float geomartyfactor(float nl, float nv,float roughness){
    float a =roughness;
    float k = pow(a+1.0,2.0)/8.0;
    float gl = nl/mix(nl,1.0,k);
    float gr = nv/mix(nv,1.0,k);
    return gl*gr;
}

void main()
{
    
    vec3 lightDir =normalize(light.xyz);
    vec3 lightColor = vec3(light.a,light.a,light.a);

    vec3 diffuseColor =vec3(0.0,0.0,0.0);
    vec3 specColor =vec3(0.0,0.0,0.0);

    //prepare basic 
    vec2 uv = vec2(o_uv.x,-o_uv.y);
    vec3 albedo = texture2D(texColor,uv).xyz;
	vec3 normalTS = texture2D(texNormal,uv).xyz*2.0-1.0;
    mat3 tbn = mtxFromCols(o_tangent, o_bitangent, o_normal);
    vec3 normalWS = normalize(mul(tbn,normalTS));
    vec3 viewDir = normalize(o_viewDir);
    vec3 aorm = texture2D(texaorm,uv).xyz;
    float ao = aorm.x;
    float roughness = aorm.y;
    float metallic = aorm.z;


    float proughness = sqrt(roughness);
    
	float nv = max(saturate(dot(normalWS, viewDir)), 0.000001);
    vec3 F0 = mix(vec3(0.05,0.05,0.05),albedo,metallic);

    //prepare secondary
    vec3 halfVector = normalize(lightDir + viewDir);
    float nl = max(saturate(dot(normalWS, lightDir)), 0.000001);
	float vh = max(saturate(dot(viewDir, halfVector)), 0.000001);
	float lh = max(saturate(dot(lightDir, halfVector)), 0.000001);
	float nh = max(saturate(dot(normalWS, halfVector)), 0.000001);

    //directlight diffuse 
    float Fd90 = 0.5 + 2.0*proughness*lh*lh;
    float lightscatter = (1.0+(Fd90-1.0)*pow(1.0-nl,5.0));
    float viewscatter = (1.0+(Fd90-1.0)*pow(1.0-nv,5.0));
    float fd = lightscatter*viewscatter;
    

    //directlight spec
    float D = NDF(nh,roughness);
    float G = geomartyfactor(nl,nv,roughness);
    
    
    vec3 F = fresnelTerm(F0,lh);
    vec3 kdDir = (vec3(1.0,1.0,1.0)-F)*(1.0-metallic);
    vec3 spec = (D*G*F*0.25)/(nv*nl);
    
    specColor += lightColor*spec*nl*PI;
    diffuseColor += albedo*fd*ao*nl*lightColor*kdDir;

    vec3 directresult  = specColor+diffuseColor;

    vec3 froughness = fresnelSchlickRoughness(nv,F0,roughness);
    vec3 kd = (1.0 - froughness)*(1.0-metallic);
    vec3 irradiance = max(vec3(0.0,0.0,0.0),toLinear(textureCube(skyboxIrr, normalWS).rgb));
    vec3 ibldiffuse = irradiance*kd*albedo;

    //microfacet based brdf
    float m = roughness*roughness;
    const float fEps = 1.192092896e-07F;
    const float mipcount = 6.0;
    float n =  (2.0 / max(fEps, m * m)) - 2.0;
    n /= 4.0;
    float mip_roughness = pow( 2.0 / (n + 2.0), 0.25);

    float mip = mip_roughness*mipcount;

    vec3 reflectdir = -reflect(-viewDir,normalWS);


    vec3 radience  = toLinear(textureCubeLod(skybox, reflectdir, mip).rgb);
    vec2 envBDRF = texture2D(Lut, vec2(mix(0.0, 0.99 ,nv),mix(0.0, 0.99, roughness))).rg; 

    vec3 iblspec = radience*(froughness* envBDRF.r + envBDRF.g);

    vec3 indirectresult = (iblspec+ibldiffuse) *ao;

    vec3 color = indirectresult+directresult;

    color = toFilmic(color);

	gl_FragColor = vec4(color,1);
	
}
