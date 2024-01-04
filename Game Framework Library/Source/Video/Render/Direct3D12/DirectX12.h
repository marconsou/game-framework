#ifndef DIRECTX12_H
#define DIRECTX12_H

#include "../../../App/Windows/WindowsCompact.h"

#include <directx/dxgiformat.h>
#include <directx/d3d12.h>
#include <directx/d3dx12.h>
#include <dxguids/dxguids.h>

#include <dxgi1_6.h>

#pragma warning( push )
#pragma warning( disable : 5260 )
#include <directxmath/DirectXMath.h>
#pragma warning( pop )

#include <directxmath/DirectXColors.h>

#ifdef _DEBUG
#include <dxgidebug.h>
#endif

#endif