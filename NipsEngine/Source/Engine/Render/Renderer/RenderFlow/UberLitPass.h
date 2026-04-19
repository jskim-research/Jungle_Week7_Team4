#pragma once
#include "RenderPass.h"

class FUberLitRenderPass : public FBaseRenderPass
{
public:
    bool Initialize() override;
    bool Release() override;

protected:
    bool Begin(const FRenderPassContext* Context) override;
    bool DrawCommand(const FRenderPassContext* Context) override;
    bool End(const FRenderPassContext* Context) override;
};
