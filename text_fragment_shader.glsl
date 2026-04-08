#version 130

in vec4 color_interp;
in vec2 uv_interp;

uniform sampler2D onetex;

const int num_columns = 10;
const int num_rows = 9;

float char_width = 1.0 / float(num_columns);
float char_height = 1.0 / float(num_rows);

uniform int text_len;
uniform int text_content[40];

void main()
{
    if (text_len > 0) {

        int string_index = int(floor(uv_interp.x * float(text_len)));

        int ch = text_content[string_index];
        int char_offset = 0;

        if (ch >= 65 && ch <= 90) char_offset = -25;       // uppercase
        else if (ch >= 97 && ch <= 122) char_offset = 71;  // lowercase
        else if (ch >= 48 && ch <= 57) char_offset = -4;   // numbers
        else if (ch == 58) char_offset = -7;               // colon

        int char_index = ch - char_offset;

        int row = char_index / num_columns;
        int col = char_index - row * num_columns;

        vec2 cuv;
        cuv.x = uv_interp.x * float(text_len) - float(string_index);
        cuv.y = uv_interp.y;

        vec2 fuv;
        fuv.x = (cuv.x + float(col)) * char_width;
        fuv.y = (cuv.y + float(row)) * char_height;

        vec4 color = texture2D(onetex, fuv);
        gl_FragColor = color;

        if (color.a < 1.0) {
            discard;
        }

    } else {

        vec4 color = texture2D(onetex, uv_interp);
        gl_FragColor = color;

        if (color.a < 1.0) {
            discard;
        }
    }
}