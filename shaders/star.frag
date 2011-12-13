varying float depth;

uniform sampler2D sunTex;

void main() {
    gl_FragColor = vec4(1.5*texture2D(sunTex, gl_TexCoord[0].st).rgb,depth);
    // gl_FragColor = vec4(depth,depth,depth,1);
}
