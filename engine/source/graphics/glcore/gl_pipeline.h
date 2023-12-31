#pragma once

#include "gl_shader.h"

#include <functional>

namespace sputnik::graphics::gl
{

enum class CompareFunction : u8
{
    Never,
    Less,
    Equal,
    LessEqual,
    Greater,
    NotEqual,
    GreaterEqual,
    Always
};

enum class StencilOperation : u8
{
	Keep,
	Zero,
	Replace,
	IncrementAndClamp,
	DecrementAndClamp,
	Invert,
	IncrementAndWrap,
	DecrementAndWrap
};

enum class CullFace : u8
{
    None,
	Front,
	Back,
	FrontAndBack
};

enum class PolygonMode : u8
{
	Point,
	Line,
	Fill
};

enum class BlendFactor : u8
{
	Zero,
	One,
	SrcColor,
	OneMinusSrcColor,
	DstColor,
	OneMinusDstColor,
	SrcAlpha,
	OneMinusSrcAlpha,
	DstAlpha,
	OneMinusDstAlpha,
	ConstantColor,
	OneMinusConstantColor,
	ConstantAlpha,
	OneMinusConstantAlpha,
	SrcAlphaSaturate,
	Src1Color,
	OneMinusSrc1Color,
	Src1Alpha,
	OneMinusSrc1Alpha
};

enum class BlendEquation : u8
{
	Add,
	Subtract,
	ReverseSubtract,
	Min,
	Max
};

enum class BlendColorMask : u8
{
	Red   = 1 << 0,
	Green = 1 << 1,
	Blue  = 1 << 2,
	Alpha = 1 << 3,
	All   = Red | Green | Blue | Alpha
};

struct DepthTestSpecification
{
    bool enabled;
    bool write_enabled;
    bool comparison_enabled;
};

struct StencilTestSpecification
{
    bool enabled;
    bool write_enabled;
    bool comparison_enabled;
};

struct CullFaceSpecification
{
    bool enabled;
    bool front_face_enabled;
    bool back_face_enabled;
};

struct BlendSpecification
{
    bool enabled;
    bool source_color_enabled;
    bool destination_color_enabled;
    bool source_alpha_enabled;
    bool destination_alpha_enabled;
};

struct DepthBiasSpecification
{
    bool  enabled;
    float constant_factor;
    float clamp;
    float slope_factor;
};

struct DepthClampSpecification
{
    bool enabled;
};

struct RasterizationSpecification
{
    bool discard_enabled;
    bool polygon_mode_enabled;
};

struct PipelineRasterizationSpecification
{
    DepthTestSpecification     depth_test_specification;
    StencilTestSpecification   stencil_test_specification;
    CullFaceSpecification      cull_face_specification;
    BlendSpecification         blend_specification;
    DepthBiasSpecification     depth_bias_specification;
    DepthClampSpecification    depth_clamp_specification;
    RasterizationSpecification rasterization_specification;
};

struct PipelineShaderStageSpecification
{
    // ShaderType shader_type;

    std::string path;
};

struct PipelineSpecification
{
    std::string vertex_shader_path;
    std::string fragment_shader_path;
    std::string geometry_shader_path;
    std::string tessellation_control_shader_path;
    std::string tessellation_evaluation_shader_path;
    std::string compute_shader_path;
};

// Deffered shading has multiple render passes
// Each render pass has a pipeline
// Each pipeline has a shader program
// Each shader program has a shader stage
// Each shader stage has a shader
class RenderPassSpecification
{
};

class OglPipeline
{

public:
    OglPipeline(const PipelineSpecification& specification);
    ~OglPipeline();

    OglPipeline(OglPipeline&& other) noexcept;
    OglPipeline& operator=(OglPipeline&& other) noexcept;

    void configure();

    void setExecuteFunction(std::function<void()> execute_callback);
    void execute(std::function<void()> execute_callback);

protected:

    void validate();

private:
    OglPipeline(const OglPipeline&)            = delete;
    OglPipeline& operator=(const OglPipeline&) = delete;

private:
    PipelineSpecification m_specification;

    std::function<void()> m_execute_function;
};

} // namespace sputnik::graphics::gl