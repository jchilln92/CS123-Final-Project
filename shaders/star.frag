uniform sampler2D sunTex;

void main() {
    gl_FragColor = texture2D(sunTex, gl_TexCoord[0].st);
}
