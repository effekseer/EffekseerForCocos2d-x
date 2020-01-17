
#pragma once

#include "../LLGI.Compiler.h"
#include "LLGI.BaseDX12.h"

namespace LLGI
{

enum class CompilerDX12Option : int32_t
{
	None = 0,
	RowMajor = (1 << 0),
	ColumnMajor = (2 << 0),
};

class CompilerDX12 : public Compiler
{
private:
	CompilerDX12Option option_;

public:
	CompilerDX12(const CompilerDX12Option& option = CompilerDX12Option::None);
	virtual ~CompilerDX12() = default;

	void Initialize() override;
	void Compile(CompilerResult& result, const char* code, ShaderStageType shaderStage) override;

	DeviceType GetDeviceType() const override { return DeviceType::DirectX12; }
};

} // namespace LLGI
