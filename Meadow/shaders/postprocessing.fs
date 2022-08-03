#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

uniform bool negate;
uniform bool grayscale;
uniform bool sharpen;

// Kernel pixel offset
const float offsetx = 1.0 / 1920.0;
const float offsety = 1.0 / 1080.0;

void getKernelOffsets(inout vec2[9] arr);
vec4 calculateKernel(float kernel[9],  vec2 offsets[9]);
void main()
{
    // Sample fragment color from texture
    vec4 outCol = texture(screenTexture, TexCoords);

    // kernel and offset matrix
    float kernel[9];
    vec2 offsets[9];
    getKernelOffsets(offsets);
    bool kernelUsed = false;

    // Do post processing based on flags
    if (sharpen){
        kernel = float[](
          0,  -1,   0,
         -1,   5,  -1,
          0,  -1,   0
        );
        kernelUsed = true;
    }

    if (kernelUsed){
        outCol = calculateKernel(kernel, offsets);
    }

    if (negate)
        outCol = 1 - outCol;

    if (grayscale){
        float average = 0.2126 * outCol.r + 0.7152 * outCol.g + 0.0722 * outCol.b;
        outCol = vec4(average, average, average, 1.0);
    }

    // Gamma correction
    // -----------------
    float gamma = 2.2;
    outCol.rgb = pow(outCol.rgb, vec3(1.0/gamma));

    // -----------------

    // Ouput result
    FragColor = vec4(outCol.rgb,1.0);
}

void getKernelOffsets(inout vec2[9] arr){
    arr = vec2[9](
        vec2(-offsetx,  offsety), // top-left
        vec2( 0.0,      offsety), // top-center
        vec2( offsetx,  offsety), // top-right
        vec2(-offsetx,  0.0),   // center-left
        vec2( 0.0,      0.0),   // center-center
        vec2( offsetx,  0.0),   // center-right
        vec2(-offsetx,  -offsety), // bottom-left
        vec2( 0.0,      -offsety), // bottom-center
        vec2( offsetx,  -offsety)  // bottom-right    
    );

}
vec4 calculateKernel(float kernel[9],  vec2 offsets[9]){
    vec3 sampleTex[9];
        for(int i = 0; i < 9; i++)
        {
            sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
        }
        vec3 col = vec3(0.0);
        for(int i = 0; i < 9; i++)
            col += sampleTex[i] * kernel[i];
        return vec4(col, 1.0);
}