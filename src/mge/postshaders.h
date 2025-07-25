#pragma once

#include "doublesurface.h"

#include <memory>
#include <string>
#include <vector>
#include <thread>
#include <atomic>

enum EffectVariableID {
    EV_lastshader, EV_lastpass, EV_depthframe, EV_watertexture,
    EV_eyevec, EV_eyepos, EV_sunvec, EV_suncol, EV_sunamb, EV_sunpos, EV_sunvis, EV_HDR,
    EV_mview, EV_mproj, EV_fogcol, EV_fognearcol, EV_fogstart, EV_fogrange, EV_fognearstart, EV_fognearrange,
    EV_rcpres, EV_fov, EV_time, EV_waterlevel, EV_isinterior, EV_isunderwater
};

struct MGEShader {
    ID3DXEffect* effect;
    bool enabled;
    int disableFlags;
    int priority;
    DWORD timestamp;
    std::string name;
    D3DXHANDLE ehVars[32];

    void SetTexture(EffectVariableID id, LPDIRECT3DBASETEXTURE9 tex);
    void SetMatrix(EffectVariableID id, const D3DXMATRIX* m);
    void SetFloatArray(EffectVariableID id, const float* x, int n);
    void SetFloat(EffectVariableID id, float x);
    void SetInt(EffectVariableID id, int x);
    void SetBool(EffectVariableID id, bool b);
};

typedef void (*MGEShaderUpdateFunc)(MGEShader*);

class PostShaders {
    static IDirect3DDevice9* device;
    static std::vector<std::unique_ptr<MGEShader>> shaders;
    static std::vector<D3DXMACRO> features;
    static IDirect3DTexture9* texLastShader;
    static IDirect3DSurface9* surfaceLastShader;
    static SurfaceDoubleBuffer doublebuffer;
    static IDirect3DVertexBuffer9* vbPost;
    static IDirect3DSurface9* surfReadqueue, *surfReadback;
    static D3DXVECTOR4 adaptPoint;
    static float rcpRes[2];
    static std::thread shaderLoadThread;
    static std::atomic<bool> isLoading; // Add this member declaration

public:
    static bool init(IDirect3DDevice9* realDevice);
    static bool initShaderChain();
    static bool loadNewShader(const char* name);
    static bool updateShaderChain();
    static bool checkShaderVersion(ID3DXEffect* effect);
    static void initShader(MGEShader* shader);
    static void loadShaderDependencies(MGEShader* shader);
    static void orderShaders();
    static bool initBuffers();
    static void release();
    static bool initShaderChainThreaded();
    static void waitForShaderLoading();

    static MGEShader* findShader(const char* shaderName);
    static const std::vector<std::unique_ptr<MGEShader>>& listShaders();
    static bool setShaderVar(const char* shaderName, const char* varName, bool b);
    static bool setShaderVar(const char* shaderName, const char* varName, int x);
    static bool setShaderVar(const char* shaderName, const char* varName, float x);
    static bool setShaderVar(const char* shaderName, const char* varName, float* v);
    static bool setShaderEnable(const char* shaderName, bool enable);

    static void evalAdaptHDR(IDirect3DSurface9* source, int environmentFlags, float dt);
    enum HDRReadbackState {
        HDR_IDLE,
        HDR_DOWNSAMPLING,
        HDR_WAITING,
        HDR_READY_TO_READ
    };

    static void shaderTime(MGEShaderUpdateFunc updateVarsFunc, int environmentFlags, float frameTime);
    static IDirect3DTexture9* borrowBuffer(int n);
    static void applyBlend();
};
