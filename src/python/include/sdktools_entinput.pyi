from typing import Any, list, Callable, Union


def AcceptEntityInput(dest: int, input: str, activator: int = ..., caller: int = ..., outputid: int = ...) -> bool:
    """Invokes a named input method on an entity.

After completion (successful or not), the current global variant is re-initialized.

@param dest          Destination entity index.
@param input         Input action.
@param activator     Entity index which initiated the sequence of actions (-1 for a NULL entity).
@param caller        Entity index from which this event is sent (-1 for a NULL entity).
@param outputid      Unknown.
@return              True if successful otherwise false.
@error               Invalid entity index or no mod support."""
    pass