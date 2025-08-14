# Refactoring Plan for zf_util_ui.inc

This document outlines the plan to refactor and streamline the `zf_util_ui.inc` file to improve code clarity, reduce redundancy, and enhance maintainability.

## 1. Abstract HUD Display Logic
*   **Problem**: The `updateHud` and `updateTeammateHud` functions contain significant duplicated code, especially for fetching and formatting player stats (Attack, Defense, Crit, Speed).
*   **Solution**: Create a new helper function, `FormatStatString`, to handle the formatting of numbers with a leading "+". Then, create another function, `ShowPlayerStatsHud`, to centralize the display of player stat HUDs. This will significantly reduce code duplication in `updateHud` and `updateTeammateHud`.

## 2. Unify Perk Name Retrieval
*   **Problem**: In `updateHud`, there are two nearly identical blocks of code for getting and displaying perk names based on whether the player is a survivor or a zombie. This is also true for `panel_PrintSurPerkList` and `panel_PrintZomPerkList`.
*   **Solution**: Create a generic function, `GetPlayerPerkDisplayName`, which internally checks if the target player is a survivor or zombie and calls the appropriate `GetSurPerkDisplayName` or `GetZomPerkDisplayName`. This single function can then be used wherever a perk name needs to be displayed.

## 3. Simplify Menu Creation
*   **Problem**: The code for `panel_PrintSurPerkList` and `panel_PrintZomPerkList` is almost identical, differing only in the functions called and the titles. The same applies to `panel_PrintSurPerkSelect` and `panel_PrintZomPerkSelect`.
*   **Solution**: Merge these paired functions into more generic ones, such as `panel_PrintPerkList(int client, bool isZombie)` and `panel_PrintPerkSelect(int client, int perk, bool isZombie)`. A boolean parameter will be used to differentiate between creating menus for survivors or zombies.

## 4. Remove Duplicate Help Menu Panels
*   **Problem**: There is a lot of repeated panel creation and item drawing code in `panel_PrintHelpTeam`, `panel_PrintHelpSurClass`, `panel_PrintHelpZomClass`, and `panel_PrintClass`. The `switch` statement in `panel_PrintClass` is particularly repetitive.
*   **Solution**: Create a generic help panel function, `panel_PrintHelpPage(int client, const char[][] lines, int lineCount)`, and refactor `panel_PrintClass` to use a data-driven approach instead of a large `switch` statement, thereby reducing duplication.

## 5. Consolidate Debug Menus
*   **Problem**: The logic for creating and handling debug menus is scattered across multiple functions, such as `panel_DebugModifyStats` and `panel_DebugModifyStatValue`.
*   **Solution**: Consolidate these features into fewer functions and use more generic menu handlers to simplify the code.

## 6. Review and Remove Redundant Code
*   **Problem**: The file may contain unused variables, functions, or duplicate `#include` statements. For instance, `zf_perk.inc` includes `zf_util_ui.inc`, which might also be included elsewhere.
*   **Solution**: After completing the refactoring steps above, conduct a full review of the file to remove any code and declarations that are no longer necessary, ensuring the final code is clean.
