varying float depth;
uniform bool render_depth;

uniform sampler2D sunTex;

void main() {
    if (render_depth) {
        gl_FragColor = vec4(depth, depth, depth, 1.0);
    } else {
        gl_FragColor = vec4(1.7*texture2D(sunTex, gl_TexCoord[0].st).rgb, 1.0);
    }
    // gl_FragColor = vec4(depth,depth,depth,1);
}
