
#pragma once

#include "../LLGI.Compiler.h"
#include "LLGI.BaseVulkan.h"

namespace LLGI
{

class CompilerVulkan : public Compiler
{
private:
public:
	CompilerVulkan() = default;
	virtual ~CompilerVulkan() = default;

	void Initialize() override;
	void Compile(CompilerResult& result, const char* code, ShaderStageType shaderStage) override;

	DeviceType GetDeviceType() const override { return DeviceType::Default; }
};

} // namespace LLGI
