#include <stdio.h>
#include <stdlib.h>
#include "fluid.h"
#include "renderer.h"
#include "font_gl.h"
#include <ft2build.h>
#include FT_FREETYPE_H

void create_shaders(FONT_T *state)
{
    // Compile vertex shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    #ifdef GLES
        compile_shader(vertex_shader, "SPH/font_es.vert");
    #else
        compile_shader(vertex_shader, "font.vert");
    #endif

    // Compile fragment shader
    GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    #ifdef GLES
        compile_shader(frag_shader, "SPH/font_es.frag");
    #else
        compile_shader(frag_shader, "font.frag");
    #endif

    // Create shader program
    state->program = glCreateProgram();
    glAttachShader(state->program, vertex_shader);
    glAttachShader(state->program ,fragment_shader);

    // Link and use program
    glLinkProgram(state->program);

    state->coord_location = glGetAttribLocation(state->program, "coord");

    // Enable blend
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void create_buffers(FONT_T *state)
{
    // VAO is required for OpenGL 3+ when using VBO I believe
    #ifndef GLES
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    #endif

    // Generate vertex buffer
    glGenBuffers(1, &state->vbo)

}

void create_texture(FONT_T *state)
{
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &state->tex);
    glBindTexture(GL_TEXTURE_2D, state->tex);
    glUniform1i(uniform_tex, 0);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Set single byte alignment
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

} 

void render_text(FONT_T *state, const char *text, float x, float y, float sx, float sy)
{
    // Setup environment
    glVertexAttribPointer(state->coord_location, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(state->coord_location);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, state->tex);
    glUniformli(state->tex_uniform, 0)

    // Render text
    const char *p;

    for(p = text; *p; p++) {
        if(FT_Load_Char(state->face, *p, FT_LOAD_RENDER))
            continue;

        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

        float x2 = x + g->bitmap_left * sx;
        float y2 = -y - g->bitmap_top * sy;
        float w = g->bitmap.width * sx;
        float h = g->bitmap.rows * sy;

        GLfloat box[4][4] = {
        {x2,   -y2,    0, 0},
        {x2+w, -y2,    1, 0},
        {x2,   -y2-h,  0, 1},
        {x2+w, -y2-h,  1, 1}
        };
  
        glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        x+=(g->advance.x >> 6) * sx;
        y+=(g->advance.y >> 6) * sy;
    }
}

void init_font(FONT_T *state, int screen_width, int screen_height)
{
    // Initialize FreeType library
    if(FT_Init_FreeType(&state->ft)) {
        printf("Error initializing FreeType library\n");
        exit(EXIT_FAILURE);
    }

    state->screen_width = screen_width;
    state->screen_height = screen_height;

    // Load font face
    if(FT_New_Face(state->ft, "SPH/DroidSerif-Regular.tff", 0, &state->face)) {
        printf("Error loading font face\n");
        exit(EXIT_FAILURE);
    }

    // Set pixel size
    FT_Set_Pixel_Sizes(state->face, 0, 48);

    state->g = state->face->glyph;
}

void render_fps(FONT_T *font_state, double fps)
{
    float dx, dy, lh;

    // Set font color
    GLfloat black[4] = {0, 0, 0, 1};
    glUniform4fv(state->color_uniform, 1, black);

    // dx,dy,lh is internally handled by fontstash in pixels...arg
    dx = font_state->screen_width/2.0 - 5.0*lh;
    dy = font_state->screen_height/2.0 - lh;

    // Buffer to create strings in
    char buffer[64];

    // Gravity
    sprintf( buffer, "FPS: %.2f", fps);
    fonsDrawText(font_state->fs, dx, dy, buffer, NULL); 

}

void render_parameters(FONT_T *font_state, parameters selected_param, double gravity, double viscosity, double density, double pressure, double elasticity)
{
/*
    // Render some text
    float dx, dy, lh;
    unsigned int color = glfonsRGBA(255,255,100,255);

    // Get font height
    fonsVertMetrics(font_state->fs, NULL, NULL, &lh);

    // dx,dy,lh is internally handled by fontstash in pixels...arg
    dx = -font_state->screen_width/2.0 + 10.0f;
    dy = font_state->screen_height/2.0 - lh;

    // Buffer to create strings in
    char buffer[100];

    // Add blur to parameters
    //unblur selected parameter below
    fonsSetBlur(font_state->fs, 2.0f);

    // Gravity
    if(selected_param == GRAVITY)
        fonsSetBlur(font_state->fs, 0);
    sprintf( buffer, "Gravity: %.1f", gravity);
    fonsDrawText(font_state->fs, dx, dy, buffer, NULL);
    fonsSetBlur(font_state->fs, 2.0f);

    // Viscocity
    if(selected_param == VISCOSITY)
        fonsSetBlur(font_state->fs, 0);
    sprintf( buffer, "Viscosity: %.1f", viscosity);
    dy -= lh;
    fonsDrawText(font_state->fs, dx, dy, buffer, NULL);
    fonsSetBlur(font_state->fs, 2.0f);

    // Density
    if(selected_param == DENSITY)
        fonsSetBlur(font_state->fs, 0);
    sprintf( buffer, "Density: %.1f", density);
    dy -= lh;
    fonsDrawText(font_state->fs, dx, dy, buffer, NULL);
    fonsSetBlur(font_state->fs, 2.0f);

    // Pressure
    if(selected_param == PRESSURE)
        fonsSetBlur(font_state->fs, 0);
    sprintf( buffer, "Pressure: %.1f", pressure);
    dy -= lh;
    fonsDrawText(font_state->fs, dx, dy, buffer, NULL);
    fonsSetBlur(font_state->fs, 2.0f);

    // Elasticity
    if(selected_param == ELASTICITY)
        fonsSetBlur(font_state->fs, 0);
    sprintf( buffer, "Elasticity: %.1f", elasticity);
    dy -= lh;
    fonsDrawText(font_state->fs, dx, dy, buffer, NULL);


    // Unset blur
    fonsSetBlur(font_state->fs, 0);  
*/
}

void remove_font(FONT_T *font_state)
{

}
