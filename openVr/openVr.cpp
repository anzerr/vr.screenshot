
#include <sys/stat.h>
#include <d3d11.h>
#include <vector>
#include <memory>
#include <algorithm>
#include <fstream>

#pragma comment(lib, "d3d11.lib")

#include "openvr.h"
#ifdef _WIN64
#pragma comment(lib, "lib/win64/openvr_api.lib")
#else
#pragma comment(lib, "lib/win32/openvr_api.lib")
#endif

#define log(log_level, message, ...) \
		printf("[openvr] [%s] %s", log_level, message, ##__VA_ARGS__)

#define debug(message, ...) \
		log("LOG_DEBUG", message, ##__VA_ARGS__)
#define warn(message, ...) \
		log("LOG_WARNING", message, ##__VA_ARGS__)

struct openVrEye {
	ID3D11ShaderResourceView* source;
	ID3D11Resource* pSource;
	ID3D11Texture2D* texture;
};

struct openVrContext {
	ID3D11Device* dev11;
	ID3D11DeviceContext* pContext;

	DWORD lastCheckTick;

	bool init;
	bool vrInit;
};

static bool initEye(vr::Hmd_Eye type, openVrContext* context, openVrEye* eye) {
	vr::VRCompositor()->GetMirrorTextureD3D11(type, context->dev11, (void**)&eye->source);
	if (!eye->source) {
		warn("initEye: GetMirrorTextureD3D11 failed");
		return false;
	}

	eye->source->GetResource(&eye->pSource);
	if (!eye->pSource) {
		warn("initEye: GetResource failed");
		return false;
	}

	ID3D11Texture2D* tex2D;
	eye->pSource->QueryInterface<ID3D11Texture2D>(&tex2D);
	if (!tex2D) {
		warn("initEye: QueryInterface failed");
		return false;
	}

	D3D11_TEXTURE2D_DESC desc;
	tex2D->GetDesc(&desc);
	desc.BindFlags = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tex2D->Release();

	HRESULT hr = context->dev11->CreateTexture2D(&desc, NULL, &eye->texture);
	if (FAILED(hr)) {
		warn("initEye: CreateTexture2D failed");
		return false;
	}
	return true;
}

static void openVrInit(openVrContext* context, openVrEye* right, openVrEye* left, bool forced = false) {
	if (context->init) {
		return;
	}
	if (GetTickCount() - 1000 < context->lastCheckTick && !forced) {
		return;
	}

	vr::EVRInitError err = vr::VRInitError_None;
	vr::VR_Init(&err, vr::VRApplication_Background);
	if (err != vr::VRInitError_None) {
		debug("OpenVR not available");
		context->lastCheckTick = GetTickCount();
		return;
	}
	context->vrInit = true;

	HRESULT hr;
	D3D_FEATURE_LEVEL featureLevel;
	hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, 0, 0, 0, 0, D3D11_SDK_VERSION, &context->dev11, &featureLevel, &context->pContext);
	if (FAILED(hr)) {
		warn("openVrInit: D3D11CreateDevice failed");
		return;
	}

	if (!initEye(vr::Eye_Right, context, right)) {
		warn("openVrInit: failed to init right eye");
		return;
	}
	if (!initEye(vr::Eye_Left, context, left)) {
		warn("openVrInit: failed to init left eye");
		return;
	}

	context->init = true;
}

static void openVrdeinitEye(openVrEye* eye) {
	if (eye->pSource) {
		eye->pSource->Release();
	}
	if (eye->texture) {
		eye->texture->Release();
	}

	eye->source = NULL;
	eye->pSource = NULL;
	eye->texture = NULL;
}

static void openVrdeinit(openVrContext* context, openVrEye* right, openVrEye* left) {
	context->init = false;

	openVrdeinitEye(right);
	openVrdeinitEye(left);

	if (context->vrInit) {
		context->vrInit = false;
		vr::VR_Shutdown();
	}

	if (context->pContext) {
		context->pContext->Release();
	}
	if (context->dev11) {
		if (context->dev11->Release() != 0) {
			warn("openVrdeinit: device refcount not zero!");
		}
	}

	context->pContext = NULL;
	context->dev11 = NULL;
}

static void writeDate(const char* fileName, openVrContext* context, openVrEye* eye) {
	HRESULT hr;
	unsigned char* m_captureData = nullptr;

	ID3D11Texture2D* tex2D;
	eye->pSource->QueryInterface<ID3D11Texture2D>(&tex2D);

	D3D11_TEXTURE2D_DESC desc;
	tex2D->GetDesc(&desc);

	unsigned int size = desc.Width * desc.Height;
	m_captureData = new unsigned char[size * 4];

	context->pContext->CopyResource(eye->texture, tex2D);
	
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	UINT subresource = D3D11CalcSubresource(0, 0, 0);
	hr = context->pContext->Map(
		eye->texture,
		subresource,
		D3D11_MAP_READ_WRITE,
		0,
		&mappedResource
	);
	if (FAILED(hr)) {
		warn("Failed to map staging texture\n");
	}

	const int pitch = desc.Width << 2;
	const unsigned char* source = static_cast<const unsigned char*>(mappedResource.pData);
	unsigned char* dest = m_captureData;
	for (int i = 0; i < desc.Height; ++i) {
		memcpy(dest, source, desc.Width * 4);
		source += mappedResource.RowPitch;
		dest += pitch;
	}

	std::ofstream file(fileName, std::ios::binary);
	file.write((char*)&m_captureData[0], size * 4);

	printf("a: w:%d h:%d s:%d\n", desc.Width, desc.Height, size);
	printf("b: r:%d d:%d\n", mappedResource.RowPitch, mappedResource.DepthPitch);
}

int main() {
	openVrContext context {nullptr, nullptr, 0, false, false};
	openVrEye left {nullptr, nullptr, nullptr};
	openVrEye right {nullptr, nullptr, nullptr};
	openVrInit(&context, &right, &left);
	if (context.init) {
		writeDate("right.dump", &context, &right);
		writeDate("left.dump", &context, &left);
	}
	openVrdeinit(&context, &right, &left);
}
