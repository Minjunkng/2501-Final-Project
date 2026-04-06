#version 130

in vec2 uv_interp;

uniform sampler2D onetex;
uniform int tile_texture;
uniform float tile_factor;
uniform int ghost;

void main()
{
    vec2 uv = uv_interp;

    if (tile_texture == 1) {
        uv *= tile_factor;
    }

    vec4 color = texture2D(onetex, uv);

    if (ghost == 1) {
        // Convert to grayscale using luminance
        float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));
        color = vec4(gray, gray, gray, color.a);
    }

    gl_FragColor = color;

    if (color.a < 1.0) {
        discard;
    }
}