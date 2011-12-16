uniform sampler2D tex;
uniform sampler2D depthTex;

uniform float width;
uniform float height;

void main(void) { 
    vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
    float depth = texture2D(depthTex, gl_TexCoord[0].st).r;
    depth = tanh(3*depth);
    int size = int(5.0 - 5.0*depth);

    int arraySize = 2*size+1;
    arraySize *= arraySize;
    int x, y;
    for (y = -size; y <= size; y++) {
        for (x = -size; x <= size; x++) {
            color.rgb += 1.0/arraySize * texture2D(tex, vec2(x/width, y/height) + gl_TexCoord[0].st).rgb;
        }
    }

    gl_FragColor = color;
}
