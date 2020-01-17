#pragma once

#include "../LLGI.Graphics.h"
#import <MetalKit/MetalKit.h>
#include <functional>
#include <unordered_map>

namespace LLGI
{

struct Graphics_Impl;
struct RenderPass_Impl;
struct RenderPassPipelineState_Impl;

class GraphicsMetal;
class RenderPassMetal;
class RenderPassPipelineStateMetal;
class TextureMetal;
    
struct RenderPassPipelineStateMetalKey
{
    MTLPixelFormat format;
	MTLPixelFormat depthStencilFormat = MTLPixelFormatInvalid;	// MTLPixelFormatInvalid if texture is not set.
        
    bool operator==(const RenderPassPipelineStateMetalKey& value) const { return (format == value.format && depthStencilFormat == value.depthStencilFormat); }
        
    struct Hash
    {
        typedef std::size_t result_type;
            
        std::size_t operator()(const RenderPassPipelineStateMetalKey& key) const
        {
            return std::hash<std::int32_t>()(static_cast<int>(key.format) + (static_cast<int>(key.depthStencilFormat) * 10000));
        }
    };
};

struct GraphicsView
{
	id<CAMetalDrawable> drawable;
};

class GraphicsMetal : public Graphics
{
	Graphics_Impl* impl = nullptr;

    //! cached
	std::unordered_map<RenderPassPipelineStateMetalKey, std::shared_ptr<RenderPassPipelineStateMetal>, RenderPassPipelineStateMetalKey::Hash>
		renderPassPipelineStates;

	std::shared_ptr<RenderPassMetal> renderPass_ = nullptr;
	std::function<GraphicsView()> getGraphicsView_;
    std::vector<CommandList*> executingCommandList_;
    
public:
	GraphicsMetal();
	virtual ~GraphicsMetal();

	bool Initialize(std::function<GraphicsView()> getGraphicsView);

	void SetWindowSize(const Vec2I& windowSize) override;

	void Execute(CommandList* commandList) override;

	void WaitFinish() override;

	//RenderPass* GetCurrentScreen(const Color8& clearColor, bool isColorCleared, bool isDepthCleared) override;

	VertexBuffer* CreateVertexBuffer(int32_t size) override;

	IndexBuffer* CreateIndexBuffer(int32_t stride, int32_t count) override;

	Shader* CreateShader(DataStructure* data, int32_t count) override;

	PipelineState* CreatePiplineState() override;

	SingleFrameMemoryPool* CreateSingleFrameMemoryPool(int32_t constantBufferPoolSize, int32_t drawingCount) override;

	CommandList* CreateCommandList(SingleFrameMemoryPool* memoryPool) override;

	ConstantBuffer* CreateConstantBuffer(int32_t size) override;

	RenderPass* CreateRenderPass(const Texture** textures, int32_t textureCount, Texture* depthTexture) override;

	Texture* CreateTexture(const TextureInitializationParameter& parameter) override;
	
	Texture* CreateRenderTexture(const RenderTextureInitializationParameter& parameter) override;

	Texture* CreateDepthTexture(const DepthTextureInitializationParameter& parameter) override;
	
	Texture* CreateTexture(uint64_t id) override;

    //! internal function
	std::shared_ptr<RenderPassPipelineStateMetal> CreateRenderPassPipelineState(MTLPixelFormat format, MTLPixelFormat depthStencilFormat);

    RenderPassPipelineState* CreateRenderPassPipelineState(RenderPass* renderPass) override;
    
	std::vector<uint8_t> CaptureRenderTarget(Texture* renderTarget) override;

	Graphics_Impl* GetImpl() const;

    RenderPassMetal* GetRenderPass() const { return renderPass_.get(); }
};

} // namespace LLGI
