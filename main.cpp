#if _MSC_VER >= 1600  /* 1600 is Microsoft Visual Studio 2010 */
#pragma execution_character_set("utf-8")
#endif

// Std
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <codecvt>
#include <locale>
// GLAD
#include <glad/glad.h> 
// GLFW
#include <GLFW/glfw3.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// Freetype
#include <ft2build.h>
#include FT_FREETYPE_H  

#include "shader.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const GLuint WIDTH = 800, HEIGHT = 600;

struct Character {
    GLuint TextureID;   	// ID handle of the glyph texture
    glm::ivec2 Size;    	// Size of glyph
    glm::ivec2 Bearing;  	// Offset from baseline to left/top of glyph
    GLuint Advance;    		// Horizontal offset to advance to next glyph
};

std::map<unsigned long, Character> Characters;
GLuint VAO, VBO;

void RenderText(Shader &shader, const char text[], GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

int main(void) {
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	GLFWwindow* window = glfwCreateWindow(800, 600, "Lightning Vault", NULL, NULL);
	if(window == NULL){
		std::cout << "Failed to open GLFW window.\n" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

	glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Freetype
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	FT_Face face;
	// if (FT_New_Face(ft, "fonts/SourceHanSerifSC/SourceHanSerifSC-Regular.otf", 0, &face))
	// 	std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;  

    if (FT_New_Face(ft, "./fonts/SourceHanSerifSC/SourceHanSerifSC-Regular.otf", 0, &face))
	    std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;  

	Shader textShader("shaders/text.vs", "shaders/text.fs");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(WIDTH), 0.0f, static_cast<GLfloat>(HEIGHT));
	textShader.use();
	textShader.setMat4("projection", projection);

    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    const auto str = u8"😀⚡😀闪电Lightning Vaulttest";
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
    auto str32 = cv.from_bytes(str);

    for(auto c : str32){
        std::cout << uint_least32_t(c) << '\n';
        FT_UInt  glyph_index = FT_Get_Char_Index(face, uint_least32_t(c));

        if (FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT)) {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        /* convert to an anti-aliased bitmap */
        if (FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL)) {
            std::cout << "ERROR::FREETYTPE: Failed to Render Glyph" << std::endl;
            continue;
        }

        // if (face->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_BGRA)
        //     std::cout << "glyph is colored";
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<unsigned long, Character>(uint_least32_t(c), character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // Configure VAO/VBO for texture quads
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Game loop
    while (!glfwWindowShouldClose(window)) {
        // Check and call events
        glfwPollEvents();

        // Clear the colorbuffer
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        RenderText(textShader, u8"Lightning Vault", 25.0f, 25.0f, 1.0f, glm::vec3(0.2, 0.0f, 0.8f));
		RenderText(textShader, u8"😀闪电⚡Lightning", 0.0f, 250.0f, 1.0f, glm::vec3(0.2, 0.0f, 0.8f));
        RenderText(textShader, u8"test", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void RenderText(Shader &shader, const char text[], GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
    auto str32 = cv.from_bytes(text);

    // Activate corresponding render state	
    shader.use();
    //glUniform3f(glGetUniformLocation(shader.Program, "textColor"), color.x, color.y, color.z);
	shader.setVec3("textColor", color.x, color.y, color.z);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // Iterate through all characters
    //std::wstring::const_iterator c;
    //for (int i = 0; i < sizeof(text) / sizeof(const char*); ++i) {

    for(auto c : str32) {
        Character ch = Characters[uint_least32_t(c)];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },            
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }           
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}