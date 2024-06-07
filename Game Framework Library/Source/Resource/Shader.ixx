export module Shader;

export namespace gfl
{
	class Shader
	{
	public:
		Shader() = default;
		Shader(const Shader&) = delete;
		Shader(Shader&&) = delete;
		Shader& operator=(const Shader&) = delete;
		Shader& operator=(Shader&&) = delete;
		virtual ~Shader() = 0 {};
	};
}