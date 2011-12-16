varying float depth;
uniform bool render_depth;

void main() {
    gl_Position = ftransform();

    depth = max(1.0-abs(gl_Position.z-10.0)/10.0,0.0);
    depth *= depth;

    gl_TexCoord[0] = gl_MultiTexCoord0;
}
