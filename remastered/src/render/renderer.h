#ifndef GEOCUBE_RENDER_RENDERER_H
#define GEOCUBE_RENDER_RENDERER_H

#include "core/game_world.h"
#include "render/matrix.h"
#include "render/model_loader.h"

#include <SDL3/SDL_opengl.h>

#include <array>
#include <filesystem>
#include <string>
#include <vector>

namespace geocube::render {

using GlProcAddress = void* (*)(const char*);

class Renderer final {
public:
  Renderer() = default;
  ~Renderer();

  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;

  bool initialize(GlProcAddress loader, const std::filesystem::path& root);
  void shutdown();
  void resize(int width, int height);
  void render(const core::GameWorld& world);
  void drawUiRect(float x, float y, float width, float height,
                  const core::Vec3& color, float alpha = 1.0f);
  void drawUiCircle(float centerX, float centerY, float radius,
                    const core::Vec3& color, float alpha = 1.0f);
  void drawUiTriangle(float tipX, float tipY, float baseLeftX, float baseLeftY,
                      float baseRightX, float baseRightY,
                      const core::Vec3& color, float alpha = 1.0f);
  void drawUiLine(float startX, float startY, float endX, float endY,
                  float width, const core::Vec3& color,
                  float alpha = 1.0f);
  void drawUiText(const std::string& text, float x, float y, float scale,
                  const core::Vec3& color, float alpha = 1.0f);
  bool projectWorldToUi(const core::GameWorld& world,
                        const core::Vec3& position, float& x, float& y,
                        bool& inFront) const;
  int width() const { return m_width; }
  int height() const { return m_height; }

  const std::string& error() const { return m_error; }
  bool texturePipelineReady() const { return m_canvasTexture != 0; }

private:
  struct GlApi;
  struct GpuModel {
    struct Part {
      GLuint vertexArray = 0;
      GLuint vertexBuffer = 0;
      GLuint indexBuffer = 0;
      GLsizei indexCount = 0;
      core::Vec3 diffuse{1.0f, 1.0f, 1.0f};
    };
    std::vector<Part> parts;
  };

  bool loadApi(GlProcAddress loader);
  bool createProgram();
  bool createModel(const Model& model, GpuModel& gpuModel);
  Model createShieldModel() const;
  bool loadPlayerModel(const std::filesystem::path& root);
  void createPrimitiveModels(const std::filesystem::path& root);
  bool loadReferenceModels(const std::filesystem::path& root);
  Model createPrimitive(core::RockType type) const;
  Model createShipFallback() const;
  void drawModel(const GpuModel& model, const Mat4& modelMatrix,
                 const Mat4& viewProjection, const core::Vec3& color,
                 GLenum primitive = GL_TRIANGLES);
  void drawBoundaryGrid(const Mat4& viewProjection);
  void drawWorld(const core::GameWorld& world);
  void drawUiVertices(GLenum primitive, const std::vector<core::Vec3>& vertices,
                      const core::Vec3& color, float alpha);
  void setError(std::string message);

  GlApi* m_gl = nullptr;
  GLuint m_program = 0;
  GLint m_mvpLocation = -1;
  GLint m_colorLocation = -1;
  GLint m_alphaLocation = -1;
  GLuint m_lineVertexArray = 0;
  GLuint m_lineVertexBuffer = 0;
  GLsizei m_lineVertexCount = 0;
  GLuint m_uiVertexArray = 0;
  GLuint m_uiVertexBuffer = 0;
  GLuint m_canvasTexture = 0;
  GpuModel m_playerModel;
  GpuModel m_shieldModel;
  std::array<GpuModel, 5> m_rockModels{};
  std::array<GpuModel, 4> m_referenceModels{};
  float m_playerScale = 20.0f;
  std::filesystem::path m_root;
  std::string m_error;
  int m_width = 1;
  int m_height = 1;
};

} // namespace geocube::render

#endif
