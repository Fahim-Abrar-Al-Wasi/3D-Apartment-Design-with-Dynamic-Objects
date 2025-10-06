#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D mirrorTexture;

void main() {
    FragColor = texture(mirrorTexture, TexCoord);
}
