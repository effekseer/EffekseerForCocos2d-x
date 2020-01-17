
#include "LLGI.CommandListDX12.h"
#include "LLGI.ConstantBufferDX12.h"
#include "LLGI.DescriptorHeapDX12.h"
#include "LLGI.GraphicsDX12.h"
#include "LLGI.IndexBufferDX12.h"
#include "LLGI.PipelineStateDX12.h"
#include "LLGI.RenderPassDX12.h"
#include "LLGI.TextureDX12.h"
#include "LLGI.VertexBufferDX12.h"

namespace LLGI
{

CommandListDX12::CommandListDX12() {}

CommandListDX12::~CommandListDX12()
{
	SafeRelease(fence_);

	if (fenceEvent_ != nullptr)
	{
		CloseHandle(fenceEvent_);
		fenceEvent_ = nullptr;
	}
}

bool CommandListDX12::Initialize(GraphicsDX12* graphics, int32_t drawingCount)
{
	HRESULT hr;

	SafeAddRef(graphics);
	graphics_ = CreateSharedPtr(graphics);

	// Command Allocator
	ID3D12CommandAllocator* commandAllocator = nullptr;
	hr = graphics_->GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
	if (FAILED(hr))
	{
		SafeRelease(graphics_);
		goto FAILED_EXIT;
	}
	commandAllocator_ = CreateSharedPtr(commandAllocator);

	// Command List
	ID3D12GraphicsCommandList* commandList = nullptr;
	hr = graphics_->GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, NULL, IID_PPV_ARGS(&commandList));
	if (FAILED(hr))
	{
		SafeRelease(graphics_);
		SafeRelease(commandAllocator_);
		goto FAILED_EXIT;
	}
	commandList->Close();
	commandList_ = CreateSharedPtr(commandList);

	cbreDescriptorHeap_ = std::make_shared<DescriptorHeapDX12>(
		graphics_, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, drawingCount * NumTexture + 2, 2);

	// the maximum render target is defined temporary
	rtDescriptorHeap_ =
		std::make_shared<DescriptorHeapDX12>(graphics_, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV, drawingCount / 2, 2);
	smpDescriptorHeap_ = std::make_shared<DescriptorHeapDX12>(
		graphics_, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, drawingCount * NumTexture, 2);
	dtDescriptorHeap_ =
		std::make_shared<DescriptorHeapDX12>(graphics_, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV, drawingCount / 2, 2);

	hr = graphics_->GetDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	if (FAILED(hr))
	{
		goto FAILED_EXIT;
	}
	fenceEvent_ = CreateEvent(NULL, FALSE, FALSE, NULL);
	return true;

FAILED_EXIT:;
	return false;
}

void CommandListDX12::Begin()
{
	commandAllocator_->Reset();
	commandList_->Reset(commandAllocator_.get(), nullptr);

	cbreDescriptorHeap_->Reset();
	rtDescriptorHeap_->Reset();
	dtDescriptorHeap_->Reset();
	smpDescriptorHeap_->Reset();

	currentCommandList_ = commandList_.get();

	CommandList::Begin();
}

void CommandListDX12::End()
{
	assert(currentCommandList_ != nullptr);
	currentCommandList_->Close();
	currentCommandList_ = nullptr;
}

bool CommandListDX12::BeginWithPlatform(void* platformContextPtr)
{
	auto ptr = reinterpret_cast<PlatformContextDX12*>(platformContextPtr);

	cbreDescriptorHeap_->Reset();
	rtDescriptorHeap_->Reset();
	dtDescriptorHeap_->Reset();
	smpDescriptorHeap_->Reset();

	currentCommandList_ = ptr->commandList;

	return CommandList::BeginWithPlatform(platformContextPtr);
}

void CommandListDX12::EndWithPlatform()
{
	assert(currentCommandList_ != nullptr);
	currentCommandList_ = nullptr;
	CommandList::EndWithPlatform();
}

void CommandListDX12::BeginRenderPass(RenderPass* renderPass)
{
	assert(currentCommandList_ != nullptr);

	SafeAddRef(renderPass);
	renderPass_ = CreateSharedPtr((RenderPassDX12*)renderPass);

	if (renderPass != nullptr)
	{
		// Set render target
		renderPass_->ReinitializeRenderTargetViews(this, rtDescriptorHeap_.get(), dtDescriptorHeap_.get());
		currentCommandList_->OMSetRenderTargets(renderPass_->GetCount(), renderPass_->GetHandleRTV(), FALSE, renderPass_->GetHandleDSV());

		// Reset scissor
		D3D12_RECT rects[D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT];
		D3D12_VIEWPORT viewports[D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT];
		for (int i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT && i < renderPass_->GetCount(); i++)
		{
			auto size = (renderPass_->GetRenderTarget(i)->texture_ != nullptr) ? renderPass_->GetRenderTarget(i)->texture_->GetSizeAs2D()
																			   : renderPass_->GetScreenSize();
			rects[i].top = 0;
			rects[i].left = 0;
			rects[i].right = size.X;
			rects[i].bottom = size.Y;

			viewports[i].TopLeftX = 0.0f;
			viewports[i].TopLeftY = 0.0f;
			viewports[i].Width = static_cast<float>(size.X);
			viewports[i].Height = static_cast<float>(size.Y);
			viewports[i].MinDepth = 0.0f;
			viewports[i].MaxDepth = 1.0f;
		}
		currentCommandList_->RSSetScissorRects(renderPass_->GetCount(), rects);
		currentCommandList_->RSSetViewports(renderPass_->GetCount(), viewports);

		if (renderPass_->GetIsColorCleared())
		{
			Clear(renderPass_->GetClearColor());
		}

		if (renderPass_->GetIsDepthCleared())
		{
			ClearDepth();
		}
	}
}

void CommandListDX12::EndRenderPass() { renderPass_.reset(); }

void CommandListDX12::Draw(int32_t pritimiveCount)
{
	assert(currentCommandList_ != nullptr);

	BindingVertexBuffer vb_;
	BindingIndexBuffer ib_;
	ConstantBuffer* cb = nullptr;
	PipelineState* pip_ = nullptr;

	bool isVBDirtied = false;
	bool isIBDirtied = false;
	bool isPipDirtied = false;

	GetCurrentVertexBuffer(vb_, isVBDirtied);
	GetCurrentIndexBuffer(ib_, isIBDirtied);
	GetCurrentPipelineState(pip_, isPipDirtied);

	assert(vb_.vertexBuffer != nullptr);
	assert(ib_.indexBuffer != nullptr);
	assert(pip_ != nullptr);

	auto vb = static_cast<VertexBufferDX12*>(vb_.vertexBuffer);
	auto ib = static_cast<IndexBufferDX12*>(ib_.indexBuffer);
	auto pip = static_cast<PipelineStateDX12*>(pip_);

	{
		D3D12_VERTEX_BUFFER_VIEW vertexView;
		vertexView.BufferLocation = vb->Get()->GetGPUVirtualAddress() + vb_.offset;
		vertexView.StrideInBytes = vb_.stride;
		vertexView.SizeInBytes = vb_.vertexBuffer->GetSize() - vb_.offset;
		if (vb_.vertexBuffer != nullptr)
		{
			currentCommandList_->IASetVertexBuffers(0, 1, &vertexView);
		}
	}

	if (ib != nullptr)
	{
		D3D12_INDEX_BUFFER_VIEW indexView;
		indexView.BufferLocation = ib->Get()->GetGPUVirtualAddress() + ib_.offset;
		indexView.SizeInBytes = ib->GetStride() * ib->GetCount() - ib_.offset;
		indexView.Format = ib->GetStride() == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
		currentCommandList_->IASetIndexBuffer(&indexView);
	}

	if (pip != nullptr)
	{
		currentCommandList_->SetGraphicsRootSignature(pip->GetRootSignature());
		auto p = pip->GetPipelineState();
		currentCommandList_->SetPipelineState(p);
	}

	{
		// set using descriptor heaps
		ID3D12DescriptorHeap* heaps[] = {
			cbreDescriptorHeap_->GetHeap(),
			smpDescriptorHeap_->GetHeap(),
		};
		currentCommandList_->SetDescriptorHeaps(2, heaps);

		// set descriptor tables
		currentCommandList_->SetGraphicsRootDescriptorTable(0, cbreDescriptorHeap_->GetGpuHandle());
		currentCommandList_->SetGraphicsRootDescriptorTable(1, smpDescriptorHeap_->GetGpuHandle());
	}

	int increment = NumTexture * static_cast<int>(ShaderStageType::Max);

	// constant buffer
	{
		for (int stage_ind = 0; stage_ind < static_cast<int>(ShaderStageType::Max); stage_ind++)
		{
			GetCurrentConstantBuffer(static_cast<ShaderStageType>(stage_ind), cb);
			if (cb != nullptr)
			{
				auto _cb = static_cast<ConstantBufferDX12*>(cb);
				D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
				desc.BufferLocation = _cb->Get()->GetGPUVirtualAddress() + _cb->GetOffset();
				desc.SizeInBytes = _cb->GetActualSize();
				auto cpuHandle = cbreDescriptorHeap_->GetCpuHandle();
				graphics_->GetDevice()->CreateConstantBufferView(&desc, cpuHandle);
			}
			else
			{
				// set dummy values
				D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
				desc.BufferLocation = D3D12_GPU_VIRTUAL_ADDRESS();
				desc.SizeInBytes = 0;
				auto cpuHandle = cbreDescriptorHeap_->GetCpuHandle();
				graphics_->GetDevice()->CreateConstantBufferView(&desc, cpuHandle);
			}
			cbreDescriptorHeap_->IncrementCpuHandle(1);
		}
		cbreDescriptorHeap_->IncrementGpuHandle(static_cast<int>(ShaderStageType::Max));
	}

	{
		for (int stage_ind = 0; stage_ind < static_cast<int>(ShaderStageType::Max); stage_ind++)
		{
			for (int unit_ind = 0; unit_ind < currentTextures[stage_ind].size(); unit_ind++)
			{
				if (currentTextures[stage_ind][unit_ind].texture != nullptr)
				{
					auto texture = static_cast<TextureDX12*>(currentTextures[stage_ind][unit_ind].texture);
					auto wrapMode = currentTextures[stage_ind][unit_ind].wrapMode;
					auto minMagFilter = currentTextures[stage_ind][unit_ind].minMagFilter;

					// Make barrior to use a render target
					if (texture->GetType() == TextureType::Render)
					{
						if (stage_ind == static_cast<int>(ShaderStageType::Pixel))
						{
							texture->ResourceBarrior(currentCommandList_, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
						}
						else
						{
							texture->ResourceBarrior(currentCommandList_, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
						}
					}

					// SRV
					{
						D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
						srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
						srvDesc.Format = texture->GetDXGIFormat();
						srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
						srvDesc.Texture2D.MipLevels = 1;
						srvDesc.Texture2D.MostDetailedMip = 0;

						auto cpuHandle = cbreDescriptorHeap_->GetCpuHandle();
						graphics_->GetDevice()->CreateShaderResourceView(texture->Get(), &srvDesc, cpuHandle);
					}

					// Sampler
					{
						D3D12_SAMPLER_DESC samplerDesc = {};

						// TODO
						samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;

						if (wrapMode == TextureWrapMode::Repeat)
						{
							samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
							samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
							samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
						}
						else
						{
							samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
							samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
							samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
						}
						samplerDesc.MipLODBias = 0;
						samplerDesc.MaxAnisotropy = 0;
						samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
						samplerDesc.MinLOD = 0.0f;
						samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;

						auto cpuHandle = smpDescriptorHeap_->GetCpuHandle();
						graphics_->GetDevice()->CreateSampler(&samplerDesc, cpuHandle);
					}
				}
				cbreDescriptorHeap_->IncrementCpuHandle(1);
				smpDescriptorHeap_->IncrementCpuHandle(1);
			}
		}
		cbreDescriptorHeap_->IncrementGpuHandle(increment);
		smpDescriptorHeap_->IncrementGpuHandle(increment);
	}

	// setup a topology (triangle)
	currentCommandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// draw polygon
	currentCommandList_->DrawIndexedInstanced(pritimiveCount * 3 /*triangle*/, 1, 0, 0, 0);

	CommandList::Draw(pritimiveCount);
}

void CommandListDX12::Clear(const Color8& color)
{
	assert(currentCommandList_ != nullptr);

	auto rt = renderPass_;
	if (rt == nullptr)
		return;

	float color_[] = {color.R / 255.0f, color.G / 255.0f, color.B / 255.0f, color.A / 255.0f};

	auto handle = rt->GetHandleRTV();
	for (int i = 0; i < rt->GetCount(); i++)
	{
		currentCommandList_->ClearRenderTargetView(handle[i], color_, 0, nullptr);
	}
}

void CommandListDX12::ClearDepth()
{
	assert(currentCommandList_ != nullptr);

	auto rt = renderPass_;
	if (rt == nullptr)
		return;

	auto handle = rt->GetHandleDSV();
	for (int i = 0; i < rt->GetCount(); i++)
	{
		currentCommandList_->ClearDepthStencilView(handle[i], D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	}
}

ID3D12GraphicsCommandList* CommandListDX12::GetCommandList() const { return commandList_.get(); }

ID3D12Fence* CommandListDX12::GetFence() const { return fence_; }

UINT64 CommandListDX12::GetAndIncFenceValue()
{
	auto ret = fenceValue_;
	fenceValue_ += 1;
	return ret;
}

void CommandListDX12::WaitUntilCompleted()
{

	if (fence_->GetCompletedValue() < fenceValue_ - 1)
	{
		auto hr = fence_->SetEventOnCompletion(fenceValue_ - 1, fenceEvent_);
		if (FAILED(hr))
		{
			return;
		}
		WaitForSingleObject(fenceEvent_, INFINITE);
	}
}

} // namespace LLGI
