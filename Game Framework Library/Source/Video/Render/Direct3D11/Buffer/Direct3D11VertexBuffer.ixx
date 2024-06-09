module;

#include "../Direct3D11.h"

export module Direct3D11VertexBuffer;

import Log;
import WindowsApi;

export namespace gfl
{
	class Direct3D11VertexBuffer
	{
	public:
		Direct3D11VertexBuffer(UINT size, ID3D11Device1* device, Log* log);
		void SetVertexBuffer(UINT strides, ID3D11DeviceContext1* context) const;

		template<typename T>
		void MapData(const T& data, ID3D11DeviceContext1* context) const
		{
			D3D11_MAPPED_SUBRESOURCE mapped{};
			if (WindowsApi::Failed(context->Map(this->buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
				this->log->Error("Map");

			memcpy(mapped.pData, data, sizeof(data));

			context->Unmap(this->buffer.Get(), 0);
		}
	private:
		const UINT size;
		Log* log{};
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	};
}