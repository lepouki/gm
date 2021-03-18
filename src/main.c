#define GLFW_INCLUDE_NONE
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stb/stb_image_write.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
  Status_Success,
  Status_GlfwInitFailed,
  Status_DummyWindowCreationFailed,
  Status_GlLoadingFailed,
  Status_ShaderCompilationFailed,
  Status_ProgramLinkingFailed,
  Status_FramebufferCreationFailed,
  Status_ImageOutputFailed
} Status;

Status Init();
void PrintStatus(FILE *stream, int status_code);

Status CheckShaderCompilationStatus(GLuint shader);
Status CheckProgramLinkStatus(GLuint program);

void Terminate();

const char *const kVShaderSource =
    "#version 330 core \n"
    "layout(location = 0) in vec2 a_Position;\n"
    "void main() {\n"
    "  gl_Position = vec4(a_Position, 1.0, 1.0);\n"
    "}\n";

const char *const kFShaderSource =
    "#version 330 core\n"
    "out vec4 f_Color;\n"
    "void main() {\n"
    "  f_Color = vec4(1.0);\n"
    "}\n";

const int kImageWidth = 500;
const int kImageHeight = 500;

#ifndef NDEBUG
#  define PRINT_GL_ERROR_STATUS(info) \
    fprintf(stdout, "Error status: %d (%s)\n", glGetError(), info)
#else
#  define PRINT_GL_ERROR_STATUS(...)
#endif

int main() {
  // Initialize the context.
  const Status kInitStatus = Init();

  if (kInitStatus) {
    PrintStatus(stderr, kInitStatus);
    return kInitStatus;
  } else {
    PrintStatus(stdout, kInitStatus);
  }

  // Create the shaders.
  GLuint v_shader = glCreateShader(GL_VERTEX_SHADER);
  GLuint f_shader = glCreateShader(GL_FRAGMENT_SHADER);
  PRINT_GL_ERROR_STATUS("create shaders");

  glShaderSource(v_shader, 1, &kVShaderSource, NULL);
  glShaderSource(f_shader, 1, &kFShaderSource, NULL);
  PRINT_GL_ERROR_STATUS("shader sources");

  glCompileShader(v_shader);
  glCompileShader(f_shader);
  PRINT_GL_ERROR_STATUS("compile shaders");

  if (CheckShaderCompilationStatus(v_shader) ||
      CheckShaderCompilationStatus(f_shader)) {
    return Status_ShaderCompilationFailed;
  }

  // Create the program.
  GLuint program = glCreateProgram();
  PRINT_GL_ERROR_STATUS("create program");

  glAttachShader(program, v_shader);
  glAttachShader(program, f_shader);
  PRINT_GL_ERROR_STATUS("attach shaders");

  glLinkProgram(program);
  PRINT_GL_ERROR_STATUS("link program");

  if (CheckProgramLinkStatus(program)) {
    return Status_ProgramLinkingFailed;
  }

  // Now that the program is linked, delete the shaders.
  glDeleteShader(v_shader);
  glDeleteShader(f_shader);
  PRINT_GL_ERROR_STATUS("delete shaders");

  // Create the quad data.
  // clang-format off
  const GLfloat kVertices[] = {-1.0f, -1.0f,
                               -1.0f,  1.0f,
                                1.0f,  1.0f,
                                1.0f, -1.0f};

  const GLshort kIndices[] = {0, 1, 3,
                              1, 2, 3};
  // clang-format on

  // Create the quad vertex array.
  GLuint quad_va;
  glGenVertexArrays(1, &quad_va);
  glBindVertexArray(quad_va);

  // Create the quad buffers.
  GLuint buffers[2];
  glGenBuffers(2, buffers);
  PRINT_GL_ERROR_STATUS("gen buffers");

  // Send the data to the buffers.
  glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(kVertices), kVertices, GL_STATIC_DRAW);
  PRINT_GL_ERROR_STATUS("vertex data");

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(kIndices), kIndices,
               GL_STATIC_DRAW);
  PRINT_GL_ERROR_STATUS("index data");

  // Vertex layout.
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
  PRINT_GL_ERROR_STATUS("vertex attrib");

  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Create and initialize the framebuffer renderbuffer.
  GLuint image_buf;
  glGenRenderbuffers(1, &image_buf);
  glBindRenderbuffer(GL_RENDERBUFFER, image_buf);
  PRINT_GL_ERROR_STATUS("create image buf");

  glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB8, kImageWidth, kImageHeight);
  PRINT_GL_ERROR_STATUS("image buf storage");

  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  // Create the framebuffer and use it.
  GLuint fb;
  glGenFramebuffers(1, &fb);
  glBindFramebuffer(GL_FRAMEBUFFER, fb);
  PRINT_GL_ERROR_STATUS("create fb");

  // Attach it to the framebuffer.
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                            GL_RENDERBUFFER, image_buf);
  PRINT_GL_ERROR_STATUS("attach image buf to fb");

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    return Status_FramebufferCreationFailed;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // Not doing this results in the image missing some parts.
  glViewport(0, 0, kImageWidth, kImageHeight);
  PRINT_GL_ERROR_STATUS("set viewport");

  // Render a quad on the framebuffer.
  glBindFramebuffer(GL_FRAMEBUFFER, fb);
  glUseProgram(program);
  glBindVertexArray(quad_va);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
  PRINT_GL_ERROR_STATUS("draw elements");

  // Retrieve the image data from the framebuffer.
  unsigned char *data = malloc(kImageWidth * kImageHeight * 3);  // 3 for RGB.

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, fb);
  PRINT_GL_ERROR_STATUS("bind fb to read framebuffer");

  glReadBuffer(GL_COLOR_ATTACHMENT0);
  PRINT_GL_ERROR_STATUS("set read buffer");

  glReadPixels(0, 0, kImageWidth, kImageHeight, GL_RGB, GL_UNSIGNED_BYTE, data);
  PRINT_GL_ERROR_STATUS("read pixels");

  // Write the image data to a file.
  const int kResult = stbi_write_png("output.png", kImageWidth, kImageHeight, 3,
                                     data, 3 * kImageWidth);

  if (!kResult) {
    fputs("Failed to write image\n", stderr);
    return Status_ImageOutputFailed;
  }

  // Cleanup data.
  glDeleteRenderbuffers(1, &image_buf);
  glDeleteFramebuffers(1, &fb);
  glDeleteProgram(program);
  glDeleteBuffers(2, buffers);
  PRINT_GL_ERROR_STATUS("delete objects");

  Terminate();
  return Status_Success;
}

/**
 * Dummy window used to create an OpenGL context.
 */
GLFWwindow *window;

Status Init() {
  if (!glfwInit()) {
    return Status_GlfwInitFailed;
  }

  glfwDefaultWindowHints();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

  window = glfwCreateWindow(1, 1, "", NULL, NULL);
  if (!window) {
    return Status_DummyWindowCreationFailed;
  }

  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    return Status_GlLoadingFailed;
  }

  return Status_Success;
}

const char *GetStatusInfo(int status_code);

void PrintStatus(FILE *stream, int status_code) {
  const char *const kStatusInfo = GetStatusInfo(status_code);
  fprintf(stream, "%s\n", kStatusInfo);
}

const char *GetStatusInfo(int status_code) {
  switch (status_code) {
    case Status_Success:
      return "Initialization successful";
    case Status_GlfwInitFailed:
      return "GLFW initialization failed";
    case Status_DummyWindowCreationFailed:
      return "Creation of the dummy window failed";
    case Status_GlLoadingFailed:
      return "Failed to load OpenGL functions";
    case Status_ShaderCompilationFailed:
      return "Failed to compile a shader";
    case Status_FramebufferCreationFailed:
      return "Failed to create the image framebuffer";
    default:
      return "Unknown status";
  }
}

Status CheckShaderCompilationStatus(GLuint shader) {
  int compile_status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);

  if (!compile_status) {
    int length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

    GLchar *info_log = malloc(length);
    glGetShaderInfoLog(shader, length, NULL, info_log);

    fprintf(stderr, "Shader compilation failed:\n%s", info_log);
    return Status_ShaderCompilationFailed;
  }

  return Status_Success;
}

Status CheckProgramLinkStatus(GLuint program) {
  int link_status;
  glGetProgramiv(program, GL_LINK_STATUS, &link_status);

  if (!link_status) {
    int length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

    GLchar *info_log = malloc(length);
    glGetProgramInfoLog(program, length, NULL, info_log);

    fprintf(stderr, "Program linking failed:\n%s", info_log);
    return Status_ProgramLinkingFailed;
  }

  return Status_Success;
}

void Terminate() {
  glfwDestroyWindow(window);
  glfwTerminate();
}
