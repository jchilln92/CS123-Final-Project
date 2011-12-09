varying float height;
varying float intensity;

varying vec4 norm;

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D tex4;

void main() {
    // gl_FragColor = intensity;
    gl_FragColor = texture2D(tex1, gl_TexCoord[0].st) * intensity;
    // gl_FragColor = norm;
    // gl_FragColor.rg = gl_TexCoord[0].st;
}
