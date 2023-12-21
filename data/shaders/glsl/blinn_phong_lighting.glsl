
vec3 CalculateDiffuseComponent(const in vec3 L, const in vec3 N, const in vec3 Kd) {
  float diffuse_strength = max(dot(N, L), 0.0);
  return diffuse_strength * Kd;
}

float CalculateSpecularComponent(const in vec3 V, const in vec3 L, const in vec3 N, const in vec3 Ks, const in float shininess) {
  if(dot(N, L) > 0.0) {
    vec3 H = normalize(V + L);
    float specular_strength = pow(max(dot(H, N), 0.0), shininess);
    return specular_strength * Ks;
  }
  return vec3(0.0);
}

float CalculateAttenuation(const in float constant, const in float linear, const in float exponent, const in float D) {
  float attenuation = constant + linear * D + exponent * D * D;
  return max(1.0, attenuation);
}
