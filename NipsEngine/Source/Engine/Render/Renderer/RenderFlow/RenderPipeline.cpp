#include "RenderPipeline.h"
#include "OpaqueRenderPass.h"
#include "DecalRenderPass.h"
#include "FogRenderPass.h"
#include "FXAARenderPass.h"
#include "FontRenderPass.h"
#include "SubUVRenderPass.h"
#include "TranslucentRenderPass.h"
#include "SelectionMaskRenderPass.h"
#include "GridRenderPass.h"
#include "EditorRenderPass.h"
#include "DepthLessRenderPass.h"
#include "PostProcessOutlineRenderPass.h"

bool FRenderPipeline::Initialize()
{
    OpaqueRenderPass = std::make_shared<FOpaqueRenderPass>();
    OpaqueRenderPass->Initialize();

    DecalRenderPass = std::make_shared<FDecalRenderPass>();
    DecalRenderPass->Initialize();

	FogRenderPass = std::make_shared<FFogRenderPass>();
    FogRenderPass->Initialize();

	FXAARenderPass = std::make_shared<FFXAARenderPass>();
    FXAARenderPass->Initialize();

    FontRenderPass = std::make_shared<FFontRenderPass>();
    FontRenderPass->Initialize();

    SubUVRenderPass = std::make_shared<FSubUVRenderPass>();
    SubUVRenderPass->Initialize();

    TranslucentRenderPass = std::make_shared<FTranslucentRenderPass>();
    TranslucentRenderPass->Initialize();

    SelectionMaskRenderPass = std::make_shared<FSelectionMaskRenderPass>();
    SelectionMaskRenderPass->Initialize();

    GridRenderPass = std::make_shared<FGridRenderPass>();
    GridRenderPass->Initialize();

    EditorRenderPass = std::make_shared<FEditorRenderPass>();
    EditorRenderPass->Initialize();

    DepthLessRenderPass = std::make_shared<FDepthLessRenderPass>();
    DepthLessRenderPass->Initialize();

    PostProcessOutlineRenderPass = std::make_shared<FPostProcessOutlineRenderPass>();
    PostProcessOutlineRenderPass->Initialize();

	//LightRenderPass->SetSkipWireframe(true);
	FogRenderPass->SetSkipWireframe(true);
    FXAARenderPass->SetSkipWireframe(true);

	/**
	 * 각 Render Pass 는 자신의 출력 SRV/RTV 를 다음 패스로 넘긴다.
	 * 마지막 패스가 남긴 OutSRV/OutRTV 가 RenderTargets.FinalSRV/FinalRTV 가 된다.
	 */
	RenderPasses.push_back(OpaqueRenderPass);
    RenderPasses.push_back(DecalRenderPass);
    //RenderPasses.push_back(LightRenderPass);

    RenderPasses.push_back(FogRenderPass);
    RenderPasses.push_back(FXAARenderPass); 
	RenderPasses.push_back(FontRenderPass);
    RenderPasses.push_back(SubUVRenderPass);
    RenderPasses.push_back(TranslucentRenderPass);
    RenderPasses.push_back(SelectionMaskRenderPass);
    RenderPasses.push_back(GridRenderPass);
    RenderPasses.push_back(EditorRenderPass);
    RenderPasses.push_back(DepthLessRenderPass);
    RenderPasses.push_back(PostProcessOutlineRenderPass);

    return true;
}

bool FRenderPipeline::Render(const FRenderPassContext* Context)
{
    OutSRV = nullptr;
    OutRTV = nullptr;

	for (std::shared_ptr<FBaseRenderPass> Pass : RenderPasses)
	{
        Pass->SetPrevPassSRV(OutSRV);
        Pass->SetPrevPassRTV(OutRTV);
        Pass->Render(Context);
        OutSRV = Pass->GetOutSRV();
        OutRTV = Pass->GetOutRTV();
	}

	Context->RenderTargets->FinalSRV = OutSRV;
    Context->RenderTargets->FinalRTV = OutRTV;

    return true;
}

void FRenderPipeline::Release()
{
	if (OpaqueRenderPass)
    {
        OpaqueRenderPass->Release();
        OpaqueRenderPass.reset();
	}

    if (DecalRenderPass)
    {
        DecalRenderPass->Release();
        DecalRenderPass.reset();
    }

	//if (LightRenderPass)
	//{
 //       LightRenderPass->Release();
 //       LightRenderPass.reset();
	//}

	if (FogRenderPass)
	{
        FogRenderPass->Release();
        FogRenderPass.reset();
	}

	if (FXAARenderPass)
	{
        FXAARenderPass->Release();
        FXAARenderPass.reset();
	}

    if (FontRenderPass)
    {
        FontRenderPass->Release();
        FontRenderPass.reset();
    }

    if (SubUVRenderPass)
    {
        SubUVRenderPass->Release();
        SubUVRenderPass.reset();
    }

    if (TranslucentRenderPass)
    {
        TranslucentRenderPass->Release();
        TranslucentRenderPass.reset();
    }

    if (SelectionMaskRenderPass)
    {
        SelectionMaskRenderPass->Release();
        SelectionMaskRenderPass.reset();
    }

    if (GridRenderPass)
    {
        GridRenderPass->Release();
        GridRenderPass.reset();
    }

    if (EditorRenderPass)
    {
        EditorRenderPass->Release();
        EditorRenderPass.reset();
    }

    if (DepthLessRenderPass)
    {
        DepthLessRenderPass->Release();
        DepthLessRenderPass.reset();
    }

    if (PostProcessOutlineRenderPass)
    {
        PostProcessOutlineRenderPass->Release();
        PostProcessOutlineRenderPass.reset();
    }
}
