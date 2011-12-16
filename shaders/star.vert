varying float depth;

uniform float focus_depth;
uniform bool render_depth;

void main() {
    gl_Position = ftransform();

    depth = max(1.0-abs(gl_Position.z-focus_depth)/focus_depth,0.0);
    depth *= depth;

    gl_TexCoord[0] = gl_MultiTexCoord0;
}
