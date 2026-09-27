#pragma once

#include "UI/Legacy/UIControls.h"
#include "UI/NewUI/NewUIBase.h"

namespace SEASON3B
{
class CNewUIMyInventory;

class CInventoryStackSplitPopup : public CNewUIObj
{
public:
    void Initialize(CNewUIMyInventory* owner);
    void Open(int itemX, int itemY, int itemWidth, int sourceSlot, int count);
    void Close();
    bool IsOpen() const { return m_Open; }
    int GetSourceSlot() const { return m_SourceSlot; }
    int GetExpectedCount() const { return m_ExpectedCount; }
    bool IsPending() const { return m_Pending; }
    void SetPending(bool pending) { m_Pending = pending; }
    void ShowNoSpace();
    void ShowChanged();
    bool HandleMouse(int& takenAmount);
    bool UpdateMouseEvent() override;
    bool UpdateKeyEvent() override;
    bool Update() override { return true; }
    bool Render() override;
    float GetLayerDepth() override { return 20.0f; }
    float GetKeyEventOrder() override { return 11.0f; }
    bool IsVisible() const override { return m_Open; }

private:
    void SetAmount(int amount);
    void SyncAmountFromInput();

    static constexpr int Width = 146;
    static constexpr int Height = 100;
    int m_X = 0;
    int m_Y = 0;
    int m_SourceSlot = -1;
    int m_ExpectedCount = 0;
    int m_Amount = 1;
    bool m_Open = false;
    bool m_Pending = false;
    bool m_Dragging = false;
    bool m_NoSpace = false;
    bool m_Changed = false;
    CNewUIMyInventory* m_Owner = nullptr;
    CUITextInputBox m_Input;
};
}
