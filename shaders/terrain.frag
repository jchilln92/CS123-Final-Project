varying float height;
varying float intensity;

uniform sampler2D testTexture;

void main() {
    //gl_FragColor = texture2D(testTexture, gl_TexCoord[0].st) * intensity;
    gl_FragColor = (height+.1)/.2 * vec4(1.0, 1.0, 1.0, 1.0);
}
