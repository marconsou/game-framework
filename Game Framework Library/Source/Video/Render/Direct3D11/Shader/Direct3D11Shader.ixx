module;

#include "../Direct3D11.h"

export module Direct3D11Shader;

import Shader;

export namespace gfl
{
	class Direct3D11Shader : public Shader
	{
	public:
		Direct3D11Shader() = default;
		Direct3D11Shader(const Direct3D11Shader&) = delete;
		Direct3D11Shader(Direct3D11Shader&&) = delete;
		Direct3D11Shader& operator=(const Direct3D11Shader&) = delete;
		Direct3D11Shader& operator=(Direct3D11Shader&&) = delete;
		virtual ~Direct3D11Shader() = default;
		virtual void SetShader(ID3D11DeviceContext1* context) const = 0;
	};
}