#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

enum {
  Status_Success,
  Status_GlfwInitFailed,
  Status_DummyWindowCreationFailed,
  Status_GlLoadingFailed,
  Status_ShaderCompilationFailed
};

int Init();
const char *GetStatusInfo(int status_code);
int CheckShaderCompilationStatus(GLuint shader);
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

int main() {
  const int kInitResult = Init();
  const char *const kStatusInfo = GetStatusInfo(kInitResult);

  if (kInitResult) {  // An error was returned.
    fputs(kStatusInfo, stderr);
    return kInitResult;
  } else {
    fputs(kStatusInfo, stdout);
  }

  GLuint v_shader = glCreateShader(GL_VERTEX_SHADER);
  GLuint f_shader = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(v_shader, 1, &kVShaderSource, NULL);
  glShaderSource(f_shader, 1, &kFShaderSource, NULL);

  glCompileShader(v_shader);
  glCompileShader(f_shader);

  if (CheckShaderCompilationStatus(v_shader) ||
      CheckShaderCompilationStatus(f_shader)) {
    return Status_ShaderCompilationFailed;
  }

  GLuint program = glCreateProgram();
  // TODO: Create and link.

  glDeleteShader(v_shader);
  glDeleteShader(f_shader);

  Terminate();
  return Status_Success;
}

/**
 * Dummy window used to create an OpenGL context.
 */
GLFWwindow *window;

int Init() {
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
    default:
      return "Unknown status";
  }
}

int CheckShaderCompilationStatus(GLuint shader) {
  int compile_status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);

  if (compile_status == GL_FALSE) {
    int length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

    GLchar *info_log = malloc(length);
    glGetShaderInfoLog(shader, length, NULL, info_log);

    fprintf(stderr, "Shader compilation failed:\n%s", info_log);
    return Status_ShaderCompilationFailed;
  }

  return Status_Success;
}

void Terminate() {
  glfwDestroyWindow(window);
  glfwTerminate();
}
