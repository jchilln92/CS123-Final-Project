uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D tex4;

uniform float tex1_min, tex1_max;
uniform float tex2_min, tex2_max;
uniform float tex3_min, tex3_max;
uniform float tex4_min, tex4_max;

uniform bool has_water;

varying float height;
varying float intensity;

varying float depth;

vec4 sampleTextures() {
    float tex1_rng = tex1_max-tex1_min;
    float tex1_w = max(0.0,(tex1_rng-abs(height-tex1_max))/tex1_rng);

    float tex2_rng = tex2_max-tex2_min;
    float tex2_w = max(0.0,(tex2_rng-abs(height-tex2_max))/tex2_rng);

    float tex3_rng = tex3_max-tex3_min;
    float tex3_w = max(0.0,(tex3_rng-abs(height-tex3_max))/tex3_rng);

    float tex4_rng = tex4_max-tex4_min;
    float tex4_w = max(0.0,(tex4_rng-abs(height-tex4_max))/tex4_rng);

    if (has_water) {
        if (height <= 0.0) {
            tex1_w = 1.0;
            tex2_w = tex3_w = tex4_w = 0.0;
        } else {
            tex1_w = 0.0;
        }
    }

    vec4 sample = tex1_w*texture2D(tex1, gl_TexCoord[0].st * 6.0);
    sample += tex2_w*texture2D(tex2, gl_TexCoord[0].st * 6.0);
    sample += tex3_w*texture2D(tex3, gl_TexCoord[0].st * 6.0);
    sample += tex4_w*texture2D(tex4, gl_TexCoord[0].st * 6.0);

    //return vec4(tex1_w,tex2_w,tex3_w,tex4_w);
    return sample;

}

void main() {
    gl_FragColor = vec4((sampleTextures() * intensity).rgb,depth);
    // gl_FragColor = vec4(depth,depth,depth,1);
}
