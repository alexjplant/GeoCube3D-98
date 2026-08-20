#include "render/renderer.h"

#include "core/collision.h"
#include "render/matrix.h"
#include "render/texture_loader.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <fstream>
#include <sstream>
#include <utility>

namespace geocube::render {

#ifndef APIENTRY
#define APIENTRY
#endif

struct Renderer::GlApi {
  PFNGLCREATESHADERPROC createShader = nullptr;
  PFNGLSHADERSOURCEPROC shaderSource = nullptr;
  PFNGLCOMPILESHADERPROC compileShader = nullptr;
  PFNGLGETSHADERIVPROC getShaderiv = nullptr;
  PFNGLGETSHADERINFOLOGPROC getShaderInfoLog = nullptr;
  PFNGLDELETESHADERPROC deleteShader = nullptr;
  PFNGLCREATEPROGRAMPROC createProgram = nullptr;
  PFNGLATTACHSHADERPROC attachShader = nullptr;
  PFNGLLINKPROGRAMPROC linkProgram = nullptr;
  PFNGLGETPROGRAMIVPROC getProgramiv = nullptr;
  PFNGLGETPROGRAMINFOLOGPROC getProgramInfoLog = nullptr;
  PFNGLDELETEPROGRAMPROC deleteProgram = nullptr;
  PFNGLUSEPROGRAMPROC useProgram = nullptr;
  PFNGLGETUNIFORMLOCATIONPROC getUniformLocation = nullptr;
  PFNGLUNIFORMMATRIX4FVPROC uniformMatrix4fv = nullptr;
  PFNGLUNIFORM3FPROC uniform3f = nullptr;
  PFNGLGENVERTEXARRAYSPROC genVertexArrays = nullptr;
  PFNGLBINDVERTEXARRAYPROC bindVertexArray = nullptr;
  PFNGLDELETEVERTEXARRAYSPROC deleteVertexArrays = nullptr;
  PFNGLGENBUFFERSPROC genBuffers = nullptr;
  PFNGLBINDBUFFERPROC bindBuffer = nullptr;
  PFNGLBUFFERDATAPROC bufferData = nullptr;
  PFNGLDELETEBUFFERSPROC deleteBuffers = nullptr;
  PFNGLENABLEVERTEXATTRIBARRAYPROC enableVertexAttribArray = nullptr;
  PFNGLVERTEXATTRIBPOINTERPROC vertexAttribPointer = nullptr;
  using DrawElementsProc = void (APIENTRY*)(GLenum, GLsizei, GLenum,
                                             const void*);
  using DrawArraysProc = void (APIENTRY*)(GLenum, GLint, GLsizei);
  DrawElementsProc drawElements = nullptr;
  DrawArraysProc drawArrays = nullptr;
};

namespace {

template <typename Function>
bool loadFunction(Function& function, GlProcAddress loader, const char* name)
{
  function = reinterpret_cast<Function>(loader(name));
  return function != nullptr;
}

template <typename Function>
bool loadRequired(Function& function, GlProcAddress loader, const char* name,
                  std::string& error)
{
  if (loadFunction(function, loader, name))
    return true;
  error = std::string("missing OpenGL function: ") + name;
  return false;
}

const char* vertexShaderSource()
{
#ifdef __EMSCRIPTEN__
  return R"glsl(#version 300 es
precision highp float;
layout(location = 0) in vec3 aPosition;
uniform mat4 uMvp;
void main()
{
  gl_Position = uMvp * vec4(aPosition, 1.0);
}
)glsl";
#else
  return R"glsl(#version 330 core
layout(location = 0) in vec3 aPosition;
uniform mat4 uMvp;
void main()
{
  gl_Position = uMvp * vec4(aPosition, 1.0);
}
)glsl";
#endif
}

const char* fragmentShaderSource()
{
#ifdef __EMSCRIPTEN__
  return R"glsl(#version 300 es
precision highp float;
uniform vec3 uColor;
out vec4 FragColor;
void main()
{
  FragColor = vec4(uColor, 1.0);
}
)glsl";
#else
  return R"glsl(#version 330 core
uniform vec3 uColor;
out vec4 FragColor;
void main()
{
  FragColor = vec4(uColor, 1.0);
}
)glsl";
#endif
}

Model makeCube()
{
  Model model;
  model.vertices = {{-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f},
                    {0.5f, 0.5f, -0.5f},   {-0.5f, 0.5f, -0.5f},
                    {-0.5f, -0.5f, 0.5f},  {0.5f, -0.5f, 0.5f},
                    {0.5f, 0.5f, 0.5f},    {-0.5f, 0.5f, 0.5f}};
  model.indices = {0, 1, 2, 2, 3, 0, 1, 5, 6, 6, 2, 1,
                   5, 4, 7, 7, 6, 5, 4, 0, 3, 3, 7, 4,
                   3, 2, 6, 6, 7, 3, 4, 5, 1, 1, 0, 4};
  return model;
}

Model makeSphere()
{
  constexpr int rings = 8;
  constexpr int segments = 12;
  Model model;
  for (int ring = 0; ring <= rings; ++ring) {
    const float theta = 3.14159265359f * ring / rings;
    const float y = std::cos(theta);
    const float radius = std::sin(theta);
    for (int segment = 0; segment <= segments; ++segment) {
      const float phi = 2.0f * 3.14159265359f * segment / segments;
      model.vertices.push_back(
          {radius * std::cos(phi), y, radius * std::sin(phi)});
    }
  }
  for (int ring = 0; ring < rings; ++ring) {
    for (int segment = 0; segment < segments; ++segment) {
      const std::uint32_t row = ring * (segments + 1);
      const std::uint32_t nextRow = (ring + 1) * (segments + 1);
      const std::uint32_t current = row + segment;
      const std::uint32_t next = row + segment + 1;
      const std::uint32_t below = nextRow + segment;
      const std::uint32_t belowNext = nextRow + segment + 1;
      model.indices.insert(model.indices.end(),
                           {current, below, next, next, below, belowNext});
    }
  }
  return model;
}

Model makeCone()
{
  constexpr int segments = 12;
  Model model;
  model.vertices.push_back({0.0f, 1.0f, 0.0f});
  model.vertices.push_back({0.0f, -1.0f, 0.0f});
  for (int segment = 0; segment < segments; ++segment) {
    const float angle = 2.0f * 3.14159265359f * segment / segments;
    model.vertices.push_back({std::cos(angle), -1.0f, std::sin(angle)});
  }
  for (int segment = 0; segment < segments; ++segment) {
    const std::uint32_t first = 2 + segment;
    const std::uint32_t second = 2 + ((segment + 1) % segments);
    model.indices.insert(model.indices.end(), {0, first, second,
                                               1, second, first});
  }
  return model;
}

Model makeRod()
{
  Model model = makeCube();
  for (core::Vec3& vertex : model.vertices)
    vertex.y *= 2.5f;
  return model;
}

std::array<std::uint8_t, 7> glyphPattern(char character)
{
  switch (static_cast<char>(std::toupper(static_cast<unsigned char>(character)))) {
  case 'A': return {0x0E, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11};
  case 'B': return {0x1E, 0x11, 0x11, 0x1E, 0x11, 0x11, 0x1E};
  case 'C': return {0x0F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x0F};
  case 'D': return {0x1E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1E};
  case 'E': return {0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x1F};
  case 'F': return {0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x10};
  case 'G': return {0x0F, 0x10, 0x10, 0x17, 0x11, 0x11, 0x0F};
  case 'H': return {0x11, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11};
  case 'I': return {0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x1F};
  case 'J': return {0x01, 0x01, 0x01, 0x01, 0x11, 0x11, 0x0E};
  case 'K': return {0x11, 0x12, 0x14, 0x18, 0x14, 0x12, 0x11};
  case 'L': return {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F};
  case 'M': return {0x11, 0x1B, 0x15, 0x15, 0x11, 0x11, 0x11};
  case 'N': return {0x11, 0x19, 0x15, 0x13, 0x11, 0x11, 0x11};
  case 'O': return {0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E};
  case 'P': return {0x1E, 0x11, 0x11, 0x1E, 0x10, 0x10, 0x10};
  case 'Q': return {0x0E, 0x11, 0x11, 0x11, 0x15, 0x12, 0x0D};
  case 'R': return {0x1E, 0x11, 0x11, 0x1E, 0x14, 0x12, 0x11};
  case 'S': return {0x0F, 0x10, 0x10, 0x0E, 0x01, 0x01, 0x1E};
  case 'T': return {0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04};
  case 'U': return {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E};
  case 'V': return {0x11, 0x11, 0x11, 0x11, 0x11, 0x0A, 0x04};
  case 'W': return {0x11, 0x11, 0x11, 0x15, 0x15, 0x1B, 0x11};
  case 'X': return {0x11, 0x11, 0x0A, 0x04, 0x0A, 0x11, 0x11};
  case 'Y': return {0x11, 0x11, 0x0A, 0x04, 0x04, 0x04, 0x04};
  case 'Z': return {0x1F, 0x01, 0x02, 0x04, 0x08, 0x10, 0x1F};
  case '0': return {0x0E, 0x11, 0x13, 0x15, 0x19, 0x11, 0x0E};
  case '1': return {0x04, 0x0C, 0x04, 0x04, 0x04, 0x04, 0x0E};
  case '2': return {0x0E, 0x11, 0x01, 0x02, 0x04, 0x08, 0x1F};
  case '3': return {0x1E, 0x01, 0x01, 0x0E, 0x01, 0x01, 0x1E};
  case '4': return {0x02, 0x06, 0x0A, 0x12, 0x1F, 0x02, 0x02};
  case '5': return {0x1F, 0x10, 0x10, 0x1E, 0x01, 0x01, 0x1E};
  case '6': return {0x0E, 0x10, 0x10, 0x1E, 0x11, 0x11, 0x0E};
  case '7': return {0x1F, 0x01, 0x02, 0x04, 0x08, 0x08, 0x08};
  case '8': return {0x0E, 0x11, 0x11, 0x0E, 0x11, 0x11, 0x0E};
  case '9': return {0x0E, 0x11, 0x11, 0x0F, 0x01, 0x01, 0x0E};
  case ':': return {0x00, 0x04, 0x04, 0x00, 0x04, 0x04, 0x00};
  case '-': return {0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00};
  case '>': return {0x10, 0x08, 0x04, 0x02, 0x04, 0x08, 0x10};
  case '<': return {0x01, 0x02, 0x04, 0x08, 0x04, 0x02, 0x01};
  case '.': return {0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06};
  case '!': return {0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x04};
  case '@': return {0x0E, 0x11, 0x17, 0x15, 0x17, 0x10, 0x0E};
  default: return {};
  }
}

} // namespace

Renderer::~Renderer()
{
  shutdown();
}

void Renderer::setError(std::string message)
{
  m_error = std::move(message);
}

bool Renderer::loadApi(GlProcAddress loader)
{
  m_gl = new GlApi;
#define LOAD_GL(field, name) \
  if (!loadRequired(m_gl->field, loader, name, m_error)) return false
  LOAD_GL(createShader, "glCreateShader");
  LOAD_GL(shaderSource, "glShaderSource");
  LOAD_GL(compileShader, "glCompileShader");
  LOAD_GL(getShaderiv, "glGetShaderiv");
  LOAD_GL(getShaderInfoLog, "glGetShaderInfoLog");
  LOAD_GL(deleteShader, "glDeleteShader");
  LOAD_GL(createProgram, "glCreateProgram");
  LOAD_GL(attachShader, "glAttachShader");
  LOAD_GL(linkProgram, "glLinkProgram");
  LOAD_GL(getProgramiv, "glGetProgramiv");
  LOAD_GL(getProgramInfoLog, "glGetProgramInfoLog");
  LOAD_GL(deleteProgram, "glDeleteProgram");
  LOAD_GL(useProgram, "glUseProgram");
  LOAD_GL(getUniformLocation, "glGetUniformLocation");
  LOAD_GL(uniformMatrix4fv, "glUniformMatrix4fv");
  LOAD_GL(uniform3f, "glUniform3f");
  LOAD_GL(genVertexArrays, "glGenVertexArrays");
  LOAD_GL(bindVertexArray, "glBindVertexArray");
  LOAD_GL(deleteVertexArrays, "glDeleteVertexArrays");
  LOAD_GL(genBuffers, "glGenBuffers");
  LOAD_GL(bindBuffer, "glBindBuffer");
  LOAD_GL(bufferData, "glBufferData");
  LOAD_GL(deleteBuffers, "glDeleteBuffers");
  LOAD_GL(enableVertexAttribArray, "glEnableVertexAttribArray");
  LOAD_GL(vertexAttribPointer, "glVertexAttribPointer");
  LOAD_GL(drawElements, "glDrawElements");
  LOAD_GL(drawArrays, "glDrawArrays");
#undef LOAD_GL
  return true;
}

bool Renderer::createProgram()
{
  auto compileShader = [this](GLenum type, const char* source) -> GLuint {
    const GLuint shader = m_gl->createShader(type);
    m_gl->shaderSource(shader, 1, &source, nullptr);
    m_gl->compileShader(shader);
    GLint compiled = GL_FALSE;
    m_gl->getShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_TRUE)
      return shader;

    GLint logLength = 0;
    m_gl->getShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    std::string log(static_cast<std::size_t>(std::max(logLength, 1)), '\0');
    m_gl->getShaderInfoLog(shader, logLength, nullptr, log.data());
    setError("OpenGL shader compilation failed: " + log);
    m_gl->deleteShader(shader);
    return 0;
  };

  const GLuint vertex = compileShader(GL_VERTEX_SHADER, vertexShaderSource());
  const GLuint fragment =
      compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource());
  if (!vertex || !fragment)
    return false;

  m_program = m_gl->createProgram();
  m_gl->attachShader(m_program, vertex);
  m_gl->attachShader(m_program, fragment);
  m_gl->linkProgram(m_program);
  m_gl->deleteShader(vertex);
  m_gl->deleteShader(fragment);

  GLint linked = GL_FALSE;
  m_gl->getProgramiv(m_program, GL_LINK_STATUS, &linked);
  if (linked != GL_TRUE) {
    GLint logLength = 0;
    m_gl->getProgramiv(m_program, GL_INFO_LOG_LENGTH, &logLength);
    std::string log(static_cast<std::size_t>(std::max(logLength, 1)), '\0');
    m_gl->getProgramInfoLog(m_program, logLength, nullptr, log.data());
    setError("OpenGL program linking failed: " + log);
    return false;
  }

  m_mvpLocation = m_gl->getUniformLocation(m_program, "uMvp");
  m_colorLocation = m_gl->getUniformLocation(m_program, "uColor");
  return m_mvpLocation >= 0 && m_colorLocation >= 0;
}

bool Renderer::createModel(const Model& model, GpuModel& gpuModel)
{
  const std::vector<ModelGroup> groups =
      model.groups.empty()
          ? std::vector<ModelGroup>{{model.indices, {1.0f, 1.0f, 1.0f}}}
          : model.groups;
  for (const ModelGroup& group : groups) {
    if (group.indices.empty())
      continue;
    GpuModel::Part part;
    part.diffuse = group.diffuse;
    m_gl->genVertexArrays(1, &part.vertexArray);
    m_gl->genBuffers(1, &part.vertexBuffer);
    m_gl->genBuffers(1, &part.indexBuffer);
    m_gl->bindVertexArray(part.vertexArray);
    m_gl->bindBuffer(GL_ARRAY_BUFFER, part.vertexBuffer);
    m_gl->bufferData(GL_ARRAY_BUFFER,
                     static_cast<GLsizeiptr>(model.vertices.size() *
                                             sizeof(core::Vec3)),
                     model.vertices.data(), GL_STATIC_DRAW);
    m_gl->bindBuffer(GL_ELEMENT_ARRAY_BUFFER, part.indexBuffer);
    m_gl->bufferData(GL_ELEMENT_ARRAY_BUFFER,
                     static_cast<GLsizeiptr>(group.indices.size() *
                                             sizeof(std::uint32_t)),
                     group.indices.data(), GL_STATIC_DRAW);
    m_gl->enableVertexAttribArray(0);
    m_gl->vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(core::Vec3),
                              nullptr);
    m_gl->bindVertexArray(0);
    part.indexCount = static_cast<GLsizei>(group.indices.size());
    gpuModel.parts.push_back(part);
  }
  if (gpuModel.parts.empty()) {
    setError("remastered model has no drawable parts");
    return false;
  }
  return true;
}

bool Renderer::loadPlayerModel(const std::filesystem::path& root)
{
  Model model;
  std::string error;
  if (loadObjModel(root / "models/ship.obj", model, error))
    m_playerScale = 0.2f; // Preserve the legacy CreatePlayer XOF scale.
  else if (loadRemasteredMesh(root / "models/ship.mesh", model, error))
    m_playerScale = 20.0f;
  else
    model = createShipFallback();
  return createModel(model, m_playerModel);
}

Model Renderer::createPrimitive(core::RockType type) const
{
  switch (type) {
  case core::RockType::Sphere:
  case core::RockType::Rock:
    return makeSphere();
  case core::RockType::Rod:
    return makeRod();
  case core::RockType::Cone:
    return makeCone();
  case core::RockType::Cube:
    return makeCube();
  }
  return makeCube();
}

Model Renderer::createShipFallback() const
{
  Model model;
  model.vertices = {{0.0f, 0.0f, 1.5f}, {-1.0f, -0.5f, -1.0f},
                    {1.0f, -0.5f, -1.0f}, {0.0f, 0.5f, -1.0f},
                    {0.0f, 0.0f, -1.0f}};
  model.indices = {0, 1, 2, 0, 2, 3, 0, 3, 1,
                   1, 3, 4, 3, 2, 4, 2, 1, 4};
  return model;
}

void Renderer::createPrimitiveModels(const std::filesystem::path& root)
{
  Model cube;
  std::string error;
  if (!loadObjModel(root / "models/cube.obj", cube, error))
    cube = createPrimitive(core::RockType::Cube);
  createModel(cube, m_rockModels[static_cast<std::size_t>(
                       core::RockType::Cube)]);
  for (int type = 1; type < 5; ++type)
    createModel(createPrimitive(static_cast<core::RockType>(type)),
                m_rockModels[static_cast<std::size_t>(type)]);
}

bool Renderer::loadReferenceModels(const std::filesystem::path& root)
{
  constexpr std::array<const char*, 4> names{{"jack", "portal", "sphere3",
                                               "pasta"}};
  for (std::size_t i = 0; i < names.size(); ++i) {
    Model model;
    std::string error;
    if (!loadObjModel(root / "models" / (std::string(names[i]) + ".obj"),
                      model, error) ||
        !createModel(model, m_referenceModels[i])) {
      setError("failed to load converted model " + std::string(names[i]) +
               ".obj: " + error);
      return false;
    }
  }
  return true;
}

bool Renderer::initialize(GlProcAddress loader,
                          const std::filesystem::path& root)
{
  shutdown();
  m_root = root;
  m_error.clear();
  if (!loadApi(loader) || !createProgram() || !loadPlayerModel(root)) {
    shutdown();
    return false;
  }
  createPrimitiveModels(root);
  if (!loadReferenceModels(root)) {
    shutdown();
    return false;
  }

  ImageRgba image;
  std::string imageError;
  if (loadBmpRgba(root / "textures/canvas.bmp", image, imageError)) {
    glGenTextures(1, &m_canvasTexture);
    glBindTexture(GL_TEXTURE_2D, m_canvasTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image.pixels.data());
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  std::vector<core::Vec3> lines;
  constexpr float boundary = core::kWorldBoundary;
  constexpr float step = 100.0f;
  for (float value = -boundary; value <= boundary; value += step) {
    lines.push_back({-boundary, boundary, value});
    lines.push_back({boundary, boundary, value});
    lines.push_back({value, boundary, -boundary});
    lines.push_back({value, boundary, boundary});
    lines.push_back({-boundary, -boundary, value});
    lines.push_back({boundary, -boundary, value});
    lines.push_back({value, -boundary, -boundary});
    lines.push_back({value, -boundary, boundary});
    lines.push_back({-boundary, value, boundary});
    lines.push_back({boundary, value, boundary});
    lines.push_back({-boundary, value, -boundary});
    lines.push_back({boundary, value, -boundary});
  }
  m_lineVertexCount = static_cast<GLsizei>(lines.size());
  m_gl->genVertexArrays(1, &m_lineVertexArray);
  m_gl->genBuffers(1, &m_lineVertexBuffer);
  m_gl->bindVertexArray(m_lineVertexArray);
  m_gl->bindBuffer(GL_ARRAY_BUFFER, m_lineVertexBuffer);
  m_gl->bufferData(GL_ARRAY_BUFFER,
                   static_cast<GLsizeiptr>(lines.size() * sizeof(core::Vec3)),
                   lines.data(), GL_STATIC_DRAW);
  m_gl->enableVertexAttribArray(0);
  m_gl->vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(core::Vec3),
                            nullptr);
  m_gl->bindVertexArray(0);
  m_gl->genVertexArrays(1, &m_uiVertexArray);
  m_gl->genBuffers(1, &m_uiVertexBuffer);
  m_gl->bindVertexArray(m_uiVertexArray);
  m_gl->bindBuffer(GL_ARRAY_BUFFER, m_uiVertexBuffer);
  m_gl->enableVertexAttribArray(0);
  m_gl->vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(core::Vec3),
                            nullptr);
  m_gl->bindVertexArray(0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  return true;
}

void Renderer::resize(int width, int height)
{
  m_width = std::max(width, 1);
  m_height = std::max(height, 1);
  glViewport(0, 0, m_width, m_height);
}

void Renderer::drawModel(const GpuModel& model, const Mat4& modelMatrix,
                         const Mat4& viewProjection,
                         const core::Vec3& color)
{
  const Mat4 mvp = viewProjection * modelMatrix;
  for (const GpuModel::Part& part : model.parts) {
    m_gl->useProgram(m_program);
    m_gl->uniformMatrix4fv(m_mvpLocation, 1, GL_FALSE, mvp.data());
    m_gl->uniform3f(m_colorLocation, color.x * part.diffuse.x,
                    color.y * part.diffuse.y, color.z * part.diffuse.z);
    m_gl->bindVertexArray(part.vertexArray);
    m_gl->drawElements(GL_TRIANGLES, part.indexCount, GL_UNSIGNED_INT,
                       nullptr);
  }
  m_gl->bindVertexArray(0);
}

void Renderer::drawBoundaryGrid(const Mat4& viewProjection)
{
  m_gl->useProgram(m_program);
  m_gl->uniformMatrix4fv(m_mvpLocation, 1, GL_FALSE, viewProjection.data());
  m_gl->uniform3f(m_colorLocation, 0.0f, 0.5f, 1.0f);
  m_gl->bindVertexArray(m_lineVertexArray);
  m_gl->drawArrays(GL_LINES, 0, m_lineVertexCount);
  m_gl->bindVertexArray(0);
}

void Renderer::drawUiRect(float x, float y, float width, float height,
                          const core::Vec3& color)
{
  if (!m_gl || !m_uiVertexArray)
    return;
  glDisable(GL_DEPTH_TEST);
  const std::vector<core::Vec3> vertices{{x, y, 0.0f},
                                         {x + width, y, 0.0f},
                                         {x + width, y + height, 0.0f},
                                         {x, y, 0.0f},
                                         {x + width, y + height, 0.0f},
                                         {x, y + height, 0.0f}};
  const Mat4 projection =
      orthographic(0.0f, static_cast<float>(m_width), 0.0f,
                   static_cast<float>(m_height), -1.0f, 1.0f);
  m_gl->useProgram(m_program);
  m_gl->uniformMatrix4fv(m_mvpLocation, 1, GL_FALSE, projection.data());
  m_gl->uniform3f(m_colorLocation, color.x, color.y, color.z);
  m_gl->bindVertexArray(m_uiVertexArray);
  m_gl->bindBuffer(GL_ARRAY_BUFFER, m_uiVertexBuffer);
  m_gl->bufferData(GL_ARRAY_BUFFER,
                   static_cast<GLsizeiptr>(vertices.size() *
                                           sizeof(core::Vec3)),
                   vertices.data(), GL_STREAM_DRAW);
  m_gl->drawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
  m_gl->bindVertexArray(0);
  glEnable(GL_DEPTH_TEST);
}

void Renderer::drawUiText(const std::string& text, float x, float y,
                          float scaleValue, const core::Vec3& color)
{
  if (!m_gl || !m_uiVertexArray)
    return;
  glDisable(GL_DEPTH_TEST);
  std::vector<core::Vec3> vertices;
  float cursorX = x;
  float cursorY = y;
  for (char character : text) {
    if (character == '\n') {
      cursorX = x;
      cursorY += 8.0f * scaleValue;
      continue;
    }
    const auto rows = glyphPattern(character);
    for (int row = 0; row < 7; ++row) {
      for (int column = 0; column < 5; ++column) {
        if ((rows[static_cast<std::size_t>(row)] & (1u << (4 - column))) ==
            0)
          continue;
        const float pixelX = cursorX + column * scaleValue;
        const float pixelY = cursorY + row * scaleValue;
        vertices.insert(vertices.end(),
                        {{pixelX, pixelY, 0.0f},
                         {pixelX + scaleValue, pixelY, 0.0f},
                         {pixelX + scaleValue, pixelY + scaleValue, 0.0f},
                         {pixelX, pixelY, 0.0f},
                         {pixelX + scaleValue, pixelY + scaleValue, 0.0f},
                         {pixelX, pixelY + scaleValue, 0.0f}});
      }
    }
    cursorX += 6.0f * scaleValue;
  }
  if (vertices.empty())
  {
    glEnable(GL_DEPTH_TEST);
    return;
  }
  const Mat4 projection =
      orthographic(0.0f, static_cast<float>(m_width), 0.0f,
                   static_cast<float>(m_height), -1.0f, 1.0f);
  m_gl->useProgram(m_program);
  m_gl->uniformMatrix4fv(m_mvpLocation, 1, GL_FALSE, projection.data());
  m_gl->uniform3f(m_colorLocation, color.x, color.y, color.z);
  m_gl->bindVertexArray(m_uiVertexArray);
  m_gl->bindBuffer(GL_ARRAY_BUFFER, m_uiVertexBuffer);
  m_gl->bufferData(GL_ARRAY_BUFFER,
                   static_cast<GLsizeiptr>(vertices.size() *
                                           sizeof(core::Vec3)),
                   vertices.data(), GL_STREAM_DRAW);
  m_gl->drawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
  m_gl->bindVertexArray(0);
  glEnable(GL_DEPTH_TEST);
}

void Renderer::drawWorld(const core::GameWorld& world)
{
  const core::PlayerState& player = world.player();
  // Matches CAGamePlay::InitGame in legacy gameplay.cpp: the camera is a
  // child frame at (0, 2, -6), not a distant chase camera.
  const core::Vec3 camera = player.position - player.direction * 6.0f +
                            player.up * 2.0f;
  const Mat4 view =
      lookAt(camera, camera + player.direction * 100.0f, player.up);
  const Mat4 projection = perspective(world.fieldOfView(),
                                      static_cast<float>(m_width) / m_height,
                                      1.0f, 4000.0f);
  const Mat4 viewProjection = projection * view;
  drawBoundaryGrid(viewProjection);

  drawModel(m_playerModel,
            translation(player.position) *
                orientation(player.direction, player.up) *
                // The imported XOF ship points toward local -Z. Turn it so
                // local forward is the core direction and the camera sees
                // the ship's back from the legacy camera position.
                rotationY(3.14159265359f) * scale(m_playerScale),
            viewProjection, {1.0f, 1.0f, 1.0f});
  if (player.shield)
    drawModel(m_rockModels[2], translation(player.position) * scale(16.0f),
              viewProjection, {1.0f, 0.1f, 0.1f});

  for (const core::Rock& rock : world.rocks()) {
    const std::size_t type = static_cast<std::size_t>(rock.type);
    const core::Vec3 color = rock.type == core::RockType::Cube
                                 ? core::Vec3{1.0f, 1.0f, 1.0f}
                             : rock.type == core::RockType::Rock
                                 ? core::Vec3{0.75f, 0.55f, 0.35f}
                                 : core::Vec3{0.65f, 0.75f, 0.9f};
    const float modelScale = rock.type == core::RockType::Cube
                                 ? rock.radius * 0.5f
                                 : rock.radius;
    drawModel(m_rockModels[type], translation(rock.position) *
                                      scale(modelScale),
              viewProjection, color);
  }

  for (const core::Bullet& bullet : world.bullets())
    drawModel(m_rockModels[0], translation(bullet.position) * scale(4.0f),
              viewProjection, {1.0f, 0.9f, 0.1f});
}

void Renderer::render(const core::GameWorld& world)
{
  const core::Vec3 background = world.currentLevel().background;
  glClearColor(background.x * 0.15f, background.y * 0.15f,
               background.z * 0.15f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  drawWorld(world);
}

void Renderer::shutdown()
{
  if (!m_gl)
    return;

  const auto destroyModel = [this](GpuModel& model) {
    for (GpuModel::Part& part : model.parts) {
      if (part.indexBuffer)
        m_gl->deleteBuffers(1, &part.indexBuffer);
      if (part.vertexBuffer)
        m_gl->deleteBuffers(1, &part.vertexBuffer);
      if (part.vertexArray)
        m_gl->deleteVertexArrays(1, &part.vertexArray);
    }
    model.parts.clear();
  };
  for (GpuModel& model : m_rockModels)
    destroyModel(model);
  for (GpuModel& model : m_referenceModels)
    destroyModel(model);
  destroyModel(m_playerModel);
  if (m_lineVertexBuffer)
    m_gl->deleteBuffers(1, &m_lineVertexBuffer);
  if (m_lineVertexArray)
    m_gl->deleteVertexArrays(1, &m_lineVertexArray);
  if (m_uiVertexBuffer)
    m_gl->deleteBuffers(1, &m_uiVertexBuffer);
  if (m_uiVertexArray)
    m_gl->deleteVertexArrays(1, &m_uiVertexArray);
  if (m_program)
    m_gl->deleteProgram(m_program);
  if (m_canvasTexture)
    glDeleteTextures(1, &m_canvasTexture);

  delete m_gl;
  m_gl = nullptr;
  m_program = 0;
  m_canvasTexture = 0;
  m_lineVertexBuffer = 0;
  m_lineVertexArray = 0;
  m_uiVertexBuffer = 0;
  m_uiVertexArray = 0;
}

} // namespace geocube::render
