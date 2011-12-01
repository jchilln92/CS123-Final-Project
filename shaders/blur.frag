const int MAX_KERNEL_SIZE = 128;
uniform sampler2D tex;
uniform int arraySize;
uniform vec2 offsets[MAX_KERNEL_SIZE]; 
uniform float kernel[MAX_KERNEL_SIZE];
void main(void) { 
    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);

    int i;
    for (i = 0; i < arraySize; i++) {
        color += kernel[i] * texture2D(tex, offsets[i] + gl_TexCoord[0].st);
    }
    
    gl_FragColor = color;
}
