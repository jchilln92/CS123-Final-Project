const int MAX_KERNEL_SIZE = 128;
uniform sampler2D tex;
int arraySize;
vec2 offsets[MAX_KERNEL_SIZE];
float kernel[MAX_KERNEL_SIZE];

uniform float width;
uniform float height;

void setKernel(int size) {
    arraySize = 2*size+1;
    arraySize *= arraySize;
    int x,y;
    int idx = 0;
    for (y = -size; y <= size; y++) {
        for (x = -size; x <= size; x++) {
            idx++;
            kernel[idx] = 1.0/arraySize;
            offsets[idx].x = x / width;
            offsets[idx].y = y / height;
        }
    }
}

void main(void) { 
    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
    int size = int(5.0 - 5.0*texture2D(tex,gl_TexCoord[0].st).a);

    setKernel(size);

    int i;
    for (i = 0; i < arraySize; i++) {
        color += kernel[i] * texture2D(tex, offsets[i] + gl_TexCoord[0].st);
    }

    // gl_FragColor = color;
    // gl_FragColor = vec4(1.0,0.0,0.0,1.0);
    gl_FragColor = vec4(texture2D(tex,gl_TexCoord[0].st).a,texture2D(tex,gl_TexCoord[0].st).a,texture2D(tex,gl_TexCoord[0].st).a,1.0);
}
