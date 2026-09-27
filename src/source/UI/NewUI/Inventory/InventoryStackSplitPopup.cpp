#include "stdafx.h"
#include "UI/NewUI/Inventory/InventoryStackSplitPopup.h"
#include "UI/NewUI/Inventory/NewUIMyInventory.h"

#include <algorithm>
#include <cwchar>

namespace SEASON3B
{
void CInventoryStackSplitPopup::Initialize(CNewUIMyInventory* owner)
{
    m_Owner = owner;
    m_Input.Init(g_hWnd, 44, 18, 3);
    m_Input.SetOption(UIOPTION_NUMBERONLY | UIOPTION_ENTERIMECHKOFF);
    m_Input.SetTextColor(255, 255, 255, 255);
    m_Input.SetBackColor(255, 20, 26, 35);
    m_Input.SetFont(g_hFont);
    m_Input.SetState(UISTATE_HIDE);
    SetRelatedWnd(reinterpret_cast<HWND>(&m_Input));
}

void CInventoryStackSplitPopup::Open(int itemX, int itemY, int itemWidth, int sourceSlot, int count)
{
    constexpr int gap = 5;
    m_X = itemX + itemWidth + gap;
    if (m_X + Width > REFERENCE_WIDTH)
    {
        m_X = itemX - Width - gap;
    }
    m_X = std::clamp(m_X, 0, REFERENCE_WIDTH - Width);
    m_Y = std::clamp(itemY, 0, REFERENCE_HEIGHT - Height);
    m_SourceSlot = sourceSlot;
    m_ExpectedCount = count;
    m_Amount = 1;
    m_Open = true;
    m_Pending = false;
    m_Dragging = false;
    m_NoSpace = false;
    m_Changed = false;
    m_Input.SetPosition(m_X + 10, m_Y + 24);
    m_Input.SetState(UISTATE_NORMAL);
    SetAmount(1);
    m_Input.GiveFocus(true);
}

void CInventoryStackSplitPopup::Close()
{
    m_Open = false;
    m_Pending = false;
    m_Dragging = false;
    m_Input.SetState(UISTATE_HIDE);
    if (m_Input.HaveFocus())
    {
        CUITextInputBox::ReleaseFocus();
    }
}

void CInventoryStackSplitPopup::ShowNoSpace()
{
    m_Pending = false;
    m_NoSpace = true;
}

void CInventoryStackSplitPopup::ShowChanged()
{
    m_Pending = false;
    m_Changed = true;
}

void CInventoryStackSplitPopup::SetAmount(int amount)
{
    m_Amount = std::clamp(amount, 1, m_ExpectedCount);
    wchar_t text[4]{};
    std::swprintf(text, std::size(text), L"%d", m_Amount);
    m_Input.SetText(text);
}

void CInventoryStackSplitPopup::SyncAmountFromInput()
{
    wchar_t text[4]{};
    m_Input.GetText(text, 4);
    if (!text[0])
    {
        return;
    }

    const int amount = static_cast<int>(std::wcstol(text, nullptr, 10));
    if (amount < 1 || amount > m_ExpectedCount)
    {
        SetAmount(amount);
    }
    else
    {
        m_Amount = amount;
    }
}

bool CInventoryStackSplitPopup::HandleMouse(int& takenAmount)
{
    if (!m_Open)
    {
        return false;
    }

    m_Input.DoAction();
    SyncAmountFromInput();
    if (m_Pending)
    {
        return true;
    }
    if (!MouseLButton)
    {
        m_Dragging = false;
    }
    if (m_Dragging && !m_Pending)
    {
        const int trackX = std::clamp(MouseX - (m_X + 10), 0, Width - 20);
        SetAmount(1 + trackX * (m_ExpectedCount - 1) / (Width - 20));
        return true;
    }
    if (!IsPress(VK_LBUTTON))
    {
        return true;
    }

    if (CheckMouseIn(m_X + 10, m_Y + 49, Width - 20, 13) && !m_Pending)
    {
        m_Dragging = true;
        const int trackX = std::clamp(MouseX - (m_X + 10), 0, Width - 20);
        SetAmount(1 + trackX * (m_ExpectedCount - 1) / (Width - 20));
        return true;
    }

    if (CheckMouseIn(m_X + 10, m_Y + 70, 60, 20) && !m_Pending && !m_Changed)
    {
        wchar_t text[4]{};
        m_Input.GetText(text, 4);
        if (!text[0])
        {
            SetAmount(1);
        }
        SyncAmountFromInput();
        takenAmount = m_Amount;
        return true;
    }

    if (CheckMouseIn(m_X + 76, m_Y + 70, 60, 20) || !CheckMouseIn(m_X, m_Y, Width, Height))
    {
        Close();
    }
    return true;
}

bool CInventoryStackSplitPopup::UpdateMouseEvent()
{
    int takenAmount = 0;
    HandleMouse(takenAmount);
    if (takenAmount > 0 && m_Owner != nullptr)
    {
        m_Owner->TakeFromStack(takenAmount);
    }

    return false;
}

bool CInventoryStackSplitPopup::UpdateKeyEvent()
{
    if (IsPress(VK_ESCAPE))
    {
        Close();
        return false;
    }

    return true;
}

bool CInventoryStackSplitPopup::Render()
{
    if (!m_Open)
    {
        return true;
    }

    EnableAlphaTest();
    RenderColorQuadARGB(m_X, m_Y, Width, Height, 0xE51B2633u);
    RenderColorQuadARGB(m_X + 10, m_Y + 50, Width - 20, 8, 0xFF5B6673u);
    const int thumbX = m_X + 10 + (m_Amount - 1) * (Width - 20) / (m_ExpectedCount - 1);
    RenderColorQuadARGB(thumbX - 2, m_Y + 47, 5, 14, 0xFFFFD275u);
    RenderColorQuadARGB(m_X + 10, m_Y + 70, 60, 20, 0xFF386C50u);
    RenderColorQuadARGB(m_X + 76, m_Y + 70, 60, 20, 0xFF68444Au);
    g_pRenderText->SetFont(g_hFont);
    g_pRenderText->SetBgColor(0, 0, 0, 0);
    g_pRenderText->SetTextColor(255, 255, 255, 255);
    g_pRenderText->RenderText(m_X + 10, m_Y + 5, L"Take from stack");
    g_pRenderText->RenderText(m_X + 25, m_Y + 73, L"Take");
    g_pRenderText->RenderText(m_X + 91, m_Y + 73, L"Cancel");
    if (m_NoSpace)
    {
        g_pRenderText->RenderText(m_X + 61, m_Y + 27, L"No space");
    }
    else if (m_Changed)
    {
        g_pRenderText->RenderText(m_X + 61, m_Y + 27, L"Stack changed");
    }
    m_Input.Render();
    DisableAlphaBlend();
    return true;
}
}
