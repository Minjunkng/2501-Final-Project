#version 130

in vec4 color_interp;
in vec2 uv_interp;

uniform sampler2D onetex;
uniform float bar_length;

void main()
{
    vec4 color = vec4(1.0, 0.0, 0.0, 1.0);
    gl_FragColor = color;

    if (max(abs(uv_interp.x) / bar_length, abs(uv_interp.y)) > 0.3){
         discard;
    }
}