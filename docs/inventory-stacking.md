# Inventory stacks

Jewels of Bless, Soul, Chaos, Life, Creation, Guardian, and Harmony can each share an inventory slot with jewels of the same type and level. A stack holds up to 255 by default, and the number on its inventory icon shows its count. Using a jewel spends one jewel from the stack.

Drag one jewel stack onto another to combine them. Picked items are centered under the pointer. The inventory highlight snaps to the grid position covered most by the item's rectangular footprint, and dropping uses that highlighted position. For a one-slot item, this is the slot under the pointer. The destination fills first. If the total exceeds its limit, the remaining jewels stay in the source slot. A full destination or a different jewel type cannot accept the source.

Picking up jewels from the ground fills an existing stack automatically. If the dropped stack would overflow it, the remainder needs a free inventory slot; otherwise the pickup fails without changing either stack.

Server administrators can set a different limit for an item through its item definition's **Durability** value. For non-wearable stackable items, that value is the maximum stack count. New instances still begin with one item.

When a game server starts with an older configuration, it upgrades default jewel definitions whose limit is still one to 255 and saves that change. Existing jewel items keep their count of one and can then be combined by dragging one onto another, even when the inventory has no free slots.

To take items out of any stack, switch the inventory to repair mode and click a stack containing at least two items. The popup appears beside the item and repair mode turns off immediately, restoring the normal pointer. Set the amount with the number field or slider, then click **Take**. The taken items appear in a free inventory slot and the original stack keeps the rest. A successful split closes the popup. **Cancel** closes it without changing the stack. Taking the entire count also leaves the stack as it was.

Splitting requires a free inventory slot, including an unlocked inventory extension slot. If none is available, the popup shows **No space** and both stacks remain unchanged. Splitting costs no Zen.
